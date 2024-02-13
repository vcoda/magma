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
#include "rayTracingPipelineBatch.h"
#include "rayTracingPipeline.h"
#include "pipelineLayout.h"
#include "pipelineCache.h"
#include "device.h"
#include "../allocator/allocator.h"
#include "../misc/extension.h"
#include "../exceptions/errorResult.h"

namespace magma
{
#ifdef VK_NV_ray_tracing
RayTracingPipelineBatch::RayTracingPipelineBatch(uint32_t capacity /* 32 */)
{
    pipelines.reserve(capacity);
    pipelineInfos.reserve(capacity);
}

uint32_t RayTracingPipelineBatch::batchPipeline(const std::vector<PipelineShaderStage>& shaderStages,
    const std::vector<RayTracingShaderGroup>& shaderGroups, uint32_t maxRecursionDepth, std::shared_ptr<PipelineLayout> layout,
    std::shared_ptr<RayTracingPipeline> basePipeline /* nullptr */,
    VkPipelineCreateFlags flags /* 0 */)
{
    stages.push_back(shaderStages);
    groups.push_back(shaderGroups);
    layouts.push_back(layout);
    basePipelines.push_back(basePipeline);
    VkRayTracingPipelineCreateInfoNV pipelineInfo;
    pipelineInfo.sType = VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_NV;
    pipelineInfo.pNext = nullptr;
    pipelineInfo.flags = flags;
    pipelineInfo.stageCount = MAGMA_COUNT(shaderStages);
    pipelineInfo.pStages = nullptr; // Fixup later
    pipelineInfo.groupCount = MAGMA_COUNT(shaderGroups);
    pipelineInfo.pGroups = groups.back().data();
    pipelineInfo.maxRecursionDepth = maxRecursionDepth;
    pipelineInfo.layout = MAGMA_HANDLE(layouts.back());
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
        pipelineInfo.stageCount,
        pipelineInfo.groupCount,
        pipelineInfo.maxRecursionDepth);
    for (const auto& stage : shaderStages)
        hash = core::hashCombine(hash, stage.getHash());
    for (const auto& group : shaderGroups)
        hash = core::hashCombine(hash, group.hash());
    hash = core::hashCombine(hash, layout->getHash());
    hashes.push_back(hash);
    return MAGMA_COUNT(pipelineInfos) - 1;
}

void RayTracingPipelineBatch::buildPipelines(std::shared_ptr<Device> device_,
    std::shared_ptr<PipelineCache> pipelineCache /* nullptr */,
    std::shared_ptr<IAllocator> allocator /* nullptr */)
{
    device = std::move(device_);
    fixup(pipelineInfos);
    std::vector<VkPipeline> handles(pipelineInfos.size(), VK_NULL_HANDLE);
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkCreateRayTracingPipelinesNV, VK_NV_RAY_TRACING_EXTENSION_NAME);
    const VkResult result = vkCreateRayTracingPipelinesNV(MAGMA_HANDLE(device), MAGMA_OPTIONAL_HANDLE(pipelineCache),
        MAGMA_COUNT(pipelineInfos), pipelineInfos.data(), allocator.get(), handles.data());
    postCreate();
    groups.clear();
    if (VK_SUCCESS == result)
    {
        auto handle = handles.cbegin();
        auto layout = layouts.cbegin();
        auto basePipeline = basePipelines.cbegin();
        auto info = pipelineInfos.cbegin();
        auto shaderStages = stages.cbegin();
    #ifdef VK_EXT_pipeline_creation_feedback
        auto creationFeedback = creationFeedbacks.cbegin();
        auto stageFeedbacks = stageCreationFeedbacks.cbegin();
    #endif // VK_EXT_pipeline_creation_feedback
        auto hash = hashes.cbegin();
        while (handle != handles.cend())
        {
            pipelines.emplace_back(RayTracingPipeline::makeShared(
                *handle++, device, *layout++, *basePipeline++, allocator,
                info->groupCount, info->maxRecursionDepth,
                MAGMA_COUNT(*shaderStages++),
            #ifdef VK_EXT_pipeline_creation_feedback
                creationFeedbacks.empty() ? VkPipelineCreationFeedbackEXT{} : *creationFeedback++,
                stageCreationFeedbacks.empty() ? std::vector<VkPipelineCreationFeedbackEXT>{} : *stageFeedbacks++,
            #endif // VK_EXT_pipeline_creation_feedback
                *hash++));
            ++info;
        }
    }
    postBuild();
    pipelineInfos.clear();
    postBuild();
    MAGMA_HANDLE_RESULT(result, "failed to create multiple ray tracing pipelines");
}
#endif // VK_NV_ray_tracing
} // namespace magma
