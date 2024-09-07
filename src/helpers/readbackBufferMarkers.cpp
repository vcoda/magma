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
#include "../objects/commandBuffer.h"
#include "../objects/dstTransferBuffer.h"
#include "../misc/finish.h"
#include "mapScoped.h"

namespace magma::helpers
{
std::vector<uint32_t> readbackBufferMarkers(std::shared_ptr<CommandBuffer> cmdBuffer,
    const char *blockName /* magma::helpers::readbackBufferMarkers */,
    uint32_t blockColor /* 0x0 */)
{
    std::vector<uint32_t> bufferMarkers;
    const VkDeviceSize size = cmdBuffer->getMarkerOffset();
    if (size)
    {
        const std::shared_ptr<Buffer>& srcBuffer = cmdBuffer->getMarkerBuffer();
        std::shared_ptr<Buffer> dstBuffer = std::make_shared<DstTransferBuffer>(cmdBuffer->getDevice(), size);
        MAGMA_ASSERT(cmdBuffer->allowsReset());
        MAGMA_ASSERT(cmdBuffer->getState() != CommandBuffer::State::Recording);
        cmdBuffer->reset();
        if (cmdBuffer->begin(blockName, blockColor, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT))
        {   // Copy from device to host
            cmdBuffer->copyBuffer(srcBuffer, dstBuffer, 0, 0, dstBuffer->getSize());
            cmdBuffer->end();
            // Block until execution is complete
            finish(std::move(cmdBuffer));
        }
        helpers::mapScoped<uint32_t>(dstBuffer,
            [&bufferMarkers, size](const uint32_t *srcData)
            {   // Copy from host buffer to output
                MAGMA_ASSERT(size % sizeof(uint32_t) == 0);
                bufferMarkers.resize(static_cast<std::size_t>(size) / sizeof(uint32_t));
                memcpy(bufferMarkers.data(), srcData, static_cast<std::size_t>(size));
            });
    }
    return bufferMarkers;
}
} // namespace magma::helpers
