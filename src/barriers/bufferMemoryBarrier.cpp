/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

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
BufferMemoryBarrier::BufferMemoryBarrier(VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask,
    VkDeviceSize offset /* 0 */, VkDeviceSize size /* VK_WHOLE_SIZE */) noexcept:
    VkBufferMemoryBarrier{
        VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
        nullptr, // pNext
        srcAccessMask,
        dstAccessMask,
        VK_QUEUE_FAMILY_IGNORED, // srcQueueFamilyIndex
        VK_QUEUE_FAMILY_IGNORED, // dstQueueFamilyIndex
        VK_NULL_HANDLE, // buffer
        offset,
        size
    }
{}

BufferMemoryBarrier::BufferMemoryBarrier(std::shared_ptr<const Buffer> buffer_,
    VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask,
    VkDeviceSize offset /* 0 */, VkDeviceSize size /* VK_WHOLE_SIZE */) noexcept:
    VkBufferMemoryBarrier{
        VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
        nullptr, // pNext
        srcAccessMask,
        dstAccessMask,
        VK_QUEUE_FAMILY_IGNORED, // srcQueueFamilyIndex
        VK_QUEUE_FAMILY_IGNORED, // dstQueueFamilyIndex
        *buffer_,
        offset,
        size
    },
    buffer(std::move(buffer_))
{}

BufferMemoryBarrier::BufferMemoryBarrier(std::shared_ptr<const Buffer> buffer_, const BufferMemoryBarrier& barrier) noexcept:
    VkBufferMemoryBarrier{
        VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
        barrier.pNext,
        barrier.srcAccessMask,
        barrier.dstAccessMask,
        barrier.srcQueueFamilyIndex,
        barrier.dstQueueFamilyIndex,
        *buffer_,
        barrier.offset,
        barrier.size
    },
    buffer(std::move(buffer_))
{}

namespace barrier
{
const BufferMemoryBarrier hostWriteTransferRead(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT);
const BufferMemoryBarrier transferWriteHostRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_HOST_READ_BIT);
const BufferMemoryBarrier transferWriteShaderRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT);
const BufferMemoryBarrier shaderWriteTransferRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT);
} // namespace barrier
} // namespace magma
