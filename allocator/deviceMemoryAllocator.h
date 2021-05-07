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
enum VmaMemoryUsage : int;

namespace magma
{
    /* The Vulkan Memory Allocator provides a simple and easy to integrate API
       to help you allocate memory for Vulkan buffer and image storage.
       https://gpuopen.com/vulkan-memory-allocator */

    class DeviceMemoryAllocator : public IDeviceMemoryAllocator,
        public std::enable_shared_from_this<DeviceMemoryAllocator>
    {
    public:
        explicit DeviceMemoryAllocator(std::shared_ptr<Device> device,
            std::shared_ptr<IAllocator> allocator = nullptr);
        ~DeviceMemoryAllocator();
        VmaAllocator getHandle() const noexcept { return allocator; }
        virtual DeviceMemoryBlock alloc(const VkMemoryRequirements& memoryRequirements,
            VkMemoryPropertyFlags flags,
            const void *handle,
            SuballocationType suballocType) override;
        virtual std::vector<DeviceMemoryBlock> allocPages(const std::vector<VkMemoryRequirements>& memoryRequirements,
            const std::vector<VkMemoryPropertyFlags>& flags) override;
        virtual DeviceMemoryBlock realloc(DeviceMemoryBlock memory,
            VkDeviceSize size) override;
        virtual void free(DeviceMemoryBlock memory) noexcept override;
        virtual void freePages(std::vector<DeviceMemoryBlock>& memoryPages) noexcept override;
        virtual std::shared_ptr<Device> getDevice() const noexcept override { return device; }
        virtual std::shared_ptr<IAllocator> getHostAllocator() const noexcept override { return hostAllocator; }
        virtual VkDeviceMemory getMemoryHandle(DeviceMemoryBlock memory) const noexcept override;
        virtual std::vector<MemoryBudget> getBudget() const noexcept override;
        virtual VkResult checkCorruption(uint32_t memoryTypeBits) noexcept override;
        virtual VkResult beginCpuDefragmentation(std::vector<DeviceMemoryBlock>& memoryPages,
            bool incremental,
            DefragmentationStats* stats = nullptr) noexcept override;
        virtual VkResult beginGpuDefragmentation(std::shared_ptr<CommandBuffer> cmdBuffer,
            std::vector<DeviceMemoryBlock>& memoryPages,
            bool incremental,
            DefragmentationStats* stats = nullptr) noexcept override;
        virtual VkResult endDefragmentation() noexcept override;

    private:
        virtual VkResult map(DeviceMemoryBlock memory,
            VkDeviceSize offset,
            void **data) noexcept override;
        virtual void unmap(DeviceMemoryBlock memory) noexcept override;
        virtual VkResult flushMappedRange(DeviceMemoryBlock memory,
            VkDeviceSize offset,
            VkDeviceSize size) noexcept override;
        virtual VkResult invalidateMappedRange(DeviceMemoryBlock memory,
            VkDeviceSize offset,
            VkDeviceSize size) noexcept override;
        static VmaMemoryUsage chooseMemoryUsage(VkMemoryPropertyFlags flags) noexcept;

    private:
        std::shared_ptr<Device> device;
        std::shared_ptr<IAllocator> hostAllocator;
        VmaAllocator allocator;
        VmaDefragmentationContext defragmentationContext;
    };
} // namespace magma
