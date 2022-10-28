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
#include "indexBuffer.h"
#include "srcTransferBuffer.h"
#include "device.h"
#include "commandBuffer.h"

namespace magma
{
BaseIndexBuffer::BaseIndexBuffer(std::shared_ptr<Device> device, VkDeviceSize size, VkIndexType indexType,
    VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryFlags,
    const Descriptor& optional, const Sharing& sharing, std::shared_ptr<Allocator> allocator):
    Buffer(std::move(device), size,
        0, // flags
        usage,
        memoryFlags,
        optional, sharing, std::move(allocator)),
    indexType(indexType)
{}

uint32_t BaseIndexBuffer::getIndexCount() const noexcept
{
    std::size_t indexSize;
    switch (indexType)
    {
    case VK_INDEX_TYPE_UINT16:
        indexSize = sizeof(uint16_t);
        break;
    case VK_INDEX_TYPE_UINT32:
        indexSize = sizeof(uint32_t);
        break;
#ifdef VK_EXT_index_type_uint8
    case VK_INDEX_TYPE_UINT8_EXT:
        indexSize = sizeof(uint8_t);
        break;
#endif
    default:
        MAGMA_ASSERT(false);
        indexSize = 1;
    }
    return static_cast<uint32_t>(size/indexSize);
}

IndexBuffer::IndexBuffer(std::shared_ptr<CommandBuffer> cmdBuffer, VkDeviceSize size, const void *data, VkIndexType indexType,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Descriptor& optional /* default */,
    const Sharing& sharing /* default */,
    CopyMemoryFunction copyFn /* nullptr */):
    BaseIndexBuffer(cmdBuffer->getDevice(), size, indexType,
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        optional, sharing, allocator)
{
    MAGMA_ASSERT(data);
    auto srcBuffer = std::make_shared<SrcTransferBuffer>(device, size, data, std::move(allocator), Descriptor(), sharing, std::move(copyFn));
    cmdBuffer->begin();
    copyTransfer(cmdBuffer, srcBuffer, size);
    cmdBuffer->end();
    commitAndWait(std::move(cmdBuffer));
}

IndexBuffer::IndexBuffer(std::shared_ptr<CommandBuffer> cmdBuffer, std::shared_ptr<const SrcTransferBuffer> srcBuffer, VkIndexType indexType,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    VkDeviceSize size /* 0 */,
    VkDeviceSize srcOffset /* 0 */,
    const Descriptor& optional /* default */,
    const Sharing& sharing /* default */):
    BaseIndexBuffer(srcBuffer->getDevice(),
        size > 0 ? size : srcBuffer->getSize(), indexType,
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        optional, sharing, std::move(allocator))
{
    copyTransfer(std::move(cmdBuffer), std::move(srcBuffer), size, srcOffset);
}

DynamicIndexBuffer::DynamicIndexBuffer(std::shared_ptr<Device> device, VkDeviceSize size, VkIndexType indexType, bool pinnedMemory,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const void *initialData /* nullptr */,
    const Descriptor& optional /* default */,
    const Sharing& sharing /* default */,
    CopyMemoryFunction copyFn /* nullptr */):
    BaseIndexBuffer(std::move(device), size, indexType,
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        (pinnedMemory ? VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT : 0) | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        optional, sharing, std::move(allocator))
{
    if (initialData)
        copyHost(initialData, std::move(copyFn));
}

#ifdef VK_NV_ray_tracing
AccelerationStructureIndexBuffer::AccelerationStructureIndexBuffer(std::shared_ptr<CommandBuffer> cmdBuffer, VkDeviceSize size, const void *data, VkIndexType indexType,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Descriptor& optional /* default */,
    const Sharing& sharing /* default */,
    CopyMemoryFunction copyFn /* nullptr */):
    BaseIndexBuffer(cmdBuffer->getDevice(), size, indexType,
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
#ifdef VK_KHR_acceleration_structure
        (device->extensionEnabled(VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME) ?
            VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR : 0) |
#endif
        VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        optional, sharing, allocator)
{
    MAGMA_ASSERT(data);
    auto srcBuffer = std::make_shared<SrcTransferBuffer>(device, size, data, std::move(allocator), Descriptor(), sharing, std::move(copyFn));
    cmdBuffer->begin();
    copyTransfer(cmdBuffer, srcBuffer, size);
    cmdBuffer->end();
    commitAndWait(std::move(cmdBuffer));
}

AccelerationStructureIndexBuffer::AccelerationStructureIndexBuffer(std::shared_ptr<CommandBuffer> cmdBuffer, std::shared_ptr<const SrcTransferBuffer> srcBuffer, VkIndexType indexType,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    VkDeviceSize size /* 0 */,
    VkDeviceSize srcOffset /* 0 */,
    const Descriptor& optional /* default */,
    const Sharing& sharing /* default */):
    BaseIndexBuffer(srcBuffer->getDevice(),
        size > 0 ? size : srcBuffer->getSize(), indexType,
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
#ifdef VK_KHR_acceleration_structure
        (device->extensionEnabled(VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME) ?
            VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR : 0) |
#endif
        VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        optional, sharing, std::move(allocator))
{
    copyTransfer(std::move(cmdBuffer), std::move(srcBuffer), size, srcOffset);
}
#endif // VK_NV_ray_tracing
} // namespace magma
