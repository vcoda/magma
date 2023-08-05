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
#include "graphicsPipelineBatch.h"
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
GraphicsPipelineBatch::GraphicsPipelineBatch(uint32_t capacity /* 256 */)
{
    pipelines.reserve(capacity);
    pipelineInfos.reserve(capacity);
}

uint32_t GraphicsPipelineBatch::batchPipeline(const std::vector<PipelineShaderStage>& shaderStages,
    const VertexInputState& vertexInputState,
    const InputAssemblyState& inputAssemblyState,
    const TesselationState& tesselationState,
    const ViewportState& viewportState,
    const RasterizationState& rasterizationState,
    const MultisampleState& multisampleState,
    const DepthStencilState& depthStencilState,
    const ColorBlendState& colorBlendState,
    const std::vector<VkDynamicState>& dynamicStates_,
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
    dynamicStates.push_back(dynamicStates_);
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
#ifdef VK_EXT_pipeline_creation_feedback
    if (layout->getDevice()->extensionEnabled(VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME))
    {
        creationFeedbacks.push_back(VkPipelineCreationFeedbackEXT());
        stageCreationFeedbacks.emplace_back(pipelineInfo.stageCount);
        VkPipelineCreationFeedbackCreateInfoEXT creationFeedbackInfo;
        creationFeedbackInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CREATION_FEEDBACK_CREATE_INFO_EXT;
        creationFeedbackInfo.pNext = nullptr;
        creationFeedbackInfo.pPipelineCreationFeedback = &creationFeedbacks.back();
        creationFeedbackInfo.pipelineStageCreationFeedbackCount = pipelineInfo.stageCount;
        creationFeedbackInfo.pPipelineStageCreationFeedbacks = stageCreationFeedbacks.back().data();
        creationFeedbackInfos.push_back(creationFeedbackInfo);
        pipelineInfo.pNext = &creationFeedbackInfos.back();
    }
#endif // VK_EXT_pipeline_creation_feedback
    pipelineInfos.push_back(pipelineInfo);
    hash_t hash = core::hashArgs(
        pipelineInfo.sType,
        pipelineInfo.flags,
        pipelineInfo.stageCount);
    for (const auto& stage : shaderStages)
        hash = core::hashCombine(hash, stage.getHash());
    hash_t stateHash = core::combineHashList({
        vertexInputState.hash(),
        inputAssemblyState.hash(),
        tesselationState.hash(),
        viewportState.hash(),
        rasterizationState.chained()
            ? rasterizationState.chainedHash()
            : rasterizationState.hash(),
        multisampleState.hash(),
        depthStencilState.hash(),
        colorBlendState.hash()});
    for (auto state: dynamicStates_)
        hash = core::hashCombine(stateHash, core::hash(state));
    hash = core::hashCombine(hash, stateHash);
    hash = core::hashCombine(hash, layout->getHash());
    if (renderPass)
    {
        hash = core::hashCombine(hash, renderPass->getHash());
        hash = core::hashCombine(hash, core::hash(subpass));
    }
    hashes.push_back(hash);
    return MAGMA_COUNT(pipelineInfos) - 1;
}

void GraphicsPipelineBatch::buildPipelines(std::shared_ptr<Device> device, std::shared_ptr<PipelineCache> pipelineCache,
    std::shared_ptr<IAllocator> allocator /* nullptr */)
{
    fixup(pipelineInfos);
    std::vector<VkPipeline> handles(pipelineInfos.size(), VK_NULL_HANDLE);
    const VkResult result = vkCreateGraphicsPipelines(*device, MAGMA_OPTIONAL_HANDLE(pipelineCache),
        MAGMA_COUNT(pipelineInfos), pipelineInfos.data(), allocator.get(), handles.data());
    // Free storage that had to be preserved until API call
    postCreate();
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
        auto handle = handles.cbegin();
        auto layout = layouts.cbegin();
        auto basePipeline = basePipelines.cbegin();
        auto shaderStages = stages.cbegin();
    #ifdef VK_EXT_pipeline_creation_feedback
        auto creationFeedback = creationFeedbacks.cbegin();
        auto stageFeedbacks = stageCreationFeedbacks.cbegin();
    #endif // VK_EXT_pipeline_creation_feedback
        auto hash = hashes.cbegin();
        while (handle != handles.cend())
        {
            pipelines.emplace_back(new GraphicsPipeline(
                *handle++, device, *layout++, *basePipeline++, allocator,
                MAGMA_COUNT(*shaderStages++),
            #ifdef VK_EXT_pipeline_creation_feedback
                creationFeedbacks.empty() ? VkPipelineCreationFeedbackEXT{} : *creationFeedback++,
                stageCreationFeedbacks.empty() ? std::vector<VkPipelineCreationFeedbackEXT>{} : *stageFeedbacks++,
            #endif // VK_EXT_pipeline_creation_feedback
                *hash++));
        }
    }
    postBuild();
    MAGMA_THROW_FAILURE(result, "failed to create multiple graphics pipelines");
}
} // namespace magma
