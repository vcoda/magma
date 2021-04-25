/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2021 Victor Coda.

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
#include "../core/destructible.h"

namespace magma
{
    struct AllocationCallbacks : VkAllocationCallbacks
    {
        AllocationCallbacks() noexcept;
    };

    /* Vulkan provides applications the opportunity to perform host memory allocations
       on behalf of the Vulkan implementation. If this feature is not used,
       the implementation will perform its own memory allocations. Since most memory
       allocations are off the critical path, this is not meant as a performance feature.
       Rather, this can be useful for certain embedded systems, for debugging purposes
       (e.g. putting a guard page after all host allocations), or for memory allocation logging. */

    class IAllocator : public core::IDestructible, public AllocationCallbacks
    {
    public:
        virtual void *alloc(std::size_t size,
            std::size_t alignment,
            VkSystemAllocationScope allocationScope) = 0;
        virtual void *realloc(void *original,
            std::size_t size,
            std::size_t alignment,
            VkSystemAllocationScope allocationScope) = 0;
        virtual void free(void *memory) = 0;
        virtual void internalAllocationNotification(std::size_t size,
            VkInternalAllocationType allocationType,
            VkSystemAllocationScope allocationScope) = 0;
        virtual void internalFreeNotification(std::size_t size,
            VkInternalAllocationType allocationType,
            VkSystemAllocationScope allocationScope) = 0;
    };

    struct MemoryBudget
    {
        VkDeviceSize blockBytes;
        VkDeviceSize allocationBytes;
        VkDeviceSize usage;
        VkDeviceSize budget;
    };

    struct DefragmentationStats
    {
        VkDeviceSize bytesMoved;
        VkDeviceSize bytesFreed;
        uint32_t allocationsMoved;
        uint32_t deviceMemoryBlocksFreed;
    };

    class Device;
    class CommandBuffer;

    /* Previous generation APIs (OpenGL, DirectX 11) manage memory automatically.
       In contrast, Vulkan requires explicit memory management that makes it possible to:
           * Better manage memory
           * Better optimize for specific platforms
           * Alias (overlap) transient resources
       Allocation of separate memory block per resource is inefficient and limited by OS.
       This object provides an interface for memory allocator that can allocate a bigger
       memory blocks in different heaps and then sub-allocate ranges for your resources. */

    class IDeviceMemoryAllocator : public core::IDestructible
    {
    public:
        virtual std::shared_ptr<Device> getDevice() const noexcept = 0;
        virtual std::shared_ptr<IAllocator> getAllocator() const noexcept = 0;
        virtual void *alloc(const VkMemoryRequirements& memoryRequirements,
            VkMemoryPropertyFlags flags,
            bool cpuFrequentlyWriteGpuRead) = 0;
        virtual std::vector<void *> allocPages(const std::vector<VkMemoryRequirements>& memoryRequirements,
            const std::vector<VkMemoryPropertyFlags>& flags) = 0;
        virtual void *realloc(void *memory,
            VkDeviceSize size) = 0;
        virtual void free(void *memory) noexcept = 0;
        virtual void freePages(std::vector<void *>& memoryPages) noexcept = 0;
        virtual VkDeviceMemory getMemoryHandle(void *memory) const noexcept = 0;
        virtual VkResult map(void *memory,
            VkDeviceSize offset,
            void **data) noexcept = 0;
        virtual void unmap(void *memory) noexcept = 0;
        virtual VkResult flushMappedRange(void *memory,
            VkDeviceSize offset,
            VkDeviceSize size) noexcept = 0;
        virtual VkResult invalidateMappedRange(void *memory,
            VkDeviceSize offset,
            VkDeviceSize size) noexcept = 0;
        virtual std::vector<MemoryBudget> getBudget() const noexcept = 0;
        virtual VkResult checkCorruption(uint32_t memoryTypeBits) noexcept = 0;
        virtual VkResult beginCpuDefragmentation(std::vector<void *>& allocations,
            DefragmentationStats* stats = nullptr) noexcept = 0;
        virtual VkResult beginGpuDefragmentation(std::shared_ptr<CommandBuffer> cmdBuffer,
            std::vector<void *>& allocations,
            DefragmentationStats* stats = nullptr) noexcept = 0;
        virtual VkResult endDefragmentation() noexcept = 0;
    };
} // namespace magma
