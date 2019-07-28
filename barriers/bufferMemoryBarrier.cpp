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
#include "pch.h"
#pragma hdrstop
#include "bufferMemoryBarrier.h"
#include "../objects/buffer.h"

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

BufferMemoryBarrier::BufferMemoryBarrier(std::shared_ptr<const Buffer> buffer, const BufferMemoryBarrier& predefined) noexcept
{
    *this = predefined;
    this->buffer = *buffer;
}
} // namespace magma
