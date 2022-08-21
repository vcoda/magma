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
#include "immediateRender.h"
#include "graphicsPipelineCache.h"
#include "../objects/deviceMemory.h"
#include "../objects/commandBuffer.h"
#include "../objects/graphicsPipeline.h"
#include "../objects/renderPass.h"
#include "../objects/shaderModule.h"
#include "../states/vertexInputStructure.h"
#include "../states/inputAssemblyState.h"
#include "../states/viewportState.h"
#include "../states/tesselationState.h"
#include "../misc/pushConstantRange.h"

namespace magma
{
namespace aux
{
ImmediateRender::ImmediateRender(const uint32_t maxVertexCount, std::shared_ptr<RenderPass> renderPass,
    std::shared_ptr<PipelineLayout> layout, std::shared_ptr<PipelineCache> pipelineCache,
    std::shared_ptr<Allocator> allocator /* nullptr */):
    maxVertexCount(maxVertexCount),
    device(renderPass->getDevice()),
    renderPass(std::move(renderPass)),
    layout(std::move(layout)),
    pipelineCache(std::make_shared<GraphicsPipelineCache>(device, std::move(pipelineCache), MAGMA_HOST_ALLOCATOR(allocator))),
    vertexBuffer(std::make_shared<DynamicVertexBuffer>(device, sizeof(Vertex) * maxVertexCount, false, allocator, nullptr, 0, 0.f, Resource::Sharing())),
    rasterizationState(renderstate::fillCullBackCCw),
    multisampleState(renderstate::dontMultisample),
    depthStencilState(renderstate::depthAlwaysDontWrite),
    colorBlendState(renderstate::dontBlendRgba) // Make copyable
{
    setIdentity();
    if (!this->layout)
    {   // If layout not specified, create default one
        struct Transform
        {
            float m[4][4];
        };
        constexpr pushconstant::VertexConstantRange<Transform> pushConstantRange;
        this->layout = std::make_shared<PipelineLayout>(device, pushConstantRange, MAGMA_HOST_ALLOCATOR(allocator));
    }
constexpr
#include "spirv/output/immv"
constexpr
#include "spirv/output/immf"
    constexpr hash_t vsImmHash = core::hashArray(vsImm);
    constexpr hash_t fsImmHash = core::hashArray(fsImm);
    std::shared_ptr<ShaderModule> vertexShader = std::make_shared<ShaderModule>(device, vsImm, vsImmHash, MAGMA_HOST_ALLOCATOR(allocator), 0, false);
    std::shared_ptr<ShaderModule> fragmentShader = std::make_shared<ShaderModule>(device, fsImm, fsImmHash, MAGMA_HOST_ALLOCATOR(allocator), 0, false);
    shaderStages.push_back(VertexShaderStage(vertexShader, "main"));
    shaderStages.push_back(FragmentShaderStage(fragmentShader, "main"));
}

bool ImmediateRender::beginPrimitive(VkPrimitiveTopology topology,
    const char *labelName /* nullptr */,
    uint32_t labelColor /* 0xFFFFFFFF */)
{
    MAGMA_ASSERT(!insidePrimitive);
    if (insidePrimitive)
        return false;
    if (!current)
    {
        current = vertexBuffer->getMemory()->map<Vertex>(0, VK_WHOLE_SIZE);
        if (!current)
            return false;
        // Set attributes to initial state
        normal(0.f, 0.f, 0.f);
        color(1.f, 1.f, 1.f, 1.f);
        texCoord(0.f, 0.f);
        pointSize(1.f);
    }
    Primitive primitive;
    primitive.pipeline = lookupPipeline(topology);
    primitive.lineWidth = lineWidth;
    memcpy(primitive.transform, transform, sizeof(transform));
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
    {   // Copy first to last
        const Vertex *first = current - primitives.back().vertexCount;
        *current++ = *first;
        ++primitives.back().vertexCount;
        ++vertexCount;
    }
    insidePrimitive = false;
    return true;
}

bool ImmediateRender::commitPrimitives(std::shared_ptr<CommandBuffer> cmdBuffer,
    bool freePrimitiveList /* true */) noexcept
{
    MAGMA_ASSERT(!insidePrimitive);
    if (insidePrimitive || primitives.empty())
        return false;
    cmdBuffer->bindVertexBuffer(0, vertexBuffer);
    std::shared_ptr<GraphicsPipeline> prevPipeline;
    for (const auto& primitive : primitives)
    {
#ifdef MAGMA_DEBUG_LABEL
        if (primitive.labelName)
            cmdBuffer->beginDebugLabel(primitive.labelName, primitive.labelColor);
#endif
        if (primitive.pipeline != prevPipeline)
        {
            cmdBuffer->bindPipeline(primitive.pipeline);
            prevPipeline = primitive.pipeline;
        }
        cmdBuffer->setLineWidth(primitive.lineWidth);
        if (layout)
            cmdBuffer->pushConstantBlock(layout, VK_SHADER_STAGE_VERTEX_BIT, primitive.transform);
        cmdBuffer->draw(primitive.vertexCount, primitive.firstVertex);
#ifdef MAGMA_DEBUG_LABEL
        if (primitive.labelName)
            cmdBuffer->endDebugLabel();
#endif
    }
    if (freePrimitiveList)
        reset();
    return true;
}

bool ImmediateRender::reset() noexcept
{
    MAGMA_ASSERT(!insidePrimitive);
    if (insidePrimitive)
        return false;
    if (current)
    {
        vertexBuffer->getMemory()->unmap();
        current = nullptr;
    }
    primitives.clear();
    vertexCount = 0;
    return true;
}

std::shared_ptr<GraphicsPipeline> ImmediateRender::lookupPipeline(VkPrimitiveTopology topology)
{
    static VertexInputStructure<Vertex> vertexInputState(0, {
        {0, &Vertex::position},
        {1, &Vertex::normalPSize},
        {2, &Vertex::color},
        {3, &Vertex::texcoord}});
    constexpr const InputAssemblyState *inputAssemblyStates[] = {
        &renderstate::pointList,
        &renderstate::lineList,
        &renderstate::lineStrip,
        &renderstate::triangleList,
        &renderstate::triangleStrip,
        &renderstate::triangleFan,
        &renderstate::lineListWithAdjacency,
        &renderstate::lineStripWithAdjacency,
        &renderstate::triangleListWithAdjacency,
        &renderstate::triangleStripWithAdjacency,
        &renderstate::patchList};
    // Create new or grab existing graphics pipeline
    return pipelineCache->lookupPipeline(shaderStages,
        vertexInputState, *inputAssemblyStates[topology],
        TesselationState(), ViewportState(),
        rasterizationState, multisampleState, depthStencilState, colorBlendState,
        {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR, VK_DYNAMIC_STATE_LINE_WIDTH},
        layout, renderPass, 0);
}
} // namespace aux
} // namespace magma
