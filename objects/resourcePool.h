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
#pragma once
#include "../core/noncopyable.h"

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
        struct ResourceStatistics
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

    public:
        ResourceStatistics countResources() const noexcept;
        VkDeviceSize countAllocatedDeviceLocalMemory() const noexcept;
        VkDeviceSize countAllocatedHostVisibleMemory() const noexcept;
        bool hasAnyResource() const noexcept;

    private:
        template<typename Type>
        class Pool final : public core::NonCopyable
        {
            std::unordered_set<const Type *> resources;

        public:
            void registerResource(const Type *resource) noexcept;
            void unregisterResource(const Type *resouce) noexcept;
            uint32_t count() const noexcept;
            template<typename ChildType>
            void forEach(const std::function<void(const ChildType *resource)>& fn) const noexcept;
        };

        template<typename Type>
        Pool<Type>& getPool() noexcept;

        // Core types
        Pool<NonDispatchable<VkSemaphore>> semaphores;
        Pool<NonDispatchable<VkFence>> fences;
        Pool<NonDispatchable<VkDeviceMemory>> deviceMemories;
        Pool<NonDispatchable<VkBuffer>> buffers;
        Pool<NonDispatchable<VkImage>> images;
        Pool<NonDispatchable<VkEvent>> events;
        Pool<NonDispatchable<VkQueryPool>> queryPools;
        Pool<NonDispatchable<VkBufferView>> bufferViews;
        Pool<NonDispatchable<VkImageView>> imageViews;
        Pool<NonDispatchable<VkShaderModule>> shaderModules;
        Pool<NonDispatchable<VkPipelineCache>> pipelineCaches;
        Pool<NonDispatchable<VkPipelineLayout>> pipelineLayouts;
        Pool<NonDispatchable<VkRenderPass>> renderPasses;
        Pool<NonDispatchable<VkPipeline>> pipelines;
        Pool<NonDispatchable<VkDescriptorSetLayout>> descriptorSetLayouts;
        Pool<NonDispatchable<VkSampler>> samplers;
        Pool<NonDispatchable<VkDescriptorPool>> descriptorPools;
        Pool<NonDispatchable<VkDescriptorSet>> descriptorSets;
        Pool<NonDispatchable<VkFramebuffer>> framebuffers;
        Pool<NonDispatchable<VkCommandPool>> commandPools;
        // Non-core types
#ifdef VK_KHR_surface
        Pool<NonDispatchable<VkSurfaceKHR>> surfaces;
#endif
#ifdef VK_KHR_swapchain
        Pool<NonDispatchable<VkSwapchainKHR>> swapchains;
#endif
#ifdef VK_KHR_display
        Pool<NonDispatchable<VkDisplayKHR>> displays;
        Pool<NonDispatchable<VkDisplayModeKHR>> displayModes;
#endif
#ifdef VK_EXT_debug_report
        Pool<NonDispatchable<VkDebugReportCallbackEXT>> debugReportCallbacks;
#endif
#ifdef VK_EXT_debug_utils
        Pool<NonDispatchable<VkDebugUtilsMessengerEXT>> debugUtilsMessengers;
#endif
#ifdef VK_NV_ray_tracing
        Pool<NonDispatchable<VkAccelerationStructureNV>> accelerationStructures;
#endif
    };
} // namespace magma

#include "resourcePool.inl"
