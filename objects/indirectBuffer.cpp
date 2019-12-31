/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2019 Victor Coda.

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
#include "indirectBuffer.h"
#include "device.h"
#include "deviceMemory.h"

namespace magma
{
IndirectBuffer::IndirectBuffer(std::shared_ptr<Device> device,
    uint32_t drawCommandCount /* 1 */,
    VkBufferCreateFlags flags /* 0 */,
    const Sharing& sharing /* default */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Buffer(std::move(device), sizeof(VkDrawIndirectCommand) * drawCommandCount,
        VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT,
        flags, sharing, std::move(allocator),
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
{}

void IndirectBuffer::writeDrawCommand(uint32_t vertexCount,
    uint32_t firstVertex /* 0 */,
    uint32_t cmdIndex /* 0 */) noexcept
{
    const VkDeviceSize offset = cmdIndex * sizeof(VkDrawIndirectCommand);
    if (void *buffer = memory->map(offset, sizeof(VkDrawIndirectCommand)))
    {
        VkDrawIndirectCommand *drawCmd = reinterpret_cast<VkDrawIndirectCommand *>(buffer);
        drawCmd->vertexCount = vertexCount;
        drawCmd->instanceCount = 1;
        drawCmd->firstVertex = firstVertex;
        drawCmd->firstInstance = 0;
        memory->unmap();
    }
}

void IndirectBuffer::writeDrawCommand(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance,
    uint32_t cmdIndex /* 0 */) noexcept
{
    const VkDeviceSize offset = cmdIndex * sizeof(VkDrawIndirectCommand);
    if (void *buffer = memory->map(offset, sizeof(VkDrawIndirectCommand)))
    {
        VkDrawIndirectCommand *drawCmd = reinterpret_cast<VkDrawIndirectCommand *>(buffer);
        drawCmd->vertexCount = vertexCount;
        drawCmd->instanceCount = instanceCount;
        drawCmd->firstVertex = firstVertex;
        drawCmd->firstInstance = firstInstance;
        memory->unmap();
    }
}

void IndirectBuffer::writeDrawCommand(const VkDrawIndirectCommand& drawCmd,
    uint32_t cmdIndex /* 0 */) noexcept
{
    const VkDeviceSize offset = cmdIndex * sizeof(VkDrawIndirectCommand);
    if (void *buffer = memory->map(offset, sizeof(VkDrawIndirectCommand)))
    {
        memcpy(buffer, &drawCmd, sizeof(VkDrawIndirectCommand));
        memory->unmap();
    }
}

void IndirectBuffer::writeDrawCommands(const std::vector<VkDrawIndirectCommand>& drawCmdList) noexcept
{
    if (void *buffer = memory->map(0, sizeof(VkDrawIndirectCommand)))
    {
        VkDrawIndirectCommand *drawCmdArray = reinterpret_cast<VkDrawIndirectCommand *>(buffer);
        for (const VkDrawIndirectCommand& drawCmd : drawCmdList)
            memcpy(drawCmdArray++, &drawCmd, sizeof(VkDrawIndirectCommand));
        memory->unmap();
    }
}
} // namespace magma
