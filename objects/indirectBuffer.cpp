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
#include <cstring>
#include "indirectBuffer.h"
#include "device.h"
#include "deviceMemory.h"

namespace magma
{
IndirectBuffer::IndirectBuffer(std::shared_ptr<const Device> device,
    uint32_t drawCount /* 1 */,
    VkBufferCreateFlags flags /* 0 */):
    Buffer(device, sizeof(VkDrawIndirectCommand) * drawCount, VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT, flags,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
{}

void IndirectBuffer::updateDraw(const VkDrawIndirectCommand& parameters) noexcept
{
    if (void *drawParameters = memory->map(0, sizeof(VkDrawIndirectCommand)))
    {
        memcpy(drawParameters, &parameters, sizeof(VkDrawIndirectCommand));
        memory->unmap();
    }
}

void IndirectBuffer::updateDraw(uint32_t vertexCount, uint32_t firstVertex) noexcept
{
    if (void *data = memory->map(0, sizeof(VkDrawIndirectCommand)))
    {
        VkDrawIndirectCommand *drawParameters = reinterpret_cast<VkDrawIndirectCommand *>(data);
        drawParameters->vertexCount = vertexCount;
        drawParameters->instanceCount = 1;
        drawParameters->firstVertex = firstVertex;
        drawParameters->firstInstance = 0;
        memory->unmap();
    }
}
} // namespace magma
