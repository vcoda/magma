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
#include "countBuffer.h"
#include "commandBuffer.h"
#include "../barriers/bufferMemoryBarrier.h"

namespace magma
{
CountBuffer::CountBuffer(std::shared_ptr<Device> device, VkPipelineStageFlags stageMask,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Sharing& sharing /* default */):
    BaseCountBuffer(std::move(device), 1, stageMask, std::move(allocator), sharing)
{}

void CountBuffer::setValue(uint32_t value, lent_ptr<CommandBuffer> cmdBuffer) noexcept
{
    cmdBuffer->getLean().fillBuffer(this, value);
    cmdBuffer->pipelineBarrier(VK_PIPELINE_STAGE_TRANSFER_BIT, stageMask,
        BufferMemoryBarrier(this, barrier::buffer::transferWriteShaderRead));
}

uint32_t CountBuffer::getValue() const noexcept
{
    uint32_t value = 0;
    const void *data = stagingBuffer->getMemory()->map();
    if (data)
    {
        value = *reinterpret_cast<const uint32_t *>(data);
        stagingBuffer->getMemory()->unmap();
    }
    return value;
}

DispatchCountBuffer::DispatchCountBuffer(std::shared_ptr<Device> device, VkPipelineStageFlags stageMask,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Sharing& sharing /* default */) :
    BaseCountBuffer(std::move(device), 3, stageMask, std::move(allocator), sharing)
{}

void DispatchCountBuffer::setValues(uint32_t x, uint32_t y, uint32_t z, lent_ptr<CommandBuffer> cmdBuffer) noexcept
{
    auto& leanCmd = cmdBuffer->getLean();
    leanCmd.fillBuffer(this, x, sizeof(uint32_t), 0);
    leanCmd.fillBuffer(this, y, sizeof(uint32_t), sizeof(uint32_t));
    leanCmd.fillBuffer(this, z, sizeof(uint32_t), sizeof(uint32_t) * 2);
    leanCmd.pipelineBarrier(VK_PIPELINE_STAGE_TRANSFER_BIT, stageMask,
        BufferMemoryBarrier(this, barrier::buffer::transferWriteShaderRead));
}

std::array<uint32_t, 3> DispatchCountBuffer::getValues() const noexcept
{
    std::array<uint32_t, 3> values;
    const void *data = stagingBuffer->getMemory()->map();
    if (data)
    {
        const uint32_t *counters = reinterpret_cast<const uint32_t *>(data);
        values[0] = counters[0];
        values[1] = counters[1];
        values[2] = counters[2];
        stagingBuffer->getMemory()->unmap();
    }
    else
    {
        values[0] = 0;
        values[1] = 0;
        values[2] = 0;
    }
    return values;
}
} // namespace magma
