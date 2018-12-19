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
#include "bufferMemoryBarrier.h"
#include "../objects/buffer.h"
#include "../internal/shared.h"

namespace magma
{
BufferMemoryBarrier::BufferMemoryBarrier(std::shared_ptr<const Buffer> buffer,
    VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask) noexcept
{
    sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
    pNext = nullptr;
    this->srcAccessMask = srcAccessMask;
    this->dstAccessMask = dstAccessMask;
    srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    this->buffer = MAGMA_OPTIONAL_HANDLE(buffer);
    offset = 0;
    size = VK_WHOLE_SIZE;
}

BufferMemoryBarrier::BufferMemoryBarrier(VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask) noexcept:
    BufferMemoryBarrier(nullptr, srcAccessMask, dstAccessMask)
{}

BufferMemoryBarrier::BufferMemoryBarrier(std::shared_ptr<const Buffer> buffer, const BufferMemoryBarrier& predefined) noexcept
{
    *this = predefined;
    this->buffer = *buffer;
}

namespace barriers
{
const BufferMemoryBarrier hostWriteTransferRead(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT);
const BufferMemoryBarrier transferWriteHostRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_HOST_READ_BIT);
const BufferMemoryBarrier transferWriteShaderRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT);
const BufferMemoryBarrier shaderWriteTransferRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT);
} // namespace barriers
} // namespace magma
