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
#include "primaryCommandBuffer.h"

namespace magma
{
PrimaryCommandBuffer::PrimaryCommandBuffer(lent_ptr<CommandPool> cmdPool):
    CommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, cmdPool.get())
{}

PrimaryCommandBuffer::PrimaryCommandBuffer(VkCommandBuffer handle, const CommandPool *cmdPool):
    CommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, handle, cmdPool)
{}

void PrimaryCommandBuffer::executeCommands(lent_ptr<CommandBuffer> cmdBuffer) noexcept
{
    MAGMA_ASSERT(!cmdBuffer->primary());
    const LeanCommandBuffer *leanCmdBuffer = &cmdBuffer->getLean();
    leanCmd.executeCommands(1, &leanCmdBuffer);
}

void PrimaryCommandBuffer::executeCommands(const std::initializer_list<lent_ptr<CommandBuffer>>& cmdBuffers) noexcept
{
    MAGMA_VLA(const LeanCommandBuffer*, leanCmdBuffers, cmdBuffers.size());
    for (auto const& cmdBuffer: cmdBuffers)
    {
        MAGMA_ASSERT(!cmdBuffer->primary());
        leanCmdBuffers.put(&cmdBuffer->getLean());
    }
    leanCmd.executeCommands(leanCmdBuffers.count(), leanCmdBuffers);
}

void PrimaryCommandBuffer::executeCommands(const std::vector<lent_ptr<CommandBuffer>>& cmdBuffers) noexcept
{
    MAGMA_VLA(const LeanCommandBuffer*, leanCmdBuffers, cmdBuffers.size());
    for (auto const& cmdBuffer: cmdBuffers)
    {
        MAGMA_ASSERT(!cmdBuffer->primary());
        leanCmdBuffers.put(&cmdBuffer->getLean());
    }
    leanCmd.executeCommands(leanCmdBuffers.count(), leanCmdBuffers);
}
} // namespace magma
