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
#pragma once
#include "commandBuffer.h"

namespace magma
{
    /* Secondary command buffer can be recorded to a primary
       command buffer via vkCmdExecuteCommands (and not directly
       submitted to the queue). There is no inheritance of state
       across primary and secondary command buffers, or between
       secondary command buffers. */

    class SecondaryCommandBuffer : public CommandBuffer
    {
    public:
        explicit SecondaryCommandBuffer(const std::unique_ptr<CommandPool>& cmdPool):
            CommandBuffer(VK_COMMAND_BUFFER_LEVEL_SECONDARY, cmdPool.get()) {}

    private:
        MAGMA_MAKE_UNIQUE(SecondaryCommandBuffer)
        SecondaryCommandBuffer(VkCommandBuffer handle, const CommandPool *cmdPool):
            CommandBuffer(VK_COMMAND_BUFFER_LEVEL_SECONDARY, handle, cmdPool) {}
        friend CommandPool;
    };
} // namespace magma
