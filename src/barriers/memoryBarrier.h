/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2026 Victor Coda.

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
    constexpr MemoryBarrier hostWriteShaderWrite(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_SHADER_WRITE_BIT);
    constexpr MemoryBarrier hostWriteShaderReadWrite(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT);
    constexpr MemoryBarrier hostWriteTransferRead(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT);
    constexpr MemoryBarrier hostWriteTransferWrite(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_TRANSFER_WRITE_BIT);
    constexpr MemoryBarrier hostWriteTransferReadWrite(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT | VK_ACCESS_TRANSFER_WRITE_BIT);
    constexpr MemoryBarrier hostWriteRead(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_HOST_READ_BIT);
    constexpr MemoryBarrier hostWriteWrite(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_HOST_WRITE_BIT);
    constexpr MemoryBarrier hostWriteReadWrite(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_HOST_READ_BIT | VK_ACCESS_HOST_WRITE_BIT);
    constexpr MemoryBarrier hostWriteMemoryRead(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_MEMORY_READ_BIT);
    constexpr MemoryBarrier hostWriteMemoryWrite(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_MEMORY_WRITE_BIT);
    constexpr MemoryBarrier hostWriteMemoryReadWrite(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT);

    constexpr MemoryBarrier transferWriteIndirectCommandRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_INDIRECT_COMMAND_READ_BIT);
    constexpr MemoryBarrier transferWriteIndexRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_INDEX_READ_BIT);
    constexpr MemoryBarrier transferWriteVertexAttributeRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT);
    constexpr MemoryBarrier transferWriteUniformRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_UNIFORM_READ_BIT);
    constexpr MemoryBarrier transferWriteInputAttachmentRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_INPUT_ATTACHMENT_READ_BIT);
    constexpr MemoryBarrier transferWriteShaderRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT);
    constexpr MemoryBarrier transferWriteShaderWrite(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_WRITE_BIT);
    constexpr MemoryBarrier transferWriteShaderReadWrite(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT);
    constexpr MemoryBarrier transferWriteColorAttachmentRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT);
    constexpr MemoryBarrier transferWriteColorAttachmentWrite(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
    constexpr MemoryBarrier transferWriteColorAttachmentReadWrite(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
    constexpr MemoryBarrier transferWriteDepthStencilAttachmentRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT);
    constexpr MemoryBarrier transferWriteDepthStencilAttachmentWrite(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);
    constexpr MemoryBarrier transferWriteDepthStencilAttachmentReadWrite(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);
    constexpr MemoryBarrier transferWriteRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT);
    constexpr MemoryBarrier transferWriteWrite(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_TRANSFER_WRITE_BIT);
    constexpr MemoryBarrier transferWriteReadWrite(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT | VK_ACCESS_TRANSFER_WRITE_BIT);
    constexpr MemoryBarrier transferWriteHostRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_HOST_READ_BIT);
    constexpr MemoryBarrier transferWriteHostWrite(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_HOST_WRITE_BIT);
    constexpr MemoryBarrier transferWriteHostReadWrite(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_HOST_READ_BIT | VK_ACCESS_HOST_WRITE_BIT);
    constexpr MemoryBarrier transferWriteMemoryRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_MEMORY_READ_BIT);
    constexpr MemoryBarrier transferWriteMemoryWrite(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_MEMORY_WRITE_BIT);
    constexpr MemoryBarrier transferWriteMemoryReadWrite(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT);

    constexpr MemoryBarrier shaderWriteIndirectCommandRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_INDIRECT_COMMAND_READ_BIT);
    constexpr MemoryBarrier shaderWriteIndexRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_INDEX_READ_BIT);
    constexpr MemoryBarrier shaderWriteVertexAttributeRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT);
    constexpr MemoryBarrier shaderWriteUniformRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_UNIFORM_READ_BIT);
    constexpr MemoryBarrier shaderWriteInputAttachmentRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_INPUT_ATTACHMENT_READ_BIT);
    constexpr MemoryBarrier shaderWriteRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT);
    constexpr MemoryBarrier shaderWriteWrite(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_SHADER_WRITE_BIT);
    constexpr MemoryBarrier shaderWriteReadWrite(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT);
    constexpr MemoryBarrier shaderWriteColorAttachmentRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT);
    constexpr MemoryBarrier shaderWriteColorAttachmentWrite(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
    constexpr MemoryBarrier shaderWriteColorAttachmentReadWrite(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
    constexpr MemoryBarrier shaderWriteDepthStencilAttachmentRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT);
    constexpr MemoryBarrier shaderWriteDepthStencilAttachmentWrite(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);
    constexpr MemoryBarrier shaderWriteDepthStencilAttachmentReadWrite(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);
    constexpr MemoryBarrier shaderWriteTransferRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT);
    constexpr MemoryBarrier shaderWriteTransferWrite(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_TRANSFER_WRITE_BIT);
    constexpr MemoryBarrier shaderWriteTransferReadWrite(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT | VK_ACCESS_TRANSFER_WRITE_BIT);
    constexpr MemoryBarrier shaderWriteHostRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_HOST_READ_BIT);
    constexpr MemoryBarrier shaderWriteHostWrite(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_HOST_WRITE_BIT);
    constexpr MemoryBarrier shaderWriteHostReadWrite(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_HOST_READ_BIT | VK_ACCESS_HOST_WRITE_BIT);
    constexpr MemoryBarrier shaderWriteMemoryRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_MEMORY_READ_BIT);
    constexpr MemoryBarrier shaderWriteMemoryWrite(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_MEMORY_WRITE_BIT);
    constexpr MemoryBarrier shaderWriteMemoryReadWrite(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT);

    constexpr MemoryBarrier colorAttachmentWriteShaderRead(VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT);
    constexpr MemoryBarrier colorAttachmentWriteShaderWrite(VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_ACCESS_SHADER_WRITE_BIT);
    constexpr MemoryBarrier colorAttachmentWriteShaderReadWrite(VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT);
    constexpr MemoryBarrier colorAttachmentWriteRead(VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT);
    constexpr MemoryBarrier colorAttachmentWriteWrite(VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
    constexpr MemoryBarrier colorAttachmentWriteReadWrite(VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
    constexpr MemoryBarrier colorAttachmentWriteTransferRead(VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT);

    constexpr MemoryBarrier depthStencilAttachmentWriteShaderRead(VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT);
    constexpr MemoryBarrier depthStencilAttachmentWriteShaderWrite(VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, VK_ACCESS_SHADER_WRITE_BIT);
    constexpr MemoryBarrier depthStencilAttachmentWriteShaderReadWrite(VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT);
    constexpr MemoryBarrier depthStencilAttachmentWriteRead(VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT);
    constexpr MemoryBarrier depthStencilAttachmentWriteWrite(VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);
    constexpr MemoryBarrier depthStencilAttachmentWriteReadWrite(VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);
    constexpr MemoryBarrier depthStencilAttachmentWriteTransferRead(VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT);

    constexpr MemoryBarrier memoryWriteShaderRead(VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT);
    constexpr MemoryBarrier memoryWriteShaderWrite(VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_SHADER_WRITE_BIT);
    constexpr MemoryBarrier memoryWriteShaderReadWrite(VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT);
    constexpr MemoryBarrier memoryWriteTransferRead(VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT);
    constexpr MemoryBarrier memoryWriteTransferWrite(VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_TRANSFER_WRITE_BIT);
    constexpr MemoryBarrier memoryWriteTransferReadWrite(VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT | VK_ACCESS_TRANSFER_WRITE_BIT);
    constexpr MemoryBarrier memoryWriteHostRead(VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_HOST_READ_BIT);
    constexpr MemoryBarrier memoryWriteHostWrite(VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_HOST_WRITE_BIT);
    constexpr MemoryBarrier memoryWriteHostReadWrite(VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_HOST_READ_BIT | VK_ACCESS_HOST_WRITE_BIT);
    constexpr MemoryBarrier memoryWriteRead(VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_MEMORY_READ_BIT);
    constexpr MemoryBarrier memoryWriteWrite(VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_MEMORY_WRITE_BIT);
    constexpr MemoryBarrier memoryWriteReadWrite(VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT);

#ifdef VK_EXT_conditional_rendering
    constexpr MemoryBarrier transferWriteConditionalRenderingRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_CONDITIONAL_RENDERING_READ_BIT_EXT);
    constexpr MemoryBarrier shaderWriteConditionalRenderingRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_CONDITIONAL_RENDERING_READ_BIT_EXT);
    constexpr MemoryBarrier memoryWriteConditionalRenderingRead(VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_CONDITIONAL_RENDERING_READ_BIT_EXT);
