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
#include "pipelineLibrary.h"
#include "pipelineCache.h"
#include "device.h"
#include "../allocator/allocator.h"
#include "../misc/extension.h"
#include "../exceptions/errorResult.h"

namespace magma
{
#ifdef VK_NV_ray_tracing
RayTracingPipelineBatch::RayTracingPipelineBatch(std::shared_ptr<Device> device) noexcept:
    BasePipelineBatch<RayTracingPipeline, VkRayTracingPipelineCreateInfoNV>(std::move(device))
{}

uint32_t RayTracingPipelineBatch::batchPipeline(const std::vector<PipelineShaderStage>& shaderStages,
    const std::vector<RayTracingShaderGroup>& shaderGroups, uint32_t maxRecursionDepth, std::shared_ptr<PipelineLayout> layout,
    std::shared_ptr<RayTracingPipeline> basePipeline /* nullptr */,
    VkPipelineCreateFlags flags /* 0 */)
{
    stages.push_front(shaderStages);
    groups.push_front(shaderGroups);
    layouts.push_front(layout);
    basePipelines.push_front(basePipeline);
    VkRayTracingPipelineCreateInfoNV pipelineInfo;
    pipelineInfo.sType = VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_NV;
    pipelineInfo.pNext = nullptr;
    pipelineInfo.flags = flags;
    pipelineInfo.stageCount = MAGMA_COUNT(shaderStages);
    pipelineInfo.pStages = nullptr; // Fixup later
    pipelineInfo.groupCount = MAGMA_COUNT(shaderGroups);
    pipelineInfo.pGroups = groups.front().data();
    pipelineInfo.maxRecursionDepth = maxRecursionDepth;
    pipelineInfo.layout = *layouts.front();
    pipelineInfo.basePipelineHandle = MAGMA_OPTIONAL_HANDLE(basePipelines.front());
    pipelineInfo.basePipelineIndex = -1;
#ifdef VK_EXT_pipeline_creation_feedback
    if (device->extensionEnabled(VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME))
    {
        creationFeedbacks.push_front(VkPipelineCreationFeedbackEXT());
        stageCreationFeedbacks.emplace_front(pipelineInfo.stageCount);
        VkPipelineCreationFeedbackCreateInfoEXT creationFeedbackInfo;
        creationFeedbackInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CREATION_FEEDBACK_CREATE_INFO_EXT;
        creationFeedbackInfo.pNext = nullptr;
        creationFeedbackInfo.pPipelineCreationFeedback = &creationFeedbacks.front();
        creationFeedbackInfo.pipelineStageCreationFeedbackCount = pipelineInfo.stageCount;
        creationFeedbackInfo.pPipelineStageCreationFeedbacks = stageCreationFeedbacks.front().data();
        creationFeedbackInfos.push_front(creationFeedbackInfo);
        pipelineInfo.pNext = &creationFeedbackInfos.front();
    }
#endif // VK_EXT_pipeline_creation_feedback
    pipelineInfos.push_back(pipelineInfo);
    hash_t hash = core::hashArgs(
        pipelineInfo.sType,
        pipelineInfo.flags,
        pipelineInfo.stageCount,
        pipelineInfo.groupCount,
        pipelineInfo.maxRecursionDepth);
    for (auto const& stage: shaderStages)
        hash = core::hashCombine(hash, stage.getHash());
    for (auto const& group: shaderGroups)
        hash = core::hashCombine(hash, group.hash());
    hash = core::hashCombine(hash, layout->getHash());
    hashes.push_front(hash);
    return MAGMA_COUNT(pipelineInfos) - 1;
}

void RayTracingPipelineBatch::buildPipelines(std::shared_ptr<PipelineCache> pipelineCache /* nullptr */,
#ifdef VK_KHR_pipeline_library
    std::shared_ptr<PipelineLibrary> pipelineLibrary /* nullptr */,
#endif
    std::shared_ptr<IAllocator> allocator /* nullptr */)
{
    fixup();
#ifdef VK_KHR_pipeline_library
    if (device->extensionEnabled(VK_KHR_PIPELINE_LIBRARY_EXTENSION_NAME))
        linkPipelineLibrary(std::move(pipelineLibrary));
#endif
    std::vector<VkPipeline> handles(pipelineInfos.size(), VK_NULL_HANDLE);
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkCreateRayTracingPipelinesNV, VK_NV_RAY_TRACING_EXTENSION_NAME);
    const VkResult result = vkCreateRayTracingPipelinesNV(getNativeDevice(), MAGMA_OPTIONAL_HANDLE(pipelineCache),
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
            pipelines.emplace_front(RayTracingPipeline::makeShared(
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
