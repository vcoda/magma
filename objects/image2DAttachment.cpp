/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2019 Victor Coda.

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
#include "image2DAttachment.h"

namespace magma
{
ColorAttachment2D::ColorAttachment2D(std::shared_ptr<Device> device,
    VkFormat colorFormat,
    const VkExtent2D& extent,
    uint32_t mipLevels,
    uint32_t samples,
    bool sampled /* true */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Image2D(std::move(device), colorFormat, extent, mipLevels, samples,
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | (sampled ? VK_IMAGE_USAGE_SAMPLED_BIT : 0),
        Sharing(), std::move(allocator))
{}

DepthStencilAttachment2D::DepthStencilAttachment2D(std::shared_ptr<Device> device,
    VkFormat depthStencilFormat,
    const VkExtent2D& extent,
    uint32_t mipLevels,
    uint32_t samples,
    bool sampled /* false */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Image2D(std::move(device), depthStencilFormat, extent, mipLevels, samples,
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | (sampled ? VK_IMAGE_USAGE_SAMPLED_BIT : 0),
        Sharing(), std::move(allocator))
{}

SwapchainColorAttachment2D::SwapchainColorAttachment2D(std::shared_ptr<Device> device,
    VkImage handle, VkFormat format, const VkExtent2D& extent):
    Image2D(std::move(device), handle, format, extent)
{
    layout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
}

SwapchainColorAttachment2D::~SwapchainColorAttachment2D()
{   // vkDestroyImage() shouldn't have effect on it as it was not created via vkCreateImage().
    handle = VK_NULL_HANDLE;
}
} // namespace magma
