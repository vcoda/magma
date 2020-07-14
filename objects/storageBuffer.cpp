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
#include "storageBuffer.h"
#include "srcTransferBuffer.h"
#include "commandBuffer.h"
#include "deviceMemory.h"

namespace magma
{
StorageBuffer::StorageBuffer(std::shared_ptr<CommandBuffer> cmdBuffer, const void *data, VkDeviceSize size,
    VkBufferCreateFlags flags /* 0 */,
    const Sharing& sharing /* default */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    CopyMemoryFunction copyFn /* nullptr */):
    Buffer(std::move(cmdBuffer->getDevice()), size,
        VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        flags, sharing, allocator)
{
    auto buffer = std::make_shared<SrcTransferBuffer>(device, size, data, 0,
        sharing, std::move(allocator), std::move(copyFn));
    copyTransfer(std::move(cmdBuffer), std::move(buffer));
}

StorageBuffer::StorageBuffer(std::shared_ptr<CommandBuffer> cmdBuffer, std::shared_ptr<const SrcTransferBuffer> buffer,
    VkBufferCreateFlags flags /* 0 */,
    const Sharing& sharing /* default */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Buffer(std::move(cmdBuffer->getDevice()), buffer->getSize(),
        VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        flags, sharing, std::move(allocator))
{
    copyTransfer(std::move(cmdBuffer), std::move(buffer));
}

DynamicStorageBuffer::DynamicStorageBuffer(std::shared_ptr<Device> device, VkDeviceSize size,
    const void *initial /* nullptr */,
    VkBufferCreateFlags flags /* 0 */,
    const Sharing& sharing /* default */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    CopyMemoryFunction copyFn /* nullptr */):
    Buffer(std::move(device), size,
        VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        flags, sharing, std::move(allocator))
{
    copyHost(initial, std::move(copyFn));
}
} // namespace magma
