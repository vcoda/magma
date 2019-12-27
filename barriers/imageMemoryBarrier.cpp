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
#include "pch.h"
#pragma hdrstop
#include "imageMemoryBarrier.h"
#include "../objects/image.h"
#include "../misc/imageSubresourceRange.h"
#include "../misc/exception.h"

namespace magma
{
ImageMemoryBarrier::ImageMemoryBarrier(std::shared_ptr<Image> image,
    VkImageLayout oldLayout, VkImageLayout newLayout):
    ImageMemoryBarrier(image, oldLayout, newLayout, ImageSubresourceRange(image))
{}

ImageMemoryBarrier::ImageMemoryBarrier(std::shared_ptr<Image> image,
    VkImageLayout oldLayout, VkImageLayout newLayout, VkImageSubresourceRange subresourceRange):
    resource(std::move(image))
{
    sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    pNext = nullptr;
    switch (oldLayout)
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
    case VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR:
        srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        break;
    case VK_IMAGE_LAYOUT_SHADING_RATE_OPTIMAL_NV:
        srcAccessMask = VK_ACCESS_SHADING_RATE_IMAGE_READ_BIT_NV;
        break;
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
    case VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR:
        dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        break;
    case VK_IMAGE_LAYOUT_SHADING_RATE_OPTIMAL_NV:
        dstAccessMask = VK_ACCESS_SHADING_RATE_IMAGE_READ_BIT_NV;
        break;
    default:
        MAGMA_THROW_NOT_IMPLEMENTED;
    }
    this->oldLayout = oldLayout;
    this->newLayout = newLayout;
    srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    this->image = MAGMA_HANDLE(resource);
    this->subresourceRange = subresourceRange;
}

ImageMemoryBarrier::ImageMemoryBarrier(std::shared_ptr<Image> image, const ImageMemoryBarrier& predefined) noexcept:
    resource(std::move(image))
{
    *this = predefined;
    this->image = MAGMA_HANDLE(resource);
}
} // namespace magma
