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
#include "colorFramebuffer.h"
#include "../objects/device.h"
#include "../objects/image2DAttachment.h"
#include "../objects/imageView.h"
#include "../objects/renderPass.h"
#include "../objects/framebuffer.h"

namespace magma
{
namespace aux
{
ColorFramebuffer::ColorFramebuffer(std::shared_ptr<Device> device,
    const VkFormat colorFormat, const VkFormat depthStencilFormat, const VkExtent2D& extent,
    const VkComponentMapping& swizzle /* VK_COMPONENT_SWIZZLE_IDENTITY */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Framebuffer(1)
{   // Create color/depth attachments
    color = std::make_shared<ColorAttachment>(device, colorFormat, extent, 1, 1, true, allocator);
    if (depthStencilFormat != VK_FORMAT_UNDEFINED)
        depthStencil = std::make_shared<DepthStencilAttachment>(device, depthStencilFormat, extent, 1, 1, false, allocator);
    // Create attachment views
    colorView = std::make_shared<ImageView>(color, swizzle, allocator);
    if (depthStencilFormat != VK_FORMAT_UNDEFINED)
        depthStencilView = std::make_shared<ImageView>(depthStencil, swizzle, allocator);
    // Setup attachment descriptors
    const AttachmentDescription colorAttachment(colorFormat, 1,
        op::clearStore, // Clear color, store
        op::dontCare, // Stencil not applicable
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    if (depthStencilFormat != VK_FORMAT_UNDEFINED)
    {
        const AttachmentDescription depthStencilAttachment(depthStencilFormat, 1,
            op::clearStore, // Clear depth, store
            op::clearStore, // Clear stencil, store
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
        // Create color/depth framebuffer
        renderPass = std::make_shared<RenderPass>(std::move(device), std::initializer_list<AttachmentDescription>{
            colorAttachment, depthStencilAttachment});
        framebuffer = std::make_shared<magma::Framebuffer>(renderPass, std::vector<std::shared_ptr<ImageView>>{
            colorView, depthStencilView});
    }
    else
    {   // Create color only framebuffer
        renderPass = std::make_shared<RenderPass>(std::move(device), colorAttachment, allocator);
        framebuffer = std::make_shared<magma::Framebuffer>(renderPass, colorView, 0, std::move(allocator));
    }
}
} // namespace aux
} // namespace magma
