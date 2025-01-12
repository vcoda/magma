/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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
#include "pipelineLibrary.h"
#include "pipelineCache.h"
#include "device.h"
#include "renderPass.h"
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
GraphicsPipelineBatch::GraphicsPipelineBatch(std::shared_ptr<Device> device) noexcept:
    BasePipelineBatch<GraphicsPipeline, VkGraphicsPipelineCreateInfo>(std::move(device))
{}

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
    variant_ptr<PipelineLayout> layout,
    std::shared_ptr<RenderPass> renderPass,
    uint32_t subpass,
    std::shared_ptr<GraphicsPipeline> basePipeline /* nullptr */,
    VkPipelineCreateFlags flags /* 0 */,
    const StructureChain& extendedInfo /* default */)
{
    stages.push_front(shaderStages);
    vertexInputStates.push_front(vertexInputState);
    inputAssemblyStates.push_front(inputAssemblyState);
    tesselationStates.push_front(tesselationState);
    viewportStates.push_front(viewportState);
    rasterizationStates.push_front(rasterizationState);
    multisampleStates.push_front(multisampleState);
    depthStencilStates.push_front(depthStencilState);
    colorBlendStates.push_front(colorBlendState);
    dynamicStates.push_front(dynamicStates_);
    if (!layout)
        layout = std::make_unique<PipelineLayout>(device);
    renderPasses.emplace_front(std::move(renderPass));
    basePipelines.emplace_front(std::move(basePipeline));
    VkPipelineDynamicStateCreateInfo dynamicStateInfo;
    dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicStateInfo.pNext = nullptr;
    dynamicStateInfo.flags = 0;
    dynamicStateInfo.dynamicStateCount = core::countof(dynamicStates.front());
    dynamicStateInfo.pDynamicStates = dynamicStates.front().data();
    dynamicStateInfos.push_front(dynamicStateInfo);
    VkGraphicsPipelineCreateInfo pipelineInfo;
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.pNext = extendedInfo.headNode();
    pipelineInfo.flags = flags;
    if (basePipelines.front())
        pipelineInfo.flags |= VK_PIPELINE_CREATE_DERIVATIVE_BIT;
    pipelineInfo.stageCount = core::countof(shaderStages);
    pipelineInfo.pStages = nullptr; // Fixup later
    pipelineInfo.pVertexInputState = &vertexInputStates.front();
    pipelineInfo.pInputAssemblyState = &inputAssemblyStates.front();
    pipelineInfo.pTessellationState = &tesselationStates.front();
    pipelineInfo.pViewportState = &viewportStates.front();
    pipelineInfo.pRasterizationState = &rasterizationStates.front();
    pipelineInfo.pMultisampleState = &multisampleStates.front();
    pipelineInfo.pDepthStencilState = &depthStencilStates.front();
    pipelineInfo.pColorBlendState = &colorBlendStates.front();
    pipelineInfo.pDynamicState = &dynamicStateInfos.front();
    pipelineInfo.layout = *layout;
    pipelineInfo.renderPass = *renderPasses.front();
    pipelineInfo.subpass = subpass;
    pipelineInfo.basePipelineHandle = MAGMA_OPTIONAL_HANDLE(basePipelines.front());
    pipelineInfo.basePipelineIndex = -1;
#ifdef VK_AMD_pipeline_compiler_control
    if (device->extensionEnabled(VK_AMD_PIPELINE_COMPILER_CONTROL_EXTENSION_NAME))
    {
        VkPipelineCompilerControlCreateInfoAMD pipelineCompilerControlInfo;
        pipelineCompilerControlInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COMPILER_CONTROL_CREATE_INFO_AMD;
        pipelineCompilerControlInfo.pNext = nullptr;
        pipelineCompilerControlInfo.compilerControlFlags = compilerControlFlags;
        pipelineCompilerControlInfos.push_front(pipelineCompilerControlInfo);
        linkNode(pipelineInfo, pipelineCompilerControlInfos.front());
    }
#endif // VK_AMD_pipeline_compiler_control
#ifdef VK_EXT_pipeline_creation_feedback
    if (device->extensionEnabled(VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME))
    {
        creationFeedbacks.push_front(VkPipelineCreationFeedbackEXT());
        stageCreationFeedbacks.emplace_front(pipelineInfo.stageCount);
        VkPipelineCreationFeedbackCreateInfoEXT creationFeedbackInfo;
        creationFeedbackInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CREATION_FEEDBACK_CREATE_INFO_EXT;
        creationFeedbackInfo.pNext = pipelineInfo.pNext;
        creationFeedbackInfo.pPipelineCreationFeedback = &creationFeedbacks.front();
        creationFeedbackInfo.pipelineStageCreationFeedbackCount = pipelineInfo.stageCount;
        creationFeedbackInfo.pPipelineStageCreationFeedbacks = stageCreationFeedbacks.front().data();
        creationFeedbackInfos.push_front(creationFeedbackInfo);
        linkNode(pipelineInfo, creationFeedbackInfos.front());
    }
#endif // VK_EXT_pipeline_creation_feedback
    pipelineInfos.push_back(pipelineInfo);
    std::pair<hash_t, hash_t> hash = psoHash(
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
        dynamicStates_,
        layout.get(),
        renderPasses.front().get(),
        subpass,
        extendedInfo);
    hashes.push_front(hash.first);
    rsHashes.push_front(hash.second);
    layouts.push_front(std::move(layout));
    return core::countof(pipelineInfos) - 1;
}

