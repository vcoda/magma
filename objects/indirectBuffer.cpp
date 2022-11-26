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
#include "indirectBuffer.h"
#include "device.h"
#include "deviceMemory.h"

namespace magma
{
IndirectBuffer::IndirectBuffer(std::shared_ptr<Device> device,
    uint32_t maxDrawCommands, std::size_t stride, bool persistentlyMapped,
    const Descriptor& optional, const Sharing& sharing, std::shared_ptr<Allocator> allocator):
    Buffer(std::move(device),
        static_cast<VkDeviceSize>(maxDrawCommands * stride),
        0, // flags
        VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        optional, sharing, std::move(allocator)),
    maxDrawCommands(maxDrawCommands),
    stride(static_cast<uint32_t>(stride)),
    persistent(persistentlyMapped),
    cmdCount(0)
{}

IndirectBuffer::~IndirectBuffer()
{
    if (persistent)
        memory->unmap();
}

DrawIndirectBuffer::DrawIndirectBuffer(std::shared_ptr<Device> device, uint32_t maxDrawCommands,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    bool persistentlyMapped /* false */,
    const Descriptor& optional /* default */,
    const Sharing& sharing /* Sharing() */):
    IndirectBuffer(std::move(device), maxDrawCommands, sizeof(VkDrawIndirectCommand),
        persistentlyMapped, optional, sharing, std::move(allocator)),
    mappedData(persistentlyMapped ? memory->map<VkDrawIndirectCommand>() : nullptr)
{}

uint32_t DrawIndirectBuffer::writeDrawCommand(uint32_t vertexCount,
    uint32_t firstVertex /* 0 */) noexcept
{
    DrawIndirectCommand<VkDrawIndirectCommand> drawCmd(this, mappedData);
    if (drawCmd)
    {
        drawCmd->vertexCount = vertexCount;
        drawCmd->instanceCount = 1;
        drawCmd->firstVertex = firstVertex;
        drawCmd->firstInstance = 0;
    }
    return ++cmdCount;
}

uint32_t DrawIndirectBuffer::writeDrawInstancedCommand(uint32_t vertexCount, uint32_t instanceCount,
    uint32_t firstVertex /* 0 */,
    uint32_t firstInstance /* 0 */) noexcept
{
    DrawIndirectCommand<VkDrawIndirectCommand> drawCmd(this, mappedData);
    if (drawCmd)
    {
        drawCmd->vertexCount = vertexCount;
        drawCmd->instanceCount = instanceCount;
        drawCmd->firstVertex = firstVertex;
        drawCmd->firstInstance = firstInstance;
    }
    return ++cmdCount;
}

uint32_t DrawIndirectBuffer::writeDrawCommand(const VkDrawIndirectCommand& drawCmd_) noexcept
{
    DrawIndirectCommand<VkDrawIndirectCommand> drawCmd(this, mappedData);
    if (drawCmd)
        *drawCmd = drawCmd_;
    return ++cmdCount;
}

DrawIndexedIndirectBuffer::DrawIndexedIndirectBuffer(std::shared_ptr<Device> device, uint32_t maxDrawIndexedCommands,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    bool persistentlyMapped /* false */,
    const Descriptor& optional /* default */,
    const Sharing& sharing /* Sharing() */):
    IndirectBuffer(std::move(device), maxDrawIndexedCommands, sizeof(VkDrawIndexedIndirectCommand),
        persistentlyMapped, optional, sharing, std::move(allocator)),
    mappedData(persistentlyMapped ? memory->map<VkDrawIndexedIndirectCommand>() : nullptr)
{}

uint32_t DrawIndexedIndirectBuffer::writeDrawIndexedCommand(uint32_t indexCount,
    uint32_t firstIndex /* 0 */,
    uint32_t vertexOffset /* 0 */) noexcept
{
    DrawIndirectCommand<VkDrawIndexedIndirectCommand> drawIndexedCmd(this, mappedData);
    if (drawIndexedCmd)
    {
        drawIndexedCmd->indexCount = indexCount;
        drawIndexedCmd->instanceCount = 1;
        drawIndexedCmd->firstIndex = firstIndex;
        drawIndexedCmd->vertexOffset = vertexOffset;
        drawIndexedCmd->firstInstance = 0;
    }
    return ++cmdCount;
}

uint32_t DrawIndexedIndirectBuffer::writeDrawIndexedInstancedCommand(uint32_t indexCount, uint32_t instanceCount,
    uint32_t firstIndex /* 0 */,
    uint32_t vertexOffset /* 0 */,
    uint32_t firstInstance /* 0 */) noexcept
{
    DrawIndirectCommand<VkDrawIndexedIndirectCommand> drawIndexedCmd(this, mappedData);
    if (drawIndexedCmd)
    {
        drawIndexedCmd->indexCount = indexCount;
        drawIndexedCmd->instanceCount = instanceCount;
        drawIndexedCmd->firstIndex = firstIndex;
        drawIndexedCmd->vertexOffset = vertexOffset;
        drawIndexedCmd->firstInstance = firstInstance;
    }
    return ++cmdCount;
}

uint32_t DrawIndexedIndirectBuffer::writeDrawIndexedCommand(const VkDrawIndexedIndirectCommand& drawIndexedCmd_) noexcept
{
    DrawIndirectCommand<VkDrawIndexedIndirectCommand> drawIndexedCmd(this, mappedData);
    if (drawIndexedCmd)
        *drawIndexedCmd = drawIndexedCmd_;
    return ++cmdCount;
}

#if defined(VK_EXT_mesh_shader) || defined(VK_NV_mesh_shader)
DrawMeshTasksIndirectBuffer::DrawMeshTasksIndirectBuffer(std::shared_ptr<Device> device, uint32_t maxDrawMeshTasksCommands,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    bool persistentlyMapped /* false */,
    const Descriptor& optional /* default */,
    const Sharing& sharing /* Sharing() */):
    IndirectBuffer(device, maxDrawMeshTasksCommands, extensionDependentStride(device),
        persistentlyMapped, optional, sharing, std::move(allocator)),
    mappedData(persistentlyMapped ? memory->map() : nullptr),
    EXT_mesh_shader(false)
{
#ifdef VK_EXT_mesh_shader
    EXT_mesh_shader = device->extensionEnabled(VK_EXT_MESH_SHADER_EXTENSION_NAME);
#endif
}

uint32_t DrawMeshTasksIndirectBuffer::writeDrawMeshTaskCommand(uint32_t groupCountX,
    uint32_t groupCountY /* 1 */,
    uint32_t groupCountZ /* 1 */)
{
    if (EXT_mesh_shader)
    {
    #ifdef VK_EXT_mesh_shader
        DrawIndirectCommand<VkDrawMeshTasksIndirectCommandEXT> drawMeshTaskCmd(this, mappedData);
        if (drawMeshTaskCmd)
        {
            drawMeshTaskCmd->groupCountX = groupCountX;
            drawMeshTaskCmd->groupCountY = groupCountY;
            drawMeshTaskCmd->groupCountZ = groupCountZ;
        }
    #endif // VK_EXT_mesh_shader
    }
    else
    {
    #ifdef VK_NV_mesh_shader
        DrawIndirectCommand<VkDrawMeshTasksIndirectCommandNV> drawMeshTaskCmd(this, mappedData);
        if (drawMeshTaskCmd)
        {
            MAGMA_ASSERT(1 == groupCountY);
            MAGMA_ASSERT(1 == groupCountZ);
            MAGMA_UNUSED(groupCountY);
            MAGMA_UNUSED(groupCountZ);
            drawMeshTaskCmd->taskCount = groupCountX;
            drawMeshTaskCmd->firstTask = 0; // Not used as it isn't present in VK_EXT_mesh_shader
        }
    #endif // VK_NV_mesh_shader
    }
    return ++cmdCount;
}

std::size_t DrawMeshTasksIndirectBuffer::extensionDependentStride(std::shared_ptr<Device> device) noexcept
{
#ifdef VK_EXT_mesh_shader
    if (device->extensionEnabled(VK_EXT_MESH_SHADER_EXTENSION_NAME))
        return sizeof(VkDrawMeshTasksIndirectCommandEXT);
#endif
    return sizeof(VkDrawMeshTasksIndirectCommandNV);
}
#endif // VK_EXT_mesh_shader || VK_NV_mesh_shader
} // namespace magma
