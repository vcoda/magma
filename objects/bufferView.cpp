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
#include "bufferView.h"
#include "buffer.h"
#include "device.h"
#include "../allocator/allocator.h"
#include "../misc/exception.h"
#include "../shared.h"

namespace magma
{
BufferView::BufferView(std::shared_ptr<const Buffer> resource,
    VkFormat format,
    VkDeviceSize offset /* 0 */,
    VkDeviceSize range /* VK_WHOLE_SIZE */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    NonDispatchable(VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_VIEW_EXT, std::move(resource->getDevice()), std::move(allocator)),
    buffer(std::move(resource))
{
    MAGMA_ASSERT(resource->getUsage() &
        (VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT |
         VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT));
    VkBufferViewCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.buffer = *buffer;
    info.format = format;
    info.offset = offset;
    info.range = range;
    const VkResult create = vkCreateBufferView(MAGMA_HANDLE(device), &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create buffer view");
}

BufferView::~BufferView()
{
    vkDestroyBufferView(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(allocator));
}
} // namespace magma
