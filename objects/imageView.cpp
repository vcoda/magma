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
#include "imageView.h"
#include "image.h"
#include "device.h"
#include "../allocator/allocator.h"
#include "../misc/format.h"
#include "../misc/exception.h"
#include "../internal/shared.h"

namespace magma
{
ImageView::ImageView(std::shared_ptr<const Image> resource,
    uint32_t mipLevelCount, /* 0 */
    const VkComponentMapping& swizzle /* VK_COMPONENT_SWIZZLE_IDENTITY */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    NonDispatchable(VK_OBJECT_TYPE_IMAGE_VIEW, std::move(resource->getDevice()), std::move(allocator)),
    image(std::move(resource))
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
    if (!format.depth())
        info.components = swizzle;
    else
        info.components = {VK_COMPONENT_SWIZZLE_IDENTITY,
                           VK_COMPONENT_SWIZZLE_IDENTITY,
                           VK_COMPONENT_SWIZZLE_IDENTITY,
                           VK_COMPONENT_SWIZZLE_IDENTITY};
    if (format.depthStencil())
        info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
    else if (format.depth())
        info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    else
        info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    info.subresourceRange.baseMipLevel = 0;
    if (!mipLevelCount)
        info.subresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
    else
        info.subresourceRange.levelCount = mipLevelCount;
    info.subresourceRange.baseArrayLayer = 0;
    info.subresourceRange.layerCount = VK_REMAINING_ARRAY_LAYERS;
    const VkResult create = vkCreateImageView(MAGMA_HANDLE(device), &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create image view");
}

ImageView::~ImageView()
{
    vkDestroyImageView(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(allocator));
}
} // namespace magma
