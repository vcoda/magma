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
#include "commandBuffer.h"
#include "transferBuffer.h"
#include "deviceMemory.h"
#include "queue.h"
#include "../helpers/alignedMemcpy.h"

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
        helpers::alignedMemcpy(buffer, data, static_cast<size_t>(size));
        memory->unmap();
    }
}

IndexBuffer::IndexBuffer(std::shared_ptr<CommandBuffer> copyCmdBuffer, const void *data, VkDeviceSize size, VkIndexType indexType,
    VkBufferCreateFlags flags /* 0 */):
    Buffer(copyCmdBuffer->getDevice(), size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, flags,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
    indexType(indexType)
{
    std::shared_ptr<SourceTransferBuffer> srcBuffer(new SourceTransferBuffer(device, data, size));
    copyCmdBuffer->begin();
    {
        VkBufferCopy region;
        region.srcOffset = 0;
        region.dstOffset = 0;
        region.size = size;
        vkCmdCopyBuffer(*copyCmdBuffer, *srcBuffer, handle, 1, &region);
    }
    copyCmdBuffer->end();
    std::shared_ptr<Queue> queue(device->getQueue(VK_QUEUE_TRANSFER_BIT, 0));
    queue->submit(copyCmdBuffer, 0, nullptr, nullptr, nullptr);
    queue->waitIdle();
}
} // namespace magma
