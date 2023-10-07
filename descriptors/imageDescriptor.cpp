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
#include "imageDescriptor.h"
#include "../objects/image.h"
#include "../objects/sampler.h"
#include "../misc/format.h"

namespace magma
{
namespace descriptor
{
void ImageDescriptor::write(VkDescriptorSet dstSet, VkWriteDescriptorSet& writeDescriptorSet) const noexcept
{
    MAGMA_ASSERT(associatedWithResource());
    writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet.pNext = nullptr;
    writeDescriptorSet.dstSet = dstSet;
    writeDescriptorSet.dstBinding = binding.binding;
    writeDescriptorSet.dstArrayElement = 0;
    writeDescriptorSet.descriptorCount = binding.descriptorCount;
    writeDescriptorSet.descriptorType = binding.descriptorType;
    writeDescriptorSet.pImageInfo = &descriptor;
    writeDescriptorSet.pBufferInfo = nullptr;
    writeDescriptorSet.pTexelBufferView = nullptr;
    updated = false;
}

void ImageDescriptor::updateImageView(std::shared_ptr<const ImageView> imageView, std::shared_ptr<const magma::Sampler> sampler,
    VkImageUsageFlags requiredUsage) noexcept
{
    MAGMA_UNUSED(requiredUsage);
    MAGMA_ASSERT(imageView);
    const std::shared_ptr<Image>& image = imageView->getImage();
    MAGMA_ASSERT(image->getUsage() & requiredUsage);
    if ((descriptor.imageView != *imageView) ||
        (sampler && descriptor.sampler != *sampler))
    {
        setImageType(image->getType());
        descriptor = imageView->getDescriptor(std::move(sampler)); // May be null
        if (VK_IMAGE_LAYOUT_UNDEFINED == descriptor.imageLayout)
        {   // Assume that later image layout will be changed,
            // e.g. when a render pass instance ends.
            const Format format(image->getFormat());
            if (format.depth() || format.stencil() || format.depthStencil())
                descriptor.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
            else
                descriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        }
        updated = true;
    }
}
} // namespace descriptor
} // namespace magma
