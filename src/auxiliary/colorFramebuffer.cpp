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
#include "pch.h"
#pragma hdrstop
#include "colorFramebuffer.h"
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
ColorFramebuffer::ColorFramebuffer(std::shared_ptr<Device> device, const VkFormat colorFormat, const VkExtent2D& extent,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const bool colorClearOp /* true */,
    const VkComponentMapping& swizzle /* VK_COMPONENT_SWIZZLE_IDENTITY */):
    ColorFramebuffer(std::move(device), colorFormat, VK_FORMAT_UNDEFINED, extent, std::move(allocator), colorClearOp, false, swizzle)
{}

ColorFramebuffer::ColorFramebuffer(std::shared_ptr<Device> device, const VkFormat colorFormat,
    const VkFormat depthStencilFormat, const VkExtent2D& extent,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const bool colorClearOp /* true */,
    const bool depthSampled, /* false */
    const VkComponentMapping& swizzle /* VK_COMPONENT_SWIZZLE_IDENTITY */):
    Framebuffer(colorFormat, depthStencilFormat, 1),
    colorClearOp(colorClearOp)
{   // Let it know what view format will be paired with the image
    Image::Descriptor imageFormatList;
    imageFormatList.viewFormats.push_back(colorFormat);
    // Create color attachment
    constexpr bool colorSampled = true;
    color = std::make_shared<ColorAttachment>(device, colorFormat, extent, 1, 1, colorSampled,
        allocator, imageFormatList);
    if (depthStencilFormat != VK_FORMAT_UNDEFINED)
    {   // Create depth/stencil attachment
        imageFormatList.viewFormats.back() = depthStencilFormat;
        depthStencil = std::make_shared<DepthStencilAttachment>(device, depthStencilFormat, extent, 1, 1, depthSampled,
            allocator, imageFormatList);
    }
    // Create color view
    colorView = std::make_shared<ImageView>(color, swizzle);
    if (depthStencilFormat != VK_FORMAT_UNDEFINED)
    {   // Create depth/stencil view
        depthStencilView = std::make_shared<ImageView>(depthStencil);
    }
    // Setup attachment descriptors
    const AttachmentDescription colorAttachment(colorFormat, 1,
        colorClearOp ? op::clearStore : op::store, // Clear (optionally) color, store
        op::dontCare, // Stencil not applicable
        VK_IMAGE_LAYOUT_UNDEFINED, // Don't care
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL); // Color image will be transitioned to when a render pass instance ends
    if (depthStencilFormat != VK_FORMAT_UNDEFINED)
    {   // Choose optimal depth/stencil layout
        const VkImageLayout depthStencilLayout = optimalDepthStencilLayout(device, depthStencilFormat, depthSampled);
        const AttachmentDescription depthStencilAttachment(depthStencilFormat, 1,
            op::clearStore, // Clear depth, store
            hasStencil() ? op::clearStore : op::dontCare, // Clear stencil if present
            VK_IMAGE_LAYOUT_UNDEFINED, // Don't care
            depthStencilLayout); // Depth image will be transitioned to when a render pass instance ends
        // Create color/depth framebuffer
        renderPass = std::make_shared<RenderPass>(std::move(device),
            std::initializer_list<AttachmentDescription>{colorAttachment, depthStencilAttachment},
            MAGMA_HOST_ALLOCATOR(allocator));
        framebuffer = std::make_shared<magma::Framebuffer>(renderPass,
            std::vector<std::shared_ptr<ImageView>>{colorView, depthStencilView},
            MAGMA_HOST_ALLOCATOR(allocator), 0);
    }
    else
    {   // Create color only framebuffer
        renderPass = std::make_shared<RenderPass>(std::move(device), colorAttachment, MAGMA_HOST_ALLOCATOR(allocator));
        framebuffer = std::make_shared<magma::Framebuffer>(renderPass, colorView, MAGMA_HOST_ALLOCATOR(allocator), 0);
    }
}
} // namespace aux
} // namespace magma
