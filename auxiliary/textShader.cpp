/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2022 Victor Coda.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://www.gnu.org/licenses/>.
*/
#include "pch.h"
#pragma hdrstop
#include "textShader.h"
#include "fillRectangleVertexShader.h"
#include "../objects/commandBuffer.h"
#include "../objects/storageBuffer.h"
#include "../objects/renderPass.h"
#include "../objects/shaderModule.h"
#include "../objects/descriptorPool.h"
#include "../objects/descriptorSet.h"
#include "../objects/descriptorSetLayout.h"
#include "../objects/pipelineLayout.h"
#include "../objects/graphicsPipeline.h"
#include "../shaders/shaderStages.h"
#include "../shaders/shaderReflection.h"
#include "../descriptors/descriptorSetLayoutReflection.h"
#include "../states/vertexInputStructure.h"
#include "../states/inputAssemblyState.h"
#include "../states/rasterizationState.h"
#include "../states/multisampleState.h"
#include "../states/depthStencilState.h"
#include "../states/colorBlendState.h"
#include "../allocator/allocator.h"
#include "../helpers/mapScoped.h"
#include "../core/copyMemory.h"

namespace magma
{
namespace aux
{
struct alignas(16) TextShader::Uniforms
{
    uint32_t stringCount;
    uint32_t padding[3];
};

struct alignas(16) TextShader::String
{
    float x, y;
    uint32_t first;
    uint32_t last;
    float color[4];
};

struct alignas(16) TextShader::Glyph
{
    uint32_t data[4];
};

struct TextShader::SetLayout : DescriptorSetLayoutReflection
{
    binding::UniformBuffer uniforms = 0;
    binding::StorageBuffer stringBuffer = 1;
    binding::StorageBuffer glyphBuffer = 2;
    MAGMA_REFLECT(&uniforms, &stringBuffer, &glyphBuffer)
};

TextShader::TextShader(const std::shared_ptr<RenderPass> renderPass,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    std::shared_ptr<PipelineCache> pipelineCache /* nullptr */):
    allocator(std::move(allocator))
{
    std::shared_ptr<Device> device = renderPass->getDevice();
    std::shared_ptr<IAllocator> hostAllocator = MAGMA_HOST_ALLOCATOR(this->allocator);
    // Create uniform and storage buffers
    uniforms = std::make_shared<UniformBuffer<Uniforms>>(device, this->allocator);
    stringBuffer = std::make_shared<DynamicStorageBuffer>(device, 8 * sizeof(String), false, this->allocator);
    glyphBuffer = std::make_shared<DynamicStorageBuffer>(device, 256 * sizeof(Glyph), false, this->allocator); // 4Kb
    // Define descriptor set layout
    setLayout = std::make_unique<SetLayout>();
    setLayout->uniforms = uniforms;
    setLayout->stringBuffer = stringBuffer;
    setLayout->glyphBuffer = glyphBuffer;
    // Create descriptor set
    descriptorPool = std::make_shared<DescriptorPool>(device, 1,
        std::vector<Descriptor>{
            descriptor::UniformBuffer(1),
            descriptor::StorageBuffer(2)
        },
        hostAllocator);
    descriptorSet = std::make_shared<DescriptorSet>(descriptorPool, *setLayout, VK_SHADER_STAGE_FRAGMENT_BIT, hostAllocator);
    // Load fullscreen vertex shader
    auto vertexShader = std::make_unique<FillRectangleVertexShader>(device, hostAllocator);
constexpr
#include "spirv/output/fontf"
    constexpr hash_t fsFontHash = core::hashArray(fsFont);
    std::shared_ptr<ShaderModule> fragmentShader = std::make_shared<ShaderModule>(device, fsFont, fsFontHash, hostAllocator, 0, true);
    const std::vector<PipelineShaderStage> shaderStages = {
        VertexShaderStage(vertexShader->getShader(), vertexShader->getEntryPointName()),
        FragmentShaderStage(fragmentShader, fragmentShader->getReflection() ? fragmentShader->getReflection()->getEntryPointName(0) : "main")
    };
    // Create font pipeline
    std::shared_ptr<PipelineLayout> pipelineLayout = std::make_shared<PipelineLayout>(descriptorSet->getLayout());
    pipeline = std::make_shared<GraphicsPipeline>(std::move(device),
        shaderStages,
        renderstate::nullVertexInput,
        renderstate::triangleList,
        renderstate::fillCullNoneCCw,
        renderstate::dontMultisample,
        renderstate::depthAlwaysDontWrite,
        renderstate::blendNormalRgb,
        std::initializer_list<VkDynamicState>{VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR},
        std::move(pipelineLayout),
        std::move(renderPass), 0,
        std::move(hostAllocator),
        std::move(pipelineCache),
        nullptr); // basePipeline
}

void TextShader::draw(std::shared_ptr<CommandBuffer> cmdBuffer) const noexcept
{
    cmdBuffer->bindDescriptorSet(pipeline, 0, descriptorSet);
    cmdBuffer->bindPipeline(pipeline);
    cmdBuffer->draw(3);
}

void TextShader::begin()
{
    strings.clear();
    chars.clear();
    chars.reserve(1024);
    offset = 0;
}

void TextShader::end()
{
    helpers::mapScoped<Uniforms>(uniforms,
        [this](auto *constants)
        {   // Globals
            constants->stringCount = MAGMA_COUNT(strings);
        });
    if (!strings.empty())
    {
        const VkDeviceSize maxStrings = stringBuffer->getSize()/sizeof(String);
        if (maxStrings < strings.size())
        {   // Reallocate if not enough memory
            stringBuffer->realloc(strings.size() * sizeof(String), allocator);
            setLayout->stringBuffer = stringBuffer;
        }
        helpers::mapScoped<String>(stringBuffer,
            [this](auto *data)
            {   // Copy string descriptions
                for (auto& str : strings)
                    *data++ = str;
            });
    }
    if (!chars.empty())
    {
        const VkDeviceSize maxChars = glyphBuffer->getSize()/sizeof(Glyph);
        if (maxChars < chars.size())
        {   // Reallocate if not enough memory
            glyphBuffer->realloc(chars.size() * sizeof(Glyph), allocator);
            setLayout->glyphBuffer = glyphBuffer;
        }
        helpers::mapScoped<Glyph>(glyphBuffer,
            [this](auto *data)
            {   // Copy glyph data
                core::copyMemory(data, chars.data(), chars.size() * sizeof(Glyph));
            });
    }
}

void TextShader::print(uint32_t x, uint32_t y, uint32_t color, const char *format, ...)
{
    constexpr Glyph glyphs[] = {
        #include "data/glyphs"
    };
    char sz[MAGMA_MAX_STRING];
    va_list args;
    va_start(args, format);
    int written;
#ifdef _MSC_VER
    written = vsprintf_s(sz, format, args);
#else
    written = sprintf(sz, format, args);
#endif
    MAGMA_ASSERT(written != -1);
    const uint32_t length = static_cast<uint32_t>(strlen(sz));
    String str;
    str.x = (float)x;
    str.y = (float)y;
    str.first = offset;
    str.last = offset + length - 1;
    str.color[0] = ((color >> 24) & 0xFF) / 255.f; // R
    str.color[1] = ((color >> 16) & 0xFF) / 255.f; // G
    str.color[2] = ((color >> 8) & 0xFF) / 255.f; // B
    str.color[3] = (color & 0xFF) / 255.f; // A
    strings.push_back(str);
    for (uint32_t i = 0; i < length; ++i)
        chars.push_back(glyphs[sz[i]]);
    offset += length;
}
} // namespace aux
} // namespace magma
