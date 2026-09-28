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

namespace magma
{
    class Image;

    /* Image memory barriers only apply to memory accesses
       involving a specific image subresource range. That is,
       a memory dependency formed from an image memory barrier
       is scoped to access via the specified image subresource
       range. Image memory barriers can also be used to define
       image layout transitions or a queue family ownership
       transfer for the specified image subresource range. */

    struct ImageMemoryBarrier : VkImageMemoryBarrier
    {
        constexpr ImageMemoryBarrier(VkAccessFlags srcAccessMask,
            VkAccessFlags dstAccessMask) noexcept;
        ImageMemoryBarrier(Image *image,
            VkImageLayout newLayout) noexcept;
        ImageMemoryBarrier(Image *image,
            VkImageLayout newLayout,
            const VkImageSubresourceRange& subresourceRange) noexcept;
        ImageMemoryBarrier(Image *image,
            VkImageLayout newLayout,
            VkAccessFlags srcAccessMask,
            VkAccessFlags dstAccessMask) noexcept;
        ImageMemoryBarrier(Image *image,
            VkImageLayout newLayout,
            const ImageMemoryBarrier& barrier) noexcept;

    private:
        // Image layout updated by CommandBuffer::pipelineBarrier()
        bool updateImageLayout() const noexcept;
        friend class CommandBuffer;
    };
} // namespace magma

#include "imageMemoryBarrier.inl"

