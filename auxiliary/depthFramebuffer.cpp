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
#include "depthFramebuffer.h"
#include "../objects/device.h"
#include "../objects/physicalDevice.h"
#include "../objects/image2DAttachment.h"
#include "../objects/imageView.h"
#include "../objects/renderPass.h"
#include "../objects/framebuffer.h"

namespace magma
{
namespace aux
{
DepthFramebuffer::DepthFramebuffer(std::shared_ptr<Device> device, const VkFormat depthFormat, const VkExtent2D& extent,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Framebuffer(1)
{
    depth = std::make_shared<DepthStencilAttachment>(device, depthFormat, extent,
         1, // mipLevels
         1, // samples
         true, // VK_IMAGE_USAGE_SAMPLED_BIT
         allocator);
    depthView = std::make_shared<ImageView>(depth, VkComponentMapping{
        VK_COMPONENT_SWIZZLE_IDENTITY,
        VK_COMPONENT_SWIZZLE_IDENTITY,
        VK_COMPONENT_SWIZZLE_IDENTITY,
        VK_COMPONENT_SWIZZLE_IDENTITY},
        allocator);
    // We should be able to read depth in the shader when a render pass instance ends
    const VkImageLayout finalLayout = finalDepthStencilLayout(device, depthFormat, true);
    const AttachmentDescription depthAttachment(depthFormat, 1,
         op::clearStore, // Clear depth, store
         op::dontCare, // Stencil don't care
         VK_IMAGE_LAYOUT_UNDEFINED, // Don't care
         finalLayout); // Depth image will be transitioned to when a render pass instance ends
    renderPass = std::make_shared<RenderPass>(std::move(device), depthAttachment, allocator);
    framebuffer = std::make_shared<magma::Framebuffer>(renderPass, depthView, 0, std::move(allocator));
}
} // namespace aux
} // namespace magma
