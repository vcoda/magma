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
#include "graphicsPipeline.h"
#include "pipelineLayout.h"
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
#include "../misc/exception.h"
#include "../internal/hash.h"
#include "../helpers/stackArray.h"

namespace magma
{
GraphicsPipeline::GraphicsPipeline(std::shared_ptr<Device> device, std::shared_ptr<PipelineCache> cache,
    const std::vector<PipelineShaderStage>& stages,
    const VertexInputState& vertexInputState,
    const InputAssemblyState& inputAssemblyState,
    const RasterizationState& rasterizationState,
    const MultisampleState& multisampleState,
    const DepthStencilState& depthStencilState,
    const ColorBlendState& colorBlendState,
    const std::initializer_list<VkDynamicState>& dynamicStates,
    std::shared_ptr<PipelineLayout> layout,
    std::shared_ptr<RenderPass> renderPass,
    uint32_t subpass /* 0 */,
    std::shared_ptr<GraphicsPipeline> basePipeline /* nullptr */,
    VkPipelineCreateFlags flags /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    GraphicsPipeline(std::move(device), std::move(cache), stages,
        vertexInputState, inputAssemblyState,
        TesselationState(), // No tesselation state
        ViewportState(), // No viewport state (supposed to be dynamic)
        rasterizationState, multisampleState, depthStencilState, colorBlendState, dynamicStates,
        std::move(layout), std::move(renderPass), subpass, std::move(basePipeline), flags,
        std::move(allocator))
{}

GraphicsPipeline::GraphicsPipeline(std::shared_ptr<Device> device, std::shared_ptr<PipelineCache> cache,
    const std::vector<PipelineShaderStage>& stages,
    const VertexInputState& vertexInputState,
    const InputAssemblyState& inputAssemblyState,
    const TesselationState& tesselationState,
    const ViewportState& viewportState,
    const RasterizationState& rasterizationState,
    const MultisampleState& multisampleState,
    const DepthStencilState& depthStencilState,
    const ColorBlendState& colorBlendState,
    const std::initializer_list<VkDynamicState>& dynamicStates,
    std::shared_ptr<PipelineLayout> layout,
    std::shared_ptr<RenderPass> renderPass,
    uint32_t subpass /* 0 */,
    std::shared_ptr<GraphicsPipeline> basePipeline /* nullptr */,
    VkPipelineCreateFlags flags /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Pipeline(std::move(device), std::move(layout), std::move(basePipeline), std::move(cache), std::move(allocator))
{
    if (stages.empty())
        MAGMA_THROW("shader stages are empty");
	VkPipelineVertexInputStateCreateInfo pipelineVertexInput = {};
	VkVertexInputBindingDescription vertexBindingDesc = {};
    VkGraphicsPipelineCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = flags;
    if (this->basePipeline)
        info.flags |= VK_PIPELINE_CREATE_DERIVATIVE_BIT;
    MAGMA_STACK_ARRAY(VkPipelineShaderStageCreateInfo, dereferencedStages, stages.size());
    for (auto& stage : stages)
        dereferencedStages.put(stage);
    info.stageCount = MAGMA_COUNT(dereferencedStages);
    info.pStages = dereferencedStages;
    if (vertexInputState.vertexBindingDescriptionCount > 0)
        info.pVertexInputState = &vertexInputState;
    else
    {
        vertexBindingDesc = VertexInputBinding(0, vertexInputState.stride(0));
        pipelineVertexInput = vertexInputState;
        pipelineVertexInput.vertexBindingDescriptionCount = 1;
        pipelineVertexInput.pVertexBindingDescriptions = &vertexBindingDesc;
        info.pVertexInputState = &pipelineVertexInput;
    }
    info.pInputAssemblyState = &inputAssemblyState;
    if (0 == tesselationState.patchControlPoints)
        info.pTessellationState = nullptr;
    else
        info.pTessellationState = &tesselationState;
    info.pViewportState = &viewportState;
    info.pRasterizationState = &rasterizationState;
    info.pMultisampleState = &multisampleState;
    info.pDepthStencilState = &depthStencilState;
    info.pColorBlendState = &colorBlendState;
    VkPipelineDynamicStateCreateInfo dynamicState;
    if (0 == dynamicStates.size())
        info.pDynamicState = nullptr;
    else
    {
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.pNext = 0;
        dynamicState.flags = 0;
        dynamicState.dynamicStateCount = MAGMA_COUNT(dynamicStates);
        dynamicState.pDynamicStates = dynamicStates.begin();
        info.pDynamicState = &dynamicState;
    }
    info.layout = MAGMA_HANDLE(layout);
    info.renderPass = *renderPass;
    info.subpass = subpass;
    info.basePipelineHandle = MAGMA_OPTIONAL_HANDLE(this->basePipeline);
    info.basePipelineIndex = -1;
    const VkResult create = vkCreateGraphicsPipelines(MAGMA_HANDLE(device), MAGMA_OPTIONAL_HANDLE(this->cache), 1, &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create graphics pipeline");
    using namespace internal;
    hash = hashArgs(
        info.sType,
        info.flags,
        info.stageCount);
    for (const auto& stage : stages)
        hashCombine(hash, stage.hash());
    std::size_t stateHash = combineHashList({
        vertexInputState.hash(),
        inputAssemblyState.hash(),
        tesselationState.hash(),
        viewportState.hash(),
        rasterizationState.hash(),
        multisampleState.hash(),
        depthStencilState.hash(),
        colorBlendState.hash()});
    for (auto state : dynamicStates)
        hashCombine(stateHash, internal::hash(state));
    hashCombine(hash, stateHash);
    hashCombine(hash, this->layout->getHash());
    if (renderPass)
    {
        hashCombine(hash, renderPass->getHash());
        hashCombine(hash, internal::hash(subpass));
    }
}
} // namespace magma
