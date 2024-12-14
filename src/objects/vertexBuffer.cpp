/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

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
    VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryFlags,
    const Initializer& optional, const Sharing& sharing, std::shared_ptr<Allocator> allocator):
    Buffer(std::move(device), size,
        0, // flags
        usage,
        memoryFlags,
        optional, sharing, std::move(allocator)),
    vertexCount(0)
{}

VertexBuffer::VertexBuffer(std::shared_ptr<Device> device, VkDeviceSize size,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* default */):
    BaseVertexBuffer(std::move(device), size,
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        optional, sharing, allocator)
{}

VertexBuffer::VertexBuffer(lent_ptr<CommandBuffer> cmdBuffer, VkDeviceSize size, const void *data,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* default */,
    CopyMemoryFunction copyFn /* nullptr */):
    VertexBuffer(cmdBuffer->getDevice(), size, allocator, optional, sharing)
{
    copyStaged(std::move(cmdBuffer), data, std::move(allocator), std::move(copyFn));
}

VertexBuffer::VertexBuffer(lent_ptr<CommandBuffer> cmdBuffer, std::shared_ptr<const SrcTransferBuffer> srcBuffer,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    VkDeviceSize size /* 0 */,
    VkDeviceSize srcOffset /* 0 */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* default */):
    VertexBuffer(cmdBuffer->getDevice(),
        size ? size : srcBuffer->getSize(),
        std::move(allocator), optional, sharing)
{
    copyTransfer(std::move(cmdBuffer), std::move(srcBuffer), srcOffset);
}

DynamicVertexBuffer::DynamicVertexBuffer(std::shared_ptr<Device> device, VkDeviceSize size, bool stagedPool,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const void *initialData /* nullptr */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* default */,
    CopyMemoryFunction copyFn /* nullptr */):
    BaseVertexBuffer(std::move(device), size,
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT |
            (stagedPool ? VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT : 0),
        optional, sharing, std::move(allocator))
{
    if (initialData)
        copyHost(initialData, size, 0, 0, VK_WHOLE_SIZE, std::move(copyFn));
}
} // namespace magma
