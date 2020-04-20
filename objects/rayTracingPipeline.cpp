/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2019 Victor Coda.

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
#include "../allocator/allocator.h"
#include "../misc/deviceExtension.h"
#include "../misc/exception.h"
#include "../helpers/stackArray.h"

namespace magma
{
#ifdef VK_NV_ray_tracing
RayTracingPipeline::RayTracingPipeline(std::shared_ptr<Device> device,
    const std::vector<PipelineShaderStage>& stages,
    const std::vector<RayTracingShaderGroup>& groups,
    uint32_t maxRecursionDepth,
    std::shared_ptr<PipelineLayout> layout,
    std::shared_ptr<PipelineCache> pipelineCache /* nullptr */,
    std::shared_ptr<RayTracingPipeline> basePipeline /* nullptr */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkPipelineCreateFlags flags /* 0 */):
    Pipeline(VK_PIPELINE_BIND_POINT_RAY_TRACING_NV, std::move(device), std::move(layout),
        std::move(pipelineCache), std::move(basePipeline), std::move(allocator))
{
    if (stages.empty())
        MAGMA_THROW("shader stages are empty");
    if (groups.empty())
        MAGMA_THROW("shader groups are empty");
    VkRayTracingPipelineCreateInfoNV info;
    info.sType = VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_NV;
    info.pNext = nullptr;
    info.flags = flags;
    if (this->basePipeline)
        info.flags |= VK_PIPELINE_CREATE_DERIVATIVE_BIT;
    MAGMA_STACK_ARRAY(VkPipelineShaderStageCreateInfo, dereferencedStages, stages.size());
    for (auto& stage : stages)
        dereferencedStages.put(stage);
    info.stageCount = MAGMA_COUNT(dereferencedStages);
    info.pStages = dereferencedStages;
    info.groupCount = MAGMA_COUNT(groups);
    info.pGroups = groups.data();
    info.maxRecursionDepth = maxRecursionDepth;
    info.layout = MAGMA_HANDLE(layout);
    info.basePipelineHandle = MAGMA_OPTIONAL_HANDLE(this->basePipeline);
    info.basePipelineIndex = -1;
    MAGMA_DEVICE_EXTENSION(vkCreateRayTracingPipelinesNV, VK_NV_RAY_TRACING_EXTENSION_NAME);
    const VkResult create = vkCreateRayTracingPipelinesNV(MAGMA_HANDLE(device), MAGMA_OPTIONAL_HANDLE(this->pipelineCache), 1, &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create ray tracing pipeline");
    hash = core::hashArgs(
        info.sType,
        info.flags,
        info.stageCount,
        info.groupCount,
        info.maxRecursionDepth);
    for (const auto& stage : stages)
        core::hashCombine(hash, stage.getHash());
    for (const auto& group : groups)
        core::hashCombine(hash, group.hash());
    core::hashCombine(hash, this->layout->getHash());
}

std::vector<VkShaderModule> RayTracingPipeline::getShaderGroupHandles(uint32_t firstGroup, uint32_t groupCount) const
{
    std::vector<VkShaderModule> shaders(groupCount);
    const std::size_t dataSize = sizeof(VkShaderModule) * groupCount;
    MAGMA_DEVICE_EXTENSION(vkGetRayTracingShaderGroupHandlesNV, VK_NV_RAY_TRACING_EXTENSION_NAME);
    const VkResult get = vkGetRayTracingShaderGroupHandlesNV(MAGMA_HANDLE(device), handle, firstGroup, groupCount, dataSize, shaders.data());
    MAGMA_THROW_FAILURE(get, "failed to get ray tracing shader handles");
    return shaders;
}

void RayTracingPipeline::compileDeferred(uint32_t shaderIndex)
{
    MAGMA_DEVICE_EXTENSION(vkCompileDeferredNV, VK_NV_RAY_TRACING_EXTENSION_NAME);
    const VkResult compile = vkCompileDeferredNV(MAGMA_HANDLE(device), handle, shaderIndex);
    MAGMA_THROW_FAILURE(compile, "failed to compile shader deferred");
}
#endif // VK_NV_ray_tracing
} // namespace magma
