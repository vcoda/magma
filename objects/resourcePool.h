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
            uint32_t semaphoreCount;
            uint32_t fenceCount;
            uint32_t deviceMemoryCount;
            uint32_t bufferCount;
            uint32_t imageCount;
            uint32_t eventCount;
            uint32_t queryPoolCount;
            uint32_t bufferViewCount;
            uint32_t imageViewCount;
            uint32_t shaderModuleCount;
            uint32_t pipelineCacheCount;
            uint32_t pipelineLayoutCount;
            uint32_t renderPassCount;
            uint32_t graphicsPipelineCount;
            uint32_t computePipelineCount;
            uint32_t rayTracingPipelineCount;
            uint32_t descriptorSetLayoutCount;
            uint32_t samplerCount;
            uint32_t descriptorPoolCount;
            uint32_t descriptorSetCount;
            uint32_t framebufferCount;
            uint32_t commandPoolCount;
            // Non-core types
            uint32_t surfaceCount;
            uint32_t swapchainCount;
            uint32_t displayCount;
            uint32_t debugReportCallbackCount;
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
        Pool<NonDispatchable<VkSurfaceKHR>> surfaces;
        Pool<NonDispatchable<VkSwapchainKHR>> swapchains;
        Pool<NonDispatchable<VkDisplayKHR>> displays;
        Pool<NonDispatchable<VkDebugReportCallbackEXT>> debugReportCallbacks;
#ifdef VK_NV_ray_tracing
        Pool<NonDispatchable<VkAccelerationStructureNV>> accelerationStructures;
#endif
    };
} // namespace magma

#include "resourcePool.inl"