void GraphicsPipelineBatch::buildPipelines(lent_ptr<PipelineCache> pipelineCache /* nullptr */,
#ifdef VK_KHR_pipeline_library
    lent_ptr<const PipelineLibrary> pipelineLibrary /* nullptr */,
#endif
    std::shared_ptr<IAllocator> allocator /* nullptr */)
{
    fixup();
#ifdef VK_KHR_pipeline_library
    if (device->extensionEnabled(VK_KHR_PIPELINE_LIBRARY_EXTENSION_NAME))
        linkPipelineLibrary(std::move(pipelineLibrary));
#endif // VK_KHR_pipeline_library
    std::vector<VkPipeline> handles(pipelineInfos.size(), VK_NULL_HANDLE);
    const VkResult result = vkCreateGraphicsPipelines(getNativeDevice(), MAGMA_OPTIONAL_HANDLE(pipelineCache),
        core::countof(pipelineInfos), pipelineInfos.data(), allocator.get(), handles.data());
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
    if (VK_SUCCESS == result)
    {
        auto handle = handles.cbegin();
        auto layout = layouts.begin();
        auto shaderStages = stages.cbegin();
    #ifdef VK_EXT_pipeline_creation_feedback
        auto creationFeedback = creationFeedbacks.cbegin();
        auto stageFeedbacks = stageCreationFeedbacks.cbegin();
    #endif // VK_EXT_pipeline_creation_feedback
        auto hash = hashes.cbegin();
        auto rsHash = rsHashes.cbegin();
        while (handle != handles.cend())
        {
            pipelines.emplace_front(GraphicsPipeline::makeUnique(
                *handle++, device, std::move(*layout++), allocator,
                core::countof(*shaderStages++),
            #ifdef VK_EXT_pipeline_creation_feedback
                creationFeedbacks.empty() ? VkPipelineCreationFeedbackEXT{} : *creationFeedback++,
                stageCreationFeedbacks.empty() ? std::vector<VkPipelineCreationFeedbackEXT>{} : *stageFeedbacks++,
            #endif // VK_EXT_pipeline_creation_feedback
                *hash++, *rsHash++));
        }
    }
    postBuild();
    rsHashes.clear();
    MAGMA_HANDLE_RESULT(result, "failed to create multiple graphics pipelines");
}
} // namespace magma
