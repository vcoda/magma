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
    const std::vector<PipelineShaderStage>& stages,
    const std::vector<RayTracingShaderGroup>& groups,
    uint32_t maxRecursionDepth,
    std::shared_ptr<PipelineLayout> layout,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    std::shared_ptr<PipelineCache> pipelineCache /* nullptr */,
    std::shared_ptr<RayTracingPipeline> basePipeline /* nullptr */,
    VkPipelineCreateFlags flags /* 0 */):
    Pipeline(VK_PIPELINE_BIND_POINT_RAY_TRACING_NV, std::move(device), std::move(layout),
        std::move(pipelineCache), std::move(basePipeline), std::move(allocator)),
    groupCount(MAGMA_COUNT(groups)),
    maxRecursionDepth(maxRecursionDepth)
{
    if (stages.empty())
        MAGMA_THROW("shader stages are empty");
    if (groups.empty())
        MAGMA_THROW("shader groups are empty");
    VkRayTracingPipelineCreateInfoNV pipelineInfo;
    pipelineInfo.sType = VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_NV;
    pipelineInfo.pNext = nullptr;
    pipelineInfo.flags = flags;
    if (this->basePipeline)
        pipelineInfo.flags |= VK_PIPELINE_CREATE_DERIVATIVE_BIT;
    MAGMA_STACK_ARRAY(VkPipelineShaderStageCreateInfo, dereferencedStages, stages.size());
    for (auto& stage : stages)
        dereferencedStages.put(stage);
    pipelineInfo.stageCount = MAGMA_COUNT(dereferencedStages);
    pipelineInfo.pStages = dereferencedStages;
    pipelineInfo.groupCount = groupCount;
    pipelineInfo.pGroups = groups.data();
    pipelineInfo.maxRecursionDepth = maxRecursionDepth;
    pipelineInfo.layout = MAGMA_HANDLE(layout);
    pipelineInfo.basePipelineHandle = MAGMA_OPTIONAL_HANDLE(this->basePipeline);
    pipelineInfo.basePipelineIndex = -1;
    MAGMA_DEVICE_EXTENSION(vkCreateRayTracingPipelinesNV, VK_NV_RAY_TRACING_EXTENSION_NAME);
    const VkResult result = vkCreateRayTracingPipelinesNV(MAGMA_HANDLE(device), MAGMA_OPTIONAL_HANDLE(this->pipelineCache), 1, &pipelineInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to create ray tracing pipeline");
    hash = core::hashArgs(
        pipelineInfo.sType,
        pipelineInfo.flags,
        pipelineInfo.stageCount,
        pipelineInfo.groupCount,
        pipelineInfo.maxRecursionDepth);
    for (const auto& stage : stages)
        core::hashCombine(hash, stage.getHash());
    for (const auto& group : groups)
        core::hashCombine(hash, group.hash());
    core::hashCombine(hash, this->layout->getHash());
}

std::vector<uint8_t> RayTracingPipeline::getShaderGroupHandles() const
{
    const VkPhysicalDeviceRayTracingPropertiesNV& rayTracingProperties = device->getPhysicalDevice()->getRayTracingProperties();
    std::vector<uint8_t> shaderGroupHandles(groupCount * rayTracingProperties.shaderGroupHandleSize);
    MAGMA_DEVICE_EXTENSION(vkGetRayTracingShaderGroupHandlesNV, VK_NV_RAY_TRACING_EXTENSION_NAME);
    const VkResult get = vkGetRayTracingShaderGroupHandlesNV(MAGMA_HANDLE(device), handle, 0, groupCount, shaderGroupHandles.size(), shaderGroupHandles.data());
    MAGMA_THROW_FAILURE(get, "failed to get ray tracing shader handles");
    return shaderGroupHandles;
}

void RayTracingPipeline::compileDeferred(uint32_t shaderIndex)
{
    MAGMA_DEVICE_EXTENSION(vkCompileDeferredNV, VK_NV_RAY_TRACING_EXTENSION_NAME);
    const VkResult compile = vkCompileDeferredNV(MAGMA_HANDLE(device), handle, shaderIndex);
    MAGMA_THROW_FAILURE(compile, "failed to compile shader deferred");
}
#endif // VK_NV_ray_tracing
} // namespace magma
