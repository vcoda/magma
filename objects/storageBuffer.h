/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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

namespace magma
{
    class SrcTransferBuffer;

    /* A storage buffer described in a shader as a structure with various members
       that load, store, and atomic operations can be performed on. */

    class StorageBuffer : public Buffer
    {
    public:
        explicit StorageBuffer(std::shared_ptr<CommandBuffer> cmdBuffer,
            VkDeviceSize size,
            const void *data,
            std::shared_ptr<Allocator> allocator = nullptr,
            VkBufferCreateFlags flags = 0,
            const Sharing& sharing = Sharing(),
            CopyMemoryFunction copyFn = nullptr);
        template<typename Type>
        explicit StorageBuffer(std::shared_ptr<CommandBuffer> cmdBuffer,
            const std::vector<Type>& vertices,
            std::shared_ptr<Allocator> allocator = nullptr,
            VkBufferCreateFlags flags = 0,
            const Sharing& sharing = Sharing(),
            CopyMemoryFunction copyFn = nullptr);
        explicit StorageBuffer(std::shared_ptr<CommandBuffer> cmdBuffer,
            std::shared_ptr<const SrcTransferBuffer> srcBuffer,
            std::shared_ptr<Allocator> allocator = nullptr,
            VkDeviceSize size = 0,
            VkDeviceSize srcOffset = 0,
            VkBufferCreateFlags flags = 0,
            const Sharing& sharing = Sharing());
    };

    /* Dynamic storage buffer for fast data transfer from host to device 
       when using page-locked (or "pinned") memory. */

    class DynamicStorageBuffer : public Buffer
    {
    public:
        explicit DynamicStorageBuffer(std::shared_ptr<Device> device,
            VkDeviceSize size,
            bool pinnedMemory,
            std::shared_ptr<Allocator> allocator = nullptr,
            const void *initial = nullptr,
            VkBufferCreateFlags flags = 0,
            const Sharing& sharing = Sharing(),
            CopyMemoryFunction copyFn = nullptr);
    };
} // namespace magma

#include "storageBuffer.inl"
