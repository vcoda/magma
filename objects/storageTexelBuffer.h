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
#include "buffer.h"

namespace magma
{
    class SrcTransferBuffer;

    /* Texel buffer that is accessed from the shader stage through buffer view. */

    class StorageTexelBuffer : public Buffer
    {
    public:
        explicit StorageTexelBuffer(std::shared_ptr<CommandBuffer> cmdBuffer,
            VkDeviceSize size,
            const void *data,
            VkBufferCreateFlags flags = 0,
            const Sharing& sharing = Sharing(),
            std::shared_ptr<IAllocator> allocator = nullptr,
            CopyMemoryFunction copyFn = nullptr);
        explicit StorageTexelBuffer(std::shared_ptr<CommandBuffer> cmdBuffer,
            std::shared_ptr<const SrcTransferBuffer> srcBuffer,
            VkDeviceSize size = 0,
            VkDeviceSize srcOffset = 0,
            VkBufferCreateFlags flags = 0,
            const Sharing& sharing = Sharing(),
            std::shared_ptr<IAllocator> allocator = nullptr);
    };

    /* Dynamic storage texel buffer */

    class DynamicStorageTexelBuffer : public Buffer
    {
    public:
        explicit DynamicStorageTexelBuffer(std::shared_ptr<Device> device,
            VkDeviceSize size,
            const void *initialData = nullptr,
            VkBufferCreateFlags flags = 0,
            const Sharing& sharing = Sharing(),
            std::shared_ptr<IAllocator> allocator = nullptr,
            CopyMemoryFunction copyFn = nullptr);
    };
} // namespace magma
