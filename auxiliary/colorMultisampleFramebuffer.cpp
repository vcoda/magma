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
#include "colorMultisampleFramebuffer.h"
#include "../objects/device.h"
#include "../objects/image2DAttachment.h"
#include "../objects/imageView.h"
#include "../objects/renderPass.h"
#include "../objects/framebuffer.h"

namespace magma
{
namespace aux
{
ColorMultisampleFramebuffer::ColorMultisampleFramebuffer(std::shared_ptr<Device> device,
    const VkFormat colorFormat, const VkFormat depthStencilFormat,
    const VkExtent2D& extent, const uint32_t sampleCount,
    const VkComponentMapping& swizzle /* VK_COMPONENT_SWIZZLE_IDENTITY */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Framebuffer(sampleCount)
{   // Create multisample attachments
    color = std::make_shared<ColorAttachment>(device, colorFormat, extent, 1, sampleCount, true, allocator);
    if (depthStencilFormat != VK_FORMAT_UNDEFINED)
        depthStencil = std::make_shared<DepthStencilAttachment>(device, depthStencilFormat, extent, 1, sampleCount, false, allocator);
    // Create color resolve attachment
    resolve = std::make_shared<ColorAttachment>(device, colorFormat, extent, 1, 1, true, allocator);
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
        renderPass = std::make_shared<RenderPass>(device, std::initializer_list<AttachmentDescription>{
            colorAttachment, depthStencilAttachment, resolveAttachment}, allocator);
        framebuffer = std::make_shared<magma::Framebuffer>(renderPass, std::vector<std::shared_ptr<ImageView>>{
            colorView, depthStencilView, resolveView}, 0, allocator);
    }
    else
    {   // Create color only framebuffer
        renderPass = std::make_shared<RenderPass>(device, std::initializer_list<AttachmentDescription>{
            colorAttachment, resolveAttachment}, allocator);
        framebuffer = std::make_shared<magma::Framebuffer>(renderPass, std::vector<std::shared_ptr<ImageView>>{
            colorView, resolveView}, 0, allocator);
    }
}
} // namespace aux
} // namespace magma
