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
#include "imageDescriptorSet.h"
#include "../objects/image.h"
#include "../objects/imageView.h"
#include "../objects/descriptorPool.h"
#include "../objects/descriptorSet.h"
#include "../shaders/shaderReflection.h"
#include "../descriptors/imageDescriptor.h"
#include "../descriptors/descriptorSetTable.h"
#include "../exceptions/exception.h"

namespace magma::aux
{
struct ImageDescriptorSet::ImageTable : DescriptorSetTable
{
    descriptor::CombinedImageSampler image = 0;
    MAGMA_REFLECT(image)
};

struct ImageDescriptorSet::StorageImageTable : DescriptorSetTable
{
    descriptor::StorageImage image = 0;
    MAGMA_REFLECT(image)
};

ImageDescriptorSet::ImageDescriptorSet(std::shared_ptr<Device> device,
    const std::unique_ptr<const ShaderReflection>& reflection,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    imageTable(std::make_unique<ImageTable>()),
    storageImageTable(std::make_unique<StorageImageTable>()),
    binding(0)
{
    const char *entrypoint = reflection->getEntryPointName(0);
    for (auto const& binding: reflection->enumerateDescriptorBindings(entrypoint))
    {
        if (SPV_REFLECT_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER == binding->descriptor_type)
        {
            descriptorPool = std::make_shared<DescriptorPool>(device, 1, descriptor::CombinedImageSamplerPool(1), allocator, false);
            descriptorSet = std::make_shared<DescriptorSet>(descriptorPool, *imageTable, VK_SHADER_STAGE_FRAGMENT_BIT, std::move(allocator));
        } else if (SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_IMAGE == binding->descriptor_type)
        {
            descriptorPool = std::make_shared<DescriptorPool>(device, 1, descriptor::StorageImagePool(1), allocator, false);
            descriptorSet = std::make_shared<DescriptorSet>(descriptorPool, *storageImageTable, VK_SHADER_STAGE_FRAGMENT_BIT, std::move(allocator));
        }
        if (descriptorSet)
        {
            this->binding = binding->binding;
            break;
        }
    }
    if (!descriptorSet)
        MAGMA_ERROR("image binding not found");
}

ImageDescriptorSet::~ImageDescriptorSet()
{}

void ImageDescriptorSet::writeDescriptor(std::shared_ptr<const ImageView> imageView, std::shared_ptr<Sampler> sampler)
{
    if (imageView->getImage()->getUsage() & VK_IMAGE_USAGE_STORAGE_BIT)
        storageImageTable->image = imageView;
    else
        imageTable->image = {imageView, sampler};
}
} // namespace magma::aux
