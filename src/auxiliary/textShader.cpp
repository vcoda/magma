/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

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
#include <cstdarg>
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
#include "../descriptors/bufferDescriptor.h"
#include "../descriptors/descriptorSetTable.h"
#include "../states/vertexInputStructure.h"
#include "../states/inputAssemblyState.h"
#include "../states/rasterizationState.h"
#include "../states/multisampleState.h"
#include "../states/depthStencilState.h"
#include "../states/colorBlendState.h"
#include "../allocator/allocator.h"
#include "../helpers/mapScoped.h"
#include "../helpers/mapTyped.h"

namespace magma
{
namespace aux
{
struct alignas(16) TextShader::String
{
    float x, y;
    uint32_t first;
    uint32_t last;
    float r, g, b, a;
};

struct alignas(16) TextShader::Glyph
{
    uint32_t data[4];
};

struct TextShader::DescriptorSetTable : magma::DescriptorSetTable
{
    descriptor::StorageBuffer stringBuffer = 0;
    descriptor::StorageBuffer charBuffer = 1;
    MAGMA_REFLECT(stringBuffer, charBuffer)
};

struct TextShader::PushConstants
{
    uint32_t stringCount;
};

TextShader::TextShader(const std::shared_ptr<RenderPass> renderPass,
    const uint32_t maxChars /* 1024 */,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    std::shared_ptr<PipelineCache> pipelineCache /* nullptr */):
    maxStrings(16),
    maxChars(maxChars),
    allocator(std::move(allocator))
{
    std::shared_ptr<Device> device = renderPass->getDevice();
    std::shared_ptr<IAllocator> hostAllocator = MAGMA_HOST_ALLOCATOR(this->allocator);
    // Create storage buffers
    stringBuffer = std::make_shared<DynamicStorageBuffer>(device, maxStrings * sizeof(String), false, this->allocator);
    charBuffer = std::make_shared<DynamicStorageBuffer>(device, maxChars * sizeof(Glyph), false, this->allocator);
    // Define descriptor set layout
    setTable = std::make_unique<DescriptorSetTable>();
    setTable->stringBuffer = stringBuffer;
    setTable->charBuffer = charBuffer;
    // Create descriptor set
    descriptorPool = std::make_shared<DescriptorPool>(device, 1, descriptor::StorageBufferPool(2), hostAllocator);
    descriptorSet = std::make_shared<DescriptorSet>(descriptorPool, *setTable, VK_SHADER_STAGE_FRAGMENT_BIT, hostAllocator);
    // Load fullscreen vertex shader
    auto vertexShader = std::make_unique<FillRectangleVertexShader>(device, hostAllocator);
constexpr
#include "spirv/output/fontf"
    constexpr hash_t fsFontHash = core::hashArray(fsFont);
    std::shared_ptr<ShaderModule> fragmentShader = std::make_shared<ShaderModule>(device, fsFont, fsFontHash, hostAllocator, true);
    const std::vector<PipelineShaderStage> shaderStages = {
        VertexShaderStage(vertexShader->getShader(), vertexShader->getEntryPointName()),
        FragmentShaderStage(fragmentShader, fragmentShader->getReflection() ? fragmentShader->getReflection()->getEntryPointName(0) : "main")
    };
    // Create font pipeline
    constexpr push::FragmentConstantRange<PushConstants> pushConstantRange;
    pipelineLayout = std::make_shared<PipelineLayout>(descriptorSet->getLayout(), pushConstantRange);
    pipeline = std::make_shared<GraphicsPipeline>(std::move(device),
        shaderStages,
        renderstate::nullVertexInput,
        renderstate::triangleList,
        renderstate::fillCullNoneCcw,
        renderstate::dontMultisample,
        renderstate::depthAlwaysDontWrite,
        renderstate::blendNormalRgb,
        std::initializer_list<VkDynamicState>{VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR},
        pipelineLayout,
        std::move(renderPass), 0,
        std::move(hostAllocator),
        std::move(pipelineCache));
}

void TextShader::draw(std::shared_ptr<CommandBuffer> cmdBuffer) const noexcept
{
    cmdBuffer->pushConstant(pipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT, stringCount);
    cmdBuffer->bindDescriptorSet(pipeline, 0, descriptorSet);
    cmdBuffer->bindPipeline(pipeline);
    cmdBuffer->draw(3);
}

void TextShader::begin()
{
    strings = helpers::map<String>(stringBuffer);
    chars = helpers::map<Glyph>(charBuffer);
    stringCount = 0;
    charCount = 0;
    offset = 0;
}

void TextShader::end()
{
    helpers::unmap(charBuffer);
    helpers::unmap(stringBuffer);
    chars = nullptr;
    strings = nullptr;
}

bool TextShader::print(uint32_t x, uint32_t y, uint32_t color, const char *format, ...)
{
    static const Glyph glyphs[] = {
        #include "data/glyphs"
    };
    MAGMA_ASSERT(strings);
    MAGMA_ASSERT(chars);
    if (stringCount >= maxStrings)
        return false;
    char sz[MAGMA_MAX_STRING];
    va_list args;
    va_start(args, format);
    int written;
#if defined(_MSC_VER) || defined(__MINGW32__)
    written = vsprintf_s(sz, format, args);
#else
    written = sprintf(sz, format, args);
#endif
    MAGMA_ASSERT(written != -1);
    const uint32_t length = static_cast<uint32_t>(strlen(sz));
    String *str = strings++;
    str->x = (float)x;
    str->y = (float)y;
    str->first = offset;
    str->last = offset + length - 1;
    str->r = ((color >> 24) & 0xFF) / 255.f; // R
    str->g = ((color >> 16) & 0xFF) / 255.f; // G
    str->b = ((color >> 8) & 0xFF) / 255.f; // B
    str->a = (color & 0xFF) / 255.f; // A
    for (uint32_t i = 0; (i < length) && (charCount < maxChars); ++i, ++charCount)
        *chars++ = glyphs[(unsigned char)sz[i]];
    offset += length;
    ++stringCount;
    return true;
}
} // namespace aux
} // namespace magma
