/*
Magma - C++1x interface over Khronos Vulkan API.
Copyright (C) 2018 Victor Coda.

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
#include "immediateRender.h"
#include "../objects/deviceMemory.h"
#include "../objects/commandBuffer.h"
#include "../objects/pipeline.h"
#include "../states/vertexInputState.h"
#include "../states/inputAssemblyState.h"
#include "../misc/pushConstants.h"
#include "../allocator/allocator.h"

namespace magma
{
namespace helpers
{
ImmediateRender::ImmediateRender(uint32_t maxVertexCount,
    std::shared_ptr<Device> device,
    std::shared_ptr<PipelineCache> cache,
    std::shared_ptr<PipelineLayout> layout,
    std::shared_ptr<RenderPass> renderPass,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    maxVertexCount(maxVertexCount),
    device(device),
    layout(layout),
    cache(cache),
    renderPass(renderPass),
    vertexBuffer(std::make_shared<VertexBuffer>(device, nullptr, sizeof(Vertex) * maxVertexCount, 0, allocator)),
    allocator(std::move(allocator)),
    vertexBuffer(std::make_shared<VertexBuffer>(this->device, nullptr, sizeof(Vertex) * maxVertexCount, 0, this->allocator)),
    vertexShader(VertexShaderStage(createVertexShader(), "main")),
    fragmentShader(FragmentShaderStage(createFragmentShader(), "main")),
    rasterizationState(states::fillCullBackCCW),
    multisampleState(states::dontMultisample),
    depthStencilState(states::depthAlwaysDontWrite),
    colorBlendState(states::dontBlendWriteRGBA)
{
    // Set attributes to initial state
    normal(0.f, 0.f, 0.f);
    color(1.f, 1.f, 1.f, 1.f); // White is default
    texCoord(0.f, 0.f);
    pointSize(1.f); // Initial point size
    // If layout not specified, create default one
    if (!layout)
    {
        const pushconstants::VertexConstantRange<Transform> pushConstantRange;
        this->layout = std::make_shared<PipelineLayout>(this->device, std::initializer_list<VkPushConstantRange>{pushConstantRange}, this->allocator);
    }
}

bool ImmediateRender::beginPrimitive(VkPrimitiveTopology topology)
{
    MAGMA_ASSERT(!insidePrimitive);
    if (insidePrimitive)
        return false;
    if (!vert)
    {
        vert = reinterpret_cast<Vertex *>(vertexBuffer->getMemory()->map(0, VK_WHOLE_SIZE));
        if (!vert)
            return false;
    }
    Primitive primitive;
    primitive.pipeline = createPipelineState(topology);
    primitive.lineWidth = lineWidth;
    primitive.transform = transform;
    primitive.vertexCount = 0;
    primitive.firstVertex = vertexCount;
    primitives.push_back(primitive);
    insidePrimitive = true;
    return true;
}

bool ImmediateRender::endPrimitive(bool loop /* false */) noexcept
{
    MAGMA_ASSERT(insidePrimitive);
    if (!insidePrimitive)
        return false;
    if (loop && (primitives.back().vertexCount > 0))
    {
        const Vertex *first = vert - primitives.back().vertexCount;
        *vert++ = *first;
        ++primitives.back().vertexCount;
        ++vertexCount;
    }
    insidePrimitive = false;
    return true;
}

bool ImmediateRender::commitPrimitives(std::shared_ptr<CommandBuffer>& cmdBuffer, bool clear /* true */)
{
    MAGMA_ASSERT(!insidePrimitive);
    if (insidePrimitive || primitives.empty())
        return false;
    cmdBuffer->bindVertexBuffer(0, vertexBuffer);
    std::shared_ptr<GraphicsPipeline> prevPipeline;
    for (const auto& primitive : primitives)
    {
        if (primitive.pipeline != prevPipeline)
        {
            cmdBuffer->bindPipeline(primitive.pipeline);
            prevPipeline = primitive.pipeline;
        }
        cmdBuffer->setLineWidth(primitive.lineWidth);
        if (layout)
            cmdBuffer->pushConstantBlock(layout, VK_SHADER_STAGE_VERTEX_BIT, primitive.transform);
        cmdBuffer->draw(primitive.vertexCount, primitive.firstVertex);
    }
    if (clear)
        reset();
    return true;
}

