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
#include "framebufferAttachmentImage.h"
#include "../objects/image.h"
#include "../core/copy.h"

namespace magma
{
#ifdef VK_KHR_imageless_framebuffer
static_assert(sizeof(FramebufferAttachmentImage) == sizeof(VkFramebufferAttachmentImageInfoKHR),
    "framebuffer attachment image size mismatch");

FramebufferAttachmentImage::FramebufferAttachmentImage(VkImageUsageFlags usage,
    uint32_t width, uint32_t height, uint32_t layerCount, const std::vector<VkFormat>& viewFormats,
    VkImageCreateFlags flags /* 0 */) noexcept:
    VkFramebufferAttachmentImageInfoKHR{
        VK_STRUCTURE_TYPE_FRAMEBUFFER_ATTACHMENT_IMAGE_INFO_KHR,
        nullptr,
        flags,
        usage,
        width,
        height,
        layerCount,
        MAGMA_COUNT(viewFormats),
        core::copyVector(viewFormats)
    }
{}

FramebufferAttachmentImage::FramebufferAttachmentImage(std::shared_ptr<const Image> image) noexcept
{
    sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_ATTACHMENT_IMAGE_INFO_KHR;
    pNext = nullptr;
    flags = image->getFlags();
    usage = image->getUsage();
    width = image->getMipExtent(0).width;
    height = image->getMipExtent(0).height;
    layerCount = image->getArrayLayers();
    viewFormatCount = MAGMA_COUNT(image->getViewFormats());
    pViewFormats = core::copyVector(image->getViewFormats());
}

FramebufferAttachmentImage::FramebufferAttachmentImage(const FramebufferAttachmentImage& other) noexcept
{
    sType = other.sType;
    pNext = other.pNext;
    flags = other.flags;
    usage = other.usage;
    width = other.width;
    height = other.height;
    layerCount = other.layerCount;
    viewFormatCount = other.viewFormatCount;
    delete[] pViewFormats;
    pViewFormats = core::copyArray(other.pViewFormats, other.viewFormatCount);
}

FramebufferAttachmentImage& FramebufferAttachmentImage::operator=(const FramebufferAttachmentImage& other) noexcept
{
    if (this != &other)
    {
        flags = other.flags;
        usage = other.usage;
        width = other.width;
        height = other.height;
        layerCount = other.layerCount;
        viewFormatCount = other.viewFormatCount;
        delete[] pViewFormats;
        pViewFormats = core::copyArray(other.pViewFormats, other.viewFormatCount);
    }
    return *this;
}
#endif // VK_KHR_imageless_framebuffer
} // namespace magma
