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

    /* Uniform texel buffer that is accessed from the shader stage through buffer view. */

    class UniformTexelBuffer : public Buffer
    {
    public:
        explicit UniformTexelBuffer(std::shared_ptr<CommandBuffer> cmdBuffer,
            VkDeviceSize size,
            const void *data,
            VkBufferCreateFlags flags = 0,
            const Sharing& sharing = Sharing(),
            std::shared_ptr<IAllocator> allocator = nullptr,
            CopyMemoryFunction copyFn = nullptr);
        explicit UniformTexelBuffer(std::shared_ptr<CommandBuffer> cmdBuffer,
            std::shared_ptr<const SrcTransferBuffer> buffer,
            VkBufferCreateFlags flags = 0,
            const Sharing& sharing = Sharing(),
            std::shared_ptr<IAllocator> allocator = nullptr);
    };
} // namespace magma
