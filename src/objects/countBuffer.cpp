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
#include "countBuffer.h"
#include "commandBuffer.h"
#include "dstTransferBuffer.h"
#include "../barriers/bufferMemoryBarrier.h"

namespace magma
{
BaseCountBuffer::BaseCountBuffer(std::shared_ptr<Device> device, uint32_t count, VkPipelineStageFlags stageMask,
    std::shared_ptr<Allocator> allocator /* nullptr */, const Sharing& sharing /* default */):
    Buffer(std::move(device), count * sizeof(uint32_t), 0, // flags
        VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT |
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        Initializer(), sharing, std::move(allocator)),
    stageMask(stageMask)
{}

void BaseCountBuffer::readback(std::shared_ptr<CommandBuffer> cmdBuffer) const
{
    if (!hostBuffer)
        hostBuffer = std::make_unique<DstTransferBuffer>(device, size);
    cmdBuffer->pipelineBarrier(stageMask, VK_PIPELINE_STAGE_TRANSFER_BIT,
        BufferMemoryBarrier(shared_from_this(), barrier::buffer::shaderWriteTransferRead));
    const VkBufferCopy bufferCopy{0, 0, size};
    vkCmdCopyBuffer(*cmdBuffer, handle, *hostBuffer, 1, &bufferCopy);
}

CountBuffer::CountBuffer(std::shared_ptr<Device> device, VkPipelineStageFlags stageMask,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Sharing& sharing /* default */):
    BaseCountBuffer(std::move(device), 1, stageMask, std::move(allocator), sharing)
{}

void CountBuffer::setValue(uint32_t value, std::shared_ptr<CommandBuffer> cmdBuffer) noexcept
{
    auto self = shared_from_this();
    cmdBuffer->fillBuffer(self, value);
    cmdBuffer->pipelineBarrier(VK_PIPELINE_STAGE_TRANSFER_BIT, stageMask,
        BufferMemoryBarrier(std::move(self), barrier::buffer::transferWriteShaderRead));
}

uint32_t CountBuffer::getValue() const noexcept
{
    uint32_t value = 0;
    if (void *data = hostBuffer->getMemory()->map())
    {
        value = *reinterpret_cast<const uint32_t *>(data);
        hostBuffer->getMemory()->unmap();
    }
    return value;
}

DispatchCountBuffer::DispatchCountBuffer(std::shared_ptr<Device> device, VkPipelineStageFlags stageMask,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Sharing& sharing /* default */) :
    BaseCountBuffer(std::move(device), 3, stageMask, std::move(allocator), sharing)
{}

void DispatchCountBuffer::setValues(uint32_t x, uint32_t y, uint32_t z,
    std::shared_ptr<CommandBuffer> cmdBuffer) noexcept
{
    auto self = shared_from_this();
    cmdBuffer->fillBuffer(self, x, sizeof(uint32_t), 0);
    cmdBuffer->fillBuffer(self, y, sizeof(uint32_t), sizeof(uint32_t));
    cmdBuffer->fillBuffer(self, z, sizeof(uint32_t), sizeof(uint32_t) * 2);
    cmdBuffer->pipelineBarrier(VK_PIPELINE_STAGE_TRANSFER_BIT, stageMask,
        BufferMemoryBarrier(std::move(self), barrier::buffer::transferWriteShaderRead));
}

std::array<uint32_t, 3> DispatchCountBuffer::getValues() const noexcept
{
    std::array<uint32_t, 3> values = {};
    if (const uint32_t *counts = reinterpret_cast<const uint32_t *>(hostBuffer->getMemory()->map()))
    {
        values[0] = counts[0];
        values[1] = counts[1];
        values[2] = counts[2];
        hostBuffer->getMemory()->unmap();
    }
    return values;
}
} // namespace magma
