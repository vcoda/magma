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
#include "graphicsPipeline.h"
#include "pipelineLayout.h"
#include "pipelineCache.h"
#include "pipelineLibrary.h"
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
#include "../helpers/streamInsertOperators.h"

std::ostream& operator<<(std::ostream&, const VkPipelineDynamicStateCreateInfo&);

namespace magma
{
GraphicsPipeline::GraphicsPipeline(std::shared_ptr<Device> device,
    const std::vector<PipelineShaderStage>& shaderStages,
    const VertexInputState& vertexInputState,
    const InputAssemblyState& inputAssemblyState,
    const RasterizationState& rasterizationState,
    const MultisampleState& multisampleState,
    const DepthStencilState& depthStencilState,
    const ColorBlendState& colorBlendState,
    const std::vector<VkDynamicState>& dynamicStates,
    core::variant_ptr<PipelineLayout> layout,
    lent_ptr<RenderPass> renderPass,
    uint32_t subpass,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    lent_ptr<PipelineCache> pipelineCache /* nullptr */,
#ifdef VK_KHR_pipeline_library
    lent_ptr<PipelineLibrary> pipelineLibrary /* nullptr */,
#endif
    std::shared_ptr<GraphicsPipeline> basePipeline /* nullptr */,
    VkPipelineCreateFlags flags /* 0 */,
    const StructureChain& extendedInfo /* default */):
    GraphicsPipeline(std::move(device), shaderStages, vertexInputState, inputAssemblyState,
        TesselationState(), // No tesselation state
        ViewportState(), // No viewport state (supposed to be dynamic)
        rasterizationState, multisampleState, depthStencilState, colorBlendState, dynamicStates,
        std::move(layout), std::move(renderPass), subpass,
        std::move(allocator), std::move(pipelineCache),
    #ifdef VK_KHR_pipeline_library
        std::move(pipelineLibrary),
    #endif
        std::move(basePipeline),
        flags, extendedInfo)
{}

GraphicsPipeline::GraphicsPipeline(std::shared_ptr<Device> device_,
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
    core::variant_ptr<PipelineLayout> layout_,
    lent_ptr<RenderPass> renderPass,
    uint32_t subpass,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    lent_ptr<PipelineCache> pipelineCache /* nullptr */,
#ifdef VK_KHR_pipeline_library
    lent_ptr<PipelineLibrary> pipelineLibrary /* nullptr */,
#endif
    std::shared_ptr<GraphicsPipeline> basePipeline_ /* nullptr */,
    VkPipelineCreateFlags flags /* 0 */,
    const StructureChain& extendedInfo /* default */):
    Pipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, std::move(device_), std::move(layout_), std::move(basePipeline_), std::move(allocator), core::countof(shaderStages)),
    rsHash(0ull)
{
    MAGMA_VLA(VkPipelineShaderStageCreateInfo, dereferencedStages, shaderStages.size());
    for (auto const& stage: shaderStages)
        dereferencedStages.put(stage);
    VkGraphicsPipelineCreateInfo pipelineInfo;
    VkPipelineDynamicStateCreateInfo pipelineDynamicStateInfo;
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.pNext = extendedInfo.headNode();
    pipelineInfo.flags = flags;
    if (!basePipeline.expired())
        pipelineInfo.flags |= VK_PIPELINE_CREATE_DERIVATIVE_BIT;
    pipelineInfo.stageCount = dereferencedStages.count();
    pipelineInfo.pStages = dereferencedStages;
    pipelineInfo.pVertexInputState = &vertexInputState;
    pipelineInfo.pInputAssemblyState = &inputAssemblyState;
    pipelineInfo.pTessellationState = tesselationState.patchControlPoints ? &tesselationState : nullptr;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizationState;
    pipelineInfo.pMultisampleState = &multisampleState;
    pipelineInfo.pDepthStencilState = &depthStencilState;
    pipelineInfo.pColorBlendState = &colorBlendState;
    pipelineInfo.pDynamicState = nullptr;
    pipelineInfo.layout = *layout;
    pipelineInfo.renderPass = *renderPass;
    pipelineInfo.subpass = subpass;
    pipelineInfo.basePipelineHandle = MAGMA_OPTIONAL_HANDLE(basePipeline);
    pipelineInfo.basePipelineIndex = -1;
    if (!dynamicStates.empty())
    {
        pipelineInfo.pDynamicState = &pipelineDynamicStateInfo;
        pipelineDynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        pipelineDynamicStateInfo.pNext = nullptr;
        pipelineDynamicStateInfo.flags = 0;
        pipelineDynamicStateInfo.dynamicStateCount = core::countof(dynamicStates);
        pipelineDynamicStateInfo.pDynamicStates = dynamicStates.data();
    }
#ifdef VK_AMD_pipeline_compiler_control
    VkPipelineCompilerControlCreateInfoAMD pipelineCompilerControlInfo;
    if (extensionEnabled(VK_AMD_PIPELINE_COMPILER_CONTROL_EXTENSION_NAME))
    {
        pipelineCompilerControlInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COMPILER_CONTROL_CREATE_INFO_AMD;
        pipelineCompilerControlInfo.pNext = nullptr;
        pipelineCompilerControlInfo.compilerControlFlags = compilerControlFlags;
        linkNode(pipelineInfo, pipelineCompilerControlInfo);
    }
#endif // VK_AMD_pipeline_compiler_control
#ifdef VK_EXT_pipeline_creation_feedback
    VkPipelineCreationFeedbackCreateInfoEXT pipelineCreationFeedbackInfo;
    if (extensionEnabled(VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME))
    {
        stageCreationFeedbacks.resize(pipelineInfo.stageCount);
        pipelineCreationFeedbackInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CREATION_FEEDBACK_CREATE_INFO_EXT;
        pipelineCreationFeedbackInfo.pNext = nullptr;
        pipelineCreationFeedbackInfo.pPipelineCreationFeedback = &creationFeedback;
        pipelineCreationFeedbackInfo.pipelineStageCreationFeedbackCount = pipelineInfo.stageCount;
        pipelineCreationFeedbackInfo.pPipelineStageCreationFeedbacks = stageCreationFeedbacks.data();
        linkNode(pipelineInfo, pipelineCreationFeedbackInfo);
    }
#endif // VK_EXT_pipeline_creation_feedback
#ifdef VK_KHR_pipeline_library
    VkPipelineLibraryCreateInfoKHR pipelineLibraryInfo;
    if (pipelineLibrary && extensionEnabled(VK_KHR_PIPELINE_LIBRARY_EXTENSION_NAME))
    {
        pipelineLibraryInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LIBRARY_CREATE_INFO_KHR;
        pipelineLibraryInfo.pNext = nullptr;
        pipelineLibraryInfo.libraryCount = pipelineLibrary->getLibraryCount();
        pipelineLibraryInfo.pLibraries = pipelineLibrary->getLibraries();
        linkNode(pipelineInfo, pipelineLibraryInfo);
    }
#endif // VK_KHR_pipeline_library
    const VkResult result = vkCreateGraphicsPipelines(getNativeDevice(), MAGMA_OPTIONAL_HANDLE(pipelineCache),
        1, &pipelineInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    if (result != VK_SUCCESS)
    {
    #ifdef MAGMA_DEBUG
        for (auto const& stage: shaderStages)
            std::cerr << stage << std::endl;
        std::cerr << vertexInputState << std::endl
            << inputAssemblyState << std::endl
            << tesselationState << std::endl
            << viewportState << std::endl
            << rasterizationState << std::endl
            << multisampleState << std::endl
            << depthStencilState << std::endl
            << colorBlendState << std::endl
            << pipelineDynamicStateInfo << std::endl
            << *layout << std::endl
            << *renderPass << std::endl;
    #endif // MAGMA_DEBUG
        MAGMA_HANDLE_RESULT(result, "failed to create graphics pipeline");
    }
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
    hash = hashes.first;
    rsHash = hashes.second;
}

GraphicsPipeline::GraphicsPipeline(VkPipeline handle_,
    std::shared_ptr<Device> device,
    core::variant_ptr<PipelineLayout> layout,
    std::shared_ptr<Pipeline> basePipeline,
    std::shared_ptr<IAllocator> allocator,
    uint32_t stageCount,
#ifdef VK_EXT_pipeline_creation_feedback
    VkPipelineCreationFeedbackEXT creationFeedback,
    const std::vector<VkPipelineCreationFeedbackEXT>& stageCreationFeedbacks,
#endif // VK_EXT_pipeline_creation_feedback
    hash_t hash,
    hash_t rsHash):
    Pipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, std::move(device), std::move(layout), std::move(basePipeline), std::move(allocator),
        stageCount,
    #ifdef VK_EXT_pipeline_creation_feedback
        creationFeedback, stageCreationFeedbacks,
    #endif
        hash),
    rsHash(rsHash)
{
    handle = handle_;
}

