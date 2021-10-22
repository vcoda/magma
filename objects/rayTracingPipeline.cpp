/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
#include "device.h"
#include "physicalDevice.h"
#include "../allocator/allocator.h"
#include "../misc/deviceExtension.h"
#include "../exceptions/errorResult.h"
#include "../helpers/stackArray.h"

namespace magma
{
#ifdef VK_NV_ray_tracing
RayTracingPipeline::RayTracingPipeline(std::shared_ptr<Device> device,
    const std::vector<PipelineShaderStage>& shaderStages,
    const std::vector<RayTracingShaderGroup>& shaderGroups,
    uint32_t maxRecursionDepth,
    std::shared_ptr<PipelineLayout> layout,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    std::shared_ptr<PipelineCache> pipelineCache /* nullptr */,
    std::shared_ptr<RayTracingPipeline> basePipeline /* nullptr */,
    VkPipelineCreateFlags flags /* 0 */):
    Pipeline(VK_PIPELINE_BIND_POINT_RAY_TRACING_NV, std::move(device), std::move(layout), std::move(basePipeline), std::move(allocator)),
    shaderGroupCount(MAGMA_COUNT(shaderGroups)),
    maxRecursionDepth(maxRecursionDepth)
{
    if (shaderStages.empty())
        MAGMA_THROW("shader stages are empty");
    if (shaderGroups.empty())
        MAGMA_THROW("shader groups are empty");
    VkRayTracingPipelineCreateInfoNV pipelineInfo;
    pipelineInfo.sType = VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_NV;
    pipelineInfo.pNext = nullptr;
    pipelineInfo.flags = flags;
    if (this->basePipeline)
        pipelineInfo.flags |= VK_PIPELINE_CREATE_DERIVATIVE_BIT;
    MAGMA_STACK_ARRAY(VkPipelineShaderStageCreateInfo, dereferencedStages, shaderStages.size());
    for (auto& stage : shaderStages)
        dereferencedStages.put(stage);
    pipelineInfo.stageCount = MAGMA_COUNT(dereferencedStages);
    pipelineInfo.pStages = dereferencedStages;
    pipelineInfo.groupCount = shaderGroupCount;
    pipelineInfo.pGroups = shaderGroups.data();
    pipelineInfo.maxRecursionDepth = maxRecursionDepth;
    pipelineInfo.layout = MAGMA_HANDLE(layout);
    pipelineInfo.basePipelineHandle = MAGMA_OPTIONAL_HANDLE(this->basePipeline);
    pipelineInfo.basePipelineIndex = -1;
    MAGMA_DEVICE_EXTENSION(vkCreateRayTracingPipelinesNV, VK_NV_RAY_TRACING_EXTENSION_NAME);
    const VkResult result = vkCreateRayTracingPipelinesNV(MAGMA_HANDLE(device), MAGMA_OPTIONAL_HANDLE(pipelineCache), 1, &pipelineInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to create ray tracing pipeline");
    hash = core::hashArgs(
        pipelineInfo.sType,
        pipelineInfo.flags,
        pipelineInfo.stageCount,
        pipelineInfo.groupCount,
        pipelineInfo.maxRecursionDepth);
    for (const auto& stage : shaderStages)
        core::hashCombine(hash, stage.getHash());
    for (const auto& group : shaderGroups)
        core::hashCombine(hash, group.hash());
    core::hashCombine(hash, this->layout->getHash());
}

std::vector<uint8_t> RayTracingPipeline::getShaderGroupHandles() const
{
    const VkPhysicalDeviceRayTracingPropertiesNV& rayTracingProperties = device->getPhysicalDevice()->getRayTracingProperties();
    std::vector<uint8_t> shaderGroupHandles(shaderGroupCount * rayTracingProperties.shaderGroupHandleSize);
    MAGMA_DEVICE_EXTENSION(vkGetRayTracingShaderGroupHandlesNV, VK_NV_RAY_TRACING_EXTENSION_NAME);
    const VkResult get = vkGetRayTracingShaderGroupHandlesNV(MAGMA_HANDLE(device), handle, 0, shaderGroupCount, shaderGroupHandles.size(), shaderGroupHandles.data());
    MAGMA_THROW_FAILURE(get, "failed to get ray tracing shader handles");
    return shaderGroupHandles;
}

void RayTracingPipeline::compileDeferred(uint32_t shaderIndex)
{
    MAGMA_DEVICE_EXTENSION(vkCompileDeferredNV, VK_NV_RAY_TRACING_EXTENSION_NAME);
    const VkResult compile = vkCompileDeferredNV(MAGMA_HANDLE(device), handle, shaderIndex);
    MAGMA_THROW_FAILURE(compile, "failed to compile shader deferred");
}

RayTracingPipeline::RayTracingPipeline(VkPipeline pipeline,
    std::shared_ptr<Device> device,
    std::shared_ptr<PipelineLayout> layout,
    std::shared_ptr<Pipeline> basePipeline,
    std::shared_ptr<IAllocator> allocator,
    uint32_t shaderGroupCount,
    uint32_t maxRecursionDepth,
    std::size_t hash):
    Pipeline(VK_PIPELINE_BIND_POINT_RAY_TRACING_NV, std::move(device), std::move(layout), std::move(basePipeline), std::move(allocator), hash),
    shaderGroupCount(shaderGroupCount),
    maxRecursionDepth(maxRecursionDepth)
{
    handle = pipeline;
}

RayTracingPipelines::RayTracingPipelines(std::size_t capacity /* 32 */)
{
    pipelineInfos.reserve(capacity);
    rayTracingPipelines.reserve(capacity);
}

uint32_t RayTracingPipelines::newPipeline(const std::vector<PipelineShaderStage>& shaderStages,
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
    pipelineInfos.push_back(pipelineInfo);
    std::size_t hash = core::hashArgs(
        pipelineInfo.sType,
        pipelineInfo.flags,
        pipelineInfo.stageCount,
        pipelineInfo.groupCount,
        pipelineInfo.maxRecursionDepth);
    for (const auto& stage : shaderStages)
        core::hashCombine(hash, stage.getHash());
    for (const auto& group : shaderGroups)
        core::hashCombine(hash, group.hash());
    core::hashCombine(hash, layout->getHash());
    hashes.push_back(hash);
    return MAGMA_COUNT(pipelineInfos) - 1;
}

void RayTracingPipelines::buildPipelines(std::shared_ptr<Device> device, std::shared_ptr<PipelineCache> pipelineCache,
    std::shared_ptr<IAllocator> allocator /* nullptr */)
{
    this->device = std::move(device);
    fixup(pipelineInfos);
    std::vector<VkPipeline> pipelines(pipelineInfos.size(), VK_NULL_HANDLE);
    MAGMA_DEVICE_EXTENSION(vkCreateRayTracingPipelinesNV, VK_NV_RAY_TRACING_EXTENSION_NAME);
    const VkResult result = vkCreateRayTracingPipelinesNV(MAGMA_HANDLE(device), MAGMA_OPTIONAL_HANDLE(pipelineCache),
        MAGMA_COUNT(pipelineInfos), pipelineInfos.data(), allocator.get(), pipelines.data());
    // Free temporarily allocated storage that had to be preserved until API call
    stages.clear();
    groups.clear();
    if (VK_SUCCESS == result)
    {
        auto handle = pipelines.cbegin();
        auto layout = layouts.cbegin();
        auto basePipeline = basePipelines.cbegin();
        auto hash = hashes.cbegin();
        auto info = pipelineInfos.cbegin();
        rayTracingPipelines.clear();
        while (handle != pipelines.cend())
        {
            rayTracingPipelines.emplace_back(new RayTracingPipeline(*handle++, this->device, *layout++, *basePipeline++, allocator,
                info->groupCount, info->maxRecursionDepth, *hash++));
            ++info;
        }
    }
    layouts.clear();
    basePipelines.clear();
    hashes.clear();
    pipelineInfos.clear();
    MAGMA_THROW_FAILURE(result, "failed to create multiple ray tracing pipelines");
}
#endif // VK_NV_ray_tracing
} // namespace magma
