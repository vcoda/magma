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
#include "imageView.h"
#include "image.h"
#include "device.h"
#include "../allocator/allocator.h"
#include "../misc/format.h"
#include "../exceptions/errorResult.h"

namespace magma
{
ImageView::ImageView(std::shared_ptr<Image> resource,
    const VkComponentMapping& swizzle /* VK_COMPONENT_SWIZZLE_IDENTITY */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    ImageView(std::move(resource), 0, VK_REMAINING_MIP_LEVELS, 0, VK_REMAINING_ARRAY_LAYERS, swizzle,
        std::move(allocator))
{}

ImageView::ImageView(std::shared_ptr<Image> resource,
    uint32_t baseMipLevel,
    uint32_t levelCount /* VK_REMAINING_MIP_LEVELS */,
    uint32_t baseArrayLayer /* 0 */,
    uint32_t layerCount /* VK_REMAINING_ARRAY_LAYERS */,
    const VkComponentMapping& swizzle /* VK_COMPONENT_SWIZZLE_IDENTITY */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    NonDispatchable(VK_OBJECT_TYPE_IMAGE_VIEW, std::move(resource->getDevice()), std::move(allocator)),
    image(std::move(resource)),
    baseMipLevel(baseMipLevel),
    levelCount(levelCount),
    baseArrayLayer(baseArrayLayer),
    layerCount(layerCount)
{
    VkImageViewCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.image = *image;
    switch (image->getType())
    {
    case VK_IMAGE_TYPE_1D:
        if (image->getArrayLayers() == 1)
            info.viewType = VK_IMAGE_VIEW_TYPE_1D;
        else
            info.viewType = VK_IMAGE_VIEW_TYPE_1D_ARRAY;
        break;
    case VK_IMAGE_TYPE_2D:
        if (image->flags & VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT)
            info.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
        else
        {
            if (image->getArrayLayers() == 1)
                info.viewType = VK_IMAGE_VIEW_TYPE_2D;
            else
                info.viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
        }
        break;
    case VK_IMAGE_TYPE_3D:
        info.viewType = VK_IMAGE_VIEW_TYPE_3D;
        break;
    default:
        throw std::runtime_error("invalid image type");
    }
    info.format = image->getFormat();
    const Format format(info.format);
    if (!(format.depth() || format.stencil() || format.depthStencil()))
        info.components = swizzle;
    else
        info.components = {VK_COMPONENT_SWIZZLE_IDENTITY,
                           VK_COMPONENT_SWIZZLE_IDENTITY,
                           VK_COMPONENT_SWIZZLE_IDENTITY,
                           VK_COMPONENT_SWIZZLE_IDENTITY};
    if (format.depth())
        info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    else if (format.stencil())
        info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_STENCIL_BIT;
    else if (format.depthStencil())
        info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
    else
        info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    info.subresourceRange.baseMipLevel = baseMipLevel;
    info.subresourceRange.levelCount = levelCount;
    info.subresourceRange.baseArrayLayer = baseArrayLayer;
    info.subresourceRange.layerCount = layerCount;
    const VkResult create = vkCreateImageView(MAGMA_HANDLE(device), &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create image view");
}

ImageView::~ImageView()
{
    vkDestroyImageView(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(allocator));
}
} // namespace magma
