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
#include "uniformTexelBuffer.h"
#include "srcTransferBuffer.h"
#include "commandBuffer.h"

namespace magma
{
UniformTexelBuffer::UniformTexelBuffer(std::shared_ptr<CommandBuffer> cmdBuffer, VkDeviceSize size, const void *data,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    VkBufferCreateFlags flags /* 0 */,
    float memoryPriority /* MAGMA_MEMORY_PRIORITY */,
    const Sharing& sharing /* default */,
    CopyMemoryFunction copyFn /* nullptr */):
    Buffer(cmdBuffer->getDevice(), size,
        VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, flags,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, memoryPriority,
        sharing, allocator)
{
    auto srcBuffer = std::make_shared<SrcTransferBuffer>(device, size, data, std::move(allocator), 0, 0.f, sharing, std::move(copyFn));
    cmdBuffer->begin();
    copyTransfer(cmdBuffer, srcBuffer, size);
    cmdBuffer->end();
    commitAndWait(std::move(cmdBuffer));
}

UniformTexelBuffer::UniformTexelBuffer(std::shared_ptr<CommandBuffer> cmdBuffer, std::shared_ptr<const SrcTransferBuffer> srcBuffer,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    VkDeviceSize size /* 0 */,
    VkDeviceSize srcOffset /* 0 */,
    VkBufferCreateFlags flags /* 0 */,
    float memoryPriority /* MAGMA_MEMORY_PRIORITY */,
    const Sharing& sharing /* default */):
    Buffer(srcBuffer->getDevice(),
        size > 0 ? size : srcBuffer->getSize(),
        VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, flags,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, memoryPriority,
        sharing, std::move(allocator))
{
    copyTransfer(std::move(cmdBuffer), std::move(srcBuffer), size, srcOffset);
}
} // namespace magma
