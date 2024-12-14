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
#include "accelerationStructureInputBuffer.h"
#include "srcTransferBuffer.h"
#include "commandBuffer.h"

namespace magma
{
#ifdef VK_KHR_acceleration_structure
AccelerationStructureInputBuffer::AccelerationStructureInputBuffer(std::shared_ptr<Device> device, VkDeviceSize size,
    const Initializer& optional, const Sharing& sharing, std::shared_ptr<Allocator> allocator):
    Buffer(std::move(device), size, 0, // flags
        VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR |
        VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT_KHR |
        VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
        VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        optional, sharing, std::move(allocator))
{}

AccelerationStructureInputBuffer::AccelerationStructureInputBuffer(lent_ptr<CommandBuffer> cmdBuffer, VkDeviceSize size, const void *data,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* default */,
    CopyMemoryFunction copyFn /* nullptr */):
    AccelerationStructureInputBuffer(cmdBuffer->getDevice(), size, optional, sharing, allocator)
{
    copyStaged(std::move(cmdBuffer), data, std::move(allocator), std::move(copyFn));
}

AccelerationStructureInputBuffer::AccelerationStructureInputBuffer(lent_ptr<CommandBuffer> cmdBuffer, std::shared_ptr<const SrcTransferBuffer> srcBuffer,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    VkDeviceSize size /* 0 */,
    VkDeviceSize srcOffset /* 0 */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* default */):
    AccelerationStructureInputBuffer(cmdBuffer->getDevice(),
        size > 0 ? size : srcBuffer->getSize(),
        optional, sharing, std::move(allocator))
{
    copyTransfer(std::move(cmdBuffer), std::move(srcBuffer), srcOffset);
}
#endif // VK_KHR_acceleration_structure
} // namespace magma
