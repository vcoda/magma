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
#include "pch.h"
#pragma hdrstop
#include "multiAttachmentFramebuffer.h"
#include "../objects/device.h"
#include "../objects/image2DAttachment.h"
#include "../objects/imageView.h"
#include "../objects/renderPass.h"
#include "../objects/framebuffer.h"
#include "../misc/format.h"

namespace magma
{
namespace aux
{
MultiAttachmentFramebuffer::MultiAttachmentFramebuffer(std::shared_ptr<Device> device,
    const std::initializer_list<VkFormat>& colorAttachmentFormats, const VkFormat depthStencilFormat,
    const VkExtent2D& extent, bool depthSampled,
    const bool colorClearOp /* true */,
    const bool depthClearOp /* true */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    const std::vector<VkComponentMapping>& swizzles /* {} */):
    Framebuffer(1),
    depthStencilFormat(depthStencilFormat),
    colorClearOp(colorClearOp),
    depthClearOp(depthClearOp)
{
    constexpr VkComponentMapping dontSwizzle = {
        VK_COMPONENT_SWIZZLE_IDENTITY,
        VK_COMPONENT_SWIZZLE_IDENTITY,
        VK_COMPONENT_SWIZZLE_IDENTITY,
        VK_COMPONENT_SWIZZLE_IDENTITY};
    uint32_t index = 0;
    for (const VkFormat colorFormat : colorAttachmentFormats)
    {   // Create color attachment
        attachments.emplace_back(std::make_shared<ColorAttachment>(device, colorFormat, extent, 1, 1, true, allocator));
        // Create color view
        attachmentViews.emplace_back(std::make_shared<ImageView>(attachments.back(),
            swizzles.empty() ? dontSwizzle : swizzles[index++],
            allocator));
    }
    if (depthStencilFormat != VK_FORMAT_UNDEFINED)
    {   // Create depth/stencil attachment
        attachments.emplace_back(std::make_shared<DepthStencilAttachment>(device, depthStencilFormat, extent,
            1, 1, depthSampled, allocator));
        // Create depth/stencil view
        attachmentViews.push_back(std::make_shared<ImageView>(attachments.back(), dontSwizzle, allocator));
    }
    // Setup attachment descriptions
    std::vector<AttachmentDescription> attachmentDescriptions;
    for (const VkFormat format : colorAttachmentFormats)
    {
        attachmentDescriptions.emplace_back(format, 1,
            colorClearOp ? op::clearStore : op::store, // Clear (optionally) color, store
            op::dontCare, // Stencil not applicable
            VK_IMAGE_LAYOUT_UNDEFINED, // Don't care
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL); // Color image will be transitioned to when a render pass instance ends
    }
    if (depthStencilFormat != VK_FORMAT_UNDEFINED)
    {   // Choose optimal depth/stencil layout
        const VkImageLayout finalLayout = finalDepthStencilLayout(device, depthStencilFormat, depthSampled);
        const Format format(depthStencilFormat);
        attachmentDescriptions.emplace_back(depthStencilFormat, 1,
            depthClearOp ? op::clearStore : op::dontCare, // Don't care if cleared in separate depth pass
            format.depthStencil() || format.stencil() ? op::clearStore : op::dontCare,
            VK_IMAGE_LAYOUT_UNDEFINED, // Don't care
            finalLayout); // Depth image will be transitioned to when a render pass instance ends
    }
    // Create color/depth framebuffer
    renderPass = std::make_shared<RenderPass>(device, attachmentDescriptions, allocator);
    framebuffer = std::make_shared<magma::Framebuffer>(renderPass, attachmentViews, 0, allocator);
}

std::shared_ptr<magma::Framebuffer> MultiAttachmentFramebuffer::getDepthFramebuffer()
{
    lazyDepthRenderPass();
    return depthFramebuffer;
}

std::shared_ptr<const magma::Framebuffer> MultiAttachmentFramebuffer::getDepthFramebuffer() const
{
    lazyDepthRenderPass();
    return depthFramebuffer;
}

std::shared_ptr<RenderPass> MultiAttachmentFramebuffer::lazyDepthRenderPass() const
{
    if (!depthRenderPass && (depthStencilFormat != VK_FORMAT_UNDEFINED))
    {   // Lazy initialization of depth-only render pass
        const Format format(depthStencilFormat);
        AttachmentDescription depthAttachment(depthStencilFormat, 1,
            op::clearStore, // Clear depth, store
            format.depthStencil() || format.stencil() ? op::clearStore : op::dontCare,
            VK_IMAGE_LAYOUT_UNDEFINED, // Don't care
            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL); // Stay as attachment when a depth pass instance ends
        depthRenderPass = std::make_shared<RenderPass>(renderPass->getDevice(), depthAttachment, renderPass->getAllocator());
        depthFramebuffer = std::make_shared<magma::Framebuffer>(depthRenderPass, attachmentViews.back(), 0, framebuffer->getAllocator());
    }
    return depthRenderPass;
}
} // namespace aux
} // namespace magma