std::pair<hash_t, hash_t> psoHash(VkPipelineCreateFlags flags,
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
    const PipelineLayout *layout,
    const RenderPass *renderPass,
    uint32_t subpass,
    const StructureChain& extendedInfo /* default */) noexcept
{   // Erase flags that do not affect pipeline states
    // TODO: maybe clear some extension flags too?
    flags = flags & ~(VK_PIPELINE_CREATE_DISABLE_OPTIMIZATION_BIT |
                      VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT |
                      VK_PIPELINE_CREATE_DERIVATIVE_BIT);
    hash_t hash = core::hash(flags);
    for (auto const& stage: shaderStages)
        hash = core::hashCombine(hash, stage.getHash());
    hash_t rsHash = core::combineHashList(
        {   // Compute hash of render states
            vertexInputState.hash(),
            inputAssemblyState.hash(),
            tesselationState.hash(),
            viewportState.hash(),
            rasterizationState.chainedHash(),
            multisampleState.hash(),
            depthStencilState.hash(),
            colorBlendState.hash()
        });
    for (auto state: dynamicStates)
        rsHash = core::hashCombine(rsHash, core::hash(state));
    hash = core::hashCombine(hash, rsHash);
    if (layout)
        hash = core::hashCombine(hash, layout->getHash());
    if (renderPass)
    {
        hash = core::hashCombine(hash, renderPass->getHash());
        hash = core::hashCombine(hash, core::hash(subpass));
    }
    if (!extendedInfo.empty()) // TODO: not all extended info may affect pipeline state
        hash = core::hashCombine(hash, extendedInfo.hash());
    return {hash, rsHash};
}
} // namespace magma

std::ostream& operator<<(std::ostream& out, const VkPipelineDynamicStateCreateInfo& state)
{
    out << "VkPipelineDynamicStateCreateInfo [" << std::endl
        << "\tdynamicStateCount: " << state.dynamicStateCount << std::endl
        << "\tpDynamicStates:" << std::endl
        << "\t[" << std::endl;
    for (uint32_t i = 0; i < state.dynamicStateCount ; ++i)
        out << "\t\t" << state.pDynamicStates[i] << std::endl;
    out << "\t]" << std::endl
        << "]";
    return out;
}
