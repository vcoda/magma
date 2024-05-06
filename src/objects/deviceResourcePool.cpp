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
#include "deviceResourcePool.h"
#include "device.h"
#include "deviceMemory.h"
#include "buffer.h"
#include "image.h"
#include "accelerationStructure.h"
#include "pipeline.h"

namespace magma
{
#if (VK_USE_64_BIT_PTR_DEFINES == 1)
DeviceResourcePool::Resources DeviceResourcePool::countDeviceResources() const
{
    std::lock_guard<std::mutex> guard(mtx);
    Resources statistics;
    statistics.semaphoreCount = MAGMA_COUNT(semaphores);
    statistics.fenceCount = MAGMA_COUNT(fences);
    statistics.deviceMemoryCount = MAGMA_COUNT(deviceMemories);
    statistics.bufferCount = MAGMA_COUNT(buffers);
    foreach<Image>(images,
        [&statistics](const Image *image)
        {
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
    statistics.eventCount = MAGMA_COUNT(events);
    statistics.queryPoolCount = MAGMA_COUNT(queryPools);
    statistics.bufferViewCount = MAGMA_COUNT(bufferViews);
    statistics.imageViewCount = MAGMA_COUNT(imageViews);
    statistics.shaderModuleCount = MAGMA_COUNT(shaderModules);
    statistics.pipelineCacheCount = MAGMA_COUNT(pipelineCaches);
    statistics.pipelineLayoutCount = MAGMA_COUNT(pipelineLayouts);
    statistics.renderPassCount = MAGMA_COUNT(renderPasses);
    foreach<Pipeline>(pipelines,
        [&statistics](const Pipeline *pipeline)
        {
            switch (pipeline->getBindPoint())
            {
            case VK_PIPELINE_BIND_POINT_GRAPHICS:
                ++statistics.graphicsPipelineCount;
                break;
            case VK_PIPELINE_BIND_POINT_COMPUTE:
                ++statistics.computePipelineCount;
                break;
            #ifdef VK_KHR_acceleration_structure
            case VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR:
                ++statistics.rayTracingPipelineCount;
                break;
            #endif
            }
        });
    statistics.descriptorSetLayoutCount = MAGMA_COUNT(descriptorSetLayouts);
    statistics.samplerCount = MAGMA_COUNT(samplers);
    statistics.descriptorPoolCount = MAGMA_COUNT(descriptorPools);
    statistics.descriptorSetCount = MAGMA_COUNT(descriptorSets);
    statistics.framebufferCount = MAGMA_COUNT(framebuffers);
    statistics.commandPoolCount = MAGMA_COUNT(commandPools);
#if defined(VK_KHR_acceleration_structure) || defined(VK_NV_ray_tracing)
    statistics.accelerationStructureCount = MAGMA_COUNT(accelerationStructures);
#endif
#ifdef VK_EXT_validation_cache
    statistics.validationCacheCount = MAGMA_COUNT(validationCaches);
#endif
#ifdef VK_INTEL_performance_query
    statistics.performanceConfigurationCount = MAGMA_COUNT(performanceConfigurations);
#endif
#ifdef VK_KHR_deferred_host_operations
    statistics.deferredOperationCount = MAGMA_COUNT(deferredOperations);
#endif
#ifdef VK_KHR_descriptor_update_template
    statistics.descriptorUpdateTemplateCount = MAGMA_COUNT(descriptorUpdateTemplates);
#endif
#ifdef VK_NV_device_generated_commands
    statistics.indirectCommandsLayoutCount = MAGMA_COUNT(indirectCommandsLayouts);
#endif
#ifdef VK_KHR_sampler_ycbcr_conversion
    statistics.ycbcrSamplerCount = MAGMA_COUNT(ycbcrSamplers);
#endif
#ifdef VK_EXT_private_data
    statistics.privateDataSlotCount = MAGMA_COUNT(privateDataSlots);
#endif
    return statistics;
}

DeviceResourcePool::PhysicalDeviceResources DeviceResourcePool::countPhysicalDeviceResources() const
{
    std::lock_guard<std::mutex> guard(mtx);
    PhysicalDeviceResources statistics;
#ifdef VK_KHR_display
    statistics.displayCount = MAGMA_COUNT(displays);
    statistics.displayModeCount = MAGMA_COUNT(displayModes);
#endif
    return statistics;
}

VkDeviceSize DeviceResourcePool::countAllocatedDeviceLocalMemory() const
{
    std::lock_guard<std::mutex> guard(mtx);
    VkDeviceSize deviceLocalAllocatedSize = 0;
    foreach<DeviceMemory>(deviceMemories,
        [&deviceLocalAllocatedSize](const BaseDeviceMemory *memory)
        {
            if (memory->getFlags().deviceLocal)
                deviceLocalAllocatedSize += memory->getSize();
        });
    return deviceLocalAllocatedSize;
}

VkDeviceSize DeviceResourcePool::countAllocatedHostVisibleMemory() const
{
    std::lock_guard<std::mutex> guard(mtx);
    VkDeviceSize hostVisibleAllocatedSize = 0;
    foreach<DeviceMemory>(deviceMemories,
        [&hostVisibleAllocatedSize](const BaseDeviceMemory *memory)
        {
            if (memory->getFlags().hostVisible)
                hostVisibleAllocatedSize += memory->getSize();
        });
    return hostVisibleAllocatedSize;
}

VkDeviceSize DeviceResourcePool::countAllocatedBufferMemory() const
{
    std::lock_guard<std::mutex> guard(mtx);
    VkDeviceSize bufferAllocatedSize = 0;
    foreach<Buffer>(deviceMemories,
        [&bufferAllocatedSize](const Buffer *buffer)
        {
            const IDeviceMemory *memory = buffer->getMemory().get();
            if (memory)
                bufferAllocatedSize += memory->getSize();
        });
    return bufferAllocatedSize;
}

VkDeviceSize DeviceResourcePool::countAllocatedImageMemory() const
{
    std::lock_guard<std::mutex> guard(mtx);
    VkDeviceSize imageAllocatedSize = 0;
    foreach<Image>(images,
        [&imageAllocatedSize](const Image *image)
        {
            const IDeviceMemory *memory = image->getMemory().get();
            if (memory)
                imageAllocatedSize += memory->getSize();
        });
    return imageAllocatedSize;
}

VkDeviceSize DeviceResourcePool::countAllocatedAccelerationStructureMemory() const
{
    std::lock_guard<std::mutex> guard(mtx);
    VkDeviceSize accelerationStructureAllocatedSize = 0;
#ifdef VK_NV_ray_tracing
    foreach<AccelerationStructure>(accelerationStructures,
        [&accelerationStructureAllocatedSize](const AccelerationStructure *accelerationStructure)
        {
            const IDeviceMemory *memory = accelerationStructure->getMemory().get();
            if (memory)
                accelerationStructureAllocatedSize += memory->getSize();
        });
#endif // VK_NV_ray_tracing
    return accelerationStructureAllocatedSize;
}

bool DeviceResourcePool::hasAnyDeviceResource() const
{
    std::lock_guard<std::mutex> guard(mtx);
    return semaphores.size() ||
        fences.size() ||
        deviceMemories.size() ||
        buffers.size() ||
        images.size() ||
        events.size() ||
        queryPools.size() ||
        bufferViews.size() ||
        imageViews.size() ||
        shaderModules.size() ||
        pipelineCaches.size() ||
        pipelineLayouts.size() ||
        renderPasses.size() ||
        pipelines.size() ||
        descriptorSetLayouts.size() ||
        samplers.size() ||
        descriptorPools.size() ||
        descriptorSets.size() ||
        framebuffers.size() ||
        commandPools.size() ||
    #ifdef VK_KHR_deferred_host_operations
        deferredOperations.size() ||
    #endif
    #ifdef VK_KHR_swapchain
        swapchains.size() ||
    #endif
    #ifdef VK_EXT_private_data
        privateDataSlots.size() ||
    #endif
    #ifdef VK_NV_ray_tracing
        accelerationStructures.size() ||
    #endif
        false;
}
#endif // (VK_USE_64_BIT_PTR_DEFINES == 1)
} // namespace magma
