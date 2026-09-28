/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2026 Victor Coda.

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
#include "pch.h"
#pragma hdrstop
#include "bufferMemoryBarrier.h"
#include "../objects/buffer.h"

namespace magma
{
BufferMemoryBarrier::BufferMemoryBarrier(const Buffer *buffer,
    VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask,
    VkDeviceSize offset /* 0 */, VkDeviceSize size /* VK_WHOLE_SIZE */) noexcept:
    VkBufferMemoryBarrier{
        VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
        nullptr, // pNext
        srcAccessMask,
        dstAccessMask,
        VK_QUEUE_FAMILY_IGNORED,
        VK_QUEUE_FAMILY_IGNORED,
        *buffer,
        offset,
        size
    }
{}

BufferMemoryBarrier::BufferMemoryBarrier(const Buffer *buffer, const BufferMemoryBarrier& barrier,
    VkDeviceSize offset /* 0 */, VkDeviceSize size /* VK_WHOLE_SIZE */) noexcept:
    BufferMemoryBarrier(buffer, barrier.srcAccessMask, barrier.dstAccessMask, offset, size)
{}
} // namespace magma
