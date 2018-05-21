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
#include "buffer.h"
#include "device.h"

namespace magma
{
    class CommandBuffer;

    class StorageBuffer : public Buffer
    {
    public:
        StorageBuffer(std::shared_ptr<const Device> device,
            VkDeviceSize size,
            VkBufferCreateFlags flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr);
        StorageBuffer(std::shared_ptr<CommandBuffer> copyCmdBuffer,
            const void *data, VkDeviceSize size,
            VkBufferCreateFlags flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr,
            CopyMemoryFunction copyFn = nullptr);
        template<typename Type>
        StorageBuffer(std::shared_ptr<CommandBuffer> copyCmdBuffer,
            const std::vector<Type>& data,
            VkBufferCreateFlags flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr);
    };

    template<typename Type>
    inline StorageBuffer::StorageBuffer(std::shared_ptr<CommandBuffer> copyCmdBuffer, 
        const std::vector<Type>& data, 
        VkBufferCreateFlags flags /* 0 */,
        std::shared_ptr<IAllocator> allocator /* nullptr */):
        StorageBuffer(copyCmdBuffer, data.data(), static_cast<VkDeviceSize>(sizeof(Type) * data.size()), flags, allocator)
    {}
} // namespace magma
