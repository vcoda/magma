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
#include "pipeline.h"
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
#include "../shared.h"

namespace magma
{
Pipeline::Pipeline(std::shared_ptr<const Device> device):
    NonDispatchable<VkPipeline>(VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_EXT, device)
{
}

Pipeline::~Pipeline()
{
    vkDestroyPipeline(*device, handle, nullptr);
}

GraphicsPipeline::GraphicsPipeline(std::shared_ptr<const Device> device, std::shared_ptr<const PipelineCache> pipelineCache,  
    const std::vector<ShaderStage>& stages,
    const VertexInputState& vertexInputState,
    const InputAssemblyState& inputAssemblyState,
    const TesselationState& tesselationState,
    const ViewportState& viewportState,
    const RasterizationState& rasterizationState,
    const MultisampleState& multisampleState,
    const DepthStencilState& depthStencilState,
    const ColorBlendState& colorBlendState,
    std::shared_ptr<const RenderPass> renderPass,
    const std::initializer_list<VkDynamicState>& dynamicStates /* {} */,
    std::shared_ptr<const PipelineLayout> layout /* nullptr */,
    VkPipelineCreateFlags flags /* 0 */):
    Pipeline(device)
{
    if (stages.empty())
        throw Exception("shader stages are empty");
    VkGraphicsPipelineCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = flags;
    std::vector<VkPipelineShaderStageCreateInfo> dereferencedStages;
    for (auto& stage : stages)
        dereferencedStages.push_back(stage);
    info.stageCount = MAGMA_COUNT(dereferencedStages);
    info.pStages = dereferencedStages.data();
    info.pVertexInputState = &vertexInputState;
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
    if (layout)
        info.layout = *layout;
    else
    {
        defaultLayout.reset(new PipelineLayout(device));
        info.layout = *defaultLayout;
    }
    info.renderPass = *renderPass;
    info.subpass = 0;
    info.basePipelineIndex = 0;
    info.basePipelineHandle = VK_NULL_HANDLE;
    const VkResult create = vkCreateGraphicsPipelines(*device, *pipelineCache, 1, &info, nullptr, &handle);
    MAGMA_THROW_FAILURE(create, "failed to create graphics pipeline");
}

ComputePipeline::ComputePipeline(std::shared_ptr<const Device> device, std::shared_ptr<const PipelineCache> pipelineCache,
    const ShaderStage& stage,
    std::shared_ptr<const PipelineLayout> layout /* nullptr */,
    VkPipelineCreateFlags flags /* 0 */):
    Pipeline(device)
{
    VkComputePipelineCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = flags;
    info.stage = stage;
    if (layout)
        info.layout = *layout;
    else
    {
        defaultLayout.reset(new PipelineLayout(device));
        info.layout = *defaultLayout;
    }
    info.basePipelineHandle = 0;
    info.basePipelineIndex = 0;
    const VkResult create = vkCreateComputePipelines(*device, *pipelineCache, 1, &info, nullptr, &handle);
    MAGMA_THROW_FAILURE(create, "failed to create compute pipeline");
}
} // namespace magma
