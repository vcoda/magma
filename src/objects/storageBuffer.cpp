/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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

namespace magma
{
StorageBuffer::StorageBuffer(std::shared_ptr<Device> device, VkDeviceSize size,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* default */):
    Buffer(std::move(device), size,
        0, // flags
        VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        optional, sharing, std::move(allocator))
{}

StorageBuffer::StorageBuffer(lent_ptr<CommandBuffer> cmdBuffer, VkDeviceSize size, const void *data,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* default */,
    CopyMemoryFn copyMem /* nullptr */):
    StorageBuffer(cmdBuffer->getDevice(), size, allocator, optional, sharing)
{
    stagingCopy(std::move(cmdBuffer), data, size, 0, 0, VK_WHOLE_SIZE,
        std::move(allocator), std::move(copyMem));
}

StorageBuffer::StorageBuffer(lent_ptr<CommandBuffer> cmdBuffer, lent_ptr<const SrcTransferBuffer> srcBuffer,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    VkDeviceSize size /* 0 */,
    VkDeviceSize srcOffset /* 0 */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* default */):
    StorageBuffer(cmdBuffer->getDevice(),
        size ? size : srcBuffer->getSize(),
        std::move(allocator), optional, sharing)
{
    transferCopy(std::move(cmdBuffer), std::move(srcBuffer), srcOffset, 0, VK_WHOLE_SIZE);
}

HostStorageBuffer::HostStorageBuffer(std::shared_ptr<Device> device, VkDeviceSize size,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* default */):
    Buffer(std::move(device), size,
        0, // flags
        VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        optional, sharing, std::move(allocator))
{}

DynamicStorageBuffer::DynamicStorageBuffer(std::shared_ptr<Device> device, VkDeviceSize size, bool stagedPool,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const void *initialData /* nullptr */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* default */,
    CopyMemoryFn copyMem /* nullptr */):
    Buffer(std::move(device), size,
        0, // flags
        VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT |
            (stagedPool ? VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT : 0),
        optional, sharing, std::move(allocator))
{
    if (initialData)
        hostCopy(initialData, size, 0, 0, VK_WHOLE_SIZE, std::move(copyMem));
}

#ifdef VK_KHR_acceleration_structure
AccelerationStructureStorageBuffer::AccelerationStructureStorageBuffer(std::shared_ptr<Device> device, VkDeviceSize size,
    VkAccelerationStructureBuildTypeKHR buildType,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* default */):
    Buffer(std::move(device), size, 0, // flags
        VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT_KHR,
        [buildType]() -> VkMemoryPropertyFlags
        {
            switch (buildType)
            {
            case VK_ACCELERATION_STRUCTURE_BUILD_TYPE_HOST_KHR:
                return VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
            case VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR:
                return VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
            case VK_ACCELERATION_STRUCTURE_BUILD_TYPE_HOST_OR_DEVICE_KHR:
                return VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
            default:
                return VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
            }
        }(),
        optional, sharing, std::move(allocator))
{}
#endif // VK_KHR_acceleration_structure
} // namespace magma
