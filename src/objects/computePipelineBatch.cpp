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
#include "computePipelineBatch.h"
#include "computePipeline.h"
#include "pipelineLayout.h"
#include "pipelineCache.h"
#include "device.h"
#include "../allocator/allocator.h"
#include "../exceptions/errorResult.h"

namespace magma
{
ComputePipelineBatch::ComputePipelineBatch(uint32_t capacity /* 32 */)
{
    pipelines.reserve(capacity);
    pipelineInfos.reserve(capacity);
}

uint32_t ComputePipelineBatch::batchPipeline(const PipelineShaderStage& shaderStage, std::shared_ptr<PipelineLayout> layout,
    std::shared_ptr<ComputePipeline> basePipeline /* nullptr */,
    VkPipelineCreateFlags flags /* 0 */)
{
    stages.emplace_back(1, shaderStage);
    layouts.push_back(layout);
    basePipelines.push_back(basePipeline);
    VkComputePipelineCreateInfo pipelineInfo;
    pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineInfo.pNext = nullptr;
    pipelineInfo.flags = flags;
    if (basePipeline)
        pipelineInfo.flags |= VK_PIPELINE_CREATE_DERIVATIVE_BIT;
    pipelineInfo.stage = stages.back().front();
    pipelineInfo.layout = MAGMA_HANDLE(layouts.back());
    pipelineInfo.basePipelineHandle = MAGMA_OPTIONAL_HANDLE(basePipelines.back());
    pipelineInfo.basePipelineIndex = -1;
#ifdef VK_EXT_pipeline_creation_feedback
    if (layout->getDevice()->extensionEnabled(VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME))
    {
        creationFeedbacks.push_back(VkPipelineCreationFeedbackEXT());
        stageCreationFeedbacks.emplace_back(1);
        VkPipelineCreationFeedbackCreateInfoEXT creationFeedbackInfo;
        creationFeedbackInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CREATION_FEEDBACK_CREATE_INFO_EXT;
        creationFeedbackInfo.pNext = nullptr;
        creationFeedbackInfo.pPipelineCreationFeedback = &creationFeedbacks.back();
        creationFeedbackInfo.pipelineStageCreationFeedbackCount = 1;
        creationFeedbackInfo.pPipelineStageCreationFeedbacks = stageCreationFeedbacks.back().data();
        creationFeedbackInfos.push_back(creationFeedbackInfo);
        pipelineInfo.pNext = &creationFeedbackInfos.back();
    }
#endif // VK_EXT_pipeline_creation_feedback
    pipelineInfos.push_back(pipelineInfo);
    hash_t hash = core::hash(flags);
    hash = core::hashCombine(hash, shaderStage.getHash());
    hash = core::hashCombine(hash, layout->getHash());
    hashes.push_back(hash);
    return MAGMA_COUNT(pipelineInfos) - 1;
}

void ComputePipelineBatch::buildPipelines(std::shared_ptr<Device> device,
    std::shared_ptr<PipelineCache> pipelineCache /* nullptr */,
    std::shared_ptr<IAllocator> allocator /* nullptr */)
{
    std::vector<VkPipeline> handles(pipelineInfos.size(), VK_NULL_HANDLE);
    const VkResult result = vkCreateComputePipelines(*device, MAGMA_OPTIONAL_HANDLE(pipelineCache),
        MAGMA_COUNT(pipelineInfos), pipelineInfos.data(), allocator.get(), handles.data());
    postCreate();
    pipelineInfos.clear();
    if (VK_SUCCESS == result)
    {
        auto handle = handles.cbegin();
        auto layout = layouts.cbegin();
        auto basePipeline = basePipelines.cbegin();
    #ifdef VK_EXT_pipeline_creation_feedback
        auto creationFeedback = creationFeedbacks.cbegin();
        auto stageFeedbacks = stageCreationFeedbacks.cbegin();
    #endif // VK_EXT_pipeline_creation_feedback
        auto hash = hashes.cbegin();
        while (handle != handles.cend())
        {
            pipelines.emplace_back(ComputePipeline::makeShared(
                *handle++, device, *layout++, *basePipeline++, allocator,
            #ifdef VK_EXT_pipeline_creation_feedback
                creationFeedbacks.empty() ? VkPipelineCreationFeedbackEXT{} : *creationFeedback++,
                stageCreationFeedbacks.empty() ? std::vector<VkPipelineCreationFeedbackEXT>{} : *stageFeedbacks++,
            #endif // VK_EXT_pipeline_creation_feedback
                *hash++));
        }
    }
    postBuild();
    MAGMA_HANDLE_RESULT(result, "failed to create multiple compute pipelines");
}
} // namespace magma
