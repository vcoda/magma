/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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
#include "model/nondispatchable.h"
#include "ideviceMemory.h"

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
        VkMemoryPropertyFlags getPropertyFlags() const noexcept override { return memoryType.propertyFlags; }
        uint32_t getHeapIndex() const noexcept override { return memoryType.heapIndex; }
        Flags getFlags() const noexcept override { return flags; }
        uint32_t getDeviceMask() const noexcept override { return deviceMask; }
        void *getMapPointer() const noexcept override { return mapPointer; }
        VkDeviceSize getMapOffset() const noexcept override { return mapOffset; }
        VkDeviceSize getMapSize() const noexcept override { return mapSize; }
        VkMemoryMapFlags getMapFlags() const noexcept override { return mapFlags; }
        bool binded() const noexcept override { return binding != VK_NULL_HANDLE; }
        bool mapped() const noexcept override { return mapPointer != nullptr; }
        bool persistentlyMapped() const noexcept override { return mapPointer && mapPersistent; }

    protected:
        BaseDeviceMemory(std::shared_ptr<Device> device,
            const VkMemoryRequirements& memoryRequirements,
            VkMemoryPropertyFlags propertyFlags,
            std::shared_ptr<IAllocator> allocator,
            const StructureChain& extendedInfo) noexcept;
        VkMemoryType findMemoryType(VkMemoryPropertyFlags propertyFlags) const;
        std::optional<uint32_t> findTypeIndex(VkMemoryPropertyFlags propertyFlags) const noexcept;
        float clampPriority(float value) noexcept;

        VkMemoryRequirements memoryRequirements;
        const VkMemoryType memoryType;
        Flags flags;
        uint32_t deviceMask;
        float priority;
        NonDispatchableHandle binding;
        void *mapPointer;
        VkDeviceSize mapOffset;
        VkDeviceSize mapSize;
        VkMemoryMapFlags mapFlags;
        bool mapPersistent;
    };
} // namespace magma
