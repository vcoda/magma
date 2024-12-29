/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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
#include "imageView.h"
#include "image.h"
#include "sampler.h"
#include "device.h"
#include "../allocator/allocator.h"
#include "../misc/format.h"
#include "../exceptions/errorResult.h"

namespace magma
{
ImageView::ImageView(const Image *image, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount_,
    const VkComponentMapping& swizzling, VkImageViewCreateFlags flags, VkImageUsageFlags usage, const StructureChain& extendedInfo):
    NonDispatchable(VK_OBJECT_TYPE_IMAGE_VIEW, image->getDevice(), image->getHostAllocator()),
    flags(flags),
    viewType(imageToViewType(image->getType(), image->getArrayLayers(), image->getFlags())),
    format(image->getFormat()),
#ifdef VK_KHR_maintenance2
    usage(usage),
#endif
    baseMipLevel(baseMipLevel),
    baseArrayLayer(baseArrayLayer),
    layerCount(VK_REMAINING_ARRAY_LAYERS == layerCount_ ? image->getArrayLayers() - baseArrayLayer : layerCount_)
{
    MAGMA_UNUSED(usage);
    VkImageViewCreateInfo imageViewInfo;
    imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imageViewInfo.pNext = extendedInfo.headNode();
    imageViewInfo.flags = flags;
    imageViewInfo.image = *image;
    imageViewInfo.viewType = viewType;
    imageViewInfo.format = image->getFormat();
    const Format format(imageViewInfo.format);
    if (!(format.depth() || format.stencil() || format.depthStencil()))
        imageViewInfo.components = swizzling;
    else
    {   // Depth/stencil shouldn't have swizzling
        imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    }
    imageViewInfo.subresourceRange.aspectMask = image->getAspectMask();
    imageViewInfo.subresourceRange.baseMipLevel = baseMipLevel;
    imageViewInfo.subresourceRange.levelCount = levelCount;
    imageViewInfo.subresourceRange.baseArrayLayer = baseArrayLayer;
    imageViewInfo.subresourceRange.layerCount = layerCount_;
#ifdef VK_KHR_maintenance2
    VkImageViewUsageCreateInfoKHR imageViewUsageInfo;
    if (usage && extensionEnabled(VK_KHR_MAINTENANCE2_EXTENSION_NAME))
    {   // Usage of image view can be restricted compared to the parent image's usage flags
        imageViewUsageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_USAGE_CREATE_INFO_KHR;
        imageViewUsageInfo.pNext = nullptr;
        imageViewUsageInfo.usage = usage;
        linkNode(imageViewInfo, imageViewUsageInfo);
    }
#endif // VK_KHR_maintenance2
    const VkResult result = vkCreateImageView(getNativeDevice(), &imageViewInfo, MAGMA_OPTIONAL(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create image view");
}

ImageView::ImageView(const Image *image, uint32_t baseMipLevel, uint32_t baseArrayLayer, uint32_t layerCount, VkImageViewCreateFlags flags):
    NonDispatchable(VK_OBJECT_TYPE_IMAGE_VIEW, image->getDevice(), image->getHostAllocator()),
    flags(flags),
    viewType(imageToViewType(image->getType(), image->getArrayLayers(), image->getFlags())),
    format(image->getFormat()),
#ifdef VK_KHR_maintenance2
    usage(image->getUsage()),
#endif
    baseMipLevel(baseMipLevel),
    baseArrayLayer(baseArrayLayer),
    layerCount(layerCount)
{}

ImageView::~ImageView()
{
    vkDestroyImageView(getNativeDevice(), handle, MAGMA_OPTIONAL(hostAllocator));
}

VkExtent2D ImageView::getExtent2D() const noexcept
{
    const VkExtent3D extent = getExtent3D();
    return {extent.width, extent.height};
}

VkExtent3D ImageView::getExtent3D() const noexcept
{
    return getImage()->calculateMipExtent(baseMipLevel);
}

VkDescriptorImageInfo ImageView::getDescriptor(const Sampler *sampler /* nullptr */) const noexcept
{
    MAGMA_ASSERT(getImage()->hasUniformLayout());
    VkDescriptorImageInfo imageDescriptorInfo;
    imageDescriptorInfo.sampler = sampler ? sampler->getHandle() : VK_NULL_HANDLE;
    imageDescriptorInfo.imageView = handle;
    imageDescriptorInfo.imageLayout = getImage()->getLayout(0);
    return imageDescriptorInfo;
}

VkImageViewType ImageView::imageToViewType(VkImageType imageType, uint32_t arrayLayers, VkImageCreateFlags flags) noexcept
{
    switch (imageType)
    {
    case VK_IMAGE_TYPE_1D:
        if (arrayLayers > 1)
            return VK_IMAGE_VIEW_TYPE_1D_ARRAY;
        return VK_IMAGE_VIEW_TYPE_1D;
    case VK_IMAGE_TYPE_2D:
        if (flags & VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT)
        {
            if (arrayLayers > 6)
                return VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
            return VK_IMAGE_VIEW_TYPE_CUBE;
        }
        else
        {
            if (arrayLayers > 1)
                return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
            return VK_IMAGE_VIEW_TYPE_2D;
        }
    case VK_IMAGE_TYPE_3D:
    #ifdef VK_KHR_maintenance1
        if (flags & VK_IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT_KHR)
            return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
    #endif
        return VK_IMAGE_VIEW_TYPE_3D;
    default:
        MAGMA_FAILURE("unknown image type");
        return VK_IMAGE_VIEW_TYPE_MAX_ENUM;
    }
}
} // namespace magma
