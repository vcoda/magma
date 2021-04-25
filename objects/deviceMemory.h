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
#include "nondispatchable.h"

namespace magma
{
    class IDeviceMemoryAllocator;

    /* Opaque handle to memory sub-allocation.
       Each device memory allocator hides the implementation details under it. */
    typedef void *DeviceMemoryBlock;

    /* Device memory is memory that is visible to the device -
       for example the contents of the image or buffer objects,
       which can be natively used by the device. */

    class DeviceMemory : public NonDispatchable<VkDeviceMemory>
    {
    public:
        explicit DeviceMemory(std::shared_ptr<Device> device,
            VkDeviceSize size,
            VkMemoryPropertyFlags flags,
            std::shared_ptr<IAllocator> allocator = nullptr);
        explicit DeviceMemory(std::shared_ptr<IDeviceMemoryAllocator> allocator,
            DeviceMemoryBlock memory,
            VkDeviceMemory handle,
            VkDeviceSize size,
            VkMemoryPropertyFlags flags) noexcept;
#ifdef VK_KHR_device_group
        explicit DeviceMemory(std::shared_ptr<Device> device,
            uint32_t deviceMask,
            VkDeviceSize size,
            VkMemoryPropertyFlags flags,
            std::shared_ptr<IAllocator> allocator = nullptr);
#endif
        ~DeviceMemory();
        std::shared_ptr<IDeviceMemoryAllocator> getDeviceAllocator() const noexcept { return allocator; }
        DeviceMemoryBlock getAllocation() const noexcept { return memory; }
        VkDeviceSize getSize() const noexcept { return size; }
        bool local() const noexcept;
        bool hostVisible() const noexcept;
        bool hostMapped() const noexcept;
        void *map(VkDeviceSize offset = 0,
            VkDeviceSize size = VK_WHOLE_SIZE,
            VkMemoryMapFlags flags = 0) noexcept;
        template<typename Type>
        Type *map(VkDeviceSize offset = 0,
            VkDeviceSize size = VK_WHOLE_SIZE,
            VkMemoryMapFlags flags = 0) noexcept;
        void unmap() noexcept;
        bool flushMappedRange(VkDeviceSize offset = 0,
            VkDeviceSize size = VK_WHOLE_SIZE) noexcept;
        bool invalidateMappedRange(VkDeviceSize offset = 0,
            VkDeviceSize size = VK_WHOLE_SIZE) noexcept;

    private:
        uint32_t getTypeIndex(VkMemoryPropertyFlags flags) const;

        std::shared_ptr<IDeviceMemoryAllocator> allocator;
        DeviceMemoryBlock memory;
        VkDeviceSize size;
        VkMemoryPropertyFlags flags;
        bool mapped;
    };
} // namespace magma

#include "deviceMemory.inl"
