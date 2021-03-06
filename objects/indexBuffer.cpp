/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
    VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryFlags, VkBufferCreateFlags flags,
    const Sharing& sharing, std::shared_ptr<Allocator> allocator):
    Buffer(std::move(device), size, usage, memoryFlags, flags, sharing, std::move(allocator)),
    indexType(indexType)
{}

uint32_t BaseIndexBuffer::getIndexCount() const noexcept
{
    if (VK_INDEX_TYPE_UINT16 == indexType)
        return static_cast<uint32_t>(size / sizeof(uint16_t));
    return static_cast<uint32_t>(size / sizeof(uint32_t));
}

IndexBuffer::IndexBuffer(std::shared_ptr<CommandBuffer> cmdBuffer, VkDeviceSize size, const void *data, VkIndexType indexType,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    VkBufferCreateFlags flags /* 0 */,
    const Sharing& sharing /* default */,
    CopyMemoryFunction copyFn /* nullptr */):
    BaseIndexBuffer(cmdBuffer->getDevice(), size, indexType,
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        flags, sharing, allocator)
{
    MAGMA_ASSERT(data);
    auto buffer = std::make_shared<SrcTransferBuffer>(device, size, data,
        std::move(allocator), 0, sharing, std::move(copyFn));
    copyTransfer(std::move(cmdBuffer), std::move(buffer), size);
}

IndexBuffer::IndexBuffer(std::shared_ptr<CommandBuffer> cmdBuffer, std::shared_ptr<const SrcTransferBuffer> srcBuffer, VkIndexType indexType,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    VkDeviceSize size /* 0 */,
    VkDeviceSize srcOffset /* 0 */,
    VkBufferCreateFlags flags /* 0 */,
    const Sharing& sharing /* default */):
    BaseIndexBuffer(cmdBuffer->getDevice(), size > 0 ? size : srcBuffer->getSize(), indexType,
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        flags, sharing, std::move(allocator))
{
    copyTransfer(std::move(cmdBuffer), std::move(srcBuffer), size, srcOffset);
}

DynamicIndexBuffer::DynamicIndexBuffer(std::shared_ptr<Device> device, VkDeviceSize size, VkIndexType indexType, bool pinnedMemory,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const void *initial /* nullptr */,
    VkBufferCreateFlags flags /* 0 */,
    const Sharing& sharing /* default */,
    CopyMemoryFunction copyFn /* nullptr */):
    BaseIndexBuffer(std::move(device), size, indexType,
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        (pinnedMemory ? VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT : 0) | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        flags, sharing, std::move(allocator))
{
    if (initial)
        copyHost(initial, std::move(copyFn));
}

#ifdef VK_NV_ray_tracing
AccelerationStructureIndexBuffer::AccelerationStructureIndexBuffer(std::shared_ptr<CommandBuffer> cmdBuffer, VkDeviceSize size, const void *data, VkIndexType indexType,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    VkBufferCreateFlags flags /* 0 */,
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
        flags, sharing, allocator)
{
    MAGMA_ASSERT(data);
    auto buffer = std::make_shared<SrcTransferBuffer>(device, size, data,
        std::move(allocator), 0, sharing, std::move(copyFn));
    copyTransfer(std::move(cmdBuffer), std::move(buffer), size);
}

AccelerationStructureIndexBuffer::AccelerationStructureIndexBuffer(std::shared_ptr<CommandBuffer> cmdBuffer, std::shared_ptr<const SrcTransferBuffer> srcBuffer, VkIndexType indexType,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    VkDeviceSize size /* 0 */,
    VkDeviceSize srcOffset /* 0 */,
    VkBufferCreateFlags flags /* 0 */,
    const Sharing& sharing /* default */):
    BaseIndexBuffer(cmdBuffer->getDevice(), size > 0 ? size : srcBuffer->getSize(), indexType,
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
#ifdef VK_KHR_acceleration_structure
        (device->extensionEnabled(VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME) ?
            VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR : 0) |
#endif
        VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        flags, sharing, std::move(allocator))
{
    copyTransfer(std::move(cmdBuffer), std::move(srcBuffer), size, srcOffset);
}
#endif // VK_NV_ray_tracing
} // namespace magma