bool ImmediateRender::reset()
{
    MAGMA_ASSERT(!insidePrimitive);
    if (insidePrimitive)
        return false;
    if (vert)
    {
        vertexBuffer->getMemory()->unmap();
        vert = nullptr;
    }
    primitives.clear();
    vertexCount = 0;
    return true;
}

std::shared_ptr<ShaderModule> ImmediateRender::createVertexShader()
{   /*
    #version 450

    layout(location = 0) in vec4 position;
    layout(location = 1) in vec4 normalPointSize;
    layout(location = 2) in vec4 color;
    layout(location = 3) in vec2 texCoord;

    layout(location = 0) out vec3 oNormal;
    layout(location = 1) out vec4 oColor;
    layout(location = 2) out vec2 oTexCoord;

    layout(push_constant) uniform PushConstants
    {
        mat4 transform;
    };

    out gl_PerVertex
    {
        vec4 gl_Position;
        float gl_PointSize;
    };

    void main()
    {
        oNormal = normalPointSize.xyz;
        oColor = color;
        oTexCoord = texCoord;
        gl_Position = transform * position;
        gl_PointSize = normalPointSize.w;
    } */
    const uint32_t vertexShaderBytecode[] =
    {
        0x07230203,0x00010000,0x00080002,0x00000031,0x00000000,0x00020011,0x00000001,0x0006000b,
        0x00000001,0x4c534c47,0x6474732e,0x3035342e,0x00000000,0x0003000e,0x00000000,0x00000001,
        0x000d000f,0x00000000,0x00000004,0x6e69616d,0x00000000,0x00000009,0x0000000c,0x00000010,
        0x00000011,0x00000015,0x00000017,0x0000001b,0x00000025,0x00030003,0x00000002,0x000001c2,
        0x00040005,0x00000004,0x6e69616d,0x00000000,0x00040005,0x00000009,0x726f4e6f,0x006c616d,
        0x00060005,0x0000000c,0x6d726f6e,0x6f506c61,0x53746e69,0x00657a69,0x00040005,0x00000010,
        0x6c6f436f,0x0000726f,0x00040005,0x00000011,0x6f6c6f63,0x00000072,0x00050005,0x00000015,
        0x7865546f,0x726f6f43,0x00000064,0x00050005,0x00000017,0x43786574,0x64726f6f,0x00000000,
        0x00060005,0x00000019,0x505f6c67,0x65567265,0x78657472,0x00000000,0x00060006,0x00000019,
        0x00000000,0x505f6c67,0x7469736f,0x006e6f69,0x00070006,0x00000019,0x00000001,0x505f6c67,
        0x746e696f,0x657a6953,0x00000000,0x00030005,0x0000001b,0x00000000,0x00060005,0x0000001f,
        0x68737550,0x736e6f43,0x746e6174,0x00000073,0x00060006,0x0000001f,0x00000000,0x6e617274,
        0x726f6673,0x0000006d,0x00030005,0x00000021,0x00000000,0x00050005,0x00000025,0x69736f70,
        0x6e6f6974,0x00000000,0x00040047,0x00000009,0x0000001e,0x00000000,0x00040047,0x0000000c,
        0x0000001e,0x00000001,0x00040047,0x00000010,0x0000001e,0x00000001,0x00040047,0x00000011,
        0x0000001e,0x00000002,0x00040047,0x00000015,0x0000001e,0x00000002,0x00040047,0x00000017,
        0x0000001e,0x00000003,0x00050048,0x00000019,0x00000000,0x0000000b,0x00000000,0x00050048,
        0x00000019,0x00000001,0x0000000b,0x00000001,0x00030047,0x00000019,0x00000002,0x00040048,
        0x0000001f,0x00000000,0x00000005,0x00050048,0x0000001f,0x00000000,0x00000023,0x00000000,
        0x00050048,0x0000001f,0x00000000,0x00000007,0x00000010,0x00030047,0x0000001f,0x00000002,
        0x00040047,0x00000025,0x0000001e,0x00000000,0x00020013,0x00000002,0x00030021,0x00000003,
        0x00000002,0x00030016,0x00000006,0x00000020,0x00040017,0x00000007,0x00000006,0x00000003,
        0x00040020,0x00000008,0x00000003,0x00000007,0x0004003b,0x00000008,0x00000009,0x00000003,
        0x00040017,0x0000000a,0x00000006,0x00000004,0x00040020,0x0000000b,0x00000001,0x0000000a,
        0x0004003b,0x0000000b,0x0000000c,0x00000001,0x00040020,0x0000000f,0x00000003,0x0000000a,
        0x0004003b,0x0000000f,0x00000010,0x00000003,0x0004003b,0x0000000b,0x00000011,0x00000001,
        0x00040017,0x00000013,0x00000006,0x00000002,0x00040020,0x00000014,0x00000003,0x00000013,
        0x0004003b,0x00000014,0x00000015,0x00000003,0x00040020,0x00000016,0x00000001,0x00000013,
        0x0004003b,0x00000016,0x00000017,0x00000001,0x0004001e,0x00000019,0x0000000a,0x00000006,
        0x00040020,0x0000001a,0x00000003,0x00000019,0x0004003b,0x0000001a,0x0000001b,0x00000003,
        0x00040015,0x0000001c,0x00000020,0x00000001,0x0004002b,0x0000001c,0x0000001d,0x00000000,
        0x00040018,0x0000001e,0x0000000a,0x00000004,0x0003001e,0x0000001f,0x0000001e,0x00040020,
        0x00000020,0x00000009,0x0000001f,0x0004003b,0x00000020,0x00000021,0x00000009,0x00040020,
        0x00000022,0x00000009,0x0000001e,0x0004003b,0x0000000b,0x00000025,0x00000001,0x0004002b,
        0x0000001c,0x00000029,0x00000001,0x00040015,0x0000002a,0x00000020,0x00000000,0x0004002b,
        0x0000002a,0x0000002b,0x00000003,0x00040020,0x0000002c,0x00000001,0x00000006,0x00040020,
        0x0000002f,0x00000003,0x00000006,0x00050036,0x00000002,0x00000004,0x00000000,0x00000003,
        0x000200f8,0x00000005,0x0004003d,0x0000000a,0x0000000d,0x0000000c,0x0008004f,0x00000007,
        0x0000000e,0x0000000d,0x0000000d,0x00000000,0x00000001,0x00000002,0x0003003e,0x00000009,
        0x0000000e,0x0004003d,0x0000000a,0x00000012,0x00000011,0x0003003e,0x00000010,0x00000012,
        0x0004003d,0x00000013,0x00000018,0x00000017,0x0003003e,0x00000015,0x00000018,0x00050041,
        0x00000022,0x00000023,0x00000021,0x0000001d,0x0004003d,0x0000001e,0x00000024,0x00000023,
        0x0004003d,0x0000000a,0x00000026,0x00000025,0x00050091,0x0000000a,0x00000027,0x00000024,
        0x00000026,0x00050041,0x0000000f,0x00000028,0x0000001b,0x0000001d,0x0003003e,0x00000028,
        0x00000027,0x00050041,0x0000002c,0x0000002d,0x0000000c,0x0000002b,0x0004003d,0x00000006,
        0x0000002e,0x0000002d,0x00050041,0x0000002f,0x00000030,0x0000001b,0x00000029,0x0003003e,
        0x00000030,0x0000002e,0x000100fd,0x00010038
    };
    return std::make_shared<ShaderModule>(device, vertexShaderBytecode, sizeof(vertexShaderBytecode), allocator);
}

