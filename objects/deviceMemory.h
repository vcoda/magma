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
#include "nondispatchable.h"

namespace magma
{
    class Allocator;
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
            const VkMemoryRequirements& memoryRequirements,
            VkMemoryPropertyFlags flags,
            float priority,
            const void *object,
            VkObjectType objectType,
            std::shared_ptr<Allocator> allocator = nullptr);
#ifdef VK_KHR_device_group
        explicit DeviceMemory(std::shared_ptr<Device> device,
            uint32_t deviceMask,
            const VkMemoryRequirements& memoryRequirements,
            VkMemoryPropertyFlags flags,
            float priority,
            std::shared_ptr<Allocator> allocator = nullptr);
#endif
        ~DeviceMemory();
        std::shared_ptr<IDeviceMemoryAllocator> getDeviceAllocator() const noexcept { return deviceAllocator; }
        DeviceMemoryBlock getAllocation() const noexcept { return memory; }
        VkDeviceSize getOffset() const noexcept { return offset; }
        VkDeviceSize getSize() const noexcept { return memoryRequirements.size; }
        VkDeviceSize getAlignment() const noexcept { return memoryRequirements.alignment; }
        uint32_t getMemoryTypeBits() const noexcept { return memoryRequirements.memoryTypeBits; }
        float getPriority() const noexcept { return priority; }
        bool local() const noexcept;
        bool pinned() const noexcept;
        bool hostVisible() const noexcept;
        bool hostCached() const noexcept;
        bool mapped() const noexcept;
        void realloc(VkDeviceSize newSize,
            float priority,
            const void *object,
            VkObjectType objectType,
            std::shared_ptr<Allocator> allocator = nullptr);
        void bind(const void *object,
            VkObjectType objectType,
            VkDeviceSize offset /* 0 */);
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
        void onDefragment() noexcept;

    private:
        uint32_t getTypeIndex(VkMemoryPropertyFlags flags) const;

        VkMemoryRequirements memoryRequirements;
        const VkMemoryPropertyFlags flags;
        const float priority;
        std::shared_ptr<IDeviceMemoryAllocator> deviceAllocator;
        DeviceMemoryBlock memory;
        VkDeviceSize offset;
        void *mappedRange;
    };
} // namespace magma

#include "deviceMemory.inl"
