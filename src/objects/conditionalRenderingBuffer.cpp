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
#include "conditionalRenderingBuffer.h"
#include "srcTransferBuffer.h"
#include "commandBuffer.h"

namespace magma
{
#ifdef VK_EXT_conditional_rendering
ConditionalRenderingBuffer::ConditionalRenderingBuffer(lent_ptr<CommandBuffer> cmdBuffer, VkDeviceSize size, const void *data,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* default */,
    CopyMemoryFn copyMemFn /* nullptr */):
    Buffer(cmdBuffer->getDevice(), size,
        0, // flags
        VK_BUFFER_USAGE_CONDITIONAL_RENDERING_BIT_EXT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        optional, sharing, allocator)
{
    copyStaged(std::move(cmdBuffer), data, std::move(allocator), std::move(copyMemFn));
}

ConditionalRenderingBuffer::ConditionalRenderingBuffer(lent_ptr<CommandBuffer> cmdBuffer, lent_ptr<const SrcTransferBuffer> srcBuffer,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    VkDeviceSize size /* 0 */,
    VkDeviceSize srcOffset /* 0 */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* default */):
    Buffer(cmdBuffer->getDevice(),
        size > 0 ? size : srcBuffer->getSize(),
        0, // flags
        VK_BUFFER_USAGE_CONDITIONAL_RENDERING_BIT_EXT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        optional, sharing, std::move(allocator))
{
    copyTransfer(std::move(cmdBuffer), std::move(srcBuffer), size, srcOffset);
}
#endif // VK_EXT_conditional_rendering
} // namespace magma
