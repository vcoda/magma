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
        BufferMemoryBarrier(const Buffer *buffer,
            VkAccessFlags srcAccessMask,
            VkAccessFlags dstAccessMask,
            VkDeviceSize offset = 0,
            VkDeviceSize size = VK_WHOLE_SIZE) noexcept;
        BufferMemoryBarrier(const Buffer *buffer,
            const BufferMemoryBarrier& barrier) noexcept;
        BufferMemoryBarrier(const Buffer *buffer,
            const BufferMemoryBarrier& barrier,
            VkDeviceSize offset,
            VkDeviceSize size) noexcept;
    };
} // namespace magma

#include "bufferMemoryBarrier.inl"

namespace magma::barrier::buffer
{
    constexpr BufferMemoryBarrier hostWriteIndirectCommandRead(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_INDIRECT_COMMAND_READ_BIT);
    constexpr BufferMemoryBarrier hostWriteIndexRead(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_INDEX_READ_BIT);
    constexpr BufferMemoryBarrier hostWriteVertexAttributeRead(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT);
    constexpr BufferMemoryBarrier hostWriteUniformRead(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_UNIFORM_READ_BIT);
    constexpr BufferMemoryBarrier hostWriteShaderRead(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT);
    constexpr BufferMemoryBarrier hostWriteTransferRead(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT);
    constexpr BufferMemoryBarrier hostWriteMemoryRead(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_MEMORY_READ_BIT);

    constexpr BufferMemoryBarrier transferWriteIndirectCommandRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_INDIRECT_COMMAND_READ_BIT);
    constexpr BufferMemoryBarrier transferWriteIndexRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_INDEX_READ_BIT);
    constexpr BufferMemoryBarrier transferWriteVertexAttributeRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT);
    constexpr BufferMemoryBarrier transferWriteUniformRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_UNIFORM_READ_BIT);
    constexpr BufferMemoryBarrier transferWriteInputAttachmentRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_INPUT_ATTACHMENT_READ_BIT);
    constexpr BufferMemoryBarrier transferWriteShaderRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT);
    constexpr BufferMemoryBarrier transferWriteColorAttachmentRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT);
    constexpr BufferMemoryBarrier transferWriteDepthStencilAttachmentRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT);
    constexpr BufferMemoryBarrier transferWriteHostRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_HOST_READ_BIT);
    constexpr BufferMemoryBarrier transferWriteMemoryRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_MEMORY_READ_BIT);
#ifdef VK_EXT_transform_feedback
    constexpr BufferMemoryBarrier transferWriteTransformFeedbackCounterRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT);
#endif // VK_EXT_transform_feedback
#ifdef VK_EXT_conditional_rendering
    constexpr BufferMemoryBarrier transferWriteConditionalRenderingRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_CONDITIONAL_RENDERING_READ_BIT_EXT);
#endif
#ifdef VK_KHR_acceleration_structure
    constexpr BufferMemoryBarrier transferWriteAccelerationStructureRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_KHR);
#endif
    constexpr BufferMemoryBarrier shaderWriteIndirectCommandRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_INDIRECT_COMMAND_READ_BIT);
    constexpr BufferMemoryBarrier shaderWriteIndexRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_INDEX_READ_BIT);
    constexpr BufferMemoryBarrier shaderWriteVertexAttributeRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT);
    constexpr BufferMemoryBarrier shaderWriteUniformRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_UNIFORM_READ_BIT);
    constexpr BufferMemoryBarrier shaderWriteInputAttachmentRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_INPUT_ATTACHMENT_READ_BIT);
    constexpr BufferMemoryBarrier shaderWriteColorAttachmentRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT);
    constexpr BufferMemoryBarrier shaderWriteDepthStencilAttachmentRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT);
    constexpr BufferMemoryBarrier shaderWriteTransferRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT);
    constexpr BufferMemoryBarrier shaderWriteHostRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_HOST_READ_BIT);
    constexpr BufferMemoryBarrier shaderWriteMemoryRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_MEMORY_READ_BIT);
#ifdef VK_EXT_transform_feedback
    constexpr BufferMemoryBarrier shaderWriteTransformFeedbackCounterRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT);
#endif // VK_EXT_transform_feedback
#ifdef VK_EXT_conditional_rendering
    constexpr BufferMemoryBarrier shaderWriteConditionalRenderingRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_CONDITIONAL_RENDERING_READ_BIT_EXT);
#endif
#ifdef VK_KHR_acceleration_structure
    constexpr BufferMemoryBarrier shaderWriteAccelerationStructureRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_KHR);
#endif
#ifdef VK_EXT_transform_feedback
    constexpr BufferMemoryBarrier transformFeedbackWriteVertexAttributeRead(VK_ACCESS_TRANSFORM_FEEDBACK_WRITE_BIT_EXT, VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT);
    constexpr BufferMemoryBarrier transformFeedbackCounterWriteIndirectCommandRead(VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT, VK_ACCESS_INDIRECT_COMMAND_READ_BIT);
    constexpr BufferMemoryBarrier transformFeedbackCounterWriteRead(VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT, VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT);
#endif // VK_EXT_transform_feedback
#ifdef VK_KHR_acceleration_structure
    constexpr BufferMemoryBarrier accelerationStructureWriteRead(VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR, VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_KHR);
    constexpr BufferMemoryBarrier accelerationStructureWriteShaderRead(VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR, VK_ACCESS_SHADER_READ_BIT);
#endif // VK_KHR_acceleration_structure
} // namespace magma::barrier::buffer
