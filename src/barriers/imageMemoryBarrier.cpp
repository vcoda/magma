/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2025 Victor Coda.

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
#include "imageMemoryBarrier.h"
#include "../objects/image.h"
#include "../misc/imageSubresourceRange.h"

namespace magma
{
ImageMemoryBarrier::ImageMemoryBarrier(Image *image, VkImageLayout newLayout) noexcept:
    ImageMemoryBarrier(image, newLayout, ImageSubresourceRange(image))
{}

ImageMemoryBarrier::ImageMemoryBarrier(Image *image, VkImageLayout newLayout, const VkImageSubresourceRange& subresourceRange) noexcept:
    VkImageMemoryBarrier{
        VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        nullptr, // pNext
        VK_ACCESS_NONE_KHR, // srcAccessMask
        VK_ACCESS_NONE_KHR, // dstAccessMask
        image->getLayout(subresourceRange.baseMipLevel),
        newLayout,
        VK_QUEUE_FAMILY_IGNORED,
        VK_QUEUE_FAMILY_IGNORED,
        *image,
        subresourceRange
    }
{
    switch (oldLayout)
    {
    case VK_IMAGE_LAYOUT_UNDEFINED:
        // Image layout transitions with VK_IMAGE_LAYOUT_UNDEFINED
        // allow the implementation to discard the image subresource
        // range, which can provide performance or power benefits.
        // Tile-based architectures may be able to avoid flushing
        // tile data to memory, and immediate style renderers may be
        // able to achieve fast metadata clears to reinitialize
        // frame buffer compression state, or similar.
        break;
    case VK_IMAGE_LAYOUT_GENERAL:
        // Specifies all read/write accesses. It is always valid in any
        // access mask, and is treated as equivalent to setting all
        // READ/WRITE access flags that are valid where it is used.
        srcAccessMask = VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT;
        break;
    case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
        srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        break;
    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
        srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        break;
    case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
        srcAccessMask = VK_ACCESS_NONE_KHR;
        break;
    case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
        srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        break;
    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
        srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        break;
    case VK_IMAGE_LAYOUT_PREINITIALIZED:
        srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
        break;
#ifdef VK_KHR_swapchain
    case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
    #ifdef VK_KHR_shared_presentable_image
    case VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR:
    #endif
        break;
#endif // VK_KHR_swapchain
    default:
        MAGMA_FAILURE("unknown old image layout");
    }
    switch (newLayout)
    {
    case VK_IMAGE_LAYOUT_UNDEFINED:
        MAGMA_FAILURE("image cannot be transitioned into VK_IMAGE_LAYOUT_UNDEFINED layout");
        break;
    case VK_IMAGE_LAYOUT_GENERAL:
        dstAccessMask = VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT;
        break;
    case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
        MAGMA_ASSERT(image->getUsage() & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
        dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        break;
    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
        MAGMA_ASSERT(image->getUsage() & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
        dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        break;
    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL:
        // Specifies a layout for both the depth and stencil aspects of
        // a depth/stencil format image allowing read only access as
        // a depth/stencil attachment or in shaders as a sampled image,
        // combined image/sampler, or input attachment.
        MAGMA_ASSERT(image->getUsage() & (VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT));
        dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
        [[fallthrough]];
    case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
        if (image->getUsage() & VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT)
            dstAccessMask |= VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
        else if (image->getUsage() & VK_IMAGE_USAGE_SAMPLED_BIT)
            dstAccessMask |= VK_ACCESS_SHADER_READ_BIT;
        break;
    case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
        MAGMA_ASSERT(image->getUsage() & VK_IMAGE_USAGE_TRANSFER_SRC_BIT);
        dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        break;
    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
        MAGMA_ASSERT(image->getUsage() & VK_IMAGE_USAGE_TRANSFER_DST_BIT);
        dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        break;
    case VK_IMAGE_LAYOUT_PREINITIALIZED:
        MAGMA_FAILURE("image cannot be transitioned into VK_IMAGE_LAYOUT_PREINITIALIZED layout");
        break;
#ifdef VK_KHR_maintenance2
    case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL_KHR:
    case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL_KHR:
        // Two image layouts for depth stencil images to allow either the depth
        // or stencil aspect to be read-only while the other aspect is writable.
        MAGMA_ASSERT(image->getUsage() & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
        MAGMA_ASSERT(image->getUsage() & (VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT));
        dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
                        VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT |
                        VK_ACCESS_SHADER_READ_BIT;
        break;
#endif // VK_KHR_maintenance2
#ifdef VK_KHR_swapchain
    case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
    #ifdef VK_KHR_shared_presentable_image
    case VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR:
    #endif
        // When transitioning the image to VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR
        // or VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, there is no need to delay
        // subsequent processing, or perform any visibility operations
        // (as vkQueuePresentKHR performs automatic visibility operations).
        // To achieve this, the dstAccessMask member of the VkImageMemoryBarrier
        // should be set to 0, and the dstStageMask parameter should be set to
        // VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT.
        dstAccessMask = VK_ACCESS_NONE_KHR;
        break;
#endif // VK_KHR_swapchain
#ifdef VK_EXT_fragment_density_map
    case VK_IMAGE_LAYOUT_FRAGMENT_DENSITY_MAP_OPTIMAL_EXT:
        MAGMA_ASSERT(image->getUsage() & VK_IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT);
        dstAccessMask = VK_ACCESS_FRAGMENT_DENSITY_MAP_READ_BIT_EXT;
        break;
#endif // VK_EXT_fragment_density_map
#ifdef VK_NV_shading_rate_image
    case VK_IMAGE_LAYOUT_SHADING_RATE_OPTIMAL_NV:
        MAGMA_ASSERT(image->getUsage() & VK_IMAGE_USAGE_SHADING_RATE_IMAGE_BIT_NV);
        dstAccessMask = VK_ACCESS_SHADING_RATE_IMAGE_READ_BIT_NV;
        break;
#endif // VK_NV_shading_rate_image
    default:
        MAGMA_FAILURE("unknown new image layout");
    }
}

ImageMemoryBarrier::ImageMemoryBarrier(Image *image, VkImageLayout newLayout,
    VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask) noexcept:
    VkImageMemoryBarrier{
        VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        nullptr, // pNext
        srcAccessMask,
        dstAccessMask,
        image->getLayout(0),
        newLayout,
        VK_QUEUE_FAMILY_IGNORED,
        VK_QUEUE_FAMILY_IGNORED,
        *image,
        ImageSubresourceRange(image)
    }
{}

bool ImageMemoryBarrier::updateImageLayout() const noexcept
{
    Resource *resource = Resource::get(image);
    Image *barrierImage = dynamic_cast<Image *>(resource);
    MAGMA_ASSERT(barrierImage);
    if (!barrierImage)
        return false;
    uint32_t levelCount = subresourceRange.levelCount;
    if (VK_REMAINING_MIP_LEVELS == levelCount)
        levelCount = barrierImage->getMipLevels() - subresourceRange.baseMipLevel;
    MAGMA_ASSERT(levelCount);
    if (!levelCount)
        return false;
    for (uint32_t level = 0; level < levelCount; ++level)
        barrierImage->setLayout(subresourceRange.baseMipLevel + level, newLayout);
    return true;
}
} // namespace magma
