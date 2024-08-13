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
#include "../objects/device.h"
#include "../objects/commandPool.h"
#include "../objects/commandBuffer.h"
#include "../objects/queue.h"
#include "../objects/fence.h"

namespace magma
{
void finish(std::shared_ptr<CommandBuffer> cmdBuffer, const std::unique_ptr<Queue>& queue,
    bool waitIdle /* false */)
{
    if (waitIdle)
    {   // Wait for queue operations to finish
        queue->submit(cmdBuffer);
        queue->waitIdle();
    }
    else
    {   // Wait for command buffer operations to finish
        auto& fence = cmdBuffer->getFence();
        if (Fence::State::Signaled == fence->getStatus())
            fence->reset();
        queue->submit(cmdBuffer, 0, nullptr, nullptr, fence);
        fence->wait();
        fence->reset();
    }
    cmdBuffer->finishedExecution();
}

void finish(std::shared_ptr<CommandBuffer> cmdBuffer, bool waitIdle /* false */)
{
    std::shared_ptr<CommandPool> cmdPool = cmdBuffer->getCommandPool();
    MAGMA_ASSERT(cmdPool);
    const uint32_t queueFamilyIndex = cmdPool->getQueueFamilyIndex();
    // Get queue associated with command buffer
    const std::unique_ptr<Queue>& queue = cmdPool->getDevice()->getQueueByFamily(queueFamilyIndex);
    MAGMA_ASSERT(queue);
    finish(std::move(cmdBuffer), queue, waitIdle);
}
} // namespace magma
