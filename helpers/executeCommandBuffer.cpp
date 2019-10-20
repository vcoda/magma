/*
Magma - C++1x interface over Khronos Vulkan API.
Copyright (C) 2018 Victor Coda.

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
#include "../objects/commandBuffer.h"
#include "../objects/queue.h"
#include "../objects/fence.h"

namespace magma
{
namespace helpers
{
bool executeCommandBuffer(std::shared_ptr<CommandPool> cmdPool,
    std::function<void(std::shared_ptr<CommandBuffer>)> callback,
    bool primaryLevel /* true */,
    VkQueueFlagBits queueType /* VK_QUEUE_GRAPHICS_BIT */,
    const char *blockName /* magma::helpers::executeCommandBuffer */,
    uint32_t blockColor /* 0x0 */)
{
    std::shared_ptr<CommandBuffer> cmdBuffer = cmdPool->allocateCommandBuffer(primaryLevel);
    if (cmdBuffer->begin(blockName, blockColor))
    {
        callback(cmdBuffer);
        cmdBuffer->end();
    }
    std::shared_ptr<magma::Fence> fence(cmdBuffer->getFence());
    std::shared_ptr<Queue> queue = cmdPool->getDevice()->getQueue(queueType, 0);
    const bool submitted = queue->submit(cmdBuffer, 0, nullptr, nullptr, fence);
    const bool waited = fence->wait();
    cmdPool->freeCommandBuffer(cmdBuffer);
    return submitted && waited;
}
} // namespace helpers
} // namespace magma
