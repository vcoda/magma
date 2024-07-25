/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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
                srcAccessMask, dstAccessMask
            }
        {}
    };

    namespace barrier
    {
        namespace memory
        {
            constexpr MemoryBarrier transferWriteShaderRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT);
            constexpr MemoryBarrier shaderWriteTransferRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT);

            constexpr MemoryBarrier shaderWriteIndirectCmdRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_INDIRECT_COMMAND_READ_BIT);
            constexpr MemoryBarrier shaderWriteIndexRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_INDEX_READ_BIT);
            constexpr MemoryBarrier shaderWriteVertexAttribRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT);
            constexpr MemoryBarrier shaderWriteUniformRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_UNIFORM_READ_BIT);
            constexpr MemoryBarrier shaderWriteInputRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_INPUT_ATTACHMENT_READ_BIT);
            constexpr MemoryBarrier shaderReadWrite(VK_ACCESS_SHADER_READ_BIT, VK_ACCESS_SHADER_WRITE_BIT);
            constexpr MemoryBarrier shaderWriteRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT);
            constexpr MemoryBarrier shaderWriteColorRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT);
            constexpr MemoryBarrier shaderWriteDepthStencilRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT);

        #ifdef VK_KHR_acceleration_structure
            constexpr MemoryBarrier transferWriteAccelerationStructureRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_KHR);
            constexpr MemoryBarrier accelerationStructureWriteRead(VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR, VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_KHR);
            constexpr MemoryBarrier accelerationStructureWriteShaderRead(VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR, VK_ACCESS_SHADER_READ_BIT);
        #endif // VK_KHR_acceleration_structure
        } // namespace memory
    } // namespace barrier
} // namespace magma
