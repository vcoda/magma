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
#include "pch.h"
#pragma hdrstop
#include "primaryCommandBuffer.h"

namespace magma
{
void PrimaryCommandBuffer::executeCommands(const std::vector<std::shared_ptr<CommandBuffer>>& cmdBuffers) noexcept
{
    MAGMA_STACK_ARRAY(VkCommandBuffer, dereferencedCmdBuffers, cmdBuffers.size());
    for (const auto& cmdBuffer : cmdBuffers)
    {
        MAGMA_ASSERT(cmdBuffer->secondary());
        dereferencedCmdBuffers.put(*cmdBuffer);
    }
    vkCmdExecuteCommands(handle, dereferencedCmdBuffers.size(), dereferencedCmdBuffers);
}
} // namespace magma
