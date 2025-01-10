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
#include "pch.h"
#pragma hdrstop
#include "framebuffer.h"
#include "../objects/device.h"
#include "../objects/imageAttachment.h"
#include "../objects/swapchainImage.h"
#include "../objects/imageView.h"
#include "../objects/renderPass.h"
#include "../objects/framebuffer.h"
#include "../states/multisampleState.h"
#include "../misc/featureQuery.h"
#include "../misc/format.h"

namespace magma::aux
{
Framebuffer::Framebuffer(std::shared_ptr<Device> device, VkFormat colorFormat, VkFormat depthStencilFormat,
    const VkExtent2D& extent, uint32_t sampleCount, bool loadOpClear, bool depthStencilSampled,
    bool explicitResolve /* false */, std::shared_ptr<Allocator> allocator /* nullptr */,
    const VkComponentMapping& swizzling /* VK_COMPONENT_SWIZZLE_IDENTITY */,
    VkRenderPassCreateFlags renderPassFlags /* 0 */, VkFramebufferCreateFlags framebufferFlags /* 0 */):
    colorFormats{colorFormat},
    depthStencilFormat(depthStencilFormat),
    sampleCount(sampleCount),
    loadOpClear(loadOpClear),
    explicitResolve(explicitResolve)
{
    if (colorFormat != VK_FORMAT_UNDEFINED)
    {   // Let driver know what view format will be paired with color attachment
        Image::Initializer initializer;
        initializer.viewFormats.push_back(colorFormat);
        // Create color attachment
        const bool sampled = (sampleCount == 1) ? true : false;
        std::unique_ptr<Image> colorAttachment = std::make_unique<ColorAttachment>(device,
            colorFormat, extent, 1, sampleCount, sampled, allocator, explicitResolve, initializer);
        std::shared_ptr<ImageView> colorView = std::make_shared<UniqueImageView>(std::move(colorAttachment), swizzling);
        colorViews.emplace_back(std::move(colorView));
        if (sampleCount > 1)
        {   // Create multisample resolve attachment
            std::unique_ptr<Image> resolveAttachment = std::make_unique<ColorAttachment>(device,
                colorFormat, extent, 1, 1, true, allocator, explicitResolve, initializer);
            resolveView = std::make_shared<UniqueImageView>(std::move(resolveAttachment));
        }
    }
    // Create depth/stencil attachment if necessary
    if (depthStencilFormat != VK_FORMAT_UNDEFINED)
    {   // Let driver know what view format will be paired with depth attachment
        Image::Initializer initializer;
        initializer.viewFormats.push_back(depthStencilFormat);
        std::unique_ptr<Image> depthStencilAttachment = std::make_unique<DepthStencilAttachment>(device,
            depthStencilFormat, extent, 1, sampleCount, depthStencilSampled, allocator, explicitResolve, initializer);
        depthStencilView = std::make_shared<UniqueImageView>(std::move(depthStencilAttachment));
    }
    // Configure color attachment description
    std::vector<AttachmentDescription> attachments;
    if (colorFormat != VK_FORMAT_UNDEFINED)
    {
        const LoadStoreOp loadStoreOp = (1 == sampleCount) ?
            (loadOpClear ? op::clearStore : op::store) : // Clear (optionally) color, store
            // Typically, after the multisampled image is resolved, the
            // previous contents within the area need not be preserved.
            // https://static.docs.arm.com/100971/0101/arm_mali_application_developer_best_practices_developer_guide_100971_0101_00_en_00.pdf
            (loadOpClear ? op::clear : op::dontCare); // Clear (optionally), don't preserve
        // Choose optimal color layout
        const VkImageLayout finalLayout = (1 == sampleCount)
            ? VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
            : VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        attachments.emplace_back(colorFormat, sampleCount,
            loadStoreOp,
            op::dontCare, // Stencil not applicable
            VK_IMAGE_LAYOUT_UNDEFINED, // Don't care
            // Color image will be transitioned to this layout when a render pass instance ends
            finalLayout);
    }
    // Configure depth/stencil attachment description
    if (depthStencilFormat != VK_FORMAT_UNDEFINED)
    {   // Choose optimal depth/stencil layout
        const VkImageLayout finalLayout = optimalDepthStencilLayout(device, depthStencilFormat, depthStencilSampled);
        const Format format(depthStencilFormat);
        attachments.emplace_back(depthStencilFormat, sampleCount,
            op::clearStore, // Clear depth, store
            format.stencil() || format.depthStencil() ? op::clearStore : op::dontCare, // Clear stencil if present
            VK_IMAGE_LAYOUT_UNDEFINED, // Don't care
            // Depth image will be transitioned to this layout when a render pass instance ends
            finalLayout);
    }
    if ((colorFormat != VK_FORMAT_UNDEFINED) && (sampleCount > 1) && !explicitResolve)
    {   // Push implicit resolve attachment to the end to avoid extra
        // clear color in the VkRenderPassBeginInfo::pClearValues array.
        attachments.emplace_back(colorFormat, 1,
            op::store, // Don't care about clear for multisample resolve attachment
            op::dontCare, // Stencil not applicable
            VK_IMAGE_LAYOUT_UNDEFINED, // Don't care
            // Resolved image will be transitioned to read only layout when a render pass instance ends
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    }
    // Create render pass and framebuffer
    std::shared_ptr<IAllocator> hostAllocator = MAGMA_HOST_ALLOCATOR(allocator);
    std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>(device, attachments, hostAllocator, renderPassFlags);
    if (!depthStencilView && !resolveView)
        framebuffer = std::make_unique<magma::Framebuffer>(std::move(renderPass), colorViews, std::move(hostAllocator), framebufferFlags);
    else
    {
        std::vector<std::shared_ptr<ImageView>> imageViews;
        imageViews.push_back(colorViews.front());
        if (depthStencilView)
            imageViews.push_back(depthStencilView);
        if ((colorFormat != VK_FORMAT_UNDEFINED) && (sampleCount > 1) && !explicitResolve)
            imageViews.push_back(resolveView);
        framebuffer = std::make_unique<magma::Framebuffer>(std::move(renderPass), imageViews, std::move(hostAllocator), framebufferFlags);
    }
}

Framebuffer::Framebuffer(std::shared_ptr<Device> device, const std::initializer_list<VkFormat>& colorFormats, VkFormat depthStencilFormat,
    const VkExtent2D& extent, bool loadOpClear, bool depthStencilSampled, std::shared_ptr<Allocator> allocator /* nullptr */,
    const std::vector<VkComponentMapping>& swizzles /* empty */,
    VkRenderPassCreateFlags renderPassFlags /* 0 */, VkFramebufferCreateFlags framebufferFlags /* 0 */):
    colorFormats(colorFormats),
    depthStencilFormat(depthStencilFormat),
    sampleCount(1),
    loadOpClear(loadOpClear),
    explicitResolve(false)
{   // Create an array of color attachments
    Image::Initializer initializer;
    initializer.viewFormats.push_back(VK_FORMAT_UNDEFINED);
    size_t index = 0;
    for (const VkFormat colorFormat: colorFormats)
    {   // Let driver know what view formats will be paired with the image
        MAGMA_ASSERT(colorFormat != VK_FORMAT_UNDEFINED);
        initializer.viewFormats.back() = colorFormat;
        constexpr bool sampled = true;
        std::unique_ptr<Image> colorAttachment = std::make_unique<ColorAttachment>(device,
            colorFormat, extent, 1, 1, sampled, allocator, explicitResolve, initializer);
        std::shared_ptr<ImageView> colorView;
        if (index < swizzles.size())
            colorView = std::make_shared<UniqueImageView>(std::move(colorAttachment), swizzles[index++]);
        else
            colorView = std::make_shared<UniqueImageView>(std::move(colorAttachment));
        colorViews.emplace_back(std::move(colorView));
    }
    // Create depth/stencil attachment if necessary
    if (depthStencilFormat != VK_FORMAT_UNDEFINED)
    {   // Let driver know what view format will be paired with depth attachment
        initializer.viewFormats.back() = depthStencilFormat;
        std::unique_ptr<Image> depthStencilAttachment = std::make_unique<DepthStencilAttachment>(device,
            depthStencilFormat, extent, 1, sampleCount, depthStencilSampled, allocator, explicitResolve, initializer);
        depthStencilView = std::make_shared<UniqueImageView>(std::move(depthStencilAttachment));
    }
    std::vector<AttachmentDescription> attachments;
    for (const VkFormat colorFormat: colorFormats)
    {   // Setup color attachment description
        attachments.emplace_back(colorFormat, 1,
            loadOpClear ? op::clearStore : op::store,
            op::dontCare, // Stencil not applicable
            VK_IMAGE_LAYOUT_UNDEFINED, // Don't care
            // Color image will be transitioned to this layout when a render pass instance ends
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    }
    // Setup depth/stencil attachment description
    if (depthStencilFormat != VK_FORMAT_UNDEFINED)
    {   // Choose optimal depth/stencil layout
        const VkImageLayout finalLayout = optimalDepthStencilLayout(device, depthStencilFormat, depthStencilSampled);
        const Format format(depthStencilFormat);
        attachments.emplace_back(depthStencilFormat, 1,
            op::clearStore, // Clear depth, store
            format.stencil() || format.depthStencil() ? op::clearStore : op::dontCare, // Clear stencil if present
            VK_IMAGE_LAYOUT_UNDEFINED, // Don't care
            // Depth image will be transitioned to this layout when a render pass instance ends
            finalLayout);
    }
    // Create render pass and framebuffer
    std::shared_ptr<IAllocator> hostAllocator = MAGMA_HOST_ALLOCATOR(allocator);
    std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>(device, attachments, hostAllocator, renderPassFlags);
    if (!depthStencilView)
        framebuffer = std::make_unique<magma::Framebuffer>(std::move(renderPass), colorViews, std::move(hostAllocator), framebufferFlags);
    else
    {
        std::vector<std::shared_ptr<ImageView>> imageViews = colorViews;
        imageViews.push_back(depthStencilView);
        framebuffer = std::make_unique<magma::Framebuffer>(std::move(renderPass), imageViews, std::move(hostAllocator), framebufferFlags);
    }
}

Framebuffer::Framebuffer(std::shared_ptr<SwapchainImage> colorAttachment,
    std::shared_ptr<ImageView> sharedDepthStencilView /* nullptr */,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const VkComponentMapping& swizzling /* VK_COMPONENT_SWIZZLE_IDENTITY */,
    VkRenderPassCreateFlags renderPassFlags /* 0 */,
    VkFramebufferCreateFlags framebufferFlags /* 0 */):
    colorFormats{colorAttachment->getFormat()},
    depthStencilFormat(sharedDepthStencilView ? sharedDepthStencilView->getImage()->getFormat() : VK_FORMAT_UNDEFINED),
    sampleCount(colorAttachment->getSamples()),
    loadOpClear(true),
    explicitResolve(false),
    depthStencilView(std::move(sharedDepthStencilView))
{
    std::shared_ptr<Device> device = colorAttachment->getDevice();
    std::shared_ptr<ImageView> colorView = std::make_shared<SharedImageView>(std::move(colorAttachment), swizzling);
    colorViews.emplace_back(std::move(colorView));
    std::vector<AttachmentDescription> attachments;
    attachments.emplace_back(
        colorViews.front()->getImage()->getFormat(),
        colorViews.front()->getImage()->getSamples(),
        op::clearStore, // Clear color, store
        op::dontCare, // Stencil not applicable
        VK_IMAGE_LAYOUT_UNDEFINED, // Don't care
        // Swapchain's attachment have to be transitioned to specific layout
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
    if (depthStencilView)
    {
        const Format depthStencilFormat(depthStencilView->getFormat());
        attachments.emplace_back(
            depthStencilFormat,
            depthStencilView->getImage()->getSamples(),
            magma::op::clearStore, // Clear depth, store
            depthStencilFormat.stencil() || depthStencilFormat.depthStencil()
                ? op::clearStore // Clear stencil, store
                : op::dontCare,  // Don't care about stencil
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
    }
    // Create render pass and framebuffer
    std::shared_ptr<IAllocator> hostAllocator = MAGMA_HOST_ALLOCATOR(allocator);
    std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>(std::move(device), attachments, hostAllocator, renderPassFlags);
    if (!depthStencilView)
        framebuffer = std::make_unique<magma::Framebuffer>(std::move(renderPass), colorViews.front(), std::move(hostAllocator), framebufferFlags);
    else
    {
        framebuffer = std::make_unique<magma::Framebuffer>(std::move(renderPass),
            std::vector<std::shared_ptr<ImageView>>{colorViews.front(), depthStencilView},
            std::move(hostAllocator), framebufferFlags);
    }
}

Framebuffer::~Framebuffer() {}

const VkExtent2D& Framebuffer::getExtent() const noexcept
{
    return framebuffer->getExtent();
}

const MultisampleState& Framebuffer::getMultisampleState() const noexcept
{
    switch (sampleCount)
    {
    case 1: return renderstate::dontMultisample;
    case 2: return renderstate::msaa2x;
    case 4: return renderstate::msaa4x;
    case 8: return renderstate::msaa8x;
    case 16: return renderstate::msaa16x;
    case 32: return renderstate::msaa32x;
    case 64: return renderstate::msaa64x;
    default: return renderstate::dontMultisample;
    }
}

const std::shared_ptr<const RenderPass>& Framebuffer::getRenderPass() const noexcept
{
    return framebuffer->getRenderPass();
}

VkImageLayout Framebuffer::optimalDepthStencilLayout(std::shared_ptr<Device> device,
    VkFormat depthStencilFormat, bool sampled) const
{
#ifdef VK_KHR_separate_depth_stencil_layouts
    if (device->checkFeatures()->separateDepthStencilLayoutsEnabled())
    {
        const Format format(depthStencilFormat);
        if (format.depth())
        {
            return sampled ? VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL_KHR :
                VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL_KHR;
        }
        if (format.stencil())
        {
            return sampled ? VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL_KHR :
                VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL_KHR;
        }
    }
#else
    MAGMA_UNUSED(device);
    MAGMA_UNUSED(depthStencilFormat);
#endif // VK_KHR_separate_depth_stencil_layouts
    return sampled ? VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL :
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
}
} // namespace magma::aux
