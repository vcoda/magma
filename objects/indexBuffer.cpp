/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2019 Victor Coda.

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
#include "indexBuffer.h"
#include "srcTransferBuffer.h"
#include "commandBuffer.h"
#include "deviceMemory.h"

namespace magma
{
BaseIndexBuffer::BaseIndexBuffer(std::shared_ptr<Device> device, VkDeviceSize size, VkIndexType indexType,
    VkBufferUsageFlags usage, VkMemoryPropertyFlags memFlags, VkBufferCreateFlags flags,
    const Sharing& sharing, std::shared_ptr<IAllocator> allocator):
    Buffer(std::move(device), size, usage, memFlags, flags, sharing, std::move(allocator)),
    indexType(indexType)
{}

IndexBuffer::IndexBuffer(std::shared_ptr<CommandBuffer> cmdBuffer, VkDeviceSize size, const void *data, VkIndexType indexType,
    VkBufferCreateFlags flags /* 0 */,
    const Sharing& sharing /* default */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    CopyMemoryFunction copyFn /* nullptr */):
    BaseIndexBuffer(std::move(cmdBuffer->getDevice()), size, indexType,
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        flags, sharing, allocator)
{
    MAGMA_ASSERT(data);
    auto buffer = std::make_shared<SrcTransferBuffer>(device, size, data,
        0, sharing, std::move(allocator), std::move(copyFn));
    copyTransfer(std::move(cmdBuffer), std::move(buffer));
}

IndexBuffer::IndexBuffer(std::shared_ptr<CommandBuffer> cmdBuffer, std::shared_ptr<const SrcTransferBuffer> srcBuffer, VkIndexType indexType,
    VkDeviceSize size /* 0 */,
    VkDeviceSize srcOffset /* 0 */,
    VkBufferCreateFlags flags /* 0 */,
    const Sharing& sharing /* default */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    BaseIndexBuffer(cmdBuffer->getDevice(), size > 0 ? size : srcBuffer->getSize(), indexType,
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        flags, sharing, std::move(allocator))
{
    copyTransfer(std::move(cmdBuffer), std::move(srcBuffer), srcOffset, 0, this->getSize());
}

DynamicIndexBuffer::DynamicIndexBuffer(std::shared_ptr<Device> device, VkDeviceSize size, VkIndexType indexType,
    const void *initial /* nullptr */,
    VkBufferCreateFlags flags /* 0 */,
    const Sharing& sharing /* default */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    CopyMemoryFunction copyFn /* nullptr */):
    BaseIndexBuffer(std::move(device), size, indexType,
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        flags, sharing, std::move(allocator))
{
    if (initial)
        copyHost(initial, std::move(copyFn));
}
} // namespace magma
