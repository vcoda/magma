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
#pragma once
#include "model/dispatchable.h"

namespace magma
{
    class Device;
    class Buffer;
    class Image;
    class CommandBuffer;
    class Semaphore;
#ifdef VK_KHR_timeline_semaphore
    class TimelineSemaphore;
#endif
#ifdef VK_KHR_external_semaphore_win32
    class D3d12ExternalSemaphore;
    #ifdef VK_KHR_timeline_semaphore
    class D3d12ExternalTimelineSemaphore;
    #endif
#endif // VK_KHR_external_semaphore_win32
    class Fence;
    class Swapchain;

    /* Queues provide an interface to the execution engines
       of a device. Commands for these execution engines are
       recorded into command buffers ahead of execution time.
       These command buffers are then submitted to queues
       with a queue submission command for execution in a
       number of batches. The device executes queue operations
       asynchronously with respect to the host. */

    class Queue : public Dispatchable<VkQueue>
    {
    public:
        VkQueueFlagBits getFlags() const noexcept { return flags; }
        uint32_t getFamilyIndex() const noexcept { return familyIndex; }
        uint32_t getIndex() const noexcept { return index; }
    #ifdef VK_EXT_swapchain_maintenance1
        void setPresentMode(VkPresentModeKHR presentMode_) noexcept { presentMode = presentMode_; }
        VkPresentModeKHR getPresentMode() const noexcept { return presentMode; }
    #endif // VK_EXT_swapchain_maintenance1
        void bindSparse(lent_ptr<const Buffer> buffer,
            const std::vector<VkSparseMemoryBind>& bufferBinds,
            lent_ptr<const Image> imageOpaque,
            const std::vector<VkSparseMemoryBind>& imageOpaqueBinds,
            lent_ptr<const Image> image,
            const std::vector<VkSparseImageMemoryBind>& imageBinds,
            lent_ptr<const Semaphore> waitSemaphore = nullptr,
            lent_ptr<Semaphore> signalSemaphore = nullptr,
            lent_ptr<Fence> fence = nullptr,
            const StructureChain& extendedInfo = StructureChain());
        void bindSparse(const std::vector<VkSparseBufferMemoryBindInfo>& bufferBinds,
            const std::vector<VkSparseImageOpaqueMemoryBindInfo>& imageOpaqueBinds,
            const std::vector<VkSparseImageMemoryBindInfo>& imageBinds,
            const std::initializer_list<lent_ptr<const Semaphore>>& waitSemaphores = {},
            const std::initializer_list<lent_ptr<Semaphore>>& signalSemaphores = {},
            lent_ptr<Fence> fence = nullptr,
            const StructureChain& extendedInfo = StructureChain());
        void submit(lent_ptr<CommandBuffer> cmdBuffer,
            VkPipelineStageFlags waitDstStageMask = 0,
            lent_ptr<const Semaphore> waitSemaphore = nullptr,
            lent_ptr<Semaphore> signalSemaphore = nullptr,
            lent_ptr<Fence> fence = nullptr,
            const StructureChain& extendedInfo = StructureChain());
        void submit(const std::vector<lent_ptr<CommandBuffer>>& cmdBuffers,
            const std::initializer_list<VkPipelineStageFlags>& waitDstStageMask = {},
            const std::initializer_list<lent_ptr<const Semaphore>>& waitSemaphores = {},
            const std::initializer_list<lent_ptr<Semaphore>>& signalSemaphores = {},
            lent_ptr<Fence> fence = nullptr,
            const StructureChain& extendedInfo = StructureChain());
    #ifdef VK_KHR_timeline_semaphore
        void submit(lent_ptr<const TimelineSemaphore> semaphore,
            uint64_t waitValue,
            uint64_t signalValue);
    #endif // VK_KHR_timeline_semaphore
    #ifdef VK_KHR_external_semaphore_win32
        void submit(lent_ptr<const D3d12ExternalSemaphore> semaphore,
            uint64_t waitValue,
            uint64_t signalValue);
        #ifdef VK_KHR_timeline_semaphore
        void submit(lent_ptr<const D3d12ExternalTimelineSemaphore> semaphore,
            uint64_t waitValue,
            uint64_t signalValue);
        #endif // VK_KHR_timeline_semaphore
    #endif // VK_KHR_external_semaphore_win32
    #ifdef VK_KHR_device_group
        void submitDeviceGroup(const std::vector<lent_ptr<CommandBuffer>>& cmdBuffers,
            const std::initializer_list<uint32_t>& cmdBufferDeviceMasks = {},
            const std::initializer_list<VkPipelineStageFlags>& waitDstStageMask = {},
            const std::initializer_list<lent_ptr<const Semaphore>>& waitSemaphores = {},
            const std::initializer_list<uint32_t>& waitSemaphoreDeviceIndices = {},
            const std::initializer_list<lent_ptr<Semaphore>>& signalSemaphores = {},
            const std::initializer_list<uint32_t>& signalSemaphoreDeviceIndices = {},
            lent_ptr<Fence> fence = nullptr);
    #endif // VK_KHR_device_group
        void waitIdle();
        void present(const std::unique_ptr<Swapchain>& swapchain,
            uint32_t imageIndex,
            lent_ptr<const Semaphore> waitSemaphore = nullptr,
            lent_ptr<Fence> presentFence = nullptr,
            const StructureChain& extendedInfo = StructureChain());
    #ifdef VK_KHR_display_swapchain
        void presentDisplay(const std::unique_ptr<Swapchain>& swapchain,
            uint32_t imageIndex,
            const VkRect2D& srcRect,
            const VkRect2D& dstRect,
            bool persistent,
            lent_ptr<const Semaphore> waitSemaphore = nullptr,
            lent_ptr<Fence> presentFence = nullptr);
    #endif // VK_KHR_display_swapchain
    #ifdef VK_NV_device_diagnostic_checkpoints
        std::vector<VkCheckpointDataNV> getCheckpoints(lent_ptr<const Device> device) const;
    #endif
        uint32_t inUseObjectCount() const noexcept;
        void onIdle();

    private:
        MAGMA_MAKE_SHARED(Queue)
        Queue(VkQueue handle, VkQueueFlagBits flags,
            uint32_t familyIndex, uint32_t index) noexcept;

        const VkQueueFlagBits flags;
        const uint32_t familyIndex;
        const uint32_t index;
    #ifdef VK_EXT_swapchain_maintenance1
        VkPresentModeKHR presentMode;
    #endif
        std::vector<CommandBuffer *> submittedCommandBuffers;
    #ifdef MAGMA_RETAIN_OBJECTS_IN_USE
        std::unordered_set<std::shared_ptr<const IObject>> inUse;
    #endif
        friend Device;
    };
} // namespace magma
