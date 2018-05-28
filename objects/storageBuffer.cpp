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
#include "storageBuffer.h"
#include "srcTransferBuffer.h"
#include "commandBuffer.h"
#include "queue.h"
#include "fence.h"
#include "../misc/exception.h"

namespace magma
{
StorageBuffer::StorageBuffer(std::shared_ptr<const Device> device, VkDeviceSize size,
    VkBufferCreateFlags flags /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Buffer(device, size, 
        VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        flags, allocator,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
{}

StorageBuffer::StorageBuffer(std::shared_ptr<CommandBuffer> copyCmdBuffer, const void *data, VkDeviceSize size,
    VkBufferCreateFlags flags /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    CopyMemoryFunction copyFn /* nullptr */):
    Buffer(copyCmdBuffer->getDevice(), size, 
        VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, 
        flags, allocator,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
{
    std::shared_ptr<SrcTransferBuffer> srcBuffer(std::make_shared<SrcTransferBuffer>(device, data, size, 0, allocator, copyFn));
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
    std::shared_ptr<Fence> fence(std::make_shared<Fence>(device));
    if (!queue->submit(copyCmdBuffer, 0, nullptr, nullptr, nullptr))
        MAGMA_THROW("failed to submit command buffer to graphics queue");
    if (!fence->wait())
        MAGMA_THROW("failed to wait for the fence");
}
} // namespace magma
