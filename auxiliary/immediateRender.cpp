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
#include "../objects/shaderModule.h"
#include "../misc/pushConstants.h"
#include "../utilities/hash.h"
#include "../utilities/hexColor.h"

namespace magma
{
namespace aux
{
ImmediateRender::ImmediateRender(uint32_t maxVertexCount,
    std::shared_ptr<Device> device,
    std::shared_ptr<PipelineCache> cache,
    std::shared_ptr<PipelineLayout> layout,
    std::shared_ptr<RenderPass> renderPass,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    maxVertexCount(maxVertexCount),
    device(std::move(device)),
    cache(std::move(cache)),
    layout(std::move(layout)),
    renderPass(std::move(renderPass)),
    allocator(std::move(allocator)),
    vertexBuffer(std::make_shared<VertexBuffer>(this->device, nullptr, sizeof(Vertex) * maxVertexCount, 0, this->allocator)),
    vertexInput(VertexInputBinding(0, sizeof(Vertex)), {
        VertexInputAttribute(0, 0, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Vertex, x)),
        VertexInputAttribute(0, 1, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Vertex, nx)),
        VertexInputAttribute(0, 2, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Vertex, r)),
        VertexInputAttribute(0, 3, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, u))
    }),
    vertexShader(VertexShaderStage(createShader(true), "main")),
    fragmentShader(FragmentShaderStage(createShader(false), "main")),
    renderStates{
        renderstates::fillCullBackCCW,
        renderstates::noMultisample,
        renderstates::depthAlwaysDontWrite,
        renderstates::dontBlendWriteRGBA
    }
{
    // Set attributes to initial state
    normal(0.f, 0.f, 0.f);
    color(1.f, 1.f, 1.f, 1.f); // White is default
    texCoord(0.f, 0.f);
    pointSize(1.f); // Initial point size
    // If layout not specified, create default one
    if (!this->layout)
    {
        const pushconstants::VertexConstantRange<Transform> pushConstantRange;
        this->layout = std::make_shared<PipelineLayout>(this->device, std::initializer_list<VkPushConstantRange>{pushConstantRange}, this->allocator);
    }
}

bool ImmediateRender::beginPrimitive(VkPrimitiveTopology topology,
    const char *labelName /* nullptr */,
    uint32_t labelColor /* 0xFFFFFFFF */)
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
    primitive.labelName = labelName;
    primitive.labelColor = labelColor;
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

bool ImmediateRender::commitPrimitives(std::shared_ptr<CommandBuffer>& cmdBuffer, bool clear /* true */) noexcept
{
    MAGMA_ASSERT(!insidePrimitive);
    if (insidePrimitive || primitives.empty())
        return false;
    cmdBuffer->bindVertexBuffer(0, vertexBuffer);
    std::shared_ptr<GraphicsPipeline> prevPipeline;
    for (const auto& primitive : primitives)
    {
#ifdef MAGMA_DEBUG
        if (primitive.labelName)
        {
            float color[4];
            utilities::hexColorToFloat4(primitive.labelColor, color);
            cmdBuffer->beginDebugLabel(primitive.labelName, color);
        }
#endif // MAGMA_DEBUG
        if (primitive.pipeline != prevPipeline)
        {
            cmdBuffer->bindPipeline(primitive.pipeline);
            prevPipeline = primitive.pipeline;
        }
        cmdBuffer->setLineWidth(primitive.lineWidth);
        if (layout)
            cmdBuffer->pushConstantBlock(layout, VK_SHADER_STAGE_VERTEX_BIT, primitive.transform);
        cmdBuffer->draw(primitive.vertexCount, primitive.firstVertex);
#ifdef MAGMA_DEBUG
        if (primitive.labelName)
            cmdBuffer->endDebugLabel();
#endif // MAGMA_DEBUG
    }
    if (clear)
        reset();
    return true;
}

bool ImmediateRender::reset() noexcept
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

std::shared_ptr<ShaderModule> ImmediateRender::createShader(bool vertexShader) const
{
    if (vertexShader)
    {
#include "spirv/immv"
        return std::make_shared<ShaderModule>(device, vsImm, sizeof(vsImm), allocator);
    }
    else
    {
#include "spirv/immf"
        return std::make_shared<ShaderModule>(device, fsImm, sizeof(fsImm), allocator);
    }
}

std::shared_ptr<GraphicsPipeline> ImmediateRender::createPipelineState(VkPrimitiveTopology topology)
{
    static const InputAssemblyState *inputAssemblyStates[] =
    {
        &renderstates::pointList,
        &renderstates::lineList,
        &renderstates::lineStrip,
        &renderstates::triangleList,
        &renderstates::triangleStrip,
        &renderstates::triangleFan,
        &renderstates::lineListWithAdjacency,
        &renderstates::lineStripWithAdjacency,
        &renderstates::triangleListWithAdjacency,
        &renderstates::triangleStripWithAdjacency,
        &renderstates::patchList
    };
    const size_t renderStatesHash = hash(inputAssemblyStates[topology]);
    // Try to find existing pipeline
    auto it = pipelines.find(renderStatesHash);
    if (it != pipelines.end())
        return it->second.pipeline;
    // Create new pipeline for unique render states
    std::shared_ptr<const GraphicsPipeline> basePipeline = findBasePipeline();
    std::shared_ptr<GraphicsPipeline> pipeline(std::make_shared<GraphicsPipeline>(device, cache,
        std::vector<PipelineShaderStage>{vertexShader, fragmentShader},
        vertexInput,
        *inputAssemblyStates[topology],
        renderStates.rasterization,
        renderStates.multisample,
        renderStates.depthStencil,
        renderStates.colorBlend,
        std::initializer_list<VkDynamicState>{
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR,
            VK_DYNAMIC_STATE_LINE_WIDTH
        },
        layout,
        renderPass, 0,
        basePipeline,
        VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT,
        allocator));
    pipelines[renderStatesHash] = {pipeline, std::make_shared<RenderStates>(renderStates)};
    return pipeline;
}

std::shared_ptr<const GraphicsPipeline> ImmediateRender::findBasePipeline() const noexcept
{
    for (const auto& it : pipelines)
    {
        if ((it.second.renderStates->rasterization == this->renderStates.rasterization) &&
            (it.second.renderStates->multisample == this->renderStates.multisample) &&
            (it.second.renderStates->depthStencil == this->renderStates.depthStencil) &&
            (it.second.renderStates->colorBlend == this->renderStates.colorBlend))
        {   // If render states are the same, child and parent are expected to have much commonality
            return it.second.pipeline;
        }
    }
    return nullptr;
}

size_t ImmediateRender::hash(const InputAssemblyState *inputAssembly) const noexcept
{
    size_t hash = 0;
    utilities::hashCombine(hash, inputAssembly->hash());
    utilities::hashCombine(hash, vertexShader.hash());
    utilities::hashCombine(hash, fragmentShader.hash());
    utilities::hashCombine(hash, renderStates.rasterization.hash());
    utilities::hashCombine(hash, renderStates.multisample.hash());
    utilities::hashCombine(hash, renderStates.depthStencil.hash());
    utilities::hashCombine(hash, renderStates.colorBlend.hash());
    return hash;
}
} // namespace aux
} // namespace magma
