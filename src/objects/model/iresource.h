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

namespace magma
{
    class IDeviceMemory;

    /* Base interface of resource object. */

    class IResource
    {
    public:
        enum class Family : uint8_t;
        virtual Family getFamily() const noexcept = 0;
        virtual const std::shared_ptr<IDeviceMemory>& getMemory() const noexcept = 0;
        virtual VkMemoryRequirements getMemoryRequirements() const noexcept = 0;
    #ifdef VK_KHR_get_memory_requirements2
        virtual VkMemoryRequirements getMemoryRequirements2(void *memoryRequirements) const = 0;
    #endif
        virtual void bindMemory(std::shared_ptr<IDeviceMemory> memory,
            VkDeviceSize offset = 0) = 0;
    #ifdef VK_KHR_device_group
        virtual void bindMemoryDeviceGroup(std::shared_ptr<IDeviceMemory> memory,
            const std::vector<uint32_t>& deviceIndices,
            const std::vector<VkRect2D>& splitInstanceBindRegions = {},
            VkDeviceSize offset = 0) = 0;
    #endif // VK_KHR_device_group
    #if defined(VK_KHR_buffer_device_address) || defined(VK_EXT_buffer_device_address)
        virtual VkDeviceAddress getDeviceAddress() const noexcept = 0;
    #endif
        virtual void onDefragment() = 0;
    };

    /* Historically GPU had only buffers and images.
       With RTX, acceleration structure has beed added. */

    enum class IResource::Family : uint8_t
    {
        Buffer, Image, AccelerationStructure
    };
} // namespace magma
