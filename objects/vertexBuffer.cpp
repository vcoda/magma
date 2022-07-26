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
#include "vertexBuffer.h"
#include "srcTransferBuffer.h"
#include "commandBuffer.h"

namespace magma
{
BaseVertexBuffer::BaseVertexBuffer(std::shared_ptr<Device> device, VkDeviceSize size,
    VkBufferUsageFlags usage, VkBufferCreateFlags flags, VkMemoryPropertyFlags memoryFlags, float memoryPriority,
    const Sharing& sharing, std::shared_ptr<Allocator> allocator):
    Buffer(std::move(device), size, usage, flags, memoryFlags, memoryPriority, sharing, std::move(allocator)),
    vertexCount(0)
{}

VertexBuffer::VertexBuffer(std::shared_ptr<CommandBuffer> cmdBuffer, VkDeviceSize size, const void *data,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    VkBufferCreateFlags flags /* 0 */,
    float memoryPriority /* MAGMA_MEMORY_PRIORITY */,
    const Sharing& sharing /* default */,
    CopyMemoryFunction copyFn /* nullptr */):
    BaseVertexBuffer(cmdBuffer->getDevice(), size,
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, flags,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, memoryPriority,
        sharing, allocator)
{
    MAGMA_ASSERT(data);
    auto srcBuffer = std::make_shared<SrcTransferBuffer>(device, size, data, std::move(allocator), 0, 0.f, sharing, std::move(copyFn));
    cmdBuffer->begin();
    copyTransfer(cmdBuffer, srcBuffer, size);
    cmdBuffer->end();
    commitAndWait(std::move(cmdBuffer));
}

VertexBuffer::VertexBuffer(std::shared_ptr<CommandBuffer> cmdBuffer, std::shared_ptr<const SrcTransferBuffer> srcBuffer,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    VkDeviceSize size /* 0 */,
    VkDeviceSize srcOffset /* 0 */,
    VkBufferCreateFlags flags /* 0 */,
    float memoryPriority /* MAGMA_MEMORY_PRIORITY */,
    const Sharing& sharing /* default */):
    BaseVertexBuffer(cmdBuffer->getDevice(),
        size > 0 ? size : srcBuffer->getSize(),
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, flags,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, memoryPriority,
        sharing, std::move(allocator))
{
    copyTransfer(std::move(cmdBuffer), std::move(srcBuffer), size, srcOffset);
}

DynamicVertexBuffer::DynamicVertexBuffer(std::shared_ptr<Device> device, VkDeviceSize size, bool pinnedMemory,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const void *initialData /* nullptr */,
    VkBufferCreateFlags flags /* 0 */,
    float memoryPriority /* MAGMA_MEMORY_PRIORITY */,
    const Sharing& sharing /* default */,
    CopyMemoryFunction copyFn /* nullptr */):
    BaseVertexBuffer(std::move(device), size,
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, flags,
        (pinnedMemory ? VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT : 0) | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        memoryPriority,
        sharing, std::move(allocator))
{
    if (initialData)
        copyHost(initialData, std::move(copyFn));
}

#ifdef VK_NV_ray_tracing
AccelerationStructureVertexBuffer::AccelerationStructureVertexBuffer(std::shared_ptr<CommandBuffer> cmdBuffer, VkDeviceSize size, const void *data,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    VkBufferCreateFlags flags /* 0 */,
    float memoryPriority /* MAGMA_MEMORY_PRIORITY */,
    const Sharing& sharing /* default */,
    CopyMemoryFunction copyFn /* nullptr */):
    BaseVertexBuffer(cmdBuffer->getDevice(), size,
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
#ifdef VK_KHR_acceleration_structure
        (device->extensionEnabled(VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME) ?
            VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR : 0) |
#endif
        VK_BUFFER_USAGE_TRANSFER_DST_BIT, flags,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, memoryPriority,
        sharing, allocator)
{
    MAGMA_ASSERT(data);
    auto srcBuffer =  std::make_shared<SrcTransferBuffer>(device, size, data, std::move(allocator), 0, 0.f, sharing, std::move(copyFn));
    cmdBuffer->begin();
    copyTransfer(cmdBuffer, srcBuffer, size);
    cmdBuffer->end();
    commitAndWait(std::move(cmdBuffer));
}

AccelerationStructureVertexBuffer::AccelerationStructureVertexBuffer(std::shared_ptr<CommandBuffer> cmdBuffer, std::shared_ptr<const SrcTransferBuffer> srcBuffer,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    VkDeviceSize size /* 0 */,
    VkDeviceSize srcOffset /* 0 */,
    VkBufferCreateFlags flags /* 0 */,
    float memoryPriority /* MAGMA_MEMORY_PRIORITY */,
    const Sharing& sharing /* default */):
    BaseVertexBuffer(cmdBuffer->getDevice(),
        size > 0 ? size : srcBuffer->getSize(),
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
#ifdef VK_KHR_acceleration_structure
        (device->extensionEnabled(VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME) ?
            VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR : 0) |
#endif
        VK_BUFFER_USAGE_TRANSFER_DST_BIT, flags,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, memoryPriority,
        sharing, std::move(allocator))
{
    copyTransfer(std::move(cmdBuffer), std::move(srcBuffer), size, srcOffset);
}
#endif // VK_NV_ray_tracing
} // namespace magma
