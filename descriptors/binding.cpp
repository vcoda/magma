/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2022 Victor Coda.

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
#include "binding.h"
#include "../objects/sampler.h"
#include "../objects/accelerationStructure.h"

namespace magma
{
namespace binding
{
DescriptorSetLayoutBinding::DescriptorSetLayoutBinding(VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t binding) noexcept:
    VkDescriptorSetLayoutBinding{binding, descriptorType, descriptorCount, 0, nullptr},
    imageDescriptor{},
    descriptorWrite{},
    changed(false)
{
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.pNext = nullptr;
    descriptorWrite.dstSet = VK_NULL_HANDLE;
    descriptorWrite.dstBinding = binding;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorCount = descriptorCount;
    descriptorWrite.descriptorType = descriptorType;
    descriptorWrite.pImageInfo = nullptr;
    descriptorWrite.pBufferInfo = nullptr;
    descriptorWrite.pTexelBufferView = nullptr;
}

void DescriptorSetLayoutBinding::writeDescriptor(const VkDescriptorImageInfo& info) noexcept
{
    imageDescriptor = info;
    descriptorWrite.pImageInfo = &imageDescriptor;
    changed = true;
}

void DescriptorSetLayoutBinding::writeDescriptor(std::shared_ptr<const Buffer> buffer) noexcept
{
    bufferDescriptor = buffer->getDescriptor();
    descriptorWrite.pBufferInfo = &bufferDescriptor;
    changed = true;
}

void DescriptorSetLayoutBinding::writeDescriptor(std::shared_ptr<const BufferView> bufferView) noexcept
{
    texelBufferView = *bufferView;
    descriptorWrite.pTexelBufferView = &texelBufferView;
    changed = true;
}

Sampler& Sampler::operator=(std::shared_ptr<const magma::Sampler> sampler) noexcept
{
    VkDescriptorImageInfo imageDescriptor;
    imageDescriptor.sampler = *sampler;
    imageDescriptor.imageView = VK_NULL_HANDLE;
    imageDescriptor.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    writeDescriptor(imageDescriptor);
    return *this;
}

ImmutableSampler& ImmutableSampler::operator=(std::shared_ptr<const magma::Sampler> sampler) noexcept
{
    MAGMA_ASSERT(!pImmutableSamplers);
    if (!pImmutableSamplers)
    {
        VkDescriptorImageInfo imageDescriptor;
        imageDescriptor.sampler = VK_NULL_HANDLE;
        imageDescriptor.imageView = VK_NULL_HANDLE;
        imageDescriptor.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        writeDescriptor(imageDescriptor);
        // If pImmutableSamplers is not NULL, then it is a pointer to an array of sampler handles
        // that will be copied into the set layout and used for the corresponding binding.
        pImmutableSamplers = sampler->getImmutableSampler();
    }
    return *this;
}

CombinedImageImmutableSampler& CombinedImageImmutableSampler::operator=(const std::pair<std::shared_ptr<const ImageView>, std::shared_ptr<const magma::Sampler>>& combinedImageSampler) noexcept
{
    std::shared_ptr<const ImageView> imageView = combinedImageSampler.first;
    writeDescriptor(imageView->getDescriptor(nullptr));
    if (!pImmutableSamplers)
    {
        std::shared_ptr<const magma::Sampler> sampler = combinedImageSampler.second;
        // If pImmutableSamplers is not NULL, then it is a pointer to an array of sampler handles
        // that will be copied into the set layout and used for the corresponding binding.
        pImmutableSamplers = sampler->getImmutableSampler();
    }
    return *this;
}

#ifdef VK_NV_ray_tracing
AccelerationStructure& AccelerationStructure::operator=(std::shared_ptr<const magma::AccelerationStructure> accelerationStructure) noexcept
{
    this->accelerationStructure = *accelerationStructure;
    accelerationStructureDescriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_NV;
    accelerationStructureDescriptorWrite.pNext = nullptr;
    accelerationStructureDescriptorWrite.accelerationStructureCount = descriptorCount;
    accelerationStructureDescriptorWrite.pAccelerationStructures = &this->accelerationStructure;
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.pNext = &accelerationStructureDescriptorWrite;
    descriptorWrite.dstSet = VK_NULL_HANDLE;
    descriptorWrite.dstBinding = binding;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorCount = descriptorCount;
    descriptorWrite.descriptorType = descriptorType;
    descriptorWrite.pImageInfo = nullptr;
    descriptorWrite.pBufferInfo = nullptr;
    descriptorWrite.pTexelBufferView = nullptr;
    changed = true;
    return *this;
}
#endif // VK_NV_ray_tracing
} // namespace binding
} // namespace magma
