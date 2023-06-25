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
        bool staged() const noexcept override;
        bool hostVisible() const noexcept override;
        bool hostCached() const noexcept override;
        bool binded() const noexcept override;
        bool mapped() const noexcept override;

    protected:
        BaseDeviceMemory(std::shared_ptr<Device> device,
            const VkMemoryRequirements& memoryRequirements,
            VkMemoryPropertyFlags flags,
            std::shared_ptr<IAllocator> allocator,
            const StructureChain& extendedInfo);
        uint32_t findTypeIndex(VkMemoryPropertyFlags flags) const;
        float clampPriority(float value) noexcept;

        VkMemoryRequirements memoryRequirements;
        const VkMemoryPropertyFlags flags;
        uint32_t deviceMask;
        float priority;
        NonDispatchableHandle binding;
        void *mapPointer;
    };
} // namespace magma

#include "baseDeviceMemory.inl"
