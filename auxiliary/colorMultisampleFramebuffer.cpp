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
#include "../misc/format.h"

namespace magma
{
namespace aux
{
ColorMultisampleFramebuffer::ColorMultisampleFramebuffer(std::shared_ptr<Device> device,
    const VkFormat colorFormat, const VkFormat depthStencilFormat,
    const VkExtent2D& extent, const uint32_t sampleCount,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    const VkComponentMapping& swizzle /* VK_COMPONENT_SWIZZLE_IDENTITY */):
    Framebuffer(sampleCount)
{   // Create multisample color attachment
    color = std::make_shared<ColorAttachment>(device, colorFormat, extent, 1, sampleCount, false, allocator);
    if (depthStencilFormat != VK_FORMAT_UNDEFINED)
    {   // Create multisample depth attachment
        depthStencil = std::make_shared<DepthStencilAttachment>(device, depthStencilFormat, extent,
            1, sampleCount, false, allocator);
    }
    // Create color resolve attachment
    constexpr bool sampled = true;
    resolve = std::make_shared<ColorAttachment>(device, colorFormat, extent, 1, 1, sampled, allocator);
    // Create color view
    colorView = std::make_shared<ImageView>(color, swizzle, allocator);
    if (depthStencilFormat != VK_FORMAT_UNDEFINED)
    {   // Create depth/stencil view
        constexpr VkComponentMapping dontSwizzle = {
            VK_COMPONENT_SWIZZLE_IDENTITY,
            VK_COMPONENT_SWIZZLE_IDENTITY,
            VK_COMPONENT_SWIZZLE_IDENTITY,
            VK_COMPONENT_SWIZZLE_IDENTITY};
        depthStencilView = std::make_shared<ImageView>(depthStencil, dontSwizzle, allocator);
    }
    // Create resolve view
    resolveView = std::make_shared<ImageView>(resolve, swizzle, allocator);
    // Setup attachment descriptors
    const AttachmentDescription colorAttachment(colorFormat, sampleCount,
        // https://static.docs.arm.com/100971/0101/arm_mali_application_developer_best_practices_developer_guide_100971_0101_00_en_00.pdf
        // Typically, after the multisampled image is resolved, we don't need the
        // multisampled image anymore. Therefore, the multisampled image must be
        // discarded by using STORE_OP_DONT_CARE.
        op::clear, // Clear color, don't care about store
        op::dontCare, // Stencil not applicable
        VK_IMAGE_LAYOUT_UNDEFINED, // Don't care
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    const AttachmentDescription resolveAttachment(colorFormat, 1,
        op::store, // Don't care about clear for MSAA resolve attachment
        op::dontCare, // Stencil not applicable
        VK_IMAGE_LAYOUT_UNDEFINED, // Don't care
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL); // Resolve image will be transitioned to when a render pass instance ends
    if (depthStencilFormat != VK_FORMAT_UNDEFINED)
    {   // Choose optimal depth/stencil layout
        const VkImageLayout finalLayout = finalDepthStencilLayout(device, depthStencilFormat, false);
        const Format format(depthStencilFormat);
        const AttachmentDescription depthStencilAttachment(depthStencilFormat, sampleCount,
            op::clear, // Depth clear, don't care about store
            format.depthStencil() || format.stencil() ? op::clear : op::dontCare,
            VK_IMAGE_LAYOUT_UNDEFINED, // Don't care
            finalLayout); // Depth image will be transitioned to when a render pass instance ends
        // Create color/depth framebuffer
        renderPass = std::make_shared<RenderPass>(std::move(device), std::initializer_list<AttachmentDescription>{
            colorAttachment, depthStencilAttachment, resolveAttachment}, allocator);
        framebuffer = std::make_shared<magma::Framebuffer>(renderPass, std::vector<std::shared_ptr<ImageView>>{
            colorView, depthStencilView, resolveView}, 0, std::move(allocator));
    }
    else
    {   // Create color only framebuffer
        renderPass = std::make_shared<RenderPass>(std::move(device), std::initializer_list<AttachmentDescription>{
            colorAttachment, resolveAttachment}, allocator);
        framebuffer = std::make_shared<magma::Framebuffer>(renderPass, std::vector<std::shared_ptr<ImageView>>{
            colorView, resolveView}, 0, std::move(allocator));
    }
}
} // namespace aux
} // namespace magma
