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
#include "baseDeviceMemory.h"

namespace magma
{
    class IDeviceMemoryAllocator;

    /* Opaque handle to memory sub-allocation.
       Each device memory allocator hides the implementation details under it. */
    typedef void *DeviceMemoryBlock;

    /* Device memory managed by Vulkan Memory Allocator or similar system. */

    class ManagedDeviceMemory : public BaseDeviceMemory
    {
    public:
        explicit ManagedDeviceMemory(std::shared_ptr<Device> device,
            VkObjectType objectType,
            NonDispatchableHandle object,
            const VkMemoryRequirements& memoryRequirements,
            VkMemoryPropertyFlags flags,
            std::shared_ptr<IAllocator> hostAllocator,
            std::shared_ptr<IDeviceMemoryAllocator> deviceAllocator,
            const StructureChain& extendedInfo = StructureChain());
        ~ManagedDeviceMemory();
        const std::shared_ptr<IDeviceMemoryAllocator>& getDeviceAllocator() const noexcept { return deviceAllocator; }
        DeviceMemoryBlock getAllocation() const noexcept { return allocation; }
        VkDeviceSize getSuballocationOffset() const noexcept override { return subOffset; }
        float getPriority() const noexcept override { return priority; }
        void setPriority(float) noexcept override {}
        bool managed() const noexcept override { return true; }
        void realloc(NonDispatchableHandle object,
            const VkMemoryRequirements& memoryRequirements,
            const StructureChain& extendedInfo = StructureChain()) override;
        void bind(NonDispatchableHandle object,
            VkObjectType objectType,
            VkDeviceSize offset = 0) override;
    #ifdef VK_KHR_device_group
        void bindDeviceGroup(NonDispatchableHandle object,
            VkObjectType objectType,
            const std::vector<uint32_t>& deviceIndices,
            const std::vector<VkRect2D>& splitInstanceBindRegions = {},
            VkDeviceSize offset = 0) override;
    #endif // VK_KHR_device_group
        void *map(VkDeviceSize offset = 0,
            VkDeviceSize size = VK_WHOLE_SIZE,
            VkMemoryMapFlags flags = 0) noexcept override;
        void unmap() noexcept override;
        bool flushMappedRange(VkDeviceSize offset = 0,
            VkDeviceSize size = VK_WHOLE_SIZE) noexcept override;
        bool invalidateMappedRange(VkDeviceSize offset = 0,
            VkDeviceSize size = VK_WHOLE_SIZE) noexcept override;
        void onDefragment() noexcept override;

    private:
        const VkObjectType objectType;
        std::shared_ptr<IDeviceMemoryAllocator> deviceAllocator;
        DeviceMemoryBlock allocation;
        VkDeviceSize subOffset = 0ull;
    };
} // namespace magma
