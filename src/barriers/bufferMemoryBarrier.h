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
#pragma once
#include "../objects/buffer.h"

namespace magma
{
    /* Buffer memory barriers only apply to memory accesses
       involving a specific buffer range. That is, a memory
       dependency formed from an buffer memory barrier is scoped
       to access via the specified buffer range. Buffer memory
       barriers can also be used to define a queue family
       ownership transfer for the specified buffer range. */

    struct BufferMemoryBarrier : VkBufferMemoryBarrier
    {
        constexpr BufferMemoryBarrier(VkAccessFlags srcAccessMask,
            VkAccessFlags dstAccessMask) noexcept;
        BufferMemoryBarrier(std::shared_ptr<const Buffer> buffer,
            VkAccessFlags srcAccessMask,
            VkAccessFlags dstAccessMask,
            VkDeviceSize offset = 0,
            VkDeviceSize size = VK_WHOLE_SIZE) noexcept;
        BufferMemoryBarrier(std::shared_ptr<const Buffer> buffer,
            const BufferMemoryBarrier& barrier) noexcept;
        BufferMemoryBarrier(std::shared_ptr<const Buffer> buffer,
            const BufferMemoryBarrier& barrier,
            VkDeviceSize offset,
            VkDeviceSize size) noexcept;
    };
} // namespace magma

#include "bufferMemoryBarrier.inl"

namespace magma
{
    namespace barrier
    {
        namespace buffer
        {
            constexpr BufferMemoryBarrier transferWriteHostRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_HOST_READ_BIT);
            constexpr BufferMemoryBarrier transferWriteShaderRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT);

            constexpr BufferMemoryBarrier shaderWriteIndirectCmdRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_INDIRECT_COMMAND_READ_BIT);
            constexpr BufferMemoryBarrier shaderWriteIndexRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_INDEX_READ_BIT);
            constexpr BufferMemoryBarrier shaderWriteVertexAttribRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT);
            constexpr BufferMemoryBarrier shaderWriteUniformRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_UNIFORM_READ_BIT);
            constexpr BufferMemoryBarrier shaderReadWrite(VK_ACCESS_SHADER_READ_BIT, VK_ACCESS_SHADER_WRITE_BIT);
            constexpr BufferMemoryBarrier shaderWriteRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT);
            constexpr BufferMemoryBarrier shaderWriteTransferRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT);
            constexpr BufferMemoryBarrier shaderWriteHostRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_HOST_READ_BIT);
        #ifdef VK_EXT_transform_feedback
            constexpr BufferMemoryBarrier transformFeedbackWriteVertexAttribRead(VK_ACCESS_TRANSFORM_FEEDBACK_WRITE_BIT_EXT, VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT);
            constexpr BufferMemoryBarrier transformFeedbackCntWriteIndirectCmdRead(VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT, VK_ACCESS_INDIRECT_COMMAND_READ_BIT);
            constexpr BufferMemoryBarrier transformFeedbackCntWriteRead(VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT, VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT);
        #endif
        #ifdef VK_EXT_conditional_rendering
            constexpr BufferMemoryBarrier shaderWriteConditionalRenderingRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_CONDITIONAL_RENDERING_READ_BIT_EXT);
        #endif
        } // namespace buffer
    } // namespace barrier
} // namespace magma


