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
#include "pch.h"
#pragma hdrstop
#include "immediateRender.h"
#include "../objects/deviceMemory.h"
#include "../objects/commandBuffer.h"
#include "../objects/pipeline.h"
#include "../objects/shaderModule.h"
#include "../states/vertexInputStructure.h"
#include "../states/inputAssemblyState.h"
#include "../states/viewportState.h"
#include "../states/tesselationState.h"
#include "../misc/pushConstants.h"

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
    vertexBuffer(std::make_shared<VertexBuffer>(this->device, nullptr, sizeof(Vertex) * maxVertexCount, 0, Resource::Sharing(), this->allocator)),
    vertexShader(VertexShaderStage(createShader(true), "main")),
    fragmentShader(FragmentShaderStage(createShader(false), "main")),
    renderStates{
        renderstates::fillCullBackCCW,
        renderstates::noMultisample,
        renderstates::depthAlwaysDontWrite,
        ManagedColorBlendState(renderstates::dontBlendWriteRgba) // Make copyable
    }
{
    // Set attributes to initial state
    normal(0.f, 0.f, 0.f);
    color(1.f, 1.f, 1.f, 1.f); // White is default
    texcoord(0.f, 0.f);
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
    if (!pvertex)
    {
        pvertex = reinterpret_cast<Vertex *>(vertexBuffer->getMemory()->map(0, VK_WHOLE_SIZE));
        if (!pvertex)
            return false;
    }
    Primitive primitive;
    primitive.pipeline = lookupPipeline(topology);
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
        const Vertex *first = pvertex - primitives.back().vertexCount;
        *pvertex++ = *first;
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
            cmdBuffer->beginDebugLabel(primitive.labelName, primitive.labelColor);
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
    if (pvertex)
    {
        vertexBuffer->getMemory()->unmap();
        pvertex = nullptr;
    }
    primitives.clear();
    vertexCount = 0;
    return true;
}

std::shared_ptr<ShaderModule> ImmediateRender::createShader(bool vertexShader) const
{
    constexpr VkShaderModuleCreateFlags flags = 0;
#include "spirv/output/immv"
    if (vertexShader)
        return std::make_shared<ShaderModule>(device, vsImm, flags, nullptr, allocator);
#include "spirv/output/immf"
    return std::make_shared<ShaderModule>(device, fsImm, flags, nullptr, allocator);
}

std::shared_ptr<GraphicsPipeline> ImmediateRender::lookupPipeline(VkPrimitiveTopology topology)
{
    static VertexInputStructure<Vertex> vertexInputState(0, {
        {0, &Vertex::position},
        {1, &Vertex::normalPSize},
        {2, &Vertex::color},
        {3, &Vertex::texcoord}});
    constexpr const InputAssemblyState *inputAssemblyStates[] =
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
    std::shared_ptr<GraphicsPipeline> pipeline;
    std::shared_ptr<GraphicsPipeline> basePipeline = lookupBasePipeline();
    const std::size_t hash = computePipelineHash(vertexInputState, *inputAssemblyStates[topology], basePipeline);
    // Fast lookup for existing pipeline
    auto it = pipelines.find(hash);
    if (it != pipelines.end())
        pipeline = it->second.pipeline;
    else
    {   // Create new pipeline for unique render states
        pipeline = std::make_shared<GraphicsPipeline>(device, cache,
            std::vector<PipelineShaderStage>{vertexShader, fragmentShader},
            vertexInputState,
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
            allocator);
        MAGMA_ASSERT(pipeline->getHash() == hash);
        pipelines[pipeline->getHash()] = {pipeline, std::make_shared<RenderStates>(renderStates)};
    }
    return pipeline;
}

std::shared_ptr<GraphicsPipeline> ImmediateRender::lookupBasePipeline() const noexcept
{
    for (const auto& it : pipelines)
    {   // If render states are the same, child and parent are expected to have much commonality
        if ((it.second.renderStates->rasterization == this->renderStates.rasterization) &&
            (it.second.renderStates->multisample == this->renderStates.multisample) &&
            (it.second.renderStates->depthStencil == this->renderStates.depthStencil) &&
            (it.second.renderStates->colorBlend == this->renderStates.colorBlend))
        {
            return it.second.pipeline;
        }
    }
    return nullptr;
}

std::size_t ImmediateRender::computePipelineHash(const VertexInputState& vertexInputState,
    const InputAssemblyState& inputAssemblyState,
    std::shared_ptr<GraphicsPipeline> basePipeline) const noexcept
{   // Keep in sync with GraphicsPipeline!
    VkGraphicsPipelineCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT;
    if (basePipeline)
        info.flags |= VK_PIPELINE_CREATE_DERIVATIVE_BIT;
    info.subpass = 0;
    std::size_t hash = internal::hashArgs(
        info.sType,
        info.flags,
        info.subpass);
    internal::hashCombine(hash, vertexShader.hash());
    internal::hashCombine(hash, fragmentShader.hash());
    internal::hashCombine(hash, internal::combineHashList(
        {
            vertexInputState.hash(),
            inputAssemblyState.hash(),
            TesselationState().hash(),
            ViewportState().hash(),
            renderStates.rasterization.hash(),
            renderStates.multisample.hash(),
            renderStates.depthStencil.hash(),
            renderStates.colorBlend.hash(),
        }));
    internal::hashCombine(hash, internal::hashArgs(
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
        VK_DYNAMIC_STATE_LINE_WIDTH));
    if (layout)
        internal::hashCombine(hash, layout->getHash());
    return hash;
}
} // namespace aux
} // namespace magma
