/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2019 Victor Coda.

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
#include "resource.h"
#include "typedefs.h"

namespace magma
{
    class CommandBuffer;

    /* Buffers represent linear arrays of data which are used
       for various purposes by binding them to a graphics or compute
       pipeline via descriptor sets or via certain commands,
       or by directly specifying them as parameters to certain commands. */

    class Buffer : public NonDispatchableResource<Buffer, VkBuffer>
    {
    public:
        ~Buffer();
        void bindMemory(std::shared_ptr<DeviceMemory> memory,
            VkDeviceSize offset = 0);
#ifdef VK_KHR_device_group
        void bindMemoryDeviceGroup(std::shared_ptr<DeviceMemory> memory,
            const std::vector<uint32_t>& deviceIndices,
            VkDeviceSize offset = 0);
#endif
        VkBufferUsageFlags getUsage() const noexcept { return usage; }
        VkMemoryRequirements getMemoryRequirements() const noexcept;
        VkDescriptorBufferInfo getDescriptor() const noexcept;

    protected:
        explicit Buffer(std::shared_ptr<Device> device,
            VkDeviceSize size,
            VkBufferUsageFlags usage,
            VkMemoryPropertyFlags memFlags,
            VkBufferCreateFlags flags,
            const Sharing& sharing,
            std::shared_ptr<IAllocator> allocator);
        explicit Buffer(std::shared_ptr<DeviceMemory> memory,
            VkDeviceSize size,
            VkDeviceSize offset,
            VkBufferUsageFlags usage,
            VkBufferCreateFlags flags,
            const Sharing& sharing,
            std::shared_ptr<IAllocator> allocator);
        void copyHost(const void *data,
            CopyMemoryFunction copyFn) noexcept;
        void copyTransfer(std::shared_ptr<CommandBuffer> cmdBuffer,
            std::shared_ptr<const Buffer> buffer,
            VkDeviceSize srcOffset = 0,
            VkDeviceSize dstOffset = 0,
            bool flush = true);

        const VkBufferUsageFlags usage;
    };
} // namespace magma
