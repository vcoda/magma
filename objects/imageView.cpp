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
#include "../misc/format.h"
#include "../shared.h"

namespace magma
{
ImageView::ImageView(std::shared_ptr<const Device> device, 
    std::shared_ptr<const Image> image,
	uint32_t mipLevelCount, /* 0 */
	VkComponentMapping swizzle /* R, G, B, A */):
    NonDispatchable(VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_VIEW_EXT, device),
    image(image)
{
	VkImageViewCreateInfo info;
	info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.image = *image;
    switch (image->getType())
    {
    case VK_IMAGE_TYPE_1D: info.viewType = VK_IMAGE_VIEW_TYPE_1D; break;
    case VK_IMAGE_TYPE_2D: info.viewType = VK_IMAGE_VIEW_TYPE_2D; break;
    case VK_IMAGE_TYPE_3D: info.viewType = VK_IMAGE_VIEW_TYPE_3D; break;
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
        info.subresourceRange.levelCount = image->getMipLevels();
    else
        info.subresourceRange.levelCount = mipLevelCount;
	info.subresourceRange.baseArrayLayer = 0;
	info.subresourceRange.layerCount = 1;
	const VkResult create = vkCreateImageView(*device, &info, nullptr, &handle);
    MAGMA_THROW_FAILURE(create, "failed to create image view");
}

ImageView::~ImageView()
{
    vkDestroyImageView(*device, handle, nullptr);
}
} // namespace magma
