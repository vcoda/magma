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
#include "../misc/structureChain.h"

namespace magma
{
    /* Device memory is memory that is visible to the device -
       for example the contents of the image or buffer objects,
       which can be natively used by the device. */

    class IDeviceMemory
    {
    public:
        virtual ~IDeviceMemory() = default;
        virtual VkDeviceMemory getNativeHandle() const noexcept = 0;
        virtual VkDeviceSize getSuballocationOffset() const noexcept = 0;
        virtual VkDeviceSize getSize() const noexcept = 0;
        virtual VkDeviceSize getAlignment() const noexcept = 0;
        virtual uint32_t getMemoryTypeBits() const noexcept = 0;
        virtual uint32_t getDeviceMask() const noexcept = 0;
        virtual float getPriority() const noexcept = 0;
        virtual void setPriority(float priority) noexcept = 0;
        virtual bool managed() const noexcept = 0;
        virtual bool local() const noexcept = 0;
        virtual bool staged() const noexcept = 0;
        virtual bool hostVisible() const noexcept = 0;
        virtual bool hostCached() const noexcept = 0;
        virtual bool binded() const noexcept = 0;
        virtual bool mapped() const noexcept = 0;
        virtual void realloc(NonDispatchableHandle object,
            const VkMemoryRequirements& memoryRequirements,
            const StructureChain& extendedInfo = StructureChain()) = 0;
        virtual void bind(NonDispatchableHandle object,
            VkObjectType objectType,
            VkDeviceSize offset = 0) = 0;
    #ifdef VK_KHR_device_group
        virtual void bindDeviceGroup(NonDispatchableHandle object,
            VkObjectType objectType,
            const std::vector<uint32_t>& deviceIndices,
            const std::vector<VkRect2D>& splitInstanceBindRegions = {},
            VkDeviceSize offset = 0) = 0;
    #endif // VK_KHR_device_group
        virtual void *map(VkDeviceSize offset = 0,
            VkDeviceSize size = VK_WHOLE_SIZE,
            VkMemoryMapFlags flags = 0) noexcept = 0;
        virtual void unmap() noexcept = 0;
        virtual bool flushMappedRange(VkDeviceSize offset = 0,
            VkDeviceSize size = VK_WHOLE_SIZE) noexcept = 0;
        virtual bool invalidateMappedRange(VkDeviceSize offset = 0,
            VkDeviceSize size = VK_WHOLE_SIZE) noexcept = 0;
        virtual void onDefragment() noexcept = 0;
    };
} // namespace magma
