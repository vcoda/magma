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
#include "rayTracingPipeline.h"
#include "pipelineLayout.h"
#include "pipelineCache.h"
#include "pipelineLibrary.h"
#include "device.h"
#include "physicalDevice.h"
#include "../shaders/pipelineShaderStage.h"
#include "../shaders/rayTracingShaderGroup.h"
#include "../allocator/allocator.h"
#include "../misc/extension.h"
#include "../exceptions/errorResult.h"
#include "../helpers/stackArray.h"

namespace magma
{
#ifdef VK_NV_ray_tracing
RayTracingPipeline::RayTracingPipeline(std::shared_ptr<Device> device_,
    const std::vector<PipelineShaderStage>& shaderStages,
    const std::vector<RayTracingShaderGroup>& shaderGroups,
    uint32_t maxRecursionDepth,
    std::shared_ptr<PipelineLayout> layout,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    std::shared_ptr<PipelineCache> pipelineCache /* nullptr */,
#ifdef VK_KHR_pipeline_library
    std::shared_ptr<PipelineLibrary> pipelineLibrary /* nullptr */,
#endif
    std::shared_ptr<RayTracingPipeline> basePipeline_ /* nullptr */,
    VkPipelineCreateFlags flags /* 0 */,
    const StructureChain& extendedInfo /* default */):
    Pipeline(VK_PIPELINE_BIND_POINT_RAY_TRACING_NV, std::move(device_), std::move(layout), std::move(basePipeline_), std::move(allocator), MAGMA_COUNT(shaderStages)),
    shaderGroupCount(MAGMA_COUNT(shaderGroups)),
    maxRecursionDepth(maxRecursionDepth)
{
    if (shaderStages.empty())
        MAGMA_ERROR("shader stages are empty");
    if (shaderGroups.empty())
        MAGMA_ERROR("shader groups are empty");
    VkRayTracingPipelineCreateInfoNV pipelineInfo;
    pipelineInfo.sType = VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_NV;
    pipelineInfo.pNext = extendedInfo.headNode();
    pipelineInfo.flags = flags;
    if (basePipeline)
        pipelineInfo.flags |= VK_PIPELINE_CREATE_DERIVATIVE_BIT;
    MAGMA_STACK_ARRAY(VkPipelineShaderStageCreateInfo, dereferencedStages, shaderStages.size());
    for (auto& stage: shaderStages)
        dereferencedStages.put(stage);
    pipelineInfo.stageCount = MAGMA_COUNT(dereferencedStages);
    pipelineInfo.pStages = dereferencedStages;
    pipelineInfo.groupCount = shaderGroupCount;
    pipelineInfo.pGroups = shaderGroups.data();
    pipelineInfo.maxRecursionDepth = maxRecursionDepth;
    pipelineInfo.layout = MAGMA_HANDLE(layout);
    pipelineInfo.basePipelineHandle = MAGMA_OPTIONAL_HANDLE(basePipeline);
    pipelineInfo.basePipelineIndex = -1;
#ifdef VK_EXT_pipeline_creation_feedback
    VkPipelineCreationFeedbackCreateInfoEXT pipelineCreationFeedbackInfo;
    if (device->extensionEnabled(VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME))
    {
        stageCreationFeedbacks.resize(pipelineInfo.stageCount);
        pipelineCreationFeedbackInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CREATION_FEEDBACK_CREATE_INFO_EXT;
        pipelineCreationFeedbackInfo.pNext = pipelineInfo.pNext;
        pipelineCreationFeedbackInfo.pPipelineCreationFeedback = &creationFeedback;
        pipelineCreationFeedbackInfo.pipelineStageCreationFeedbackCount = pipelineInfo.stageCount;
        pipelineCreationFeedbackInfo.pPipelineStageCreationFeedbacks = stageCreationFeedbacks.data();
        pipelineInfo.pNext = &pipelineCreationFeedbackInfo;
    }
#endif // VK_EXT_pipeline_creation_feedback
#ifdef VK_KHR_pipeline_library
    VkPipelineLibraryCreateInfoKHR pipelineLibraryInfo;
    if (pipelineLibrary && device->extensionEnabled(VK_KHR_PIPELINE_LIBRARY_EXTENSION_NAME))
    {
        pipelineLibraryInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LIBRARY_CREATE_INFO_KHR;
        pipelineLibraryInfo.pNext = nullptr;
        pipelineLibraryInfo.libraryCount = pipelineLibrary->getLibraryCount();
        pipelineLibraryInfo.pLibraries = pipelineLibrary->getLibraries();
        linkNode(pipelineInfo, pipelineLibraryInfo);
    }
#endif // VK_KHR_pipeline_library
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkCreateRayTracingPipelinesNV, VK_NV_RAY_TRACING_EXTENSION_NAME);
    const VkResult result = vkCreateRayTracingPipelinesNV(getNativeDevice(), MAGMA_OPTIONAL_HANDLE(pipelineCache),
        1, &pipelineInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create ray tracing pipeline");
    hash = core::hashArgs(
        pipelineInfo.sType,
        pipelineInfo.flags,
        pipelineInfo.stageCount,
        pipelineInfo.groupCount,
        pipelineInfo.maxRecursionDepth);
    for (auto const& stage: shaderStages)
        hash = core::hashCombine(hash, stage.getHash());
    for (auto const& group: shaderGroups)
        hash = core::hashCombine(hash, group.hash());
    hash = core::hashCombine(hash, this->layout->getHash());
}

RayTracingPipeline::RayTracingPipeline(VkPipeline handle_,
    std::shared_ptr<Device> device,
    std::shared_ptr<PipelineLayout> layout,
    std::shared_ptr<Pipeline> basePipeline,
    std::shared_ptr<IAllocator> allocator,
    uint32_t stageCount,
    uint32_t shaderGroupCount,
    uint32_t maxRecursionDepth,
#ifdef VK_EXT_pipeline_creation_feedback
    VkPipelineCreationFeedbackEXT creationFeedback,
    const std::vector<VkPipelineCreationFeedbackEXT>& stageCreationFeedbacks,
#endif // VK_EXT_pipeline_creation_feedback
    hash_t hash):
    Pipeline(VK_PIPELINE_BIND_POINT_RAY_TRACING_NV, std::move(device), std::move(layout), std::move(basePipeline), std::move(allocator),
        stageCount,
    #ifdef VK_EXT_pipeline_creation_feedback
        creationFeedback, stageCreationFeedbacks,
    #endif
        hash),
    shaderGroupCount(shaderGroupCount),
    maxRecursionDepth(maxRecursionDepth)
{
    handle = handle_;
}

std::vector<uint8_t> RayTracingPipeline::getShaderGroupHandles() const
{
    const VkPhysicalDeviceRayTracingPropertiesNV& rayTracingProperties = device->getPhysicalDevice()->getRayTracingProperties();
    std::vector<uint8_t> shaderGroupHandles(shaderGroupCount * rayTracingProperties.shaderGroupHandleSize);
    MAGMA_DEVICE_EXTENSION(vkGetRayTracingShaderGroupHandlesNV);
    const VkResult result = vkGetRayTracingShaderGroupHandlesNV(getNativeDevice(), handle, 0, shaderGroupCount, shaderGroupHandles.size(), shaderGroupHandles.data());
    MAGMA_HANDLE_RESULT(result, "failed to get ray tracing shader handles");
    return shaderGroupHandles;
}

void RayTracingPipeline::compileDeferred(uint32_t shaderIndex)
{
    MAGMA_DEVICE_EXTENSION(vkCompileDeferredNV);
    const VkResult result = vkCompileDeferredNV(getNativeDevice(), handle, shaderIndex);
    MAGMA_HANDLE_RESULT(result, "failed to compile shader deferred");
}
#endif // VK_NV_ray_tracing
} // namespace magma
