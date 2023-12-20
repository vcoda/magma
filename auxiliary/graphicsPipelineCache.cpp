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
#include "graphicsPipelineCache.h"
#include "../objects/graphicsPipeline.h"
#include "../objects/pipelineLayout.h"
#include "../objects/pipelineCache.h"
#include "../objects/renderPass.h"
#include "../states/vertexInputState.h"
#include "../states/inputAssemblyState.h"
#include "../states/tesselationState.h"
#include "../states/viewportState.h"
#include "../states/rasterizationState.h"
#include "../states/multisampleState.h"
#include "../states/depthStencilState.h"
#include "../states/colorBlendState.h"

namespace magma
{
namespace aux
{
GraphicsPipelineCache::GraphicsPipelineCache(std::shared_ptr<Device> device_,
    std::shared_ptr<PipelineCache> pipelineCache_,
    std::shared_ptr<IAllocator> allocator_ /* nullptr */):
    device(std::move(device_)),
    pipelineCache(std::move(pipelineCache_)),
    allocator(std::move(allocator_))
{
    if (!pipelineCache)
        pipelineCache = std::make_shared<PipelineCache>(device, allocator);
}

std::shared_ptr<GraphicsPipeline> GraphicsPipelineCache::lookupPipeline(
    const std::vector<PipelineShaderStage>& shaderStages,
    const VertexInputState& vertexInputState,
    const InputAssemblyState& inputAssemblyState,
    const TesselationState& tesselationState,
    const ViewportState& viewportState,
    const RasterizationState& rasterizationState,
    const MultisampleState& multisampleState,
    const DepthStencilState& depthStencilState,
    const ColorBlendState& colorBlendState,
    const std::vector<VkDynamicState>& dynamicStates,
    std::shared_ptr<PipelineLayout> layout,
    std::shared_ptr<RenderPass> renderPass,
    uint32_t subpass /* 0 */,
    VkPipelineCreateFlags flags /* 0 */,
    const StructureChain& extendedInfo /* default */)
{   // Compute hash of graphics pipeline object
    std::pair<hash_t, hash_t> hashes = psoHash(
        flags,
        shaderStages,
        vertexInputState,
        inputAssemblyState,
        tesselationState,
        viewportState,
        rasterizationState,
        multisampleState,
        depthStencilState,
        colorBlendState,
        dynamicStates,
        std::move(layout),
        std::move(renderPass),
        subpass,
        extendedInfo);
    // Lookup for existing pipeline
    auto it = pipelines.find(hashes.first);
    if (it != pipelines.end())
        return it->second;
    // Try to lookup base pipeline by render state hash to speed up pipeline construction
    std::shared_ptr<GraphicsPipeline> basePipeline;
    it = basePipelines.find(hashes.second);
    if (it != basePipelines.end())
        basePipeline = it->second;
    if (basePipeline)
    {   // A pipeline derivative is a child pipeline created from a parent pipeline,
        // where the child and parent are expected to have much commonality.
        flags |= VK_PIPELINE_CREATE_DERIVATIVE_BIT;
    }
    // Specify that the pipeline to be created is allowed to be 
    // the parent of a pipeline that will be created
    flags |= VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT;
    // Create new pipeline using cache (and base pipeline if exists)
    std::shared_ptr<GraphicsPipeline> pipeline = std::make_shared<GraphicsPipeline>(device,
        shaderStages, 
        vertexInputState, 
        inputAssemblyState, 
        tesselationState, 
        viewportState,
        rasterizationState, 
        multisampleState, 
        depthStencilState, 
        colorBlendState, 
        dynamicStates,
        std::move(layout), 
        std::move(renderPass), 
        subpass,
        allocator, 
        pipelineCache, 
        std::move(basePipeline), 
        flags);
    MAGMA_ASSERT(hashes.first == pipeline->getHash());
    MAGMA_ASSERT(hashes.second == pipeline->getRenderStateHash());
    pipelines.emplace(pipeline->getHash(), pipeline);
    basePipelines.emplace(pipeline->getRenderStateHash(), pipeline);
    return pipeline;
}
} // namespace aux
} // namespace magma
