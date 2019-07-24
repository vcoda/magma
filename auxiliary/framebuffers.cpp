/*
Magma - C++1x interface over Khronos Vulkan API.
Copyright (C) 2018 Victor Coda.

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
#include "framebuffers.h"
#include "../objects/device.h"
#include "../objects/framebuffer.h"
#include "../objects/image2D.h"
#include "../objects/imageView.h"
#include "../objects/renderPass.h"

namespace magma
{
namespace aux
{
NonMultisampleFramebuffer::NonMultisampleFramebuffer(std::shared_ptr<Device> device,
    const VkFormat colorFormat, const VkFormat depthStencilFormat, const VkExtent2D& extent,
    const VkComponentMapping& swizzle /* VK_COMPONENT_SWIZZLE_IDENTITY */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Framebuffer(extent)
{   // Create multisample attachments
    color = std::make_shared<ColorAttachment2D>(device, colorFormat, extent, 1, 1, true, allocator);
    if (depthStencilFormat != VK_FORMAT_UNDEFINED)
        depthStencil = std::make_shared<DepthStencilAttachment2D>(device, depthStencilFormat, extent, 1, 1, false, allocator);
    // Create attachment views
    colorView = std::make_shared<ImageView>(color, swizzle, allocator);
    if (depthStencilFormat != VK_FORMAT_UNDEFINED)
        depthStencilView = std::make_shared<ImageView>(depthStencil, swizzle, allocator);
    // Setup attachment descriptors
    const AttachmentDescription colorAttachment(colorFormat, 1,
        op::clearStore, // Clear color, store
        op::dontCare, // Stencil not applicable
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL); // !
    if (depthStencilFormat != VK_FORMAT_UNDEFINED)
    {
        const AttachmentDescription depthStencilAttachment(depthStencilFormat, 1,
            op::clearStore, // Clear depth, store
            op::clearStore, // Clear stencil, store
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
        // Create color/depth framebuffer
        renderPass = std::make_shared<RenderPass>(device, std::initializer_list<AttachmentDescription>{colorAttachment, depthStencilAttachment});
        framebuffer = std::make_shared<magma::Framebuffer>(renderPass, std::vector<std::shared_ptr<ImageView>>{colorView, depthStencilView});
    }
    else
    {   // Create color only framebuffer
        renderPass = std::make_shared<RenderPass>(device, colorAttachment, allocator);
        framebuffer = std::make_shared<magma::Framebuffer>(renderPass, colorView, 0, allocator);
    }
}

MultisampleFramebuffer::MultisampleFramebuffer(std::shared_ptr<Device> device,
    const VkFormat colorFormat, const VkFormat depthStencilFormat,
    const VkExtent2D& extent, const uint32_t sampleCount,
    const VkComponentMapping& swizzle /* VK_COMPONENT_SWIZZLE_IDENTITY */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Framebuffer(extent)
{   // Create multisample attachments
    color = std::make_shared<ColorAttachment2D>(device, colorFormat, extent, 1, sampleCount, true, allocator);
    if (depthStencilFormat != VK_FORMAT_UNDEFINED)
        depthStencil = std::make_shared<DepthStencilAttachment2D>(device, depthStencilFormat, extent, 1, sampleCount, false, allocator);
    // Create color resolve attachment
    resolve = std::make_shared<ColorAttachment2D>(device, colorFormat, extent, 1, 1, true, allocator);
    // Create attachment views
    colorView = std::make_shared<ImageView>(color, swizzle, allocator);
    if (depthStencilFormat != VK_FORMAT_UNDEFINED)
        depthStencilView = std::make_shared<ImageView>(depthStencil, swizzle, allocator);
    resolveView = std::make_shared<ImageView>(resolve, swizzle, allocator);
    // Setup attachment descriptors
    const AttachmentDescription colorAttachment(colorFormat, sampleCount,
        op::clearStore, // Clear color, store
        op::dontCare, // Stencil not applicable
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    const AttachmentDescription resolveAttachment(colorFormat, 1,
        op::dontCareStore, // Don't care about clear as attachment used for MSAA resolve
        op::dontCare, // Stencil not applicable
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    if (depthStencilFormat != VK_FORMAT_UNDEFINED)
    {
        const AttachmentDescription depthStencilAttachment(depthStencilFormat, sampleCount,
            op::clearStore, // Clear depth, store
            op::clearStore, // Clear stencil, store
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
        // Create color/depth framebuffer
        renderPass = std::make_shared<RenderPass>(device, std::initializer_list<AttachmentDescription>{colorAttachment, depthStencilAttachment, resolveAttachment}, allocator);
        framebuffer = std::make_shared<magma::Framebuffer>(renderPass, std::vector<std::shared_ptr<ImageView>>{colorView, depthStencilView, resolveView}, 0, allocator);
    }
    else
    {   // Create color only framebuffer
        renderPass = std::make_shared<RenderPass>(device, std::initializer_list<AttachmentDescription>{colorAttachment, resolveAttachment}, allocator);
        framebuffer = std::make_shared<magma::Framebuffer>(renderPass, std::vector<std::shared_ptr<ImageView>>{colorView, resolveView}, 0, allocator);
    }
}

uint32_t MultisampleFramebuffer::getSampleCount() const noexcept
{
    return color->getSamples();
}

SwapchainFramebuffer::SwapchainFramebuffer(std::shared_ptr<SwapchainColorAttachment2D> color,
    VkFormat depthFormat /* VK_FORMAT_UNDEFINED */,
    const VkComponentMapping& swizzle /* VK_COMPONENT_SWIZZLE_IDENTITY */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Framebuffer(color->getMipExtent(0))
{
    std::shared_ptr<Device> device = color->getDevice();
    std::vector<std::shared_ptr<ImageView>> attachments;
    colorView = std::make_shared<ImageView>(color, swizzle, allocator);
    attachments.push_back(colorView);
    if (depthFormat != VK_FORMAT_UNDEFINED)
    {
        depthStencil = std::make_shared<DepthStencilAttachment2D>(device, depthFormat, extent, 1, color->getSamples(), false, allocator);
        depthStencilView = std::make_shared<ImageView>(depthStencil, swizzle, allocator);
        attachments.push_back(depthStencilView);
    }
    const AttachmentDescription colorAttachment(color->getFormat(), 1,
        op::clearStore, // Clear color, store
        op::dontCare, // Stencil don't care
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
    if (depthFormat != VK_FORMAT_UNDEFINED)
    {
        const AttachmentDescription depthStencilAttachment(depthFormat, 1,
            op::clearStore, // Clear depth, store
            op::clearDontCare, // Stencil don't care
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
        renderPass = std::make_shared<RenderPass>(device, std::initializer_list<AttachmentDescription>{colorAttachment, depthStencilAttachment}, allocator);
    }
    else
    {
        renderPass = std::make_shared<RenderPass>(device, colorAttachment, allocator);
    }
    framebuffer = std::make_shared<magma::Framebuffer>(renderPass, attachments, 0, allocator);
}
} // namespace aux
} // namespace magma
