/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2022 Victor Coda.

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
            std::shared_ptr<Allocator> allocator = nullptr,
            VkBufferCreateFlags flags = 0,
            float memoryPriority = 0.f,
            const Sharing& sharing = Sharing(),
            CopyMemoryFunction copyFn = nullptr);
        explicit StorageTexelBuffer(std::shared_ptr<CommandBuffer> cmdBuffer,
            std::shared_ptr<const SrcTransferBuffer> srcBuffer,
            std::shared_ptr<Allocator> allocator = nullptr,
            VkDeviceSize size = 0,
            VkDeviceSize srcOffset = 0,
            VkBufferCreateFlags flags = 0,
            float memoryPriority = 0.f,
            const Sharing& sharing = Sharing());
    };

    /* Dynamic storage texel buffer for fast data transfer from host to device
       when using page-locked (or "pinned") memory. */

    class DynamicStorageTexelBuffer : public Buffer
    {
    public:
        explicit DynamicStorageTexelBuffer(std::shared_ptr<Device> device,
            VkDeviceSize size,
            bool pinnedMemory,
            std::shared_ptr<Allocator> allocator = nullptr,
            VkBufferCreateFlags flags = 0,
            const void *initialData = nullptr,
            float memoryPriority = 0.f,
            const Sharing& sharing = Sharing(),
            CopyMemoryFunction copyFn = nullptr);
    };
} // namespace magma
