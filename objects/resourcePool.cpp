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
#include "resourcePool.h"
#include "../objects/deviceMemory.h"
#include "../objects/pipeline.h"

namespace magma
{
ResourcePool::ResourceStatistics ResourcePool::countResources() const noexcept
{
    ResourceStatistics statistics;
    statistics.semaphoreCount = semaphores.count();
    statistics.fenceCount = fences.count();
    statistics.deviceMemoryCount = deviceMemories.count();
    statistics.bufferCount = buffers.count();
    statistics.imageCount = images.count();
    statistics.eventCount = events.count();
    statistics.queryPoolCount = queryPools.count();
    statistics.bufferViewCount = bufferViews.count();
    statistics.imageViewCount = imageViews.count();
    statistics.shaderModuleCount = shaderModules.count();
    statistics.pipelineCacheCount = pipelineCaches.count();
    statistics.pipelineLayoutCount = pipelineLayouts.count();
    statistics.renderPassCount = renderPasses.count();
    statistics.graphicsPipelineCount = 0;
    statistics.computePipelineCount = 0;
    statistics.rayTracingPipelineCount = 0;
    pipelines.forEach<Pipeline>([&statistics](const Pipeline *pipeline) {
        switch (pipeline->getBindPoint())
        {
        case VK_PIPELINE_BIND_POINT_GRAPHICS:
            ++statistics.graphicsPipelineCount;
            break;
        case VK_PIPELINE_BIND_POINT_COMPUTE:
            ++statistics.computePipelineCount;
            break;
#ifdef VK_NV_ray_tracing
        case VK_PIPELINE_BIND_POINT_RAY_TRACING_NV:
            ++statistics.rayTracingPipelineCount;
            break;
#endif
        }
    });
    statistics.descriptorSetLayoutCount = descriptorSetLayouts.count();
    statistics.samplerCount = samplers.count();
    statistics.descriptorPoolCount = descriptorPools.count();
    statistics.descriptorSetCount = descriptorSets.count();
    statistics.framebufferCount = framebuffers.count();
    statistics.commandPoolCount = commandPools.count();
    // Non-core types
    statistics.surfaceCount = surfaces.count();
    statistics.swapchainCount = swapchains.count();
    statistics.displayCount = displays.count();
    statistics.debugReportCallbackCount = debugReportCallbacks.count();
    return statistics;
}

VkDeviceSize ResourcePool::countAllocatedDeviceLocalMemory() const noexcept
{
    VkDeviceSize allocatedSize = 0;
    deviceMemories.forEach<DeviceMemory>([&allocatedSize](const DeviceMemory *memory) {
        if (memory->local())
            allocatedSize += memory->getSize();
    });
    return allocatedSize;
}

VkDeviceSize ResourcePool::countAllocatedHostVisibleMemory() const noexcept
{
    VkDeviceSize allocatedSize = 0;
    deviceMemories.forEach<DeviceMemory>([&allocatedSize](const DeviceMemory *memory) {
        if (memory->hostVisible())
            allocatedSize += memory->getSize();
    });
    return allocatedSize;
}

bool ResourcePool::hasAnyResource() const noexcept
{
    return semaphores.count() > 0 ||
        fences.count() > 0 ||
        deviceMemories.count() > 0 ||
        buffers.count() > 0 ||
        images.count() > 0 ||
        events.count() > 0 ||
        queryPools.count() > 0 ||
        bufferViews.count() > 0 ||
        imageViews.count() > 0 ||
        shaderModules.count() > 0 ||
        pipelineCaches.count() > 0 ||
        pipelineLayouts.count() > 0 ||
        renderPasses.count() > 0 ||
        pipelines.count() > 0 ||
        descriptorSetLayouts.count() > 0 ||
        samplers.count() > 0 ||
        descriptorPools.count() > 0 ||
        descriptorSets.count() > 0 ||
        framebuffers.count() > 0 ||
        commandPools.count() > 0 ||
        surfaces.count() > 0 ||
        swapchains.count() > 0 ||
        displays.count() > 0 ||
        debugReportCallbacks.count() > 0;
}
} // namespace magma
