/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2022 Victor Coda.

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
#include "colorMultisampleFramebuffer.h"
#include "../objects/device.h"
#include "../objects/imageAttachment.h"
#include "../objects/imageView.h"
#include "../objects/renderPass.h"
#include "../objects/framebuffer.h"
#include "../allocator/allocator.h"
#include "../misc/format.h"

namespace magma
{
namespace aux
{
ColorMultisampleFramebuffer::ColorMultisampleFramebuffer(std::shared_ptr<Device> device,
    const VkFormat colorFormat, const VkExtent2D& extent, const uint32_t sampleCount,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const bool colorClearOp /* true */,
    const bool msaaColorSampled /* false */,
    const bool msaaDepthSampled /* false */,
    const bool msaaStencilSampled /* false */,
    const VkComponentMapping& swizzle /* VK_COMPONENT_SWIZZLE_IDENTITY */):
    ColorMultisampleFramebuffer(std::move(device), colorFormat, VK_FORMAT_UNDEFINED, extent, sampleCount,
        std::move(allocator), colorClearOp, msaaColorSampled, msaaDepthSampled, msaaStencilSampled, swizzle)
{}

ColorMultisampleFramebuffer::ColorMultisampleFramebuffer(std::shared_ptr<Device> device,
    const VkFormat colorFormat, const VkFormat depthStencilFormat, const VkExtent2D& extent, const uint32_t sampleCount,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const bool colorClearOp /* true */,
    const bool msaaColorSampled /* false */,
    const bool msaaDepthSampled /* false */,
    const bool msaaStencilSampled /* false */,
    const VkComponentMapping& swizzle /* VK_COMPONENT_SWIZZLE_IDENTITY */):
    Framebuffer(colorFormat, depthStencilFormat, sampleCount),
    colorClearOp(colorClearOp)
{   // Let it know what view format will be paired with the image
    Image::Descriptor imageFormatList;
    imageFormatList.viewFormats.push_back(colorFormat);
    // Create multisample color attachment
    msaaColor = std::make_shared<ColorAttachment>(device, colorFormat, extent, 1, sampleCount, msaaColorSampled, // TODO: recheck it
        allocator, imageFormatList);
    // Create color resolve attachment
    constexpr bool colorSampled = true;
    resolve = std::make_shared<ColorAttachment>(device, colorFormat, extent, 1, 1, colorSampled,
        allocator, imageFormatList);
    if (depthStencilFormat != VK_FORMAT_UNDEFINED)
    {   // Create multisample depth attachment
        imageFormatList.viewFormats.back() = depthStencilFormat;
        msaaDepthStencil = std::make_shared<DepthStencilAttachment>(device, depthStencilFormat, extent, 1, sampleCount,
            msaaDepthSampled || msaaStencilSampled, allocator, imageFormatList);
    }
    // Create color and resolve views
    msaaColorView = std::make_shared<ImageView>(msaaColor, swizzle);
    resolveView = std::make_shared<ImageView>(resolve);
    if (depthStencilFormat != VK_FORMAT_UNDEFINED)
    {   // Create depth/stencil view
        msaaDepthStencilView = std::make_shared<ImageView>(msaaDepthStencil);
    }
    // Typically, after the multisampled image is resolved, we don't need the
    // multisampled image anymore. Therefore, the multisampled image must be
    // discarded by using STORE_OP_DONT_CARE.
    // https://static.docs.arm.com/100971/0101/arm_mali_application_developer_best_practices_developer_guide_100971_0101_00_en_00.pdf
    const LoadStoreOp msaaColorOp = msaaColorSampled
        ? (colorClearOp ? op::clearStore : op::store)
        : (colorClearOp ? op::clear : op::dontCare);
    // Color MSAA image will be transitioned to when a render pass instance ends
    const VkImageLayout msaaColorLayout = msaaColorSampled
        ? VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        : VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    // Setup attachment descriptors
    const AttachmentDescription msaaColorAttachment(colorFormat, sampleCount,
        msaaColorOp,
        op::dontCare, // Stencil not applicable
        VK_IMAGE_LAYOUT_UNDEFINED, // Don't care
        msaaColorLayout);
    const AttachmentDescription resolveAttachment(colorFormat, 1,
        op::store, // Don't care about clear for MSAA resolve attachment
        op::dontCare, // Stencil not applicable
        VK_IMAGE_LAYOUT_UNDEFINED, // Don't care
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL); // Resolve image will be transitioned to when a render pass instance ends
    if (depthStencilFormat != VK_FORMAT_UNDEFINED)
    {   // Clear depth/stencil, store if depth (and/or stencil) sampled
        const LoadStoreOp msaaDepthOp = msaaDepthSampled ? op::clearStore : op::clear;
        const LoadStoreOp msaaStencilOp = msaaStencilSampled ? op::clearStore : op::clear;
        // Choose optimal depth/stencil layout
        const VkImageLayout msaaDepthStencilLayout = optimalDepthStencilLayout(device, depthStencilFormat, msaaDepthSampled || msaaStencilSampled);
        const AttachmentDescription msaaDepthStencilAttachment(depthStencilFormat, sampleCount,
            msaaDepthOp,
            hasStencil() ? msaaStencilOp : op::dontCare,
            VK_IMAGE_LAYOUT_UNDEFINED, // Don't care
            msaaDepthStencilLayout); // Depth image will be transitioned to when a render pass instance ends
        // Create color/depth multisample framebuffer
        renderPass = std::make_shared<RenderPass>(std::move(device),
            std::initializer_list<AttachmentDescription>{msaaColorAttachment, msaaDepthStencilAttachment, resolveAttachment},
            MAGMA_HOST_ALLOCATOR(allocator));
        framebuffer = std::make_shared<magma::Framebuffer>(renderPass,
            std::vector<std::shared_ptr<ImageView>>{msaaColorView, msaaDepthStencilView, resolveView},
            MAGMA_HOST_ALLOCATOR(allocator), 0);
    }
    else
    {   // Create color only multisample framebuffer
        renderPass = std::make_shared<RenderPass>(std::move(device),
            std::initializer_list<AttachmentDescription>{msaaColorAttachment, resolveAttachment},
            MAGMA_HOST_ALLOCATOR(allocator));
        framebuffer = std::make_shared<magma::Framebuffer>(renderPass,
            std::vector<std::shared_ptr<ImageView>>{msaaColorView, resolveView},
            MAGMA_HOST_ALLOCATOR(allocator), 0);
    }
}
} // namespace aux
} // namespace magma
