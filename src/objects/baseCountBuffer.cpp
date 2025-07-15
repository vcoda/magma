/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2025 Victor Coda.

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
#include "baseCountBuffer.h"
#include "dstTransferBuffer.h"
#include "commandBuffer.h"
#include "../barriers/bufferMemoryBarrier.h"

namespace magma
{
BaseCountBuffer::BaseCountBuffer(std::shared_ptr<Device> device, uint32_t elementCount, VkPipelineStageFlags stageMask,
    std::shared_ptr<Allocator> allocator /* nullptr */, const Sharing& sharing /* default */):
    Buffer(device, elementCount * sizeof(uint32_t), 0, // flags
        VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT |
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        Initializer(), sharing, allocator),
    stageMask(stageMask),
    stagingBuffer(std::make_unique<DstTransferBuffer>(std::move(device), size, std::move(allocator), Initializer(), sharing))
{}

void BaseCountBuffer::readback(lent_ptr<CommandBuffer> cmdBuffer) const
{
    cmdBuffer->pipelineBarrier(stageMask, VK_PIPELINE_STAGE_TRANSFER_BIT,
        BufferMemoryBarrier(this, barrier::buffer::shaderWriteTransferRead));
    const VkBufferCopy region{0, 0, size};
    cmdBuffer->getLean().copyBuffer(this, stagingBuffer.get(), region);
    cmdBuffer->pipelineBarrier(VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_HOST_BIT,
        BufferMemoryBarrier(stagingBuffer.get(), barrier::buffer::transferWriteHostRead));
}
} // namespace magma
