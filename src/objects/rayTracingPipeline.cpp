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
#include "rayTracingPipeline.h"
#include "pipelineLayout.h"
#include "pipelineCache.h"
#include "pipelineLibrary.h"
#include "device.h"
#include "physicalDevice.h"
#include "deferredOperation.h"
#include "../shaders/pipelineShaderStage.h"
#include "../allocator/allocator.h"
#include "../misc/extension.h"
#include "../exceptions/errorResult.h"
#include "../helpers/alignment.h"

namespace magma
{
#ifdef VK_KHR_ray_tracing_pipeline
RayTracingPipeline::RayTracingPipeline(std::shared_ptr<Device> device_, const std::vector<PipelineShaderStage>& shaderStages_,
    const std::vector<RayTracingShaderGroup>& shaderGroups_, uint32_t maxRecursionDepth, core::variant_ptr<PipelineLayout> layout_,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    lent_ptr<PipelineCache> pipelineCache /* nullptr */,
#ifdef VK_KHR_pipeline_library
    lent_ptr<PipelineLibrary> pipelineLibrary /* nullptr */,
#endif
    std::shared_ptr<RayTracingPipeline> basePipeline_ /* nullptr */,
    lent_ptr<DeferredOperation> deferredOperation /* nullptr */,
    const std::vector<VkDynamicState>& dynamicStates /* empty */,
    VkPipelineCreateFlags flags /* 0 */,
    const StructureChain& extendedInfo /* default */):
    Pipeline(VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR, std::move(device_), std::move(layout_), std::move(basePipeline_), std::move(allocator), core::countof(shaderStages_)),
    shaderStages(shaderStages_),
    shaderGroups(shaderGroups_),
    maxRecursionDepth(maxRecursionDepth)
{
    MAGMA_VLA(VkPipelineShaderStageCreateInfo, dereferencedStages, shaderStages.size());
    for (auto const& stage: shaderStages)
        dereferencedStages.put(stage);
    VkRayTracingPipelineCreateInfoKHR pipelineInfo;
    VkRayTracingPipelineInterfaceCreateInfoKHR pipelineInterfaceInfo = {};
    VkPipelineLibraryCreateInfoKHR pipelineLibraryInfo = {};
    VkPipelineDynamicStateCreateInfo pipelineDynamicStateInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_KHR;
    pipelineInfo.pNext = extendedInfo.headNode();
    pipelineInfo.flags = flags;
    if (!basePipeline.expired())
        pipelineInfo.flags |= VK_PIPELINE_CREATE_DERIVATIVE_BIT;
    pipelineInfo.stageCount = dereferencedStages.count();
    pipelineInfo.pStages = dereferencedStages;
    pipelineInfo.groupCount = core::countof(shaderGroups);
    pipelineInfo.pGroups = shaderGroups.data();
    pipelineInfo.maxPipelineRayRecursionDepth = maxRecursionDepth;
    pipelineInfo.pLibraryInfo = nullptr;
    pipelineInfo.pLibraryInterface = nullptr;
    pipelineInfo.pDynamicState = nullptr;
    pipelineInfo.layout = *layout;
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
    if (pipelineLibrary)
    {
        pipelineInfo.pLibraryInfo = &pipelineLibraryInfo;
        pipelineInfo.pLibraryInterface = &pipelineInterfaceInfo;
        pipelineLibraryInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LIBRARY_CREATE_INFO_KHR;
        pipelineLibraryInfo.pNext = nullptr;
        pipelineLibraryInfo.libraryCount = pipelineLibrary->getLibraryCount();
        pipelineLibraryInfo.pLibraries = pipelineLibrary->getLibraries();
        pipelineInterfaceInfo.sType = VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_INTERFACE_CREATE_INFO_KHR;
        pipelineInterfaceInfo.pNext = nullptr;
        pipelineInterfaceInfo.maxPipelineRayPayloadSize = 0; // TODO
        pipelineInterfaceInfo.maxPipelineRayHitAttributeSize = 0; // TODO
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
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkCreateRayTracingPipelinesKHR, VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME);
    const VkResult result = vkCreateRayTracingPipelinesKHR(getNativeDevice(),
        MAGMA_OPTIONAL_HANDLE(deferredOperation), MAGMA_OPTIONAL_HANDLE(pipelineCache),
        1, &pipelineInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    if (result != VK_SUCCESS)
    {
    #ifdef MAGMA_DEBUG
        for (auto const& stage: shaderStages)
            std::cerr << stage << std::endl;
        // TODO: additional diagnostic output
    #endif // MAGMA_DEBUG
        MAGMA_HANDLE_RESULT(result, "failed to create ray tracing pipeline");
    }
    hash = psoHash(
        flags,
        shaderStages,
        shaderGroups,
        dynamicStates,
        layout.get(),
        maxRecursionDepth,
        extendedInfo);
}

RayTracingPipeline::RayTracingPipeline(VkPipeline handle_, std::shared_ptr<Device> device, core::variant_ptr<PipelineLayout> layout,
    std::shared_ptr<Pipeline> basePipeline, std::shared_ptr<IAllocator> allocator, const std::vector<PipelineShaderStage>& shaderStages,
    const std::vector<RayTracingShaderGroup>& shaderGroups, uint32_t maxRecursionDepth,
#ifdef VK_EXT_pipeline_creation_feedback
    VkPipelineCreationFeedbackEXT creationFeedback,
    const std::vector<VkPipelineCreationFeedbackEXT>& stageCreationFeedbacks,
#endif // VK_EXT_pipeline_creation_feedback
    hash_t hash):
    Pipeline(VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR, std::move(device), std::move(layout), std::move(basePipeline), std::move(allocator),
        core::countof(shaderStages),
    #ifdef VK_EXT_pipeline_creation_feedback
        creationFeedback, stageCreationFeedbacks,
    #endif
        hash),
    shaderStages(shaderStages),
    shaderGroups(shaderGroups),
    maxRecursionDepth(maxRecursionDepth)
{
    handle = handle_;
}

VkDeviceSize RayTracingPipeline::getGeneralShaderStackSize(uint32_t group) const noexcept
{
    return getShaderGroupStackSize(group, VK_SHADER_GROUP_SHADER_GENERAL_KHR);
}

VkDeviceSize RayTracingPipeline::getClosestHitShaderStackSize(uint32_t group) const noexcept
{
    return getShaderGroupStackSize(group, VK_SHADER_GROUP_SHADER_CLOSEST_HIT_KHR);
}

VkDeviceSize RayTracingPipeline::getAnyHitShaderStackSize(uint32_t group) const noexcept
{
    return getShaderGroupStackSize(group, VK_SHADER_GROUP_SHADER_ANY_HIT_KHR);
}

VkDeviceSize RayTracingPipeline::getIntersectionShaderStackSize(uint32_t group) const noexcept
{
    return getShaderGroupStackSize(group, VK_SHADER_GROUP_SHADER_INTERSECTION_KHR);
}

std::vector<uint8_t> RayTracingPipeline::getShaderGroupHandles() const
{
    const auto rayTracingPipelineProperties = device->getPhysicalDevice()->getRayTracingPipelineProperties();
    const uint32_t shaderGroupCount = core::countof(shaderGroups);
    std::vector<uint8_t> shaderGroupHandles(shaderGroupCount * rayTracingPipelineProperties.shaderGroupHandleSize);
    MAGMA_DEVICE_EXTENSION(vkGetRayTracingShaderGroupHandlesKHR);
    const VkResult result = vkGetRayTracingShaderGroupHandlesKHR(getNativeDevice(),
        handle, 0, shaderGroupCount, shaderGroupHandles.size(), shaderGroupHandles.data());
    MAGMA_HANDLE_RESULT(result, "failed to get shader group handles");
    return shaderGroupHandles;
}

std::vector<uint8_t> RayTracingPipeline::getCaptureReplayShaderGroupHandles() const
{
    const auto rayTracingPipelineProperties = device->getPhysicalDevice()->getRayTracingPipelineProperties();
    const uint32_t shaderGroupCount = core::countof(shaderGroups);
    std::vector<uint8_t> shaderGroupHandles(shaderGroupCount * rayTracingPipelineProperties.shaderGroupHandleSize);
    MAGMA_DEVICE_EXTENSION(vkGetRayTracingCaptureReplayShaderGroupHandlesKHR);
    const VkResult result = vkGetRayTracingCaptureReplayShaderGroupHandlesKHR(getNativeDevice(),
        handle, 0, shaderGroupCount, shaderGroupHandles.size(), shaderGroupHandles.data());
    MAGMA_HANDLE_RESULT(result, "failed to get shader group handles of capture replay");
    return shaderGroupHandles;
}

VkDeviceSize RayTracingPipeline::getShaderGroupStackSize(uint32_t group, VkShaderGroupShaderKHR groupShader) const noexcept
{
    MAGMA_DEVICE_EXTENSION(vkGetRayTracingShaderGroupStackSizeKHR);
    return vkGetRayTracingShaderGroupStackSizeKHR(getNativeDevice(), handle, group, groupShader);
}

hash_t psoHash(VkPipelineCreateFlags flags,
    const std::vector<PipelineShaderStage>& shaderStages,
    const std::vector<RayTracingShaderGroup>& shaderGroups,
    const std::vector<VkDynamicState>& dynamicStates,
    const PipelineLayout *layout,
    uint32_t maxRecursionDepth,
    const StructureChain& extendedInfo /* default */) noexcept
{   // Erase flags that do not affect pipeline states
    // TODO: maybe clear some extension flags too?
    flags = flags & ~(VK_PIPELINE_CREATE_DISABLE_OPTIMIZATION_BIT |
                      VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT |
                      VK_PIPELINE_CREATE_DERIVATIVE_BIT);
    hash_t hash = core::hashArgs(flags, maxRecursionDepth);
    for (auto const& stage: shaderStages)
        hash = core::hashCombine(hash, stage.getHash());
    for (auto const& group: shaderGroups)
        hash = core::hashCombine(hash, group.hash());
    for (auto state: dynamicStates)
        hash = core::hashCombine(hash, core::hash(state));
    hash = core::hashCombine(hash, layout->getHash());
    if (!extendedInfo.empty()) // TODO: not all extended info may affect pipeline state
        hash = core::hashCombine(hash, extendedInfo.hash());
    return hash;
}
#endif // VK_KHR_ray_tracing_pipeline
} // namespace magma
