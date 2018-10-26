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

namespace magma
{
    class SrcTransferBuffer;
    class CommandBuffer;

    /* Buffer data for raytracing pipeline. */

    class RaytracingBuffer : public Buffer
    {
    public:
        explicit RaytracingBuffer(std::shared_ptr<CommandBuffer> copyCmdBuffer,
            const void *data, VkDeviceSize size,
            VkBufferCreateFlags flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr,
            CopyMemoryFunction copyFn = nullptr);
        explicit RaytracingBuffer(std::shared_ptr<CommandBuffer> copyCmdBuffer,
            std::shared_ptr<SrcTransferBuffer> srcBuffer,
            VkBufferCreateFlags flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr);
    };
} // namespace magma