#endif // VK_EXT_conditional_rendering

#ifdef VK_EXT_transform_feedback
    constexpr MemoryBarrier transferWriteTransformFeedbackWrite(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_TRANSFORM_FEEDBACK_WRITE_BIT_EXT);
    constexpr MemoryBarrier transferWriteTransformFeedbackCounterRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT);
    constexpr MemoryBarrier transferWriteTransformFeedbackCounterWrite(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT);
    constexpr MemoryBarrier transferWriteTransformFeedbackCounterReadWrite(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT | VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT);
    constexpr MemoryBarrier shaderWriteTransformFeedbackWrite(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_TRANSFORM_FEEDBACK_WRITE_BIT_EXT);
    constexpr MemoryBarrier shaderWriteTransformFeedbackCounterRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT);
    constexpr MemoryBarrier shaderWriteTransformFeedbackCounterWrite(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT);
    constexpr MemoryBarrier shaderWriteTransformFeedbackCounterReadWrite(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT | VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT);
    constexpr MemoryBarrier transformFeedbackWriteShaderRead(VK_ACCESS_TRANSFORM_FEEDBACK_WRITE_BIT_EXT, VK_ACCESS_SHADER_READ_BIT);
    constexpr MemoryBarrier transformFeedbackWriteShaderWrite(VK_ACCESS_TRANSFORM_FEEDBACK_WRITE_BIT_EXT, VK_ACCESS_SHADER_WRITE_BIT);
    constexpr MemoryBarrier transformFeedbackWriteShaderReadWrite(VK_ACCESS_TRANSFORM_FEEDBACK_WRITE_BIT_EXT, VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT);
    constexpr MemoryBarrier transformFeedbackWriteVertexAttributeRead(VK_ACCESS_TRANSFORM_FEEDBACK_WRITE_BIT_EXT, VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT);
    constexpr MemoryBarrier transformFeedbackCounterWriteIndirectCommandRead(VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT, VK_ACCESS_INDIRECT_COMMAND_READ_BIT);
    constexpr MemoryBarrier transformFeedbackCounterWriteRead(VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT, VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT);
    constexpr MemoryBarrier transformFeedbackCounterWriteWrite(VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT, VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT);
    constexpr MemoryBarrier transformFeedbackCounterWriteReadWrite(VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT, VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT | VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT);
