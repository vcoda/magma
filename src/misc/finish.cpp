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
void finish(lent_ptr<CommandBuffer> cmdBuffer, std::shared_ptr<Queue> queue,
    bool waitIdle /* false */)
{
    if (waitIdle)
    {   // Wait for queue operations to finish
        queue->submit(std::move(cmdBuffer));
        queue->waitIdle();
    }
    else
    {   // Wait for command buffer operations to finish
        auto& fence = cmdBuffer->getFence();
        if (Fence::State::Signaled == fence->getStatus())
            fence->reset();
        queue->submit(std::move(cmdBuffer), 0, nullptr, nullptr, fence);
        fence->wait();
        queue->onIdle();
        fence->reset();
    }
}

void finish(lent_ptr<CommandBuffer> cmdBuffer, bool waitIdle /* false */)
{   // Get queue associated with command buffer
    const uint32_t queueFamilyIndex = cmdBuffer->getQueueFamilyIndex();
    std::shared_ptr<Queue> queue = cmdBuffer->getDevice()->getQueueByFamily(queueFamilyIndex);
    MAGMA_ASSERT(queue);
    finish(std::move(cmdBuffer), std::move(queue), waitIdle);
}
} // namespace magma
