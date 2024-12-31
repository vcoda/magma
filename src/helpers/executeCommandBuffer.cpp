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
#include "../objects/commandPool.h"
#include "../objects/primaryCommandBuffer.h"
#include "../misc/finish.h"

namespace magma::helpers
{
void executeCommandBuffer(lent_ptr<CommandPool> cmdPool,
    std::function<void(const std::unique_ptr<CommandBuffer>&)> cmdFn,
    const char *blockName /* magma::helpers::executeCommandBuffer */,
    uint32_t blockColor /* 0x0 */)
{   /* VK_COMMAND_POOL_CREATE_TRANSIENT_BIT specifies that command
       buffers allocated from the pool will be short-lived, meaning
       that they will be reset or freed in a relatively short timeframe.
       This flag may be used by the implementation to control memory
       allocation behavior within the pool. */
    MAGMA_ASSERT(cmdPool->getFlags() & VK_COMMAND_POOL_CREATE_TRANSIENT_BIT);
    std::unique_ptr<CommandBuffer> cmdBuffer = std::make_unique<PrimaryCommandBuffer>(std::move(cmdPool));
    if (cmdBuffer->beginAnnotated(blockName, blockColor, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT))
    {
        try
        {   /* Callback function may optionally throw an exception.
               We should catch throwed exception and finish command
               buffer ahead of exeption handler. */
            cmdFn(cmdBuffer);
        }
        catch (...)
        {
            cmdBuffer->end();
            MAGMA_THROW;
        }
        cmdBuffer->end();
        // Block until execution is complete
        finish(cmdBuffer);
    }
}
} // namespace magma::helpers