std::shared_ptr<ShaderModule> ImmediateRender::createFragmentShader()
{   /*
    #version 450

    layout(location = 0) in vec3 normal;
    layout(location = 1) in vec4 color;
    layout(location = 2) in vec2 texCoord;

    layout(location = 0) out vec4 oColor;

    void main() 
    {
        oColor = color;
    } */
    const uint32_t fragmentShaderBytecode[] =
    {
        0x07230203,0x00010000,0x00080002,0x00000013,0x00000000,0x00020011,0x00000001,0x0006000b,
        0x00000001,0x4c534c47,0x6474732e,0x3035342e,0x00000000,0x0003000e,0x00000000,0x00000001,
        0x0009000f,0x00000004,0x00000004,0x6e69616d,0x00000000,0x00000009,0x0000000b,0x0000000f,
        0x00000012,0x00030010,0x00000004,0x00000007,0x00030003,0x00000002,0x000001c2,0x00040005,
        0x00000004,0x6e69616d,0x00000000,0x00040005,0x00000009,0x6c6f436f,0x0000726f,0x00040005,
        0x0000000b,0x6f6c6f63,0x00000072,0x00040005,0x0000000f,0x6d726f6e,0x00006c61,0x00050005,
        0x00000012,0x43786574,0x64726f6f,0x00000000,0x00040047,0x00000009,0x0000001e,0x00000000,
        0x00040047,0x0000000b,0x0000001e,0x00000001,0x00040047,0x0000000f,0x0000001e,0x00000000,
        0x00040047,0x00000012,0x0000001e,0x00000002,0x00020013,0x00000002,0x00030021,0x00000003,
        0x00000002,0x00030016,0x00000006,0x00000020,0x00040017,0x00000007,0x00000006,0x00000004,
        0x00040020,0x00000008,0x00000003,0x00000007,0x0004003b,0x00000008,0x00000009,0x00000003,
        0x00040020,0x0000000a,0x00000001,0x00000007,0x0004003b,0x0000000a,0x0000000b,0x00000001,
        0x00040017,0x0000000d,0x00000006,0x00000003,0x00040020,0x0000000e,0x00000001,0x0000000d,
        0x0004003b,0x0000000e,0x0000000f,0x00000001,0x00040017,0x00000010,0x00000006,0x00000002,
        0x00040020,0x00000011,0x00000001,0x00000010,0x0004003b,0x00000011,0x00000012,0x00000001,
        0x00050036,0x00000002,0x00000004,0x00000000,0x00000003,0x000200f8,0x00000005,0x0004003d,
        0x00000007,0x0000000c,0x0000000b,0x0003003e,0x00000009,0x0000000c,0x000100fd,0x00010038
    };
    return std::make_shared<ShaderModule>(device, fragmentShaderBytecode, sizeof(fragmentShaderBytecode), allocator);
}

