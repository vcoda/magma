/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2022 Victor Coda.

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
#include "../core/noncopyable.h"
#include "../core/threadSafeUnorderedSet.h"

namespace magma
{
    template<typename Type>
    class NonDispatchable;
    class Device;

    /* Resource pool stores pointers to different Vulkan objects that were created by device.
       Pool allows to keep statistics about resource usage, memory allocations etc.
       This may be used for application profiling, memory usage optimization and so on.
       Each logical device has it's own resource pool. */

    class ResourcePool final : core::NonCopyable
    {
    public:
        struct DeviceResources;
        struct PhysicalDeviceResources;

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
        core::ThreadSafeUnorderedSet<Type>& getPool();

    private:
        core::ThreadSafeUnorderedSet<NonDispatchable<VkSemaphore>> semaphores;
        core::ThreadSafeUnorderedSet<NonDispatchable<VkFence>> fences;
        core::ThreadSafeUnorderedSet<NonDispatchable<VkDeviceMemory>> deviceMemories;
        core::ThreadSafeUnorderedSet<NonDispatchable<VkBuffer>> buffers;
        core::ThreadSafeUnorderedSet<NonDispatchable<VkImage>> images;
        core::ThreadSafeUnorderedSet<NonDispatchable<VkEvent>> events;
        core::ThreadSafeUnorderedSet<NonDispatchable<VkQueryPool>> queryPools;
        core::ThreadSafeUnorderedSet<NonDispatchable<VkBufferView>> bufferViews;
        core::ThreadSafeUnorderedSet<NonDispatchable<VkImageView>> imageViews;
        core::ThreadSafeUnorderedSet<NonDispatchable<VkShaderModule>> shaderModules;
        core::ThreadSafeUnorderedSet<NonDispatchable<VkPipelineCache>> pipelineCaches;
        core::ThreadSafeUnorderedSet<NonDispatchable<VkPipelineLayout>> pipelineLayouts;
        core::ThreadSafeUnorderedSet<NonDispatchable<VkRenderPass>> renderPasses;
        core::ThreadSafeUnorderedSet<NonDispatchable<VkPipeline>> pipelines;
        core::ThreadSafeUnorderedSet<NonDispatchable<VkDescriptorSetLayout>> descriptorSetLayouts;
        core::ThreadSafeUnorderedSet<NonDispatchable<VkSampler>> samplers;
        core::ThreadSafeUnorderedSet<NonDispatchable<VkDescriptorPool>> descriptorPools;
        core::ThreadSafeUnorderedSet<NonDispatchable<VkDescriptorSet>> descriptorSets;
        core::ThreadSafeUnorderedSet<NonDispatchable<VkFramebuffer>> framebuffers;
        core::ThreadSafeUnorderedSet<NonDispatchable<VkCommandPool>> commandPools;
#ifdef VK_KHR_deferred_host_operations
        core::ThreadSafeUnorderedSet<NonDispatchable<VkDeferredOperationKHR>> deferredOperations;
#endif
#ifdef VK_KHR_sampler_ycbcr_conversion
        core::ThreadSafeUnorderedSet<NonDispatchable<VkSamplerYcbcrConversionKHR>> ycbcrSamplers;
#endif
#ifdef VK_KHR_surface
        core::ThreadSafeUnorderedSet<NonDispatchable<VkSurfaceKHR>> surfaces;
#endif
#ifdef VK_KHR_swapchain
        core::ThreadSafeUnorderedSet<NonDispatchable<VkSwapchainKHR>> swapchains;
#endif
#ifdef VK_KHR_display
        core::ThreadSafeUnorderedSet<NonDispatchable<VkDisplayKHR>> displays;
        core::ThreadSafeUnorderedSet<NonDispatchable<VkDisplayModeKHR>> displayModes;
#endif
#ifdef VK_EXT_debug_report
        core::ThreadSafeUnorderedSet<NonDispatchable<VkDebugReportCallbackEXT>> debugReportCallbacks;
#endif
#ifdef VK_EXT_debug_utils
        core::ThreadSafeUnorderedSet<NonDispatchable<VkDebugUtilsMessengerEXT>> debugUtilsMessengers;
#endif
#ifdef VK_NV_ray_tracing
        core::ThreadSafeUnorderedSet<NonDispatchable<VkAccelerationStructureNV>> accelerationStructures;
#endif
        mutable std::mutex mtx;
        template<typename Type>
        friend class NonDispatchable;
    };

    struct ResourcePool::DeviceResources
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
        uint32_t ycbcrSamplerCount = 0;
        uint32_t descriptorPoolCount = 0;
        uint32_t descriptorSetCount = 0;
        uint32_t framebufferCount = 0;
        uint32_t commandPoolCount = 0;
        uint32_t deferredOperationCount = 0;
        uint32_t swapchainCount = 0;
        uint32_t accelerationStructureCount = 0;
    };

    struct ResourcePool::PhysicalDeviceResources
    {
        uint32_t displayCount = 0;
        uint32_t displayModeCount = 0;
    };

    /* The only purpose is to hide device interface. */

    class DeviceResourcePool
    {
    public:
        static std::shared_ptr<ResourcePool> getPool(std::shared_ptr<Device> device) noexcept;
    };
} // namespace magma

#include "resourcePool.inl"
