/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2025 Victor Coda.

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

#ifdef MemoryBarrier
#undef MemoryBarrier
#endif

namespace magma
{
    /* Global memory barriers apply to memory accesses involving
       all memory objects that exist at the time of its execution. */

    struct MemoryBarrier : VkMemoryBarrier
    {
        constexpr MemoryBarrier(VkAccessFlags srcAccessMask,
            VkAccessFlags dstAccessMask) noexcept:
            VkMemoryBarrier{
                VK_STRUCTURE_TYPE_MEMORY_BARRIER,
                nullptr, // pNext
                srcAccessMask,
                dstAccessMask
            }
        {}
    };
} // namespace magma

namespace magma::barrier::memory
{
    constexpr MemoryBarrier hostWriteIndirectCommandRead(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_INDIRECT_COMMAND_READ_BIT);
    constexpr MemoryBarrier hostWriteIndexRead(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_INDEX_READ_BIT);
    constexpr MemoryBarrier hostWriteVertexAttributeRead(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT);
    constexpr MemoryBarrier hostWriteUniformRead(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_UNIFORM_READ_BIT);
    constexpr MemoryBarrier hostWriteShaderRead(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT);
    constexpr MemoryBarrier hostWriteTransferRead(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT);
    constexpr MemoryBarrier hostWriteMemoryRead(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_MEMORY_READ_BIT);

    constexpr MemoryBarrier transferWriteShaderRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT);
    constexpr MemoryBarrier shaderWriteTransferRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT);

    constexpr MemoryBarrier shaderWriteIndirectCommandRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_INDIRECT_COMMAND_READ_BIT);
    constexpr MemoryBarrier shaderWriteIndexRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_INDEX_READ_BIT);
    constexpr MemoryBarrier shaderWriteVertexAttributeRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT);
    constexpr MemoryBarrier shaderWriteUniformRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_UNIFORM_READ_BIT);
    constexpr MemoryBarrier shaderWriteInputAttachmentRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_INPUT_ATTACHMENT_READ_BIT);
    constexpr MemoryBarrier shaderWriteRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT);
    constexpr MemoryBarrier shaderWriteColorAttachmentRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT);
    constexpr MemoryBarrier shaderWriteDepthStencilAttachmentRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT);

#ifdef VK_KHR_acceleration_structure
    constexpr MemoryBarrier transferWriteAccelerationStructureRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_KHR);
    constexpr MemoryBarrier accelerationStructureWriteRead(VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR, VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_KHR);
    constexpr MemoryBarrier accelerationStructureWriteShaderRead(VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR, VK_ACCESS_SHADER_READ_BIT);
#endif // VK_KHR_acceleration_structure
} // namespace magma::barrier::memory
