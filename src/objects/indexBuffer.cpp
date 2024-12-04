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
#include "indexBuffer.h"
#include "srcTransferBuffer.h"
#include "commandBuffer.h"

namespace magma
{
BaseIndexBuffer::BaseIndexBuffer(std::shared_ptr<Device> device, VkIndexType indexType, VkDeviceSize size,
    VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryFlags,
    const Initializer& optional, const Sharing& sharing, std::shared_ptr<Allocator> allocator):
    Buffer(std::move(device), size,
        0, // flags
        usage,
        memoryFlags,
        optional, sharing, std::move(allocator)),
    indexType(indexType)
{}

std::size_t BaseIndexBuffer::getIndexSize() const noexcept
{
    switch (indexType)
    {
    case VK_INDEX_TYPE_UINT16:
        return sizeof(uint16_t);
    case VK_INDEX_TYPE_UINT32:
        return sizeof(uint32_t);
#if defined(VK_KHR_index_type_uint8)
    case VK_INDEX_TYPE_UINT8_KHR:
        return sizeof(uint8_t);
#elif defined(VK_EXT_index_type_uint8)
    case VK_INDEX_TYPE_UINT8_EXT:
        return sizeof(uint8_t);
#endif // VK_EXT_index_type_uint8
    default:
        MAGMA_FAILURE("invalid index type");
        return 0;
    }
}

uint32_t BaseIndexBuffer::getIndexCount() const noexcept
{
    const std::size_t indexSize = getIndexSize();
    MAGMA_ASSERT(size % indexSize == 0);
    return static_cast<uint32_t>(size / indexSize);
}

IndexBuffer::IndexBuffer(std::shared_ptr<Device> device, VkIndexType indexType, VkDeviceSize size,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* default */):
    BaseIndexBuffer(std::move(device), indexType, size,
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        optional, sharing, allocator)
{}

IndexBuffer::IndexBuffer(const std::unique_ptr<CommandBuffer>& cmdBuffer, VkIndexType indexType, VkDeviceSize size, const void *data,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* default */,
    CopyMemoryFunction copyFn /* nullptr */):
    IndexBuffer(cmdBuffer->getDevice(), indexType, size, allocator, optional, sharing)
{
    copyStaged(cmdBuffer, data, std::move(allocator), std::move(copyFn));
}

IndexBuffer::IndexBuffer(const std::unique_ptr<CommandBuffer>& cmdBuffer, VkIndexType indexType, std::shared_ptr<const SrcTransferBuffer> srcBuffer,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    VkDeviceSize size /* 0 */,
    VkDeviceSize srcOffset /* 0 */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* default */):
    IndexBuffer(cmdBuffer->getDevice(), indexType,
        size ? size : srcBuffer->getSize(),
        std::move(allocator), optional, sharing)
{
    copyTransfer(cmdBuffer, std::move(srcBuffer), srcOffset);
}

DynamicIndexBuffer::DynamicIndexBuffer(std::shared_ptr<Device> device, VkIndexType indexType, VkDeviceSize size, bool stagedPool,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const void *initialData /* nullptr */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* default */,
    CopyMemoryFunction copyFn /* nullptr */):
    BaseIndexBuffer(std::move(device), indexType, size,
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT |
            (stagedPool ? VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT : 0),
        optional, sharing, std::move(allocator))
{
    if (initialData)
        copyHost(initialData, size, 0, 0, VK_WHOLE_SIZE, std::move(copyFn));
}
} // namespace magma
