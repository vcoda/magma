/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

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
#include "mutableImageView.h"
#include "mutableImage.h"
#include "device.h"
#include "../allocator/allocator.h"
#include "../exceptions/errorResult.h"

namespace magma
{
MutableImageView::MutableImageView(std::shared_ptr<MutableImage> image, VkFormat mutableFormat,
    const StructureChain& extendedInfo /* default */):
    MutableImageView(std::move(image), mutableFormat, 0, VK_REMAINING_MIP_LEVELS, 0, VK_REMAINING_ARRAY_LAYERS, extendedInfo)
{}

MutableImageView::MutableImageView(std::shared_ptr<MutableImage> image_, VkFormat mutableFormat,
    uint32_t baseMipLevel,
    uint32_t levelCount /* VK_REMAINING_MIP_LEVELS */,
    uint32_t baseArrayLayer /* 0 */,
    uint32_t layerCount /* VK_REMAINING_ARRAY_LAYERS */,
    const StructureChain& extendedInfo /* default */):
    ImageView(std::move(image_), baseMipLevel, levelCount, baseArrayLayer, layerCount, 0),
    mutableFormat(mutableFormat)
{
    VkImageViewCreateInfo imageViewInfo;
    imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imageViewInfo.pNext = extendedInfo.chainNodes();
    imageViewInfo.flags = 0;
    imageViewInfo.image = *image;
    imageViewInfo.viewType = imageToViewType(image->getType(), image->getArrayLayers(), image->getFlags());
    imageViewInfo.format = mutableFormat;
    imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    imageViewInfo.subresourceRange.baseMipLevel = baseMipLevel;
    imageViewInfo.subresourceRange.levelCount = levelCount;
    imageViewInfo.subresourceRange.baseArrayLayer = baseArrayLayer;
    imageViewInfo.subresourceRange.layerCount = layerCount;
    const VkResult result = vkCreateImageView(MAGMA_HANDLE(device), &imageViewInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create mutable image view");
}
} // namespace magma
