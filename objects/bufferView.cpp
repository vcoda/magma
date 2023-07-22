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
#include "pch.h"
#pragma hdrstop
#include "bufferView.h"
#include "buffer.h"
#include "device.h"
#include "../allocator/allocator.h"
#include "../exceptions/errorResult.h"

namespace magma
{
BufferView::BufferView(std::shared_ptr<Buffer> resource,
    VkFormat format,
    VkDeviceSize offset /* 0 */,
    VkDeviceSize range /* VK_WHOLE_SIZE */,
    const StructureChain& extendedInfo /* default */):
    NonDispatchable(VK_OBJECT_TYPE_BUFFER_VIEW, resource->getDevice(), resource->getHostAllocator()),
    buffer(std::move(resource)),
    format(format),
    offset(offset),
    range(range)
{
    MAGMA_ASSERT(buffer->getUsage() & (VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT));
    VkBufferViewCreateInfo bufferViewInfo;
    bufferViewInfo.sType = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;
    bufferViewInfo.pNext = extendedInfo.chainNodes();
    bufferViewInfo.flags = 0;
    bufferViewInfo.buffer = *buffer;
    bufferViewInfo.format = format;
    bufferViewInfo.offset = offset;
    bufferViewInfo.range = range;
    const VkResult result = vkCreateBufferView(MAGMA_HANDLE(device), &bufferViewInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to create buffer view");
}

BufferView::~BufferView()
{
    vkDestroyBufferView(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}
} // namespace magma
