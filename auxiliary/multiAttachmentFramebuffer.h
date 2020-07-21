/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
#include "baseFramebuffer.h"

namespace magma
{
    class Device;
    class Image2D;
    class ImageView;
    class IAllocator;

    namespace aux
    {
        /* Auxiliary frame buffer that is responsible for management of
           multiple color attachments and depth/stencil buffer. This allows
           the rendering pipeline to draw to multiple render targets at once. */

        class MultiAttachmentFramebuffer : public Framebuffer
        {
        public:
            explicit MultiAttachmentFramebuffer(std::shared_ptr<Device> device,
                const std::initializer_list<VkFormat>& colorAttachmentFormats,
                const VkFormat depthStencilFormat,
                const VkExtent2D& extent,
                bool shouldReadDepth,
                bool separateDepthPass,
                std::shared_ptr<IAllocator> allocator = nullptr,
                const std::vector<VkComponentMapping>& swizzles = {});
            std::shared_ptr<ImageView> getAttachmentView(uint32_t index) noexcept { return attachmentViews[index]; }
            std::shared_ptr<const ImageView> getAttachmentView(uint32_t index) const noexcept { return attachmentViews[index]; }
            std::shared_ptr<ImageView> getDepthStencilView() noexcept { return attachmentViews.back(); }
            std::shared_ptr<const ImageView> getDepthStencilView() const noexcept { return attachmentViews.back(); }
            std::shared_ptr<RenderPass> getDepthRenderPass() noexcept { return depthRenderPass; }
            std::shared_ptr<const RenderPass> getDepthRenderPass() const noexcept { return depthRenderPass; }
            std::shared_ptr<magma::Framebuffer> getDepthFramebuffer() noexcept { return depthFramebuffer; }
            std::shared_ptr<const magma::Framebuffer> getDepthFramebuffer() const noexcept { return depthFramebuffer; }

        private:
            std::vector<std::shared_ptr<Image2D>> attachments;
            std::vector<std::shared_ptr<ImageView>> attachmentViews;
            std::shared_ptr<RenderPass> depthRenderPass;
            std::shared_ptr<magma::Framebuffer> depthFramebuffer;
        };
    } // namespace aux
} // namespace magma
