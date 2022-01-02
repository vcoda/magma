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
#include "depthFramebuffer.h"
#include "../objects/device.h"
#include "../objects/physicalDevice.h"
#include "../objects/image2DAttachment.h"
#include "../objects/imageView.h"
#include "../objects/renderPass.h"
#include "../objects/framebuffer.h"
#include "../allocator/allocator.h"

namespace magma
{
namespace aux
{
DepthFramebuffer::DepthFramebuffer(std::shared_ptr<Device> device, const VkFormat depthFormat, const VkExtent2D& extent,
    std::shared_ptr<Allocator> allocator /* nullptr */):
    Framebuffer(VK_FORMAT_UNDEFINED, depthFormat, 1)
{   // Create depth attachment
    depth = std::make_shared<DepthStencilAttachment>(device, depthFormat, extent,
         1, // mipLevels
         1, // samples
         allocator,
         true); // VK_IMAGE_USAGE_SAMPLED_BIT
    // Create depth view
    depthView = std::make_shared<ImageView>(depth);
    // We should be able to read depth in the shader when a render pass instance ends
    const VkImageLayout finalLayout = optimalDepthStencilLayout(device, depthFormat, true);
    const AttachmentDescription depthAttachment(depthFormat, 1,
         op::clearStore, // Clear depth, store
         op::dontCare, // Stencil don't care
         VK_IMAGE_LAYOUT_UNDEFINED, // Don't care
         finalLayout); // Depth image will be transitioned to when a render pass instance ends
    renderPass = std::make_shared<RenderPass>(std::move(device), depthAttachment, MAGMA_HOST_ALLOCATOR(allocator));
    framebuffer = std::make_shared<magma::Framebuffer>(renderPass, depthView, MAGMA_HOST_ALLOCATOR(allocator), 0);
}
} // namespace aux
} // namespace magma
