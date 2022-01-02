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
#include "graphicsPipelines.h"
#include "graphicsPipeline.h"
#include "pipelineLayout.h"
#include "pipelineCache.h"
#include "device.h"
#include "renderPass.h"
#include "../shaders/pipelineShaderStage.h"
#include "../states/vertexInputState.h"
#include "../states/inputAssemblyState.h"
#include "../states/tesselationState.h"
#include "../states/viewportState.h"
#include "../states/rasterizationState.h"
#include "../states/multisampleState.h"
#include "../states/depthStencilState.h"
#include "../states/colorBlendState.h"
#include "../allocator/allocator.h"
#include "../exceptions/errorResult.h"

namespace magma
{
GraphicsPipelines::GraphicsPipelines(std::size_t capacity /* 256 */)
{
    pipelineInfos.reserve(capacity);
    graphicsPipelines.reserve(capacity);
}

uint32_t GraphicsPipelines::newPipeline(const std::vector<PipelineShaderStage>& shaderStages,
    const VertexInputState& vertexInputState,
    const InputAssemblyState& inputAssemblyState,
    const TesselationState& tesselationState,
    const ViewportState& viewportState,
    const RasterizationState& rasterizationState,
    const MultisampleState& multisampleState,
    const DepthStencilState& depthStencilState,
    const ColorBlendState& colorBlendState,
    const std::vector<VkDynamicState>& dynamicRenderStates,
    std::shared_ptr<PipelineLayout> layout,
    std::shared_ptr<RenderPass> renderPass,
    uint32_t subpass,
    std::shared_ptr<GraphicsPipeline> basePipeline /* nullptr */,
    VkPipelineCreateFlags flags /* 0 */)
{
    stages.push_back(shaderStages);
    vertexInputStates.push_back(vertexInputState);
    inputAssemblyStates.push_back(inputAssemblyState);
    tesselationStates.push_back(tesselationState);
    viewportStates.push_back(viewportState);
    rasterizationStates.push_back(rasterizationState);
    multisampleStates.push_back(multisampleState);
    depthStencilStates.push_back(depthStencilState);
    colorBlendStates.push_back(colorBlendState);
    dynamicStates.push_back(dynamicRenderStates);
    layouts.push_back(layout);
    renderPasses.push_back(renderPass);
    basePipelines.push_back(basePipeline);
    VkPipelineDynamicStateCreateInfo dynamicStateInfo;
    dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicStateInfo.pNext = 0;
    dynamicStateInfo.flags = 0;
    dynamicStateInfo.dynamicStateCount = MAGMA_COUNT(dynamicStates.back());
    dynamicStateInfo.pDynamicStates = dynamicStates.back().data();
    dynamicStateInfos.push_back(dynamicStateInfo);
    VkGraphicsPipelineCreateInfo pipelineInfo;
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.pNext = nullptr;
    pipelineInfo.flags = flags;
    if (basePipeline)
        pipelineInfo.flags |= VK_PIPELINE_CREATE_DERIVATIVE_BIT;
    pipelineInfo.stageCount = MAGMA_COUNT(shaderStages);
    pipelineInfo.pStages = nullptr; // Fixup later
    pipelineInfo.pVertexInputState = &vertexInputStates.back();
    pipelineInfo.pInputAssemblyState = &inputAssemblyStates.back();
    pipelineInfo.pTessellationState = &tesselationStates.back();
    pipelineInfo.pViewportState = &viewportStates.back();
    pipelineInfo.pRasterizationState = &rasterizationStates.back();
    pipelineInfo.pMultisampleState = &multisampleStates.back();
    pipelineInfo.pDepthStencilState = &depthStencilStates.back();
    pipelineInfo.pColorBlendState = &colorBlendStates.back();
    pipelineInfo.pDynamicState = &dynamicStateInfos.back();
    pipelineInfo.layout = MAGMA_OPTIONAL_HANDLE(layouts.back());
    pipelineInfo.renderPass = *renderPasses.back();
    pipelineInfo.subpass = subpass;
    pipelineInfo.basePipelineHandle = MAGMA_OPTIONAL_HANDLE(basePipelines.back());
    pipelineInfo.basePipelineIndex = -1;
    pipelineInfos.push_back(pipelineInfo);
    std::size_t hash = core::hashArgs(
        pipelineInfo.sType,
        pipelineInfo.flags,
        pipelineInfo.stageCount);
    for (const auto& stage : shaderStages)
        core::hashCombine(hash, stage.getHash());
    std::size_t stateHash = core::hashCombineList({
        vertexInputState.hash(),
        inputAssemblyState.hash(),
        tesselationState.hash(),
        viewportState.hash(),
        rasterizationState.hash(),
        multisampleState.hash(),
        depthStencilState.hash(),
        colorBlendState.hash()});
    for (auto state : dynamicRenderStates)
        core::hashCombine(stateHash, core::hash(state));
    core::hashCombine(hash, stateHash);
    core::hashCombine(hash, layout->getHash());
    if (renderPass)
    {
        core::hashCombine(hash, renderPass->getHash());
        core::hashCombine(hash, core::hash(subpass));
    }
    hashes.push_back(hash);
    return MAGMA_COUNT(pipelineInfos) - 1;
}

void GraphicsPipelines::buildPipelines(std::shared_ptr<Device> device, std::shared_ptr<PipelineCache> pipelineCache,
    std::shared_ptr<IAllocator> allocator /* nullptr */)
{
    fixup(pipelineInfos);
    std::vector<VkPipeline> pipelines(pipelineInfos.size(), VK_NULL_HANDLE);
    const VkResult result = vkCreateGraphicsPipelines(*device, MAGMA_OPTIONAL_HANDLE(pipelineCache), MAGMA_COUNT(pipelineInfos), pipelineInfos.data(), allocator.get(), pipelines.data());
    // Free temporarily allocated storage that had to be preserved until API call
    stages.clear();
    vertexInputStates.clear();
    inputAssemblyStates.clear();
    tesselationStates.clear();
    viewportStates.clear();
    rasterizationStates.clear();
    multisampleStates.clear();
    depthStencilStates.clear();
    colorBlendStates.clear();
    dynamicStates.clear();
    dynamicStateInfos.clear();
    renderPasses.clear();
    pipelineInfos.clear();
    if (VK_SUCCESS == result)
    {
        auto handle = pipelines.cbegin();
        auto layout = layouts.cbegin();
        auto basePipeline = basePipelines.cbegin();
        auto hash = hashes.cbegin();
        graphicsPipelines.clear();
        while (handle != pipelines.cend())
            graphicsPipelines.emplace_back(new GraphicsPipeline(*handle++, device, *layout++, *basePipeline++, allocator, *hash++));
    }
    layouts.clear();
    basePipelines.clear();
    hashes.clear();
    MAGMA_THROW_FAILURE(result, "failed to create multiple graphics pipelines");
}
} // namespace magma
