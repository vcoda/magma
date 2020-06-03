/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2020 Victor Coda.

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
#include "../states/vertexInputStructure.h"
#include "../states/inputAssemblyState.h"
#include "../states/rasterizationState.h"
#include "../states/multisampleState.h"
#include "../states/depthStencilState.h"
#include "../states/colorBlendState.h"
#include "../helpers/mapScoped.h"
#include "../core/copyMemory.h"
#include "../core/constexprHash.h"

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
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    maxChars(maxChars),
    maxStrings(maxStrings)
{
    std::shared_ptr<Device> device = renderPass->getDevice();
    // Create uniform and storage buffers
    uniforms = std::make_shared<UniformBuffer<Uniforms>>(device);
    stringBuffer = std::make_shared<DynamicStorageBuffer>(device, sizeof(String) * maxStrings);
    glyphBuffer = std::make_shared<DynamicStorageBuffer>(device, sizeof(Glyph) * maxChars);
    // Define layout of descriptor set
    constexpr Descriptor oneUniformBuffer = descriptors::UniformBuffer(1);
    constexpr Descriptor oneStorageBuffer = descriptors::StorageBuffer(1);
    descriptorPool = std::make_shared<DescriptorPool>(device, 1,
        std::vector<Descriptor>
        {
            oneUniformBuffer,
            descriptors::StorageBuffer(2)
        });
    descriptorSetLayout = std::make_shared<DescriptorSetLayout>(device,
        std::initializer_list<DescriptorSetLayout::Binding>
        {
            bindings::FragmentStageBinding(0, oneUniformBuffer),
            bindings::FragmentStageBinding(1, oneStorageBuffer),
            bindings::FragmentStageBinding(2, oneStorageBuffer)
        });
    descriptorSet = descriptorPool->allocateDescriptorSet(descriptorSetLayout);
    descriptorSet->update(0, uniforms);
    descriptorSet->update(1, stringBuffer);
    descriptorSet->update(2, glyphBuffer);
    std::shared_ptr<PipelineLayout> pipelineLayout = std::make_shared<PipelineLayout>(descriptorSetLayout);
    // Load shaders
constexpr
#include "spirv/output/blitv"
constexpr
#include "spirv/output/fontf"
    constexpr std::size_t vsBlitHash = core::hashArray(vsBlit);
    const VertexShaderStage vertexShader(std::make_shared<ShaderModule>(device, vsBlit, vsBlitHash, 0, allocator), "main");
    constexpr std::size_t fsFontHash = core::hashArray(fsFont);
    const FragmentShaderStage fragmentShader(std::make_shared<ShaderModule>(device, fsFont, fsFontHash, 0, allocator), "main");
    // Create font pipeline
    pipeline = std::make_shared<GraphicsPipeline>(std::move(device),
        std::vector<PipelineShaderStage>{vertexShader, fragmentShader},
        renderstates::nullVertexInput,
        renderstates::triangleList,
        renderstates::fillCullNoneCCW,
        renderstates::dontMultisample,
        renderstates::depthAlwaysDontWrite,
        renderstates::blendNormalRgb,
        std::initializer_list<VkDynamicState>{VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR},
        std::move(pipelineLayout),
        std::move(renderPass), 0,
        std::move(pipelineCache),
        nullptr, // basePipeline
        std::move(allocator));

    // Initialize glyphs
    core::memzero(ascii); // Zero control codes
    ascii[' '] = {0x000000,0x000000,0x000000,0x000000};
    ascii['!'] = {0x003078,0x787830,0x300030,0x300000};
    ascii['"'] = {0x006666,0x662400,0x000000,0x000000};
    ascii['#'] = {0x006C6C,0xFE6C6C,0x6CFE6C,0x6C0000};
    ascii['$'] = {0x30307C,0xC0C078,0x0C0CF8,0x303000};
    ascii['%'] = {0x000000,0xC4CC18,0x3060CC,0x8C0000};
    ascii['&'] = {0x0070D8,0xD870FA,0xDECCDC,0x760000};
    ascii['\''] = {0x003030,0x306000,0x000000,0x000000};
    ascii['('] = {0x000C18,0x306060,0x603018,0x0C0000};
    ascii[')'] = {0x006030,0x180C0C,0x0C1830,0x600000};
    ascii['*'] = {0x000000,0x663CFF,0x3C6600,0x000000};
    ascii['+'] = {0x000000,0x18187E,0x181800,0x000000};
    ascii[','] = {0x000000,0x000000,0x000038,0x386000};
    ascii['-'] = {0x000000,0x0000FE,0x000000,0x000000};
    ascii['.'] = {0x000000,0x000000,0x000038,0x380000};
    ascii['/'] = {0x000002,0x060C18,0x3060C0,0x800000};
    ascii['0'] = {0x007CC6,0xD6D6D6,0xD6D6C6,0x7C0000};
    ascii['1'] = {0x001030,0xF03030,0x303030,0xFC0000};
    ascii['2'] = {0x0078CC,0xCC0C18,0x3060CC,0xFC0000};
    ascii['3'] = {0x0078CC,0x0C0C38,0x0C0CCC,0x780000};
    ascii['4'] = {0x000C1C,0x3C6CCC,0xFE0C0C,0x1E0000};
    ascii['5'] = {0x00FCC0,0xC0C0F8,0x0C0CCC,0x780000};
    ascii['6'] = {0x003860,0xC0C0F8,0xCCCCCC,0x780000};
    ascii['7'] = {0x00FEC6,0xC6060C,0x183030,0x300000};
    ascii['8'] = {0x0078CC,0xCCEC78,0xDCCCCC,0x780000};
    ascii['9'] = {0x0078CC,0xCCCC7C,0x181830,0x700000};
    ascii[':'] = {0x000000,0x383800,0x003838,0x000000};
    ascii[';'] = {0x000000,0x383800,0x003838,0x183000};
    ascii['<'] = {0x000C18,0x3060C0,0x603018,0x0C0000};
    ascii['='] = {0x000000,0x007E00,0x7E0000,0x000000};
    ascii['>'] = {0x006030,0x180C06,0x0C1830,0x600000};
    ascii['?'] = {0x0078CC,0x0C1830,0x300030,0x300000};
    ascii['@'] = {0x007CC6,0xC6DEDE,0xDEC0C0,0x7C0000};
    ascii['A'] = {0x003078,0xCCCCCC,0xFCCCCC,0xCC0000};
    ascii['B'] = {0x00FC66,0x66667C,0x666666,0xFC0000};
    ascii['C'] = {0x003C66,0xC6C0C0,0xC0C666,0x3C0000};
    ascii['D'] = {0x00F86C,0x666666,0x66666C,0xF80000};
    ascii['E'] = {0x00FE62,0x60647C,0x646062,0xFE0000};
    ascii['F'] = {0x00FE66,0x62647C,0x646060,0xF00000};
    ascii['G'] = {0x003C66,0xC6C0C0,0xCEC666,0x3E0000};
    ascii['H'] = {0x00CCCC,0xCCCCFC,0xCCCCCC,0xCC0000};
    ascii['I'] = {0x007830,0x303030,0x303030,0x780000};
    ascii['J'] = {0x001E0C,0x0C0C0C,0xCCCCCC,0x780000};
    ascii['K'] = {0x00E666,0x6C6C78,0x6C6C66,0xE60000};
    ascii['L'] = {0x00F060,0x606060,0x626666,0xFE0000};
    ascii['M'] = {0x00C6EE,0xFEFED6,0xC6C6C6,0xC60000};
    ascii['N'] = {0x00C6C6,0xE6F6FE,0xDECEC6,0xC60000};
    ascii['O'] = {0x00386C,0xC6C6C6,0xC6C66C,0x380000};
    ascii['P'] = {0x00FC66,0x66667C,0x606060,0xF00000};
    ascii['Q'] = {0x00386C,0xC6C6C6,0xCEDE7C,0x0C1E00};
    ascii['R'] = {0x00FC66,0x66667C,0x6C6666,0xE60000};
    ascii['S'] = {0x0078CC,0xCCC070,0x18CCCC,0x780000};
    ascii['T'] = {0x00FCB4,0x303030,0x303030,0x780000};
    ascii['U'] = {0x00CCCC,0xCCCCCC,0xCCCCCC,0x780000};
    ascii['V'] = {0x00CCCC,0xCCCCCC,0xCCCC78,0x300000};
    ascii['W'] = {0x00C6C6,0xC6C6D6,0xD66C6C,0x6C0000};
    ascii['X'] = {0x00CCCC,0xCC7830,0x78CCCC,0xCC0000};
    ascii['Y'] = {0x00CCCC,0xCCCC78,0x303030,0x780000};
    ascii['Z'] = {0x00FECE,0x981830,0x6062C6,0xFE0000};
    ascii['['] = {0x003C30,0x303030,0x303030,0x3C0000};
    ascii['\\'] = {0x000080,0xC06030,0x180C06,0x020000};
    ascii[']'] = {0x003C0C,0x0C0C0C,0x0C0C0C,0x3C0000};
    ascii['^'] = {0x10386C,0xC60000,0x000000,0x000000};
    ascii['_'] = {0x000000,0x000000,0x000000,0x00FF00};
    ascii['`'] = {0x000000,0x000000,0x000000,0x000000}; // TODO: Grave accent
    ascii['a'] = {0x000000,0x00780C,0x7CCCCC,0x760000};
    ascii['b'] = {0x00E060,0x607C66,0x666666,0xDC0000};
    ascii['c'] = {0x000000,0x0078CC,0xC0C0CC,0x780000};
    ascii['d'] = {0x001C0C,0x0C7CCC,0xCCCCCC,0x760000};
    ascii['e'] = {0x000000,0x0078CC,0xFCC0CC,0x780000};
    ascii['f'] = {0x00386C,0x6060F8,0x606060,0xF00000};
    ascii['g'] = {0x000000,0x0076CC,0xCCCC7C,0x0CCC78};
    ascii['h'] = {0x00E060,0x606C76,0x666666,0xE60000};
    ascii['i'] = {0x001818,0x007818,0x181818,0x7E0000};
    ascii['j'] = {0x000C0C,0x003C0C,0x0C0C0C,0xCCCC78};
    ascii['k'] = {0x00E060,0x60666C,0x786C66,0xE60000};
    ascii['l'] = {0x007818,0x181818,0x181818,0x7E0000};
    ascii['m'] = {0x000000,0x00FCD6,0xD6D6D6,0xC60000};
    ascii['n'] = {0x000000,0x00F8CC,0xCCCCCC,0xCC0000};
    ascii['o'] = {0x000000,0x0078CC,0xCCCCCC,0x780000};
    ascii['p'] = {0x000000,0x00DC66,0x666666,0x7C60F0};
    ascii['q'] = {0x000000,0x0076CC,0xCCCCCC,0x7C0C1E};
    ascii['r'] = {0x000000,0x00EC6E,0x766060,0xF00000};
    ascii['s'] = {0x000000,0x0078CC,0x6018CC,0x780000};
    ascii['t'] = {0x000020,0x60FC60,0x60606C,0x380000};
    ascii['u'] = {0x000000,0x00CCCC,0xCCCCCC,0x760000};
    ascii['v'] = {0x000000,0x00CCCC,0xCCCC78,0x300000};
    ascii['w'] = {0x000000,0x00C6C6,0xD6D66C,0x6C0000};
    ascii['x'] = {0x000000,0x00C66C,0x38386C,0xC60000};
    ascii['y'] = {0x000000,0x006666,0x66663C,0x0C18F0};
    ascii['z'] = {0x000000,0x00FC8C,0x1860C4,0xFC0000};
    ascii['{'] = {0x001C30,0x3060C0,0x603030,0x1C0000};
    ascii['|'] = {0x001818,0x181800,0x181818,0x180000};
    ascii['}'] = {0x00E030,0x30180C,0x183030,0xE00000};
    ascii['~'] = {0x0073DA,0xCE0000,0x000000,0x000000};
    ascii[127] = {0x000000,0x10386C,0xC6C6FE,0x000000}; // Delete
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
    helpers::mapScoped<Uniforms>(uniforms, [this](auto *constants)
    {   // Globals
        constants->stringCount = MAGMA_COUNT(strings);
    });
    helpers::mapScoped<String>(stringBuffer, [this](auto *data)
    {   // Copy string descriptions
        for (auto& str : strings)
            *data++ = str;
    });
    helpers::mapScoped<Glyph>(glyphBuffer, [this](auto *data)
    {   // Copy glyph data
        core::copyMemory(data, chars.data(), sizeof(Glyph) * chars.size());
    });
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
        chars.push_back(ascii[sz[i]]);
    offset += length;
}
} // namespace aux
} // namespace magma
