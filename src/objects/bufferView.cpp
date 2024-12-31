/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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
#include "bufferView.h"
#include "buffer.h"
#include "device.h"
#include "../allocator/allocator.h"
#include "../exceptions/errorResult.h"

namespace magma
{
BufferView::BufferView(const Buffer *buffer, VkFormat format,
    VkDeviceSize offset, VkDeviceSize range, const StructureChain& extendedInfo):
    NonDispatchable(VK_OBJECT_TYPE_BUFFER_VIEW, buffer->getDevice(), buffer->getHostAllocator()),
    format(format),
    offset(offset),
    range(range)
{
    VkBufferViewCreateInfo bufferViewInfo;
    bufferViewInfo.sType = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;
    bufferViewInfo.pNext = extendedInfo.headNode();
    bufferViewInfo.flags = 0;
    bufferViewInfo.buffer = *buffer;
    bufferViewInfo.format = format;
    bufferViewInfo.offset = offset;
    bufferViewInfo.range = range;
    const VkResult result = vkCreateBufferView(getNativeDevice(), &bufferViewInfo, MAGMA_OPTIONAL(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create buffer view");
}

BufferView::~BufferView()
{
    vkDestroyBufferView(getNativeDevice(), handle, MAGMA_OPTIONAL(hostAllocator));
}
} // namespace magma
