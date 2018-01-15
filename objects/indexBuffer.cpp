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
#include "indexBuffer.h"

namespace magma
{
IndexBuffer::IndexBuffer(std::shared_ptr<const Device> device, VkDeviceSize size, VkBufferUsageFlags usage,
    VkBufferCreateFlags flags, VkIndexType indexType, VkMemoryPropertyFlags memoryFlags):
    Buffer(device, size, usage, flags, memoryFlags),
    indexType(indexType) 
{}

IndexBuffer::IndexBuffer(std::shared_ptr<const Device> device, const void *data, VkDeviceSize size, VkIndexType indexType,
    VkBufferCreateFlags flags /* 0 */):
    Buffer(device, size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, flags,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
    indexType(indexType)
{
    if (void *buffer = memory->map(0, size))
    {
        memcpy(buffer, data, static_cast<size_t>(size));
        memory->unmap();
    }
}

LocalIndexBuffer::LocalIndexBuffer(std::shared_ptr<const Device> device, VkDeviceSize size, VkIndexType indexType,
    VkBufferCreateFlags flags /* 0 */):
    IndexBuffer(device, size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, flags, indexType, 
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) 
{}
} // namespace magma
