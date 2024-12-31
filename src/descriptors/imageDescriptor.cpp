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
#include "imageDescriptor.h"
#include "../objects/image.h"
#include "../objects/imageView.h"
#include "../misc/format.h"

namespace magma::descriptor
{
void ImageDescriptor::write(VkDescriptorSet dstSet, VkWriteDescriptorSet& writeDescriptorSet) const noexcept
{
    writeDescriptor(dstSet, writeDescriptorSet);
    writeDescriptorSet.pImageInfo = &descriptor;
    writeDescriptorSet.pBufferInfo = nullptr;
    writeDescriptorSet.pTexelBufferView = nullptr;
    dirty = false;
}

void ImageDescriptor::update(const ImageView *imageView, const magma::Sampler *sampler, VkImageUsageFlags usage) noexcept
{
    MAGMA_ASSERT(imageView);
    auto image = imageView->getImage();
    MAGMA_ASSERT(MAGMA_BITWISE_AND(image->getUsage(), usage));
    if ((descriptor.imageView != *imageView) ||
        (sampler && descriptor.sampler != *sampler))
    {
        setImageType(image->getType());
        descriptor = imageView->getDescriptor(sampler); // May be null
        if (VK_IMAGE_LAYOUT_UNDEFINED == descriptor.imageLayout)
        {   // Assume that later image layout will be changed,
            // e.g. when a render pass instance ends.
            if (usage & VK_IMAGE_USAGE_SAMPLED_BIT)
            {
                const Format format(image->getFormat());
                if (format.depth() || format.stencil() || format.depthStencil())
                    descriptor.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
                else
                    descriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            }
            else if (usage & VK_IMAGE_USAGE_STORAGE_BIT)
                descriptor.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
            else
                descriptor.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        }
        dirty = true;
    }
}
} // namespace magma::descriptor
