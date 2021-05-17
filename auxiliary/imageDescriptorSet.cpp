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
#include "imageDescriptorSet.h"
#include "../objects/image.h"
#include "../objects/imageView.h"
#include "../objects/descriptorPool.h"
#include "../objects/descriptorSet.h"

namespace magma
{
namespace aux
{
ImageDescriptorSet::ImageDescriptorSet(std::shared_ptr<Device> device,
    std::shared_ptr<IAllocator> allocator /* nullptr */)
{
    const std::vector<Descriptor> descriptors{
        descriptors::CombinedImageSampler(1),
        descriptors::StorageImage(1)
    };
    descriptorPool = std::make_shared<DescriptorPool>(device, descriptorSets.size(), descriptors, allocator, false);
    for (size_t i = 0; i < descriptorSetLayouts.size(); ++i)
    {
        descriptorSetLayouts[i] = std::make_shared<DescriptorSetLayout>(device,
            bindings::FragmentStageBinding(0, descriptors[i]), allocator, 0);
        descriptorSets[i] = descriptorPool->allocateDescriptorSet(descriptorSetLayouts[i]);
    }
}

ImageDescriptorSet::~ImageDescriptorSet()
{
    for (uint32_t i = 0; i < descriptorSets.size(); ++i)
        descriptorPool->freeDescriptorSet(descriptorSets[i]);
}

void ImageDescriptorSet::writeDescriptor(std::shared_ptr<const ImageView> imageView, std::shared_ptr<Sampler> sampler)
{
    if (imageView->getImage()->storage())
        descriptorSets[ImageType::Storage]->writeDescriptor(0, imageView, nullptr);
    else
        descriptorSets[ImageType::Combined]->writeDescriptor(0, imageView, sampler);
}
} // namespace aux
} // namespace magma
