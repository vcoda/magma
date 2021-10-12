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
#include "../shaders/shaderReflection.h"
#include "../exceptions/exception.h"

namespace magma
{
namespace aux
{
struct ImageDescriptorSet::ImageSetLayout : DescriptorSetDeclaration
{
    binding::CombinedImageSampler image = 0;
    MAGMA_REFLECT(&image)
};

struct ImageDescriptorSet::StorageImageSetLayout : DescriptorSetDeclaration
{
    binding::StorageImage image = 0;
    MAGMA_REFLECT(&image)
};

ImageDescriptorSet::ImageDescriptorSet(std::shared_ptr<Device> device,
    std::shared_ptr<const ShaderReflection> reflection,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    imageSetLayout(std::make_unique<ImageSetLayout>()),
    storageImageSetLayout(std::make_unique<StorageImageSetLayout>()),
    binding(0)
{
    const char *entrypoint = reflection->getEntryPointName(0);
    for (const auto& binding : reflection->enumerateDescriptorBindings(entrypoint))
    {
        if (SPV_REFLECT_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER == binding->descriptor_type)
        {
            descriptorPool = std::make_shared<DescriptorPool>(device, 1, descriptor::CombinedImageSampler(1), allocator, false);
            descriptorSet = std::make_shared<DescriptorSet>(descriptorPool, *imageSetLayout, VK_SHADER_STAGE_FRAGMENT_BIT, std::move(allocator));
        } else if (SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_IMAGE == binding->descriptor_type)
        {
            descriptorPool = std::make_shared<DescriptorPool>(device, 1, descriptor::StorageImage(1), allocator, false);
            descriptorSet = std::make_shared<DescriptorSet>(descriptorPool, *storageImageSetLayout, VK_SHADER_STAGE_FRAGMENT_BIT, std::move(allocator));
        }
        if (descriptorSet)
        {
            this->binding = binding->binding;
            break;
        }
    }
    if (!descriptorSet)
        MAGMA_THROW("image binding not found");
}

void ImageDescriptorSet::writeDescriptor(std::shared_ptr<const ImageView> imageView, std::shared_ptr<Sampler> sampler)
{
    if (imageView->getImage()->storageImage())
        storageImageSetLayout->image = imageView;
    else
        imageSetLayout->image = {imageView, sampler};
}
} // namespace aux
} // namespace magma
