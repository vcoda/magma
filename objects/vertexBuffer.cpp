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
#include "vertexBuffer.h"
#include "commandBuffer.h"
#include "transferBuffer.h"
#include "deviceMemory.h"
#include "queue.h"
#include "../mem/copyMemory.h"

namespace magma
{
VertexBuffer::VertexBuffer(std::shared_ptr<const Device> device, VkDeviceSize size, VkBufferUsageFlags usage,
    VkBufferCreateFlags flags, std::shared_ptr<IAllocator> allocator, VkMemoryPropertyFlags memoryFlags):
    Buffer(device, size, usage, flags, allocator, memoryFlags),
    vertexCount(0)
{}

VertexBuffer::VertexBuffer(std::shared_ptr<const Device> device, const void *data, VkDeviceSize size, uint32_t vertexCount,
    VkBufferCreateFlags flags /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    CopyMemoryFunction copyFn /* nullptr */):
    Buffer(device, size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, flags, allocator, 
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
    vertexCount(vertexCount)
{
    if (data)
    {
        if (void *buffer = memory->map(0, size))
        {
            if (!copyFn)
                copyFn = copyMemory;
            copyFn(buffer, data, static_cast<size_t>(size));
            memory->unmap();
        }
    }
}

VertexBuffer::VertexBuffer(std::shared_ptr<CommandBuffer> copyCmdBuffer, const void *data, VkDeviceSize size, uint32_t vertexCount,
    VkBufferCreateFlags flags /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    CopyMemoryFunction copyFn /* nullptr */):
    Buffer(copyCmdBuffer->getDevice(), size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, flags, allocator,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
    vertexCount(vertexCount)
{        
    std::shared_ptr<SourceTransferBuffer> srcBuffer(std::make_shared<SourceTransferBuffer>(device, data, size, 0, allocator, copyFn));
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