std::shared_ptr<GraphicsPipeline> ImmediateRender::createPipelineState(VkPrimitiveTopology topology)
{
    const VertexInputState vertexInput(VertexInputBinding(0, sizeof(Vertex)),
    {
        VertexInputAttribute(0, 0, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Vertex, x)),
        VertexInputAttribute(0, 1, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Vertex, nx)),
        VertexInputAttribute(0, 2, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Vertex, r)),
        VertexInputAttribute(0, 3, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, u))
    });
    const InputAssemblyState *inputAssemblyStates[] =
    {
        &states::pointList,
        &states::lineList,
        &states::lineStrip,
        &states::triangleList,
        &states::triangleStrip,
        &states::triangleFan,
        &states::lineListWithAdjacency,
        &states::lineStripWithAdjacency,
        &states::triangleListWithAdjacency,
        &states::triangleStripWithAdjacency,
        &states::patchList
    };
    std::shared_ptr<GraphicsPipeline> pipeline(std::make_shared<GraphicsPipeline>(device, cache,
        std::vector<ShaderStage>{vertexShader, fragmentShader}, 
        vertexInput, 
        *inputAssemblyStates[topology],
        rasterizationState, 
        multisampleState, 
        depthStencilState, 
        colorBlendState,
        std::initializer_list<VkDynamicState>{
            VK_DYNAMIC_STATE_VIEWPORT, 
            VK_DYNAMIC_STATE_SCISSOR, 
            VK_DYNAMIC_STATE_LINE_WIDTH
        },
        layout,
        renderPass,
        0, 0,
        allocator));
    pipelines.insert(pipeline); // Hold unique pipelines, as they should exist during command buffer submission
    return pipeline;
}
} // namespace helpers
} // namespace magma
