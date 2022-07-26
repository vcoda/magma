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
    /* A buffer that can be used as the source of a transfer command. */

    class SrcTransferBuffer : public Buffer
    {
    public:
        explicit SrcTransferBuffer(std::shared_ptr<Device> device,
            VkDeviceSize size,
            const void *data = nullptr,
            std::shared_ptr<Allocator> allocator = nullptr,
            VkBufferCreateFlags flags = 0,
            float memoryPriority = MAGMA_MEMORY_PRIORITY,
            const Sharing& sharing = Sharing(),
            CopyMemoryFunction copyFn = nullptr);
        template<typename Type>
        explicit SrcTransferBuffer(std::shared_ptr<Device> device,
            const std::vector<Type>& data,
            std::shared_ptr<Allocator> allocator = nullptr,
            VkBufferCreateFlags flags = 0,
            float memoryPriority = MAGMA_MEMORY_PRIORITY,
            const Sharing& sharing = Sharing(),
            CopyMemoryFunction copyFn = nullptr);
    };
} // namespace magma

#include "srcTransferBuffer.inl"
