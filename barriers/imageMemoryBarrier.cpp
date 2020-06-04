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
#include "imageMemoryBarrier.h"
#include "../objects/image.h"
#include "../misc/imageSubresourceRange.h"
#include "../exceptions/notImplemented.h"

namespace magma
{
ImageMemoryBarrier::ImageMemoryBarrier(std::shared_ptr<Image> image, VkImageLayout newLayout):
    ImageMemoryBarrier(std::move(image), newLayout, ImageSubresourceRange(image))
{}

ImageMemoryBarrier::ImageMemoryBarrier(std::shared_ptr<Image> image, VkImageLayout newLayout,
    VkImageSubresourceRange subresourceRange):
    resource(std::move(image))
{
    sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    pNext = nullptr;
    switch (resource->getLayout())
    {
    case VK_IMAGE_LAYOUT_UNDEFINED:
        srcAccessMask = 0;
        break;
    case VK_IMAGE_LAYOUT_GENERAL:
        // Supports all types of device access, not recommended due to lower performance.
        srcAccessMask = VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT |
            VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
            VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT |
            VK_ACCESS_TRANSFER_READ_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
        break;
    case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
        srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        break;
    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
        srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        break;
    case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
        srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
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
    case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
#ifdef VK_KHR_shared_presentable_image
    case VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR:
#endif
        srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        break;
#ifdef VK_NV_shading_rate_image
    case VK_IMAGE_LAYOUT_SHADING_RATE_OPTIMAL_NV:
        srcAccessMask = VK_ACCESS_SHADING_RATE_IMAGE_READ_BIT_NV;
        break;
#endif // VK_NV_shading_rate_image
    default:
        MAGMA_THROW_NOT_IMPLEMENTED;
    }
    switch (newLayout)
    {
    case VK_IMAGE_LAYOUT_GENERAL:
        // Supports all types of device access, not recommended due to lower performance.
        dstAccessMask = VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT |
            VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
            VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT |
            VK_ACCESS_TRANSFER_READ_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
        break;
    case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
        dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        break;
    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
        dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        break;
    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL:
        if (VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL == oldLayout)
            MAGMA_THROW("image memory barrier not neccessary");
        dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
        break;
    case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
        if (0 == srcAccessMask)
            srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT | VK_ACCESS_HOST_WRITE_BIT;
        dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        break;
    case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
        dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        break;
    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
        dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        break;
    case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
#ifdef VK_KHR_shared_presentable_image
    case VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR:
#endif
        dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        break;
#ifdef VK_NV_shading_rate_image
    case VK_IMAGE_LAYOUT_SHADING_RATE_OPTIMAL_NV:
        dstAccessMask = VK_ACCESS_SHADING_RATE_IMAGE_READ_BIT_NV;
        break;
#endif
    default:
        MAGMA_THROW_NOT_IMPLEMENTED;
    }
    this->oldLayout = resource->getLayout();
    this->newLayout = newLayout;
    srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    this->image = MAGMA_HANDLE(resource);
    this->subresourceRange = subresourceRange;
}
} // namespace magma
