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
#include "allocator.h"

VK_DEFINE_HANDLE(VmaAllocator)
VK_DEFINE_HANDLE(VmaDefragmentationContext)

namespace magma
{
    /* The Vulkan Memory Allocator provides a simple and easy to integrate API
       to help you allocate memory for Vulkan buffer and image storage.
       https://gpuopen.com/vulkan-memory-allocator */

    class DeviceMemoryAllocator : public IDeviceMemoryAllocator
    {
    public:
        explicit DeviceMemoryAllocator(std::shared_ptr<Device> device,
            std::shared_ptr<IAllocator> allocator = nullptr);
        ~DeviceMemoryAllocator();
        VmaAllocator getHandle() const noexcept { return handle; }
        virtual std::shared_ptr<Device> getDevice() const noexcept override { return device; }
        virtual std::shared_ptr<IAllocator> getAllocator() const noexcept override { return allocator; }
        virtual void *alloc(const VkMemoryRequirements& memoryRequirements,
            VkMemoryPropertyFlags flags,
            bool cpuFrequentlyWriteGpuRead) override;
        virtual std::vector<void *> allocPages(const std::vector<VkMemoryRequirements>& memoryRequirements,
            const std::vector<VkMemoryPropertyFlags>& memoryFlags) override;
        virtual void *realloc(void *memory,
            VkDeviceSize size) override;
        virtual void free(void *memory) noexcept override;
        virtual void freePages(std::vector<void *>& memoryPages) noexcept override;
        virtual VkDeviceMemory getMemoryHandle(void *memory) const noexcept override;
        virtual VkResult map(void *memory,
            VkDeviceSize offset,
            void **data) noexcept override;
        virtual void unmap(void *memory) noexcept override;
        virtual VkResult flushMappedRange(void *memory,
            VkDeviceSize offset,
            VkDeviceSize size) noexcept override;
        virtual VkResult invalidateMappedRange(void *memory,
            VkDeviceSize offset,
            VkDeviceSize size) noexcept override;
        virtual std::vector<MemoryBudget> getBudget() const noexcept override;
        virtual VkResult checkCorruption(uint32_t memoryTypeBits) noexcept override;
        virtual VkResult beginCpuDefragmentation(std::vector<void *>& allocations,
            DefragmentationStats* stats = nullptr) noexcept override;
        virtual VkResult beginGpuDefragmentation(std::shared_ptr<CommandBuffer> cmdBuffer,
            std::vector<void *>& allocations,
            DefragmentationStats *stats = nullptr) noexcept override;
        virtual VkResult endDefragmentation() noexcept override;

    private:
        std::shared_ptr<Device> device;
        std::shared_ptr<IAllocator> allocator;
        VmaAllocator handle;
        VmaDefragmentationContext defragmentationContext;
    };
} // namespace magma
