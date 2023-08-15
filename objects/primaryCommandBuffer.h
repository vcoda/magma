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

    /* Primary command buffer, which can execute secondary
       command buffers, and which is submitted to the queue. */

    class PrimaryCommandBuffer : public CommandBuffer
    {
    public:
        explicit PrimaryCommandBuffer(std::shared_ptr<CommandPool> cmdPool);
        void executeCommands(const std::shared_ptr<CommandBuffer>& cmdBuffer) noexcept;
        void executeCommands(const std::vector<std::shared_ptr<CommandBuffer>>& cmdBuffers) noexcept;

    private:
        PrimaryCommandBuffer(VkCommandBuffer handle,
            std::shared_ptr<CommandPool> cmdPool);
        friend CommandPool;
        friend memory::LinearPlacementPool;
    };
} // namespace magma
