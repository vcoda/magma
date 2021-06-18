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
#include "../allocator/allocator.h"

namespace magma
{
namespace aux
{
MultiAttachmentFramebuffer::MultiAttachmentFramebuffer(std::shared_ptr<Device> device,
    const std::initializer_list<VkFormat>& colorAttachmentFormats,
    const VkFormat depthStencilFormat, const VkExtent2D& extent, bool depthSampled,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const bool colorClearOp /* true */,
    const bool depthStencilClearOp /* true */,
    const std::vector<VkComponentMapping>& swizzles /* {} */):
    Framebuffer(*colorAttachmentFormats.begin(), depthStencilFormat, 1),
    colorClearOp(colorClearOp),
    depthStencilClearOp(depthStencilClearOp)
{
    uint32_t index = 0;
    for (const VkFormat colorFormat : colorAttachmentFormats)
    {   // Create color attachment
        attachments.emplace_back(std::make_shared<ColorAttachment>(device, colorFormat, extent,
            1, 1, allocator, true));
        // Create color view
        constexpr VkComponentMapping dontSwizzle = {
            VK_COMPONENT_SWIZZLE_IDENTITY,
            VK_COMPONENT_SWIZZLE_IDENTITY,
            VK_COMPONENT_SWIZZLE_IDENTITY,
            VK_COMPONENT_SWIZZLE_IDENTITY};
        attachmentViews.emplace_back(std::make_shared<ImageView>(attachments.back(),
            swizzles.empty() ? dontSwizzle : swizzles[index++]));
    }
    if (depthStencilFormat != VK_FORMAT_UNDEFINED)
    {   // Create depth/stencil attachment
        attachments.emplace_back(std::make_shared<DepthStencilAttachment>(device, depthStencilFormat, extent,
            1, 1, allocator, depthSampled));
        // Create depth/stencil view
        attachmentViews.push_back(std::make_shared<ImageView>(attachments.back()));
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
        const VkImageLayout finalLayout = optimalDepthStencilLayout(device, depthStencilFormat, depthSampled);
        attachmentDescriptions.emplace_back(depthStencilFormat, 1,
            depthStencilClearOp ? op::clearStore : op::loadStore, // Clear depth or preserve from a separate depth pass
            hasStencil() ? (depthStencilClearOp ? op::clearStore : op::loadStore) // Clear stencil or preserve from a separate depth pass
                         : op::dontCare, // No stencil aspect
            VK_IMAGE_LAYOUT_UNDEFINED, // Don't care
            finalLayout); // Depth image will be transitioned to when a render pass instance ends
    }
    // Create color/depth framebuffer
    constexpr uint32_t layers = 1;
    renderPass = std::make_shared<RenderPass>(device, attachmentDescriptions, MAGMA_HOST_ALLOCATOR(allocator));
    framebuffer = std::make_shared<magma::Framebuffer>(renderPass, attachmentViews, MAGMA_HOST_ALLOCATOR(allocator), layers, 0);
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
        AttachmentDescription depthAttachment(depthStencilFormat, 1,
            op::clearStore, // Clear depth, store
            hasStencil() ? op::clearStore : op::dontCare, // Clear stencil, store
            VK_IMAGE_LAYOUT_UNDEFINED, // Don't care
            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL); // Stay as attachment when a depth pass instance ends
        constexpr uint32_t layers = 1;
        depthRenderPass = std::make_shared<RenderPass>(renderPass->getDevice(), depthAttachment, renderPass->getHostAllocator());
        depthFramebuffer = std::make_shared<magma::Framebuffer>(depthRenderPass, attachmentViews.back(), framebuffer->getHostAllocator(), layers, 0);
    }
    return depthRenderPass;
}
} // namespace aux
} // namespace magma
