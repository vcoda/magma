/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2022 Victor Coda.

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
    namespace memory
    {
        class LinearPlacementPool;
    }

    /* Secondary command buffer can be recorded to a primary
       command buffer via vkCmdExecuteCommands (and not directly
       submitted to the queue). There is no inheritance of state
       across primary and secondary command buffers, or between
       secondary command buffers. */

    class SecondaryCommandBuffer : public CommandBuffer
    {
    public:
        explicit SecondaryCommandBuffer(std::shared_ptr<CommandPool> cmdPool):
            CommandBuffer(VK_COMMAND_BUFFER_LEVEL_SECONDARY, std::move(cmdPool)) {}

    private:
        explicit SecondaryCommandBuffer(VkCommandBuffer handle,
            std::shared_ptr<CommandPool> cmdPool):
            CommandBuffer(VK_COMMAND_BUFFER_LEVEL_SECONDARY, handle, std::move(cmdPool)) {}
        friend CommandPool;
        friend memory::LinearPlacementPool;
    };
} // namespace magma
