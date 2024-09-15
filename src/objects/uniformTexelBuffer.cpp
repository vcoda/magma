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
#include "uniformTexelBuffer.h"
#include "srcTransferBuffer.h"
#include "commandBuffer.h"

namespace magma
{
UniformTexelBuffer::UniformTexelBuffer(std::shared_ptr<Device> device, VkDeviceSize size,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* default */):
    Buffer(std::move(device), size,
        0, // flags
        VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        optional, sharing, std::move(allocator))
{}

UniformTexelBuffer::UniformTexelBuffer(std::shared_ptr<CommandBuffer> cmdBuffer, VkDeviceSize size, const void *data,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* default */,
    CopyMemoryFunction copyFn /* nullptr */):
    UniformTexelBuffer(cmdBuffer->getDevice(), size, allocator, optional, sharing)
{
    copyStaged(std::move(cmdBuffer), data, std::move(allocator), std::move(copyFn));
}

UniformTexelBuffer::UniformTexelBuffer(std::shared_ptr<CommandBuffer> cmdBuffer, std::shared_ptr<const SrcTransferBuffer> srcBuffer,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    VkDeviceSize size /* 0 */,
    VkDeviceSize srcOffset /* 0 */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* default */):
    UniformTexelBuffer(cmdBuffer->getDevice(),
        size ? size : srcBuffer->getSize(),
        std::move(allocator), optional, sharing)
{
    copyTransfer(std::move(cmdBuffer), std::move(srcBuffer), srcOffset);
}
} // namespace magma
