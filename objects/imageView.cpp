/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
ImageView::ImageView(std::shared_ptr<Image> resource,
    const VkComponentMapping& swizzle /* VK_COMPONENT_SWIZZLE_IDENTITY */):
    ImageView(std::move(resource), 0, VK_REMAINING_MIP_LEVELS, 0, VK_REMAINING_ARRAY_LAYERS, swizzle)
{}

ImageView::ImageView(std::shared_ptr<Image> resource,
    uint32_t baseMipLevel,
    uint32_t levelCount /* VK_REMAINING_MIP_LEVELS */,
    uint32_t baseArrayLayer /* 0 */,
    uint32_t layerCount /* VK_REMAINING_ARRAY_LAYERS */,
    const VkComponentMapping& swizzle /* VK_COMPONENT_SWIZZLE_IDENTITY */):
    NonDispatchable(VK_OBJECT_TYPE_IMAGE_VIEW, resource->getDevice(), resource->getHostAllocator()),
    image(std::move(resource)),
    baseMipLevel(baseMipLevel),
    levelCount(levelCount),
    baseArrayLayer(baseArrayLayer),
    layerCount(layerCount)
{
    VkImageViewCreateInfo viewInfo;
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.pNext = nullptr;
    viewInfo.flags = 0;
    viewInfo.image = *image;
    switch (image->getType())
    {
    case VK_IMAGE_TYPE_1D:
        if (image->getArrayLayers() == 1)
            viewInfo.viewType = VK_IMAGE_VIEW_TYPE_1D;
        else
            viewInfo.viewType = VK_IMAGE_VIEW_TYPE_1D_ARRAY;
        break;
    case VK_IMAGE_TYPE_2D:
        if (image->getFlags() & VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT)
            viewInfo.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
        else
        {
            if (image->getArrayLayers() == 1)
                viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            else
                viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
        }
        break;
    case VK_IMAGE_TYPE_3D:
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_3D;
        break;
    default:
        throw std::runtime_error("invalid image type");
    }
    viewInfo.format = image->getFormat();
    const Format format(viewInfo.format);
    if (!(format.depth() || format.stencil() || format.depthStencil()))
        viewInfo.components = swizzle;
    else
        viewInfo.components = {VK_COMPONENT_SWIZZLE_IDENTITY,
                               VK_COMPONENT_SWIZZLE_IDENTITY,
                               VK_COMPONENT_SWIZZLE_IDENTITY,
                               VK_COMPONENT_SWIZZLE_IDENTITY};
    if (format.depth())
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    else if (format.stencil())
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_STENCIL_BIT;
    else if (format.depthStencil())
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
    else
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.baseMipLevel = baseMipLevel;
    viewInfo.subresourceRange.levelCount = levelCount;
    viewInfo.subresourceRange.baseArrayLayer = baseArrayLayer;
    viewInfo.subresourceRange.layerCount = layerCount;
    const VkResult result = vkCreateImageView(MAGMA_HANDLE(device), &viewInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to create image view");
}

ImageView::~ImageView()
{
    vkDestroyImageView(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}

VkDescriptorImageInfo ImageView::getDescriptor(std::shared_ptr<const Sampler> sampler) const noexcept
{
    const Format format(image->getFormat());
    VkDescriptorImageInfo descriptor;
    descriptor.sampler = MAGMA_OPTIONAL_HANDLE(sampler); // VK_NULL_HANDLE for storage image
    descriptor.imageView = handle;
    descriptor.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    if (image->getUsage() & VK_IMAGE_USAGE_STORAGE_BIT)
        descriptor.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
    else if (image->getUsage() & VK_IMAGE_USAGE_SAMPLED_BIT)
    {
#ifdef VK_KHR_separate_depth_stencil_layouts
        if (device->separateDepthStencilLayoutsEnabled())
        {
            if (format.depth()) // Read-only image in a shader where only the depth aspect is accessed
                descriptor.imageLayout = VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL_KHR;
            else if (format.stencil()) // Read-only image in a shader where only the stencil aspect is accessed
                descriptor.imageLayout = VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL_KHR;
            else if (format.depthStencil()) // Read-only image in a shader where both depth and stencil is accessed
                descriptor.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
        }
        else
#endif // VK_KHR_separate_depth_stencil_layouts
        if (format.depth() || format.stencil() || format.depthStencil())
            descriptor.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
        else
            descriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL; // Read-only image in a shader
    }
    return descriptor;
}
} // namespace magma
