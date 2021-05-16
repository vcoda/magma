/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
#include "../states/vertexInputStructure.h"
#include "../states/inputAssemblyState.h"
#include "../states/rasterizationState.h"
#include "../states/multisampleState.h"
#include "../states/depthStencilState.h"
#include "../states/colorBlendState.h"
#include "../allocator/allocator.h"
#include "../helpers/mapScoped.h"
#include "../core/copyMemory.h"
#include "../core/constexprHash.h"

#ifdef _MSC_VER
#pragma warning(disable : 4244) // conversion from 'const uint32_t' to 'float', possible loss of data
#endif

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

TextShader::TextShader(const uint32_t maxChars, const uint32_t maxStrings,
    const std::shared_ptr<RenderPass> renderPass,
    std::shared_ptr<PipelineCache> pipelineCache /* nullptr */,
    std::shared_ptr<Allocator> allocator /* nullptr */):
    maxChars(maxChars),
    maxStrings(maxStrings)
{
    std::shared_ptr<Device> device = renderPass->getDevice();
    std::shared_ptr<IAllocator> hostAllocator = MAGMA_HOST_ALLOCATOR(allocator);
    // Create uniform and storage buffers
    uniforms = std::make_shared<UniformBuffer<Uniforms>>(device, allocator);
    stringBuffer = std::make_shared<DynamicStorageBuffer>(device, sizeof(String) * maxStrings, false, allocator);
    glyphBuffer = std::make_shared<DynamicStorageBuffer>(device, sizeof(Glyph) * maxChars, false, allocator);
    // Define layout of descriptor set
    descriptorPool = std::make_shared<DescriptorPool>(device, 1,
        std::vector<Descriptor>{
            descriptors::UniformBuffer(1),
            descriptors::StorageBuffer(2)
        },
        hostAllocator);
    descriptorSetLayout = std::make_shared<DescriptorSetLayout>(device,
        std::initializer_list<DescriptorSetLayout::Binding>{
            bindings::FragmentStageBinding(0, descriptors::UniformBuffer(1)),
            bindings::FragmentStageBinding(1, descriptors::StorageBuffer(1)),
            bindings::FragmentStageBinding(2, descriptors::StorageBuffer(1))
        },
        std::initializer_list<DescriptorSetLayout::SamplerBinding>{},
        hostAllocator, 0);
    descriptorSet = descriptorPool->allocateDescriptorSet(descriptorSetLayout);
    descriptorSet->writeDescriptor(0, uniforms);
    descriptorSet->writeDescriptor(1, stringBuffer);
    descriptorSet->writeDescriptor(2, glyphBuffer);
    std::shared_ptr<PipelineLayout> pipelineLayout = std::make_shared<PipelineLayout>(descriptorSetLayout);
    // Load fullscreen vertex shader
    auto vertexShader = std::make_unique<FillRectangleVertexShader>(device, hostAllocator);
constexpr
#include "spirv/output/fontf"
    constexpr std::size_t fsFontHash = core::hashArray(fsFont);
    std::shared_ptr<ShaderModule> fragmentShader = std::make_shared<ShaderModule>(device, fsFont, fsFontHash, hostAllocator, 0, true);
    const std::vector<PipelineShaderStage> shaderStages = {
        VertexShaderStage(vertexShader->getShader(), vertexShader->getEntryPointName()),
        FragmentShaderStage(fragmentShader, fragmentShader->getReflection() ? fragmentShader->getReflection()->getEntryPointName(0) : "main")
    };
    // Create font pipeline
    pipeline = std::make_shared<GraphicsPipeline>(std::move(device),
        shaderStages,
        renderstates::nullVertexInput,
        renderstates::triangleList,
        renderstates::fillCullNoneCCW,
        renderstates::dontMultisample,
        renderstates::depthAlwaysDontWrite,
        renderstates::blendNormalRgb,
        std::initializer_list<VkDynamicState>{VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR},
        std::move(pipelineLayout),
        std::move(renderPass), 0,
        std::move(hostAllocator),
        std::move(pipelineCache),
        nullptr); // basePipeline

    // Initialize glyphs
    constexpr uint32_t glyphData[] = {
#include "glyphs.h"
    };
    static_assert(sizeof(glyphs) == sizeof(glyphData), "ASCII glyph data size mismatch");
    for (uint32_t i = 0, j = 0; i < MAGMA_TEXT_SHADER_GLYPHS; ++i)
    {
        glyphs[i].c[0] = glyphData[j++];
        glyphs[i].c[1] = glyphData[j++];
        glyphs[i].c[2] = glyphData[j++];
        glyphs[i].c[3] = glyphData[j++];
    }
}

void TextShader::draw(std::shared_ptr<CommandBuffer> cmdBuffer) const noexcept
{
    cmdBuffer->bindDescriptorSet(pipeline, descriptorSet);
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
    helpers::mapScoped<String>(stringBuffer,
        [this](auto *data)
        {   // Copy string descriptions
            for (auto& str : strings)
                *data++ = str;
        });
    if (!chars.empty())
    {
        helpers::mapScoped<Glyph>(glyphBuffer,
            [this](auto *data)
            {   // Copy glyph data
                core::copyMemory(data, chars.data(), sizeof(Glyph) * chars.size());
            });
    }
}

void TextShader::print(uint32_t x, uint32_t y, uint32_t color, const char *format, ...)
{
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
    if ((chars.size() + length >= maxChars) || (strings.size() >= maxStrings))
        throw std::length_error("number of strings exceeded limit");
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
