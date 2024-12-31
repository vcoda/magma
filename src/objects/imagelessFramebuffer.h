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
#include "framebuffer.h"

namespace magma
{
    class FramebufferAttachmentImage;

    /* Allows framebuffer to be created without the need for
       creating images first, allowing more flexibility in how
       they are used, and avoiding the need for many of the
       confusing compatibility rules. */

#ifdef VK_KHR_imageless_framebuffer
    class ImagelessFramebuffer : public Framebuffer
    {
    public:
        explicit ImagelessFramebuffer(std::shared_ptr<const RenderPass> renderPass,
            uint32_t width,
            uint32_t height,
            uint32_t layerCount,
            VkImageUsageFlags usage,
            const std::vector<VkFormat>& viewFormats,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkImageCreateFlags flags = 0,
            const StructureChain& extendedInfo = StructureChain());
        explicit ImagelessFramebuffer(std::shared_ptr<const RenderPass> renderPass,
            const FramebufferAttachmentImage& attachment,
            std::shared_ptr<IAllocator> allocator = nullptr,
            const StructureChain& extendedInfo = StructureChain());
        explicit ImagelessFramebuffer(std::shared_ptr<const RenderPass> renderPass,
            const std::vector<FramebufferAttachmentImage>& attachments,
            std::shared_ptr<IAllocator> allocator = nullptr,
            const StructureChain& extendedInfo = StructureChain());
    };
#endif // VK_KHR_imageless_framebuffer
} // namespace magma
