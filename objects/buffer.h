/*
Magma - C++1x interface over Khronos Vulkan API.
Copyright (C) 2018 Victor Coda.

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
#include "handle.h"

namespace magma
{
    class DeviceMemory;

    class Buffer : public NonDispatchable<VkBuffer>
    {
    public:
        ~Buffer();
        void bindMemory(std::shared_ptr<DeviceMemory> memory,
            VkDeviceSize offset = 0);
        std::shared_ptr<DeviceMemory> getMemory() const noexcept { return memory; }
        VkMemoryRequirements getMemoryRequirements() const noexcept;
        VkDescriptorBufferInfo getDescriptor() const noexcept;

    protected:
	    Buffer(std::shared_ptr<const Device> device, 
            VkDeviceSize size, 
            VkBufferUsageFlags usage, 
            VkBufferCreateFlags flags,
            VkMemoryPropertyFlags memoryFlags);

    protected:
        VkDeviceSize size;
	    std::shared_ptr<DeviceMemory> memory;
    };
} // namespace magma
