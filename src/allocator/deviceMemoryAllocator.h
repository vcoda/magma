/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2025 Victor Coda.

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
VK_DEFINE_HANDLE(VmaAllocation)
VK_DEFINE_HANDLE(VmaDefragmentationContext)

struct VmaDefragmentationPassMoveInfo;

namespace magma
{
    class IResource;

    /* The Vulkan Memory Allocator provides a simple and easy way
       to integrate API to help you allocate memory for Vulkan buffer
       and image storage. See: https://gpuopen.com/vulkan-memory-allocator */

    class DeviceMemoryAllocator : public IDeviceMemoryAllocator
    {
    public:
        explicit DeviceMemoryAllocator(std::shared_ptr<Device> device,
            std::shared_ptr<IAllocator> allocator = nullptr);
        ~DeviceMemoryAllocator();
        VmaAllocator getHandle() const noexcept { return allocator; }
        const std::shared_ptr<Device>& getDevice() const noexcept override { return device; }
        const std::shared_ptr<IAllocator>& getHostAllocator() const noexcept override { return hostAllocator; }
        DeviceMemoryBlock allocate(VkObjectType objectType,
            NonDispatchableHandle object,
            const VkMemoryRequirements& memoryRequirements,
            VkMemoryPropertyFlags flags,
            const StructureChain& extendedInfo) override;
        std::vector<DeviceMemoryBlock> allocPages(const std::vector<VkMemoryRequirements>& memoryRequirements,
            const std::vector<VkMemoryPropertyFlags>& flags,
            const std::vector<float>& priorities) override;
        DeviceMemoryBlock realloc(DeviceMemoryBlock memory,
            VkDeviceSize size) override;
        void free(DeviceMemoryBlock memory) noexcept override;
        void freePages(std::vector<DeviceMemoryBlock>& memoryPages) noexcept override;
        VkResult bindMemory(DeviceMemoryBlock memory,
            VkDeviceSize offset,
            NonDispatchableHandle object,
            VkObjectType objectType) const noexcept override;
        MemoryBlockInfo getMemoryBlockInfo(DeviceMemoryBlock memory) const noexcept override;
        std::vector<MemoryBudget> getBudget() const noexcept override;
        VkResult checkCorruption(uint32_t memoryTypeBits) noexcept override;
        VkResult beginDefragmentation(VkFlags flags) noexcept override;
        VkResult beginDefragmentationPass() override;
        VkResult endDefragmentationPass() noexcept override;
        void endDefragmentation(DefragmentationStats* stats = nullptr) noexcept override;

    private:
        VkResult map(DeviceMemoryBlock memory,
            VkDeviceSize offset,
            void **data) noexcept override;
        void unmap(DeviceMemoryBlock memory) noexcept override;
        VkResult flushMappedRange(DeviceMemoryBlock memory,
            VkDeviceSize offset,
            VkDeviceSize size) noexcept override;
        VkResult invalidateMappedRange(DeviceMemoryBlock memory,
            VkDeviceSize offset,
            VkDeviceSize size) noexcept override;
        std::vector<VmaAllocation> gatherSuballocations(const std::list<std::shared_ptr<IResource>>& resources);
        static int chooseMemoryUsage(VkMemoryPropertyFlags flags) noexcept;

    private:
        std::shared_ptr<Device> device;
        std::shared_ptr<IAllocator> hostAllocator;
        VmaAllocator allocator;
        VmaDefragmentationContext defragmentationContext;
        std::unique_ptr<VmaDefragmentationPassMoveInfo> passInfo;
        std::vector<std::shared_ptr<IResource>> defragmentationResources;
        std::vector<VkBool32> allocationsChanged;
    };
} // namespace magma
