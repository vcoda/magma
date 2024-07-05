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
CountBuffer::CountBuffer(std::shared_ptr<Device> device, VkPipelineStageFlags stageMask,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Sharing& sharing /* default */):
    Buffer(std::move(device), sizeof(uint32_t),
        0, // flags
        VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT |
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        Initializer(), sharing, std::move(allocator)),
    stageMask(stageMask)
{}

void CountBuffer::setValue(uint32_t value, std::shared_ptr<CommandBuffer> cmdBuffer) noexcept
{
    cmdBuffer->fillBuffer(shared_from_this(), value);
    cmdBuffer->pipelineBarrier(VK_PIPELINE_STAGE_TRANSFER_BIT, stageMask,
        BufferMemoryBarrier(shared_from_this(), barrier::transferWriteShaderRead));
}

void CountBuffer::readback(std::shared_ptr<CommandBuffer> cmdBuffer) const
{
    if (!hostBuffer)
        hostBuffer = std::make_shared<DstTransferBuffer>(device, sizeof(uint32_t));
    cmdBuffer->pipelineBarrier(stageMask, VK_PIPELINE_STAGE_TRANSFER_BIT,
        magma::BufferMemoryBarrier(shared_from_this(), barrier::shaderWriteTransferRead));
    cmdBuffer->copyBuffer(shared_from_this(), hostBuffer);
}

uint32_t CountBuffer::getValue() const noexcept
{
    uint32_t value = 0;
    void *data = hostBuffer->getMemory()->map();
    MAGMA_ASSERT(data);
    if (data)
    {
        value = *reinterpret_cast<uint32_t *>(data);
        hostBuffer->getMemory()->unmap();
    }
    return value;
}

DispatchCountBuffer::DispatchCountBuffer(std::shared_ptr<Device> device, VkPipelineStageFlags stageMask,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Sharing& sharing /* default */):
    Buffer(std::move(device), sizeof(uint32_t) * 3, // X, Y, Z
        0, // flags
        VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT |
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        Initializer(), sharing, std::move(allocator)),
    stageMask(stageMask)
{}

void DispatchCountBuffer::setValues(uint32_t x, uint32_t y, uint32_t z,
    std::shared_ptr<CommandBuffer> cmdBuffer) noexcept
{
    cmdBuffer->fillBuffer(shared_from_this(), x, sizeof(uint32_t), 0);
    cmdBuffer->fillBuffer(shared_from_this(), y, sizeof(uint32_t), sizeof(uint32_t));
    cmdBuffer->fillBuffer(shared_from_this(), z, sizeof(uint32_t), sizeof(uint32_t) * 2);
    cmdBuffer->pipelineBarrier(VK_PIPELINE_STAGE_TRANSFER_BIT, stageMask,
        BufferMemoryBarrier(shared_from_this(), barrier::transferWriteShaderRead));
}

void DispatchCountBuffer::readback(std::shared_ptr<CommandBuffer> cmdBuffer) const
{
    if (!hostBuffer)
        hostBuffer = std::make_shared<DstTransferBuffer>(device, size);
    cmdBuffer->pipelineBarrier(stageMask, VK_PIPELINE_STAGE_TRANSFER_BIT,
        BufferMemoryBarrier(shared_from_this(), barrier::shaderWriteTransferRead));
    cmdBuffer->copyBuffer(shared_from_this(), hostBuffer);
}

std::array<uint32_t, 3> DispatchCountBuffer::getValues() const noexcept
{
    const uint32_t *data = reinterpret_cast<const uint32_t *>(hostBuffer->getMemory()->map());
    MAGMA_ASSERT(data);
    std::array<uint32_t, 3> values = {};
    if (data)
    {
        values[0] = data[0];
        values[1] = data[1];
        values[2] = data[2];
        hostBuffer->getMemory()->unmap();
    }
    return values;
}
} // namespace magma