#endif // VK_EXT_transform_feedback

#ifdef VK_KHR_acceleration_structure
    constexpr MemoryBarrier hostWriteAccelerationStructureRead(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_KHR);
    constexpr MemoryBarrier hostWriteAccelerationStructureWrite(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR);
    constexpr MemoryBarrier hostWriteAccelerationStructureReadWrite(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_KHR | VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR);
    constexpr MemoryBarrier transferWriteAccelerationStructureRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_KHR);
    constexpr MemoryBarrier transferWriteAccelerationStructureWrite(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR);
    constexpr MemoryBarrier transferWriteAccelerationStructureReadWrite(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_KHR | VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR);
    constexpr MemoryBarrier shaderWriteAccelerationStructureRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_KHR);
    constexpr MemoryBarrier shaderWriteAccelerationStructureWrite(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR);
    constexpr MemoryBarrier shaderWriteAccelerationStructureReadWrite(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_KHR | VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR);
    constexpr MemoryBarrier accelerationStructureWriteShaderRead(VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR, VK_ACCESS_SHADER_READ_BIT);
    constexpr MemoryBarrier accelerationStructureWriteShaderWrite(VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR, VK_ACCESS_SHADER_WRITE_BIT);
    constexpr MemoryBarrier accelerationStructureWriteShaderReadWrite(VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR, VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT);
    constexpr MemoryBarrier accelerationStructureWriteTransferRead(VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR, VK_ACCESS_TRANSFER_READ_BIT);
    constexpr MemoryBarrier accelerationStructureWriteTransferWrite(VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR, VK_ACCESS_TRANSFER_WRITE_BIT);
    constexpr MemoryBarrier accelerationStructureWriteTransferReadWrite(VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR, VK_ACCESS_TRANSFER_READ_BIT | VK_ACCESS_TRANSFER_WRITE_BIT);
    constexpr MemoryBarrier accelerationStructureWriteRead(VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR, VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_KHR);
    constexpr MemoryBarrier accelerationStructureWriteWrite(VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR, VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR);
    constexpr MemoryBarrier accelerationStructureWriteReadWrite(VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR, VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_KHR | VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR);
