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
    constexpr bool shaderSampled = true;
    depth = std::make_shared<DepthStencilAttachment>(device, depthFormat, extent,
         1, // mipLevels
         1, // samples
         shaderSampled,
         allocator);
    depthView = std::make_shared<ImageView>(depth, VkComponentMapping{
        VK_COMPONENT_SWIZZLE_IDENTITY,
        VK_COMPONENT_SWIZZLE_IDENTITY,
        VK_COMPONENT_SWIZZLE_IDENTITY,
        VK_COMPONENT_SWIZZLE_IDENTITY},
        allocator);
    VkImageLayout finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
#ifdef VK_KHR_separate_depth_stencil_layouts
    if (device->getPhysicalDevice()->checkExtensionSupport("VK_KHR_separate_depth_stencil_layouts"))
        finalLayout = VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL_KHR;
#endif
    const AttachmentDescription depthAttachment(depthFormat, 1,
         op::clearStore, // Clear depth, store
         op::dontCare, // Stencil don't care
         VK_IMAGE_LAYOUT_UNDEFINED,
         finalLayout);
    renderPass = std::make_shared<RenderPass>(std::move(device), depthAttachment, allocator);
    framebuffer = std::make_shared<magma::Framebuffer>(renderPass, depthView, 0, std::move(allocator));
}
} // namespace aux
} // namespace magma
