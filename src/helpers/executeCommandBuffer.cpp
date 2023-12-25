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
#include "executeCommandBuffer.h"
#include "../objects/device.h"
#include "../objects/commandPool.h"
#include "../objects/primaryCommandBuffer.h"
#include "../objects/secondaryCommandBuffer.h"
#include "../objects/queue.h"
#include "../objects/fence.h"

namespace magma
{
namespace helpers
{
void executeCommandBuffer(std::shared_ptr<CommandPool> cmdPool,
    std::function<void(std::shared_ptr<CommandBuffer>)>& drawFn,
    bool primaryLevel /* true */,
    VkQueueFlagBits queueType /* VK_QUEUE_GRAPHICS_BIT */,
    const char *blockName /* magma::helpers::executeCommandBuffer */,
    uint32_t blockColor /* 0x0 */)
{
    std::shared_ptr<CommandBuffer> cmdBuffer;
    if (primaryLevel)
        cmdBuffer = std::make_shared<PrimaryCommandBuffer>(std::move(cmdPool));
    else
        cmdBuffer = std::make_shared<SecondaryCommandBuffer>(std::move(cmdPool));
    if (cmdBuffer->begin(blockName, blockColor))
    {
        try
        {   // Draw function may optionally throw an exception.
            // We should catch it and finish command buffer ahead of exeption handler.
            drawFn(cmdBuffer);
        }
        catch (...)
        {
            cmdBuffer->end();
            throw;
        }
        cmdBuffer->end();
    }
    std::shared_ptr<Fence> fence = cmdBuffer->getFence();
    fence->reset();
    {
        std::shared_ptr<Device> device = cmdBuffer->getDevice();
        std::shared_ptr<Queue> queue = device->getQueue(queueType, 0);
        queue->submit(cmdBuffer, 0, nullptr, nullptr, fence);
    }
    fence->wait();
}
} // namespace helpers
} // namespace magma
