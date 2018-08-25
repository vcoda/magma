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
#include "../typedefs.h"

namespace magma
{
    class DeviceMemory;

    /* Buffers represent linear arrays of data which are used
       for various purposes by binding them to a graphics or compute
       pipeline via descriptor sets or via certain commands,
       or by directly specifying them as parameters to certain commands. */

    class Buffer : public NonDispatchable<VkBuffer>,
        public std::enable_shared_from_this<Buffer>
    {
    public:
        ~Buffer();
        void bindMemory(std::shared_ptr<DeviceMemory> memory,
            VkDeviceSize offset = 0);
        void unbindMemory();
        VkBufferUsageFlags getUsage() const noexcept { return usage; }
        std::shared_ptr<DeviceMemory> getMemory() const noexcept { return memory; }
        VkMemoryRequirements getMemoryRequirements() const noexcept;
        VkDescriptorBufferInfo getDescriptor() const noexcept;

    protected:
        explicit Buffer(std::shared_ptr<Device> device,
            VkDeviceSize size,
            VkBufferUsageFlags usage,
            VkBufferCreateFlags flags,
            std::shared_ptr<IAllocator> allocator,
            VkMemoryPropertyFlags memoryFlags);

    protected:
        VkDeviceSize size;
        VkBufferUsageFlags usage;
        std::shared_ptr<DeviceMemory> memory;
    };
} // namespace magma
