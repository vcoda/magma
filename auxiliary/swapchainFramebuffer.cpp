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
#include "swapchainFramebuffer.h"
#include "../objects/device.h"
#include "../objects/image2DAttachment.h"
#include "../objects/imageView.h"
#include "../objects/renderPass.h"
#include "../objects/framebuffer.h"
#include "../allocator/allocator.h"
#include "../misc/format.h"

namespace magma
{
namespace aux
{
SwapchainFramebuffer::SwapchainFramebuffer(std::shared_ptr<SwapchainColorAttachment> color,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    VkFormat depthStencilFormat /* VK_FORMAT_UNDEFINED */,
    const VkComponentMapping& swizzle /* VK_COMPONENT_SWIZZLE_IDENTITY */):
    Framebuffer(color->getFormat(), depthStencilFormat, color->getSamples())
{   // Create color view
    std::shared_ptr<Device> device = color->getDevice();
    colorView = std::make_shared<ImageView>(color, swizzle);
    if (depthStencilFormat != VK_FORMAT_UNDEFINED)
    {   // Let it know what view format will be paired with the image
        Image::Descriptor imageFormatList;
        imageFormatList.viewFormats.push_back(colorFormat);
        // Create depth/stencil attachment
        const VkExtent2D extent{color->getMipExtent(0).width, color->getMipExtent(0).height};
        depthStencil = std::make_shared<DepthStencilAttachment>(device, depthStencilFormat, extent, 1, color->getSamples(),
            allocator, imageFormatList, false);
        depthStencilView = std::make_shared<ImageView>(depthStencil);
    }
    const AttachmentDescription colorAttachment(color->getFormat(), 1,
        op::clearStore, // Clear color, store
        op::dontCare, // Stencil don't care
        VK_IMAGE_LAYOUT_UNDEFINED, // Don't care
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
    if (depthStencilFormat != VK_FORMAT_UNDEFINED)
    {
        const AttachmentDescription depthStencilAttachment(depthStencilFormat, 1,
            op::clearStore, // Clear depth, store
            hasStencil() ? op::clearStore : op::dontCare,
            VK_IMAGE_LAYOUT_UNDEFINED, // Don't care
            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL); // Stay as attachment
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
