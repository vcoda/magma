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
#include "immediateRender.h"
#include "graphicsPipelineCache.h"
#include "../objects/physicalDevice.h"
#include "../objects/device.h"
#include "../objects/deviceMemory.h"
#include "../objects/commandBuffer.h"
#include "../objects/graphicsPipeline.h"
#include "../objects/renderPass.h"
#include "../objects/shaderModule.h"
#include "../states/vertexInputStructure.h"
#include "../states/inputAssemblyState.h"
#include "../states/viewportState.h"
#include "../states/tesselationState.h"
#include "../misc/deviceFeatures.h"
#include "../misc/featureQuery.h"
#include "../misc/pushConstantRange.h"

namespace magma::aux
{
ImmediateRender::ImmediateRender(const uint32_t maxVertexCount, std::shared_ptr<RenderPass> renderPass,
    std::shared_ptr<PipelineLayout> layout_, std::shared_ptr<PipelineCache> pipelineCache,
    std::shared_ptr<Allocator> allocator /* nullptr */):
    maxVertexCount(maxVertexCount),
    wideLinesEnabled(renderPass->getDevice()->getEnabledFeatures().wideLines),
    stippledLinesEnabled(renderPass->getDevice()->checkFeatures()->stippledLinesEnabled()),
    device(renderPass->getDevice()),
    renderPass(std::move(renderPass)),
    layout(std::move(layout_)),
    pipelineCache(std::make_shared<GraphicsPipelineCache>(device, std::move(pipelineCache),
    #ifdef VK_KHR_pipeline_library
        nullptr,
    #endif
        true, false, MAGMA_HOST_ALLOCATOR(allocator))),
    rasterizationState(renderstate::fillCullBackCcw),
    multisampleState(renderstate::dontMultisample),
    depthStencilState(renderstate::depthAlwaysDontWrite),
    colorBlendState(renderstate::dontBlendRgba)
{
    const VkDeviceSize vertexBufferSize = sizeof(Vertex) * maxVertexCount;
    const bool stagedPool = device->getFeatures()->supportsDeviceLocalHostVisibleMemory();
    vertexBuffer = std::make_shared<DynamicVertexBuffer>(device, vertexBufferSize, stagedPool, allocator);
    setIdentity();
    if (!layout)
    {   // If layout hasn't been specified, create a default one
        struct Transform
        {
            float m[4][4];
        };
        constexpr push::VertexConstantRange<Transform> pushConstantRange;
        layout = std::make_shared<PipelineLayout>(device, pushConstantRange, MAGMA_HOST_ALLOCATOR(allocator));
    }
constexpr
#include "spirv/output/immv"
constexpr
#include "spirv/output/immf"
    constexpr hash_t vsImmHash = core::hashArray(vsImm);
    constexpr hash_t fsImmHash = core::hashArray(fsImm);
    std::shared_ptr<ShaderModule> vertexShader = std::make_shared<ShaderModule>(device, vsImm, vsImmHash, MAGMA_HOST_ALLOCATOR(allocator), false);
    std::shared_ptr<ShaderModule> fragmentShader = std::make_shared<ShaderModule>(device, fsImm, fsImmHash, MAGMA_HOST_ALLOCATOR(allocator), false);
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
        current = (Vertex *)vertexBuffer->getMemory()->map();
        if (!current)
            return false;
        // Set attributes to initial state
        normal(0.f, 0.f, 0.f);
        color(1.f, 1.f, 1.f, 1.f);
        texCoord(0.f, 0.f);
        pointSize(1.f);
    }
    Primitive primitive;
    primitive.wideLineState = (rasterizationState.lineWidth > 1.f);
    primitive.stippledLineState = stippledLineState;
    primitive.pipeline = lookupPipeline(topology, primitive.wideLineState, primitive.stippledLineState);
    primitive.lineWidth = lineWidth;
    primitive.lineStippleFactor = lineStippleFactor;
    primitive.lineStipplePattern = lineStipplePattern;
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
    for (auto const& primitive: primitives)
    {
    #ifdef VK_EXT_debug_utils
        if (primitive.labelName)
            cmdBuffer->beginDebugLabel(primitive.labelName, primitive.labelColor);
    #endif // VK_EXT_debug_utils
        if (primitive.pipeline != prevPipeline)
        {
            cmdBuffer->bindPipeline(primitive.pipeline);
            prevPipeline = primitive.pipeline;
        }
        if (wideLinesEnabled && !primitive.wideLineState)
            cmdBuffer->setLineWidth(primitive.lineWidth);
    #ifdef VK_EXT_line_rasterization
        if (stippledLinesEnabled && !primitive.stippledLineState)
            cmdBuffer->setLineStipple(primitive.lineStippleFactor, primitive.lineStipplePattern);
    #endif // VK_EXT_line_rasterization
        if (layout)
            cmdBuffer->pushConstantBlock(*layout, VK_SHADER_STAGE_VERTEX_BIT, primitive.transform);
        cmdBuffer->draw(primitive.vertexCount, primitive.firstVertex);
    #ifdef VK_EXT_debug_utils
        if (primitive.labelName)
            cmdBuffer->endDebugLabel();
    #endif // VK_EXT_debug_utils
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

std::shared_ptr<GraphicsPipeline> ImmediateRender::lookupPipeline(VkPrimitiveTopology topology, bool wideLineState, bool stippledLineState)
{
    MAGMA_UNUSED(stippledLineState);
    static VertexInputStructure<Vertex> vertexInputState(0, {
        {0, &Vertex::position},
        {1, &Vertex::normalPSize},
        {2, &Vertex::color},
        {3, &Vertex::texCoord}});
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
    std::vector<VkDynamicState> dynamicStates = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
    if (wideLinesEnabled && !wideLineState)
        dynamicStates.push_back(VK_DYNAMIC_STATE_LINE_WIDTH);
#ifdef VK_EXT_line_rasterization
    if (stippledLinesEnabled && !stippledLineState)
        dynamicStates.push_back(VK_DYNAMIC_STATE_LINE_STIPPLE_EXT);
#endif
    // Create new or grab existing graphics pipeline
    return pipelineCache->lookupPipeline(shaderStages,
        vertexInputState, *inputAssemblyStates[topology],
        TesselationState(), ViewportState(),
        rasterizationState, multisampleState,
        depthStencilState, colorBlendState, dynamicStates,
        layout, renderPass, 0);
}
} // namespace magma::aux
