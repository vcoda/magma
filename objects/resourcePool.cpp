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
#include "../objects/image.h"
#include "../objects/pipeline.h"

namespace magma
{
ResourcePool::DeviceResources ResourcePool::countDeviceResources() const noexcept
{
    DeviceResources statistics;
    statistics.semaphoreCount = semaphores.count();
    statistics.fenceCount = fences.count();
    statistics.deviceMemoryCount = deviceMemories.count();
    statistics.bufferCount = buffers.count();
    images.forEach<Image>([&statistics](const Image *image) {
        switch (image->getType())
        {
        case VK_IMAGE_TYPE_1D:
            if (image->getArrayLayers() == 1)
                ++statistics.image1DCount;
            else
                ++statistics.image1DArrayCount;
            break;
        case VK_IMAGE_TYPE_2D:
            if (image->getArrayLayers() == 1)
                ++statistics.image2DCount;
            else
                ++statistics.image2DArrayCount;
            break;
        case VK_IMAGE_TYPE_3D:
            ++statistics.image3DCount;
            break;
        }
    });
    statistics.eventCount = events.count();
    statistics.queryPoolCount = queryPools.count();
    statistics.bufferViewCount = bufferViews.count();
    statistics.imageViewCount = imageViews.count();
    statistics.shaderModuleCount = shaderModules.count();
    statistics.pipelineCacheCount = pipelineCaches.count();
    statistics.pipelineLayoutCount = pipelineLayouts.count();
    statistics.renderPassCount = renderPasses.count();
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
#ifdef VK_KHR_swapchain
    statistics.swapchainCount = swapchains.count();
#endif
#ifdef VK_NV_ray_tracing
    statistics.accelerationStructureCount = accelerationStructures.count();
#endif
    return statistics;
}

ResourcePool::PhysicalDeviceResources ResourcePool::countPhysicalDeviceResources() const noexcept
{
    PhysicalDeviceResources statistics;
#ifdef VK_KHR_display
    statistics.displayCount = displays.count();
    statistics.displayModeCount = displayModes.count();
#endif
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
#ifdef VK_KHR_swapchain
        swapchains.count() > 0 ||
#endif
#ifdef VK_NV_ray_tracing
        accelerationStructures.count() > 0 ||
#endif
        false;
}
} // namespace magma