#endif // VK_KHR_acceleration_structure

#ifdef VK_NV_device_generated_commands
    constexpr MemoryBarrier transferWriteCommandPreprocessRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_COMMAND_PREPROCESS_READ_BIT_NV);
    constexpr MemoryBarrier transferWriteCommandPreprocessWrite(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_COMMAND_PREPROCESS_WRITE_BIT_NV);
    constexpr MemoryBarrier transferWriteCommandPreprocessWriteReadWrite(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_COMMAND_PREPROCESS_READ_BIT_NV | VK_ACCESS_COMMAND_PREPROCESS_WRITE_BIT_NV);
    constexpr MemoryBarrier shaderWriteCommandPreprocessRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_COMMAND_PREPROCESS_READ_BIT_NV);
    constexpr MemoryBarrier shaderWriteCommandPreprocessWrite(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_COMMAND_PREPROCESS_WRITE_BIT_NV);
    constexpr MemoryBarrier shaderWriteCommandPreprocessWriteReadWrite(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_COMMAND_PREPROCESS_READ_BIT_NV | VK_ACCESS_COMMAND_PREPROCESS_WRITE_BIT_NV);
    constexpr MemoryBarrier commandPreprocessWriteShaderRead(VK_ACCESS_COMMAND_PREPROCESS_WRITE_BIT_NV, VK_ACCESS_SHADER_READ_BIT);
    constexpr MemoryBarrier commandPreprocessWriteShaderWrite(VK_ACCESS_COMMAND_PREPROCESS_WRITE_BIT_NV, VK_ACCESS_SHADER_WRITE_BIT);
    constexpr MemoryBarrier commandPreprocessWriteShaderReadWrite(VK_ACCESS_COMMAND_PREPROCESS_WRITE_BIT_NV, VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT);
    constexpr MemoryBarrier commandPreprocessWriteRead(VK_ACCESS_COMMAND_PREPROCESS_WRITE_BIT_NV, VK_ACCESS_COMMAND_PREPROCESS_READ_BIT_NV);
    constexpr MemoryBarrier commandPreprocessWriteWrite(VK_ACCESS_COMMAND_PREPROCESS_WRITE_BIT_NV, VK_ACCESS_COMMAND_PREPROCESS_WRITE_BIT_NV);
    constexpr MemoryBarrier commandPreprocessWriteReadWrite(VK_ACCESS_COMMAND_PREPROCESS_WRITE_BIT_NV, VK_ACCESS_COMMAND_PREPROCESS_READ_BIT_NV | VK_ACCESS_COMMAND_PREPROCESS_WRITE_BIT_NV);
#endif // VK_NV_device_generated_commands
} // namespace magma::barrier::memory
