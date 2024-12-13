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
#include "rayTracingPipelineBatch.h"
#include "rayTracingPipeline.h"
#include "pipelineLayout.h"
#include "pipelineLibrary.h"
#include "pipelineCache.h"
#include "device.h"
#include "deferredOperation.h"
#include "../allocator/allocator.h"
#include "../misc/extension.h"
#include "../exceptions/errorResult.h"

namespace magma
{
#ifdef VK_KHR_ray_tracing_pipeline
RayTracingPipelineBatch::RayTracingPipelineBatch(std::shared_ptr<Device> device,
    std::shared_ptr<DeferredOperation> deferredOperation /* nullptr */) noexcept:
    BasePipelineBatch<RayTracingPipeline, VkRayTracingPipelineCreateInfoKHR>(std::move(device)),
    deferredOperation(std::move(deferredOperation))
{}

uint32_t RayTracingPipelineBatch::batchPipeline(const std::vector<PipelineShaderStage>& shaderStages,
    const std::vector<RayTracingShaderGroup>& shaderGroups, uint32_t maxRecursionDepth,
    core::variant_ptr<PipelineLayout> layout,
    std::shared_ptr<RayTracingPipeline> basePipeline /* nullptr */,
    const std::vector<VkDynamicState>& dynamicStates_ /* void */,
    VkPipelineCreateFlags flags /* 0 */,
    const StructureChain& extendedInfo /* void */)
{
    std::vector<VkShaderStageFlagBits> stageFlags;
    for (auto const& stage: shaderStages)
        stageFlags.push_back(stage.stage);
    stages.push_front(shaderStages);
    shaderStageFlags.push_front(stageFlags);
    groups.push_front(shaderGroups);
    maxRecursionDepths.push_front(maxRecursionDepth);
    dynamicStates.push_front(dynamicStates_);
    if (!layout)
        layout = std::make_unique<PipelineLayout>(device);
    basePipelines.push_front(basePipeline);
    VkPipelineDynamicStateCreateInfo dynamicStateInfo;
    dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicStateInfo.pNext = nullptr;
    dynamicStateInfo.flags = 0;
    dynamicStateInfo.dynamicStateCount = core::countof(dynamicStates.front());
    dynamicStateInfo.pDynamicStates = dynamicStates.front().data();
    dynamicStateInfos.push_front(dynamicStateInfo);
    VkRayTracingPipelineCreateInfoKHR pipelineInfo;
    pipelineInfo.sType = VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_KHR;
    pipelineInfo.pNext = extendedInfo.headNode();
    pipelineInfo.flags = flags;
    if (basePipeline)
        pipelineInfo.flags |= VK_PIPELINE_CREATE_DERIVATIVE_BIT;
    pipelineInfo.stageCount = core::countof(shaderStages);
    pipelineInfo.pStages = nullptr; // Fixup later
    pipelineInfo.groupCount = core::countof(groups.front());
    pipelineInfo.pGroups = groups.front().data();
    pipelineInfo.maxPipelineRayRecursionDepth = maxRecursionDepth;
    pipelineInfo.pLibraryInfo = nullptr;
    pipelineInfo.pLibraryInterface = nullptr;
    pipelineInfo.pDynamicState = &dynamicStateInfos.front();
    pipelineInfo.layout = *layout;
    pipelineInfo.basePipelineHandle = MAGMA_OPTIONAL_HANDLE(basePipeline);
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
        creationFeedbackInfo.pNext = nullptr;
        creationFeedbackInfo.pPipelineCreationFeedback = &creationFeedbacks.front();
        creationFeedbackInfo.pipelineStageCreationFeedbackCount = pipelineInfo.stageCount;
        creationFeedbackInfo.pPipelineStageCreationFeedbacks = stageCreationFeedbacks.front().data();
        creationFeedbackInfos.push_front(creationFeedbackInfo);
        linkNode(pipelineInfo, creationFeedbackInfos.front());
    }
#endif // VK_EXT_pipeline_creation_feedback
    pipelineInfos.push_back(pipelineInfo);
    const hash_t hash = psoHash(
        flags,
        shaderStages,
        shaderGroups,
        dynamicStates_,
        layout.get(),
        maxRecursionDepth,
        extendedInfo);
    hashes.push_front(hash);
    layouts.push_front(std::move(layout));
    return core::countof(pipelineInfos) - 1;
}

void RayTracingPipelineBatch::buildPipelines(const std::unique_ptr<PipelineCache>& pipelineCache /* nullptr */,
#ifdef VK_KHR_pipeline_library
    const std::unique_ptr<PipelineLibrary>& pipelineLibrary /* nullptr */,
#endif
    std::shared_ptr<IAllocator> allocator /* nullptr */)
{
    fixup();
    if (device->extensionEnabled(VK_KHR_PIPELINE_LIBRARY_EXTENSION_NAME))
        linkPipelineLibrary(std::move(pipelineLibrary));
    std::vector<VkPipeline> handles(pipelineInfos.size(), VK_NULL_HANDLE);
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkCreateRayTracingPipelinesKHR, VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME);
    const VkResult result = vkCreateRayTracingPipelinesKHR(getNativeDevice(),
        MAGMA_OPTIONAL_HANDLE(deferredOperation), MAGMA_OPTIONAL_HANDLE(pipelineCache),
        core::countof(pipelineInfos), pipelineInfos.data(), allocator.get(), handles.data());
    postCreate();
    groups.clear();
    dynamicStates.clear();
    dynamicStateInfos.clear();
    if (VK_SUCCESS == result)
    {
        auto handle = handles.cbegin();
        auto layout = layouts.begin();
        auto basePipeline = basePipelines.cbegin();
        auto pipelineInfo = pipelineInfos.cbegin();
        auto shaderStages = stages.cbegin();
        auto shaderGroups = groups.cbegin();
        auto maxRecursionDepth = maxRecursionDepths.cbegin();
    #ifdef VK_EXT_pipeline_creation_feedback
        auto creationFeedback = creationFeedbacks.cbegin();
        auto stageFeedbacks = stageCreationFeedbacks.cbegin();
    #endif // VK_EXT_pipeline_creation_feedback
        auto hash = hashes.cbegin();
        while (handle != handles.cend())
        {
            pipelines.emplace_front(RayTracingPipeline::makeUnique(
                *handle++, device, std::move(*layout++), *basePipeline++, allocator,
                *shaderStages++, *shaderGroups++, *maxRecursionDepth++,
            #ifdef VK_EXT_pipeline_creation_feedback
                creationFeedbacks.empty() ? VkPipelineCreationFeedbackEXT{} : *creationFeedback++,
                stageCreationFeedbacks.empty() ? std::vector<VkPipelineCreationFeedbackEXT>{} : *stageFeedbacks++,
            #endif // VK_EXT_pipeline_creation_feedback
                *hash++));
            ++pipelineInfo;
        }
    }
    postBuild();
    MAGMA_HANDLE_RESULT(result, "failed to create multiple ray tracing pipelines");
}
#endif // VK_KHR_ray_tracing_pipeline
} // namespace magma
