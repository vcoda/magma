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
#include "descriptorSetBinding.h"
#include "../objects/sampler.h"
#include "../objects/imageView.h"
#include "../objects/buffer.h"

namespace magma
{
namespace reflection
{
DescriptorSetLayoutBinding::DescriptorSetLayoutBinding(VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t binding):
    VkDescriptorSetLayoutBinding{binding, descriptorType, descriptorCount, 0, nullptr},
    updated(false)
{}

DescriptorSetLayoutBinding::~DescriptorSetLayoutBinding()
{}

Sampler& Sampler::operator=(std::shared_ptr<const magma::Sampler> sampler)
{
    imageDescriptor.sampler = *sampler;
    imageDescriptor.imageView = VK_NULL_HANDLE;
    imageDescriptor.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.pNext = nullptr;
    descriptorWrite.dstSet = VK_NULL_HANDLE;
    descriptorWrite.dstBinding = binding;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorCount = descriptorCount;
    descriptorWrite.descriptorType = descriptorType;
    descriptorWrite.pImageInfo = &imageDescriptor;
    descriptorWrite.pBufferInfo = nullptr;
    descriptorWrite.pTexelBufferView = nullptr;
    updated = true;
    return *this;
}

CombinedImageSampler& CombinedImageSampler::operator=(const std::pair<std::shared_ptr<const ImageView>, std::shared_ptr<const magma::Sampler>>& combinedImageSampler)
{
    imageDescriptor = combinedImageSampler.first->getDescriptor(combinedImageSampler.second);
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.pNext = nullptr;
    descriptorWrite.dstSet = VK_NULL_HANDLE;
    descriptorWrite.dstBinding = binding;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorCount = descriptorCount;
    descriptorWrite.descriptorType = descriptorType;
    descriptorWrite.pImageInfo = &imageDescriptor;
    descriptorWrite.pBufferInfo = nullptr;
    descriptorWrite.pTexelBufferView = nullptr;
    updated = true;
    return *this;
}

UniformBuffer& UniformBuffer::operator=(std::shared_ptr<const Buffer> buffer)
{
    bufferDescriptor.buffer = *buffer;
    bufferDescriptor.offset = 0;
    bufferDescriptor.range = VK_WHOLE_SIZE;
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.pNext = nullptr;
    descriptorWrite.dstSet = VK_NULL_HANDLE;
    descriptorWrite.dstBinding = binding;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorCount = descriptorCount;
    descriptorWrite.descriptorType = descriptorType;
    descriptorWrite.pImageInfo = nullptr;
    descriptorWrite.pBufferInfo = &bufferDescriptor;
    descriptorWrite.pTexelBufferView = nullptr;
    updated = true;
    return *this;
}
} // namespace reflection
} // namespace magma
