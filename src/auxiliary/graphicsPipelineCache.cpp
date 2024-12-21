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
#include "graphicsPipelineCache.h"
#include "../objects/graphicsPipeline.h"
#include "../objects/pipelineLayout.h"
#include "../objects/pipelineLibrary.h"
#include "../objects/renderPass.h"
#include "../states/vertexInputState.h"
#include "../states/inputAssemblyState.h"
#include "../states/tesselationState.h"
#include "../states/viewportState.h"
#include "../states/rasterizationState.h"
#include "../states/multisampleState.h"
#include "../states/depthStencilState.h"
#include "../states/colorBlendState.h"

namespace magma::aux
{
GraphicsPipelineCache::GraphicsPipelineCache(std::shared_ptr<Device> device_,
    bool useDerivativePipelines,
    bool disablePipelineOptimization,
#ifdef VK_KHR_pipeline_library
    const std::unique_ptr<PipelineLibrary>& pipelineLibrary_ /* nullptr */,
#endif
    std::shared_ptr<IAllocator> allocator_ /* nullptr */):
    device(std::move(device_)),
    pipelineCache(std::make_unique<PipelineCache>(device, allocator)),
#ifdef VK_KHR_pipeline_library
    pipelineLibrary(pipelineLibrary_),
#endif
    allocator(std::move(allocator_)),
    psoFlags(0)
{
    if (useDerivativePipelines)
    {   // Specify that the pipeline to be created is allowed to be
        // the parent of a pipeline that will be created.
        psoFlags |= VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT;
    }
    if (disablePipelineOptimization)
    {   // Pipeline will not be optimized. Using this flag may reduce
        // the time taken to create the pipeline.
        psoFlags |= VK_PIPELINE_CREATE_DISABLE_OPTIMIZATION_BIT;
    }
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
    variant_ptr<PipelineLayout> layout,
    lent_ptr<const RenderPass> renderPass,
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
        layout.get(),
        renderPass.get(),
        subpass,
        extendedInfo);
    // Lookup for existing pipeline
    auto it = pipelines.find(hashes.first);
    if (it != pipelines.end())
        return it->second;
    std::shared_ptr<GraphicsPipeline> basePipeline;
    hash_t shaderHash = 0ull;
    if (psoFlags & VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT)
    {   // Compute hash of shader stages
        for (auto const& stage: shaderStages)
            shaderHash = core::hashCombine(shaderHash, stage.getHash());
        // Try to lookup base pipeline by hash of render states
        it = basePipelinesByRenderStates.find(hashes.second);
        if (it != basePipelinesByRenderStates.end())
            basePipeline = it->second;
        else
        {   // Try to lookup base pipeline by hash of shader stages
            it = basePipelinesByShaderStages.find(shaderHash);
            if (it != basePipelinesByShaderStages.end())
                basePipeline = it->second;
        }
        if (basePipeline)
        {   // A pipeline derivative is a child pipeline created from
            // a parent pipeline, where the child and parent are expected
            // to have much commonality. The goal of derivative pipelines
            // is that they be cheaper to create using the parent as a
            // starting point, and that it be more efficient (on either host
            // or device) to switch/bind between children of the same parent.
            flags |= VK_PIPELINE_CREATE_DERIVATIVE_BIT;
        }
    }
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
    #ifdef VK_KHR_pipeline_library
        pipelineLibrary,
    #endif
        std::move(basePipeline),
        flags | psoFlags);
    MAGMA_ASSERT(pipeline->getHash() == hashes.first);
    MAGMA_ASSERT(pipeline->getRenderStateHash() == hashes.second);
    pipelines.emplace(pipeline->getHash(), pipeline);
    if (psoFlags & VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT)
    {
        basePipelinesByRenderStates.emplace(pipeline->getRenderStateHash(), pipeline);
        basePipelinesByShaderStages.emplace(shaderHash, pipeline);
    }
    return pipeline;
}
} // namespace magma::aux
