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
#include "indirectBuffer.h"
#include "device.h"
#include "deviceMemory.h"

namespace magma
{
IndirectBuffer::IndirectBuffer(std::shared_ptr<Device> device,
    uint32_t maxDrawCount, std::size_t stride, bool persistentlyMapped,
    const Initializer& optional, const Sharing& sharing, std::shared_ptr<Allocator> allocator):
    Buffer(std::move(device),
        static_cast<VkDeviceSize>(maxDrawCount * stride),
        0, // flags
        VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        optional, sharing, std::move(allocator)),
    maxDrawCount(maxDrawCount),
    stride(static_cast<uint32_t>(stride)),
    persistent(persistentlyMapped),
    drawCount(0)
{}

IndirectBuffer::~IndirectBuffer()
{
    if (persistent)
        memory->unmap();
}

DrawIndirectBuffer::DrawIndirectBuffer(std::shared_ptr<Device> device, uint32_t maxDrawCount,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    bool persistentlyMapped /* false */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* default */):
    IndirectBuffer(std::move(device), maxDrawCount, sizeof(VkDrawIndirectCommand),
        persistentlyMapped, optional, sharing, std::move(allocator)),
    mappedData(persistentlyMapped ? (VkDrawIndirectCommand *)memory->map() : nullptr)
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
    return ++drawCount;
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
    return ++drawCount;
}

uint32_t DrawIndirectBuffer::writeDrawCommand(const VkDrawIndirectCommand& drawCmd_) noexcept
{
    DrawIndirectCommand<VkDrawIndirectCommand> drawCmd(this, mappedData);
    if (drawCmd)
        *drawCmd = drawCmd_;
    return ++drawCount;
}

DrawIndexedIndirectBuffer::DrawIndexedIndirectBuffer(std::shared_ptr<Device> device, uint32_t maxDrawIndexedCount,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    bool persistentlyMapped /* false */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* default */):
    IndirectBuffer(std::move(device), maxDrawIndexedCount, sizeof(VkDrawIndexedIndirectCommand),
        persistentlyMapped, optional, sharing, std::move(allocator)),
    mappedData(persistentlyMapped ? (VkDrawIndexedIndirectCommand *)memory->map() : nullptr)
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
    return ++drawCount;
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
    return ++drawCount;
}

uint32_t DrawIndexedIndirectBuffer::writeDrawIndexedCommand(const VkDrawIndexedIndirectCommand& drawIndexedCmd_) noexcept
{
    DrawIndirectCommand<VkDrawIndexedIndirectCommand> drawIndexedCmd(this, mappedData);
    if (drawIndexedCmd)
        *drawIndexedCmd = drawIndexedCmd_;
    return ++drawCount;
}

DispatchIndirectBuffer::DispatchIndirectBuffer(std::shared_ptr<Device> device, uint32_t maxDispatchCommands,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    bool persistentlyMapped /* false */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* default */):
    IndirectBuffer(std::move(device), maxDispatchCommands, sizeof(VkDispatchIndirectCommand),
        persistentlyMapped, optional, sharing, std::move(allocator)),
    mappedData(persistentlyMapped ? (VkDispatchIndirectCommand *)memory->map() : nullptr)
{}

uint32_t DispatchIndirectBuffer::writeDispatchCommand(uint32_t x, uint32_t y, uint32_t z) noexcept
{
    DrawIndirectCommand<VkDispatchIndirectCommand> dispatchCmd(this, mappedData);
    if (dispatchCmd)
    {
        dispatchCmd->x = x;
        dispatchCmd->y = y;
        dispatchCmd->z = z;
    }
    return ++drawCount;
}

#if defined(VK_EXT_mesh_shader) || defined(VK_NV_mesh_shader)
DrawMeshTasksIndirectBuffer::DrawMeshTasksIndirectBuffer(std::shared_ptr<Device> device, uint32_t maxDrawMeshTasksCount,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    bool persistentlyMapped /* false */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* Sharing() */):
    IndirectBuffer(device, maxDrawMeshTasksCount, indirectCommandStride(device),
        persistentlyMapped, optional, sharing, std::move(allocator)),
    mappedData(persistentlyMapped ? memory->map() : nullptr),
#ifdef VK_EXT_mesh_shader
    extMeshShader(device->extensionEnabled(VK_EXT_MESH_SHADER_EXTENSION_NAME))
#else
    extMeshShader(false)
#endif
{}

uint32_t DrawMeshTasksIndirectBuffer::writeDrawMeshTaskCommand(uint32_t groupCountX,
    uint32_t groupCountY /* 1 */,
    uint32_t groupCountZ /* 1 */)
{
    MAGMA_UNUSED(groupCountX);
    MAGMA_UNUSED(groupCountY);
    MAGMA_UNUSED(groupCountZ);
    if (extMeshShader)
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
            drawMeshTaskCmd->taskCount = groupCountX;
            drawMeshTaskCmd->firstTask = 0; // Not used as it isn't present in VK_EXT_mesh_shader
        }
    #endif // VK_NV_mesh_shader
    }
    return ++drawCount;
}

std::size_t DrawMeshTasksIndirectBuffer::indirectCommandStride(std::shared_ptr<Device> device) noexcept
{
#ifdef VK_EXT_mesh_shader
    if (device->extensionEnabled(VK_EXT_MESH_SHADER_EXTENSION_NAME))
        return sizeof(VkDrawMeshTasksIndirectCommandEXT);
#endif
#ifdef VK_NV_mesh_shader
    if (device->extensionEnabled(VK_NV_MESH_SHADER_EXTENSION_NAME))
        return sizeof(VkDrawMeshTasksIndirectCommandNV);
#endif
    return 0;
}
#endif // VK_EXT_mesh_shader || VK_NV_mesh_shader
} // namespace magma
