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
#include "computePipeline.h"
#include "pipelineLayout.h"
#include "pipelineCache.h"
#include "device.h"
#include "../shaders/pipelineShaderStage.h"
#include "../allocator/allocator.h"
#include "../exceptions/errorResult.h"

namespace magma
{
ComputePipeline::ComputePipeline(std::shared_ptr<Device> device_,
    const PipelineShaderStage& shaderStage,
    std::shared_ptr<PipelineLayout> layout,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    std::shared_ptr<PipelineCache> pipelineCache /* nullptr */,
    std::shared_ptr<ComputePipeline> basePipeline_ /* nullptr */,
    VkPipelineCreateFlags flags /* 0 */,
    const StructureChain& extendedInfo /* default */):
    Pipeline(VK_PIPELINE_BIND_POINT_COMPUTE, std::move(device_), std::move(layout), std::move(basePipeline_), std::move(allocator), 1)
{
    VkComputePipelineCreateInfo pipelineInfo;
    pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineInfo.pNext = extendedInfo.chainNodes();
    pipelineInfo.flags = flags;
    if (basePipeline)
        pipelineInfo.flags |= VK_PIPELINE_CREATE_DERIVATIVE_BIT;
    pipelineInfo.stage = shaderStage;
    pipelineInfo.layout = MAGMA_HANDLE(layout);
    pipelineInfo.basePipelineHandle = MAGMA_OPTIONAL_HANDLE(basePipeline);
    pipelineInfo.basePipelineIndex = -1;
#ifdef VK_EXT_pipeline_creation_feedback
    VkPipelineCreationFeedbackCreateInfoEXT pipelineCreationFeedbackInfo;
    if (device->extensionEnabled(VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME))
    {
        stageCreationFeedbacks.resize(1);
        pipelineCreationFeedbackInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CREATION_FEEDBACK_CREATE_INFO_EXT;
        pipelineCreationFeedbackInfo.pNext = pipelineInfo.pNext;
        pipelineCreationFeedbackInfo.pPipelineCreationFeedback = &creationFeedback;
        pipelineCreationFeedbackInfo.pipelineStageCreationFeedbackCount = 1;
        pipelineCreationFeedbackInfo.pPipelineStageCreationFeedbacks = stageCreationFeedbacks.data();
        pipelineInfo.pNext = &pipelineCreationFeedbackInfo;
    }
#endif // VK_EXT_pipeline_creation_feedback
    const VkResult result = vkCreateComputePipelines(MAGMA_HANDLE(device), MAGMA_OPTIONAL_HANDLE(pipelineCache),
        1, &pipelineInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to create compute pipeline");
    hash = core::hashArgs(
        pipelineInfo.sType,
        pipelineInfo.flags);
    hash = core::hashCombine(hash, shaderStage.getHash());
    hash = core::hashCombine(hash, this->layout->getHash());
}

ComputePipeline::ComputePipeline(VkPipeline handle_,
    std::shared_ptr<Device> device,
    std::shared_ptr<PipelineLayout> layout,
    std::shared_ptr<Pipeline> basePipeline,
    std::shared_ptr<IAllocator> allocator,
#ifdef VK_EXT_pipeline_creation_feedback
    VkPipelineCreationFeedbackEXT creationFeedback,
    const std::vector<VkPipelineCreationFeedbackEXT>& stageCreationFeedbacks,
#endif
    hash_t hash):
    Pipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, std::move(device), std::move(layout), std::move(basePipeline), std::move(allocator),
        1,
    #ifdef VK_EXT_pipeline_creation_feedback
        creationFeedback, stageCreationFeedbacks,
    #endif
        hash)
{
    handle = handle_;
}
} // namespace magma
