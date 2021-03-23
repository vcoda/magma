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
#pragma once
#include "../core/pool.h"

namespace magma
{
    template<typename Type>
    class NonDispatchable;

    /* Resource pool stores pointers to different Vulkan objects that were created by device.
       Pool allows to keep statistics about resource usage, memory allocations etc.
       This may be used for application profiling, memory usage optimization and so on.
       Each logical device has it's own resource pool. */

    class ResourcePool final : public core::NonCopyable
    {
        template<typename Type>
        friend class NonDispatchable;

    public:
        struct DeviceResources
        {
            uint32_t semaphoreCount = 0;
            uint32_t fenceCount = 0;
            uint32_t deviceMemoryCount = 0;
            uint32_t bufferCount = 0;
            uint32_t image1DCount = 0;
            uint32_t image1DArrayCount = 0;
            uint32_t image2DCount = 0;
            uint32_t image2DArrayCount = 0;
            uint32_t image3DCount = 0;
            uint32_t eventCount = 0;
            uint32_t queryPoolCount = 0;
            uint32_t bufferViewCount = 0;
            uint32_t imageViewCount = 0;
            uint32_t shaderModuleCount = 0;
            uint32_t pipelineCacheCount = 0;
            uint32_t pipelineLayoutCount = 0;
            uint32_t renderPassCount = 0;
            uint32_t graphicsPipelineCount = 0;
            uint32_t computePipelineCount = 0;
            uint32_t rayTracingPipelineCount = 0;
            uint32_t descriptorSetLayoutCount = 0;
            uint32_t samplerCount = 0;
            uint32_t descriptorPoolCount = 0;
            uint32_t descriptorSetCount = 0;
            uint32_t framebufferCount = 0;
            uint32_t commandPoolCount = 0;
            // Non-core types
            uint32_t swapchainCount = 0;
            uint32_t accelerationStructureCount = 0;
        };

        struct PhysicalDeviceResources
        {
            uint32_t displayCount = 0;
            uint32_t displayModeCount = 0;
        };

    public:
        DeviceResources countDeviceResources() const;
        PhysicalDeviceResources countPhysicalDeviceResources() const;
        VkDeviceSize countAllocatedDeviceLocalMemory() const;
        VkDeviceSize countAllocatedHostVisibleMemory() const;
        VkDeviceSize countAllocatedBufferMemory() const;
        VkDeviceSize countAllocatedImageMemory() const;
        VkDeviceSize countAllocatedAccelerationStructureMemory() const;
        bool hasAnyDeviceResource() const;

        template<typename Type>
		core::Pool<Type>& getPool();

    private:
        mutable std::mutex mutex;

        // Core types
        core::Pool<NonDispatchable<VkSemaphore>> semaphores;
        core::Pool<NonDispatchable<VkFence>> fences;
        core::Pool<NonDispatchable<VkDeviceMemory>> deviceMemories;
        core::Pool<NonDispatchable<VkBuffer>> buffers;
        core::Pool<NonDispatchable<VkImage>> images;
        core::Pool<NonDispatchable<VkEvent>> events;
        core::Pool<NonDispatchable<VkQueryPool>> queryPools;
        core::Pool<NonDispatchable<VkBufferView>> bufferViews;
        core::Pool<NonDispatchable<VkImageView>> imageViews;
        core::Pool<NonDispatchable<VkShaderModule>> shaderModules;
        core::Pool<NonDispatchable<VkPipelineCache>> pipelineCaches;
        core::Pool<NonDispatchable<VkPipelineLayout>> pipelineLayouts;
        core::Pool<NonDispatchable<VkRenderPass>> renderPasses;
        core::Pool<NonDispatchable<VkPipeline>> pipelines;
        core::Pool<NonDispatchable<VkDescriptorSetLayout>> descriptorSetLayouts;
        core::Pool<NonDispatchable<VkSampler>> samplers;
        core::Pool<NonDispatchable<VkDescriptorPool>> descriptorPools;
        core::Pool<NonDispatchable<VkDescriptorSet>> descriptorSets;
        core::Pool<NonDispatchable<VkFramebuffer>> framebuffers;
        core::Pool<NonDispatchable<VkCommandPool>> commandPools;
        // Non-core types
#ifdef VK_KHR_surface
        core::Pool<NonDispatchable<VkSurfaceKHR>> surfaces;
#endif
#ifdef VK_KHR_swapchain
        core::Pool<NonDispatchable<VkSwapchainKHR>> swapchains;
#endif
#ifdef VK_KHR_display
        core::Pool<NonDispatchable<VkDisplayKHR>> displays;
        core::Pool<NonDispatchable<VkDisplayModeKHR>> displayModes;
#endif
#ifdef VK_EXT_debug_report
        core::Pool<NonDispatchable<VkDebugReportCallbackEXT>> debugReportCallbacks;
#endif
#ifdef VK_EXT_debug_utils
        core::Pool<NonDispatchable<VkDebugUtilsMessengerEXT>> debugUtilsMessengers;
#endif
#ifdef VK_NV_ray_tracing
        core::Pool<NonDispatchable<VkAccelerationStructureNV>> accelerationStructures;
#endif
    };
} // namespace magma

#include "resourcePool.inl"