namespace magma::barrier::image
{
    constexpr ImageMemoryBarrier hostWriteShaderRead(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT);
    constexpr ImageMemoryBarrier hostWriteShaderWrite(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_SHADER_WRITE_BIT);
    constexpr ImageMemoryBarrier hostWriteShaderReadWrite(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT);
    constexpr ImageMemoryBarrier hostWriteInputAttachmentRead(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_INPUT_ATTACHMENT_READ_BIT);
    constexpr ImageMemoryBarrier hostWriteColorAttachmentRead(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT);
    constexpr ImageMemoryBarrier hostWriteColorAttachmentWrite(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
    constexpr ImageMemoryBarrier hostWriteColorAttachmentReadWrite(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
    constexpr ImageMemoryBarrier hostWriteDepthStencilAttachmentRead(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT);
    constexpr ImageMemoryBarrier hostWriteDepthStencilAttachmentWrite(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);
    constexpr ImageMemoryBarrier hostWriteDepthStencilAttachmentReadWrite(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);
    constexpr ImageMemoryBarrier hostWriteTransferRead(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT);
    constexpr ImageMemoryBarrier hostWriteTransferWrite(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_TRANSFER_WRITE_BIT);
    constexpr ImageMemoryBarrier hostWriteTransferReadWrite(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT | VK_ACCESS_TRANSFER_WRITE_BIT);
    constexpr ImageMemoryBarrier hostWriteHostRead(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_HOST_READ_BIT);
    constexpr ImageMemoryBarrier hostWriteHostWrite(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_HOST_WRITE_BIT);
    constexpr ImageMemoryBarrier hostWriteHostReadWrite(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_HOST_READ_BIT | VK_ACCESS_HOST_WRITE_BIT);
    constexpr ImageMemoryBarrier hostWriteMemoryRead(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_MEMORY_READ_BIT);
    constexpr ImageMemoryBarrier hostWriteMemoryWrite(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_MEMORY_WRITE_BIT);
    constexpr ImageMemoryBarrier hostWriteMemoryReadWrite(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT);

    constexpr ImageMemoryBarrier transferReadShaderRead(VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_SHADER_READ_BIT);
    constexpr ImageMemoryBarrier transferReadShaderWrite(VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_SHADER_WRITE_BIT);
    constexpr ImageMemoryBarrier transferReadShaderReadWrite(VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT);
    constexpr ImageMemoryBarrier transferReadInputAttachmentRead(VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_INPUT_ATTACHMENT_READ_BIT);
    constexpr ImageMemoryBarrier transferReadColorAttachmentRead(VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT);
    constexpr ImageMemoryBarrier transferReadColorAttachmentWrite(VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
    constexpr ImageMemoryBarrier transferReadColorAttachmentReadWrite(VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
    constexpr ImageMemoryBarrier transferReadDepthStencilAttachmentRead(VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT);
    constexpr ImageMemoryBarrier transferReadDepthStencilAttachmentWrite(VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);
    constexpr ImageMemoryBarrier transferReadDepthStencilAttachmentReadWrite(VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);
    constexpr ImageMemoryBarrier transferReadTransferRead(VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_TRANSFER_READ_BIT);
    constexpr ImageMemoryBarrier transferReadTransferWrite(VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_TRANSFER_WRITE_BIT);
    constexpr ImageMemoryBarrier transferReadTransferReadWrite(VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_TRANSFER_READ_BIT | VK_ACCESS_TRANSFER_WRITE_BIT);
    constexpr ImageMemoryBarrier transferReadHostRead(VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_HOST_READ_BIT);
    constexpr ImageMemoryBarrier transferReadHostWrite(VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_HOST_WRITE_BIT);
    constexpr ImageMemoryBarrier transferReadHostReadWrite(VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_HOST_READ_BIT | VK_ACCESS_HOST_WRITE_BIT);
    constexpr ImageMemoryBarrier transferReadMemoryRead(VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_MEMORY_READ_BIT);
    constexpr ImageMemoryBarrier transferReadMemoryWrite(VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_MEMORY_WRITE_BIT);
    constexpr ImageMemoryBarrier transferReadMemoryReadWrite(VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT);

    constexpr ImageMemoryBarrier transferWriteShaderRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT);
    constexpr ImageMemoryBarrier transferWriteShaderWrite(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_WRITE_BIT);
    constexpr ImageMemoryBarrier transferWriteShaderReadWrite(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT);
    constexpr ImageMemoryBarrier transferWriteInputAttachmentRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_INPUT_ATTACHMENT_READ_BIT);
    constexpr ImageMemoryBarrier transferWriteColorAttachmentRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT);
    constexpr ImageMemoryBarrier transferWriteColorAttachmentWrite(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
    constexpr ImageMemoryBarrier transferWriteColorAttachmentReadWrite(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
    constexpr ImageMemoryBarrier transferWriteDepthStencilAttachmentRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT);
    constexpr ImageMemoryBarrier transferWriteDepthStencilAttachmentWrite(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);
    constexpr ImageMemoryBarrier transferWriteDepthStencilAttachmentReadWrite(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);
    constexpr ImageMemoryBarrier transferWriteTransferRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT);
    constexpr ImageMemoryBarrier transferWriteTransferWrite(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_TRANSFER_WRITE_BIT);
    constexpr ImageMemoryBarrier transferWriteTransferReadWrite(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT | VK_ACCESS_TRANSFER_WRITE_BIT);
    constexpr ImageMemoryBarrier transferWriteHostRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_HOST_READ_BIT);
    constexpr ImageMemoryBarrier transferWriteHostWrite(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_HOST_WRITE_BIT);
    constexpr ImageMemoryBarrier transferWriteHostReadWrite(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_HOST_READ_BIT | VK_ACCESS_HOST_WRITE_BIT);
    constexpr ImageMemoryBarrier transferWriteMemoryRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_MEMORY_READ_BIT);
    constexpr ImageMemoryBarrier transferWriteMemoryWrite(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_MEMORY_WRITE_BIT);
    constexpr ImageMemoryBarrier transferWriteMemoryReadWrite(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT);

    constexpr ImageMemoryBarrier shaderWriteShaderRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT);
    constexpr ImageMemoryBarrier shaderWriteShaderWrite(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_SHADER_WRITE_BIT);
    constexpr ImageMemoryBarrier shaderWriteShaderReadWrite(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT);
    constexpr ImageMemoryBarrier shaderWriteInputAttachmentRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_INPUT_ATTACHMENT_READ_BIT);
    constexpr ImageMemoryBarrier shaderWriteColorAttachmentRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT);
    constexpr ImageMemoryBarrier shaderWriteColorAttachmentWrite(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
    constexpr ImageMemoryBarrier shaderWriteColorAttachmentReadWrite(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
    constexpr ImageMemoryBarrier shaderWriteDepthStencilAttachmentRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT);
    constexpr ImageMemoryBarrier shaderWriteDepthStencilAttachmentWrite(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);
    constexpr ImageMemoryBarrier shaderWriteDepthStencilAttachmentReadWrite(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);
    constexpr ImageMemoryBarrier shaderWriteTransferRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT);
    constexpr ImageMemoryBarrier shaderWriteTransferWrite(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_TRANSFER_WRITE_BIT);
    constexpr ImageMemoryBarrier shaderWriteTransferReadWrite(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT | VK_ACCESS_TRANSFER_WRITE_BIT);
    constexpr ImageMemoryBarrier shaderWriteHostRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_HOST_READ_BIT);
    constexpr ImageMemoryBarrier shaderWriteHostWrite(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_HOST_WRITE_BIT);
    constexpr ImageMemoryBarrier shaderWriteHostReadWrite(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_HOST_READ_BIT | VK_ACCESS_HOST_WRITE_BIT);
    constexpr ImageMemoryBarrier shaderWriteMemoryRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_MEMORY_READ_BIT);
    constexpr ImageMemoryBarrier shaderWriteMemoryWrite(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_MEMORY_WRITE_BIT);
    constexpr ImageMemoryBarrier shaderWriteMemoryReadWrite(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT);

    constexpr ImageMemoryBarrier colorAttachmentWriteShaderRead(VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT);
    constexpr ImageMemoryBarrier colorAttachmentWriteShaderWrite(VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_ACCESS_SHADER_WRITE_BIT);
    constexpr ImageMemoryBarrier colorAttachmentWriteShaderReadWrite(VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT);
    constexpr ImageMemoryBarrier colorAttachmentWriteInputAttachmentRead(VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_ACCESS_INPUT_ATTACHMENT_READ_BIT);
    constexpr ImageMemoryBarrier colorAttachmentWriteColorAttachmentRead(VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT);
    constexpr ImageMemoryBarrier colorAttachmentWriteColorAttachmentWrite(VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
    constexpr ImageMemoryBarrier colorAttachmentWriteColorAttachmentReadWrite(VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
    constexpr ImageMemoryBarrier colorAttachmentWriteDepthStencilAttachmentRead(VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT);
    constexpr ImageMemoryBarrier colorAttachmentWriteDepthStencilAttachmentWrite(VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);
    constexpr ImageMemoryBarrier colorAttachmentWriteDepthStencilAttachmentReadWrite(VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);
    constexpr ImageMemoryBarrier colorAttachmentWriteTransferRead(VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT);
    constexpr ImageMemoryBarrier colorAttachmentWriteTransferWrite(VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_ACCESS_TRANSFER_WRITE_BIT);
    constexpr ImageMemoryBarrier colorAttachmentWriteTransferReadWrite(VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT | VK_ACCESS_TRANSFER_WRITE_BIT);
    constexpr ImageMemoryBarrier colorAttachmentWriteHostRead(VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_ACCESS_HOST_READ_BIT);
    constexpr ImageMemoryBarrier colorAttachmentWriteHostWrite(VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_ACCESS_HOST_WRITE_BIT);
    constexpr ImageMemoryBarrier colorAttachmentWriteHostReadWrite(VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_ACCESS_HOST_READ_BIT | VK_ACCESS_HOST_WRITE_BIT);
    constexpr ImageMemoryBarrier colorAttachmentWriteMemoryRead(VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_ACCESS_MEMORY_READ_BIT);
    constexpr ImageMemoryBarrier colorAttachmentWriteMemoryWrite(VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_ACCESS_MEMORY_WRITE_BIT);
    constexpr ImageMemoryBarrier colorAttachmentWriteMemoryReadWrite(VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT);

    constexpr ImageMemoryBarrier depthStencilAttachmentWriteShaderRead(VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT);
    constexpr ImageMemoryBarrier depthStencilAttachmentWriteShaderWrite(VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, VK_ACCESS_SHADER_WRITE_BIT);
    constexpr ImageMemoryBarrier depthStencilAttachmentWriteShaderReadWrite(VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT);
    constexpr ImageMemoryBarrier depthStencilAttachmentWriteInputAttachmentRead(VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, VK_ACCESS_INPUT_ATTACHMENT_READ_BIT);
    constexpr ImageMemoryBarrier depthStencilAttachmentWriteColorAttachmentRead(VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT);
    constexpr ImageMemoryBarrier depthStencilAttachmentWriteColorAttachmentWrite(VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
    constexpr ImageMemoryBarrier depthStencilAttachmentWriteColorAttachmentReadWrite(VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
    constexpr ImageMemoryBarrier depthStencilAttachmentWriteDepthStencilAttachmentRead(VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT);
    constexpr ImageMemoryBarrier depthStencilAttachmentWriteDepthStencilAttachmentWrite(VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);
    constexpr ImageMemoryBarrier depthStencilAttachmentWriteDepthStencilAttachmentReadWrite(VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);
    constexpr ImageMemoryBarrier depthStencilAttachmentWriteTransferRead(VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT);
    constexpr ImageMemoryBarrier depthStencilAttachmentWriteTransferWrite(VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, VK_ACCESS_TRANSFER_WRITE_BIT);
    constexpr ImageMemoryBarrier depthStencilAttachmentWriteTransferReadWrite(VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT | VK_ACCESS_TRANSFER_WRITE_BIT);
    constexpr ImageMemoryBarrier depthStencilAttachmentWriteHostRead(VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, VK_ACCESS_HOST_READ_BIT);
    constexpr ImageMemoryBarrier depthStencilAttachmentWriteHostWrite(VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, VK_ACCESS_HOST_WRITE_BIT);
    constexpr ImageMemoryBarrier depthStencilAttachmentWriteHostReadWrite(VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, VK_ACCESS_HOST_READ_BIT | VK_ACCESS_HOST_WRITE_BIT);
    constexpr ImageMemoryBarrier depthStencilAttachmentWriteMemoryRead(VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, VK_ACCESS_MEMORY_READ_BIT);
    constexpr ImageMemoryBarrier depthStencilAttachmentWriteMemoryWrite(VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, VK_ACCESS_MEMORY_WRITE_BIT);
    constexpr ImageMemoryBarrier depthStencilAttachmentWriteMemoryReadWrite(VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT);

    constexpr ImageMemoryBarrier memoryWriteShaderRead(VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT);
    constexpr ImageMemoryBarrier memoryWriteShaderWrite(VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_SHADER_WRITE_BIT);
    constexpr ImageMemoryBarrier memoryWriteShaderReadWrite(VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT);
    constexpr ImageMemoryBarrier memoryWriteInputAttachmentRead(VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_INPUT_ATTACHMENT_READ_BIT);
    constexpr ImageMemoryBarrier memoryWriteColorAttachmentRead(VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT);
    constexpr ImageMemoryBarrier memoryWriteColorAttachmentWrite(VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
    constexpr ImageMemoryBarrier memoryWriteColorAttachmentReadWrite(VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
    constexpr ImageMemoryBarrier memoryWriteDepthStencilAttachmentRead(VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT);
    constexpr ImageMemoryBarrier memoryWriteDepthStencilAttachmentWrite(VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);
    constexpr ImageMemoryBarrier memoryWriteDepthStencilAttachmentReadWrite(VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);
    constexpr ImageMemoryBarrier memoryWriteTransferRead(VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT);
    constexpr ImageMemoryBarrier memoryWriteTransferWrite(VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_TRANSFER_WRITE_BIT);
    constexpr ImageMemoryBarrier memoryWriteTransferReadWrite(VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT | VK_ACCESS_TRANSFER_WRITE_BIT);
    constexpr ImageMemoryBarrier memoryWriteHostRead(VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_HOST_READ_BIT);
    constexpr ImageMemoryBarrier memoryWriteHostWrite(VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_HOST_WRITE_BIT);
    constexpr ImageMemoryBarrier memoryWriteHostReadWrite(VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_HOST_READ_BIT | VK_ACCESS_HOST_WRITE_BIT);
    constexpr ImageMemoryBarrier memoryWriteMemoryRead(VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_MEMORY_READ_BIT);
    constexpr ImageMemoryBarrier memoryWriteMemoryWrite(VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_MEMORY_WRITE_BIT);
    constexpr ImageMemoryBarrier memoryWriteMemoryReadWrite(VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT);

    constexpr ImageMemoryBarrier memoryReadWriteShaderRead(VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT);
    constexpr ImageMemoryBarrier memoryReadWriteShaderWrite(VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_SHADER_WRITE_BIT);
    constexpr ImageMemoryBarrier memoryReadWriteShaderReadWrite(VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT);
    constexpr ImageMemoryBarrier memoryReadWriteInputAttachmentRead(VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_INPUT_ATTACHMENT_READ_BIT);
    constexpr ImageMemoryBarrier memoryReadWriteColorAttachmentRead(VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT);
    constexpr ImageMemoryBarrier memoryReadWriteColorAttachmentWrite(VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
    constexpr ImageMemoryBarrier memoryReadWriteColorAttachmentReadWrite(VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
    constexpr ImageMemoryBarrier memoryReadWriteDepthStencilAttachmentRead(VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT);
    constexpr ImageMemoryBarrier memoryReadWriteDepthStencilAttachmentWrite(VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);
    constexpr ImageMemoryBarrier memoryReadWriteDepthStencilAttachmentReadWrite(VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);
    constexpr ImageMemoryBarrier memoryReadWriteTransferRead(VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT);
    constexpr ImageMemoryBarrier memoryReadWriteTransferWrite(VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_TRANSFER_WRITE_BIT);
    constexpr ImageMemoryBarrier memoryReadWriteTransferReadWrite(VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT | VK_ACCESS_TRANSFER_WRITE_BIT);
    constexpr ImageMemoryBarrier memoryReadWriteHostRead(VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_HOST_READ_BIT);
    constexpr ImageMemoryBarrier memoryReadWriteHostWrite(VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_HOST_WRITE_BIT);
    constexpr ImageMemoryBarrier memoryReadWriteHostReadWrite(VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_HOST_READ_BIT | VK_ACCESS_HOST_WRITE_BIT);
    constexpr ImageMemoryBarrier memoryReadWriteMemoryRead(VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_MEMORY_READ_BIT);
    constexpr ImageMemoryBarrier memoryReadWriteMemoryWrite(VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_MEMORY_WRITE_BIT);
    constexpr ImageMemoryBarrier memoryReadWriteMemoryReadWrite(VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT);

#ifdef VK_KHR_maintenance2
    constexpr ImageMemoryBarrier hostWriteDepthStencilAttachmentReadWriteShaderRead(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_SHADER_READ_BIT);
    constexpr ImageMemoryBarrier transferReadDepthStencilAttachmentReadWriteShaderRead(VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_SHADER_READ_BIT);
    constexpr ImageMemoryBarrier transferWriteDepthStencilAttachmentReadWriteShaderRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_SHADER_READ_BIT);
    constexpr ImageMemoryBarrier shaderWriteDepthStencilAttachmentReadWriteShaderRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_SHADER_READ_BIT);
    constexpr ImageMemoryBarrier colorAttachmentWriteDepthStencilAttachmentReadWriteShaderRead(VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_SHADER_READ_BIT);
    constexpr ImageMemoryBarrier depthStencilAttachmentWriteDepthStencilAttachmentReadWriteShaderRead(VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_SHADER_READ_BIT);
    constexpr ImageMemoryBarrier memoryWriteDepthStencilAttachmentReadWriteShaderRead(VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_SHADER_READ_BIT);
    constexpr ImageMemoryBarrier memoryReadWriteDepthStencilAttachmentReadWriteShaderRead(VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_SHADER_READ_BIT);
#endif // VK_KHR_maintenance2

#ifdef VK_EXT_fragment_density_map
    constexpr ImageMemoryBarrier hostWriteFragmentDensityMapRead(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_FRAGMENT_DENSITY_MAP_READ_BIT_EXT);
    constexpr ImageMemoryBarrier transferReadFragmentDensityMapRead(VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_FRAGMENT_DENSITY_MAP_READ_BIT_EXT);
    constexpr ImageMemoryBarrier transferWriteFragmentDensityMapRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_FRAGMENT_DENSITY_MAP_READ_BIT_EXT);
    constexpr ImageMemoryBarrier shaderWriteFragmentDensityMapRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_FRAGMENT_DENSITY_MAP_READ_BIT_EXT);
    constexpr ImageMemoryBarrier colorAttachmentWriteFragmentDensityMapRead(VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_ACCESS_FRAGMENT_DENSITY_MAP_READ_BIT_EXT);
    constexpr ImageMemoryBarrier depthStencilAttachmentWriteFragmentDensityMapRead(VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, VK_ACCESS_FRAGMENT_DENSITY_MAP_READ_BIT_EXT);
    constexpr ImageMemoryBarrier memoryWriteFragmentDensityMapRead(VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_FRAGMENT_DENSITY_MAP_READ_BIT_EXT);
    constexpr ImageMemoryBarrier memoryReadWriteFragmentDensityMapRead(VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_FRAGMENT_DENSITY_MAP_READ_BIT_EXT);
#endif // VK_EXT_fragment_density_map

#ifdef VK_NV_shading_rate_image
    constexpr ImageMemoryBarrier hostWriteShadingRateImageRead(VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_SHADING_RATE_IMAGE_READ_BIT_NV);
    constexpr ImageMemoryBarrier transferReadShadingRateImageRead(VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_SHADING_RATE_IMAGE_READ_BIT_NV);
    constexpr ImageMemoryBarrier transferWriteShadingRateImageRead(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADING_RATE_IMAGE_READ_BIT_NV);
    constexpr ImageMemoryBarrier shaderWriteShadingRateImageRead(VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_SHADING_RATE_IMAGE_READ_BIT_NV);
    constexpr ImageMemoryBarrier colorAttachmentWriteShadingRateImageRead(VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_ACCESS_SHADING_RATE_IMAGE_READ_BIT_NV);
    constexpr ImageMemoryBarrier depthStencilAttachmentWriteShadingRateImageRead(VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, VK_ACCESS_SHADING_RATE_IMAGE_READ_BIT_NV);
    constexpr ImageMemoryBarrier memoryWriteShadingRateImageRead(VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_SHADING_RATE_IMAGE_READ_BIT_NV);
    constexpr ImageMemoryBarrier memoryReadWriteShadingRateImageRead(VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_SHADING_RATE_IMAGE_READ_BIT_NV);
#endif // VK_NV_shading_rate_image
} // namespace magma::barrier::image
