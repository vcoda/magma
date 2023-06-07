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
#include "ideviceMemory.h"
#include "nondispatchable.h"

namespace magma
{
    /* Base class of device memory. */

    class BaseDeviceMemory : public IDeviceMemory,
        public NonDispatchable<VkDeviceMemory>
    {
    public:
        VkDeviceMemory getNativeHandle() const noexcept override { return handle; }
        VkDeviceSize getSize() const noexcept override { return memoryRequirements.size; }
        VkDeviceSize getAlignment() const noexcept override { return memoryRequirements.alignment; }
        uint32_t getMemoryTypeBits() const noexcept override { return memoryRequirements.memoryTypeBits; }
        uint32_t getDeviceMask() const noexcept { return deviceMask; }
        void *getMapPointer() const noexcept { return mapPointer; }
        bool local() const noexcept override;
        bool pinned() const noexcept override;
        bool hostVisible() const noexcept override;
        bool hostCached() const noexcept override;
        bool mapped() const noexcept override;

    protected:
        BaseDeviceMemory(std::shared_ptr<Device> device,
            const VkMemoryRequirements& memoryRequirements,
            VkMemoryPropertyFlags flags,
            float priority,
            uint32_t deviceMask,
            std::shared_ptr<IAllocator> allocator);
        uint32_t findTypeIndex(VkMemoryPropertyFlags flags) const;
        float updatePriority(float value) noexcept;

        VkMemoryRequirements memoryRequirements;
        const VkMemoryPropertyFlags flags;
        float priority;
        uint32_t deviceMask;
        void *mapPointer = nullptr;
    };

    /* Device memory is memory that is visible to the device -
       for example the contents of the image or buffer objects,
       which can be natively used by the device. */

    class DeviceMemory : public BaseDeviceMemory
    {
    public:
        explicit DeviceMemory(std::shared_ptr<Device> device,
            const VkMemoryRequirements& memoryRequirements,
            VkMemoryPropertyFlags flags,
            float priority,
            std::shared_ptr<IAllocator> allocator = nullptr);
    #ifdef VK_KHR_device_group
        explicit DeviceMemory(std::shared_ptr<Device> device,
            uint32_t deviceMask,
            const VkMemoryRequirements& memoryRequirements,
            VkMemoryPropertyFlags flags,
            float priority,
            std::shared_ptr<IAllocator> allocator = nullptr);
    #endif // VK_KHR_device_group
        ~DeviceMemory();
        VkDeviceSize getSuballocationOffset() const noexcept override { return 0ull; }
        float getPriority() const noexcept override { return priority; }
        void setPriority(float priority) noexcept override;
        bool managed() const noexcept override { return false; }
        void realloc(VkDeviceSize newSize,
            float priority,
            NonDispatchableHandle object,
            VkObjectType objectType) override;
        void bind(NonDispatchableHandle object,
            VkObjectType objectType,
            VkDeviceSize offset = 0) override;
        void *map(VkDeviceSize offset = 0,
            VkDeviceSize size = VK_WHOLE_SIZE,
            VkMemoryMapFlags flags = 0) noexcept override;
        void unmap() noexcept override;
        bool flushMappedRange(VkDeviceSize offset = 0,
            VkDeviceSize size = VK_WHOLE_SIZE) noexcept override;
        bool invalidateMappedRange(VkDeviceSize offset = 0,
            VkDeviceSize size = VK_WHOLE_SIZE) noexcept override;
        void onDefragment() noexcept override;
    };
} // namespace magma

#include "deviceMemory.inl"
