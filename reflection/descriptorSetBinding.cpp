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
#include "../objects/image.h"
#include "../objects/imageView.h"
#include "../objects/buffer.h"
#include "../objects/bufferView.h"
#include "../objects/uniformTexelBuffer.h"
#include "../objects/storageTexelBuffer.h"
#include "../objects/storageBuffer.h"
#include "../objects/accelerationStructure.h"

namespace magma
{
namespace reflection
{
DescriptorSetLayoutBinding::DescriptorSetLayoutBinding(VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t binding):
    VkDescriptorSetLayoutBinding{binding, descriptorType, descriptorCount, 0, nullptr},
    descriptorWrite{},
    _dirty(false)
{}

void ImageDescriptorBinding::writeDescriptor(const VkDescriptorImageInfo& info) noexcept
{
    imageDescriptor = info;
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
    _dirty = true;
}

void BufferDescriptorBinding::writeDescriptor(std::shared_ptr<const Buffer> buffer) noexcept
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
    _dirty = true;
}

void TexelBufferDescriptorBinding::writeDescriptor(std::shared_ptr<const BufferView> bufferView) noexcept
{
    texelBufferView = *bufferView;
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.pNext = nullptr;
    descriptorWrite.dstSet = VK_NULL_HANDLE;
    descriptorWrite.dstBinding = binding;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorCount = descriptorCount;
    descriptorWrite.descriptorType = descriptorType;
    descriptorWrite.pImageInfo = nullptr;
    descriptorWrite.pBufferInfo = nullptr;
    descriptorWrite.pTexelBufferView = &texelBufferView;
    _dirty = true;
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

CombinedImageSampler& CombinedImageSampler::operator=(const std::pair<std::shared_ptr<const ImageView>, std::shared_ptr<const magma::Sampler>>& combinedImageSampler) noexcept
{
    writeDescriptor(combinedImageSampler.first->getDescriptor(combinedImageSampler.second));
    return *this;
}

SampledImage& SampledImage::operator=(std::shared_ptr<const ImageView> imageView) noexcept
{
    writeDescriptor(imageView->getDescriptor(nullptr));
    return *this;
}

StorageImage& StorageImage::operator=(std::shared_ptr<const ImageView> imageView) noexcept
{
    MAGMA_ASSERT(imageView->getImage()->storageImage());
    writeDescriptor(imageView->getDescriptor(nullptr));
    return *this;
}

UniformTexelBuffer& UniformTexelBuffer::operator=(std::shared_ptr<const BufferView> bufferView) noexcept
{
    MAGMA_ASSERT(bufferView->getBuffer()->getUsage() & VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT);
    writeDescriptor(std::move(bufferView));
    return *this;
}

StorageTexelBuffer& StorageTexelBuffer::operator=(std::shared_ptr<const BufferView> bufferView) noexcept
{
    MAGMA_ASSERT(bufferView->getBuffer()->getUsage() & VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT);
    writeDescriptor(std::move(bufferView));
    return *this;
}

UniformBuffer& UniformBuffer::operator=(std::shared_ptr<const Buffer> buffer) noexcept
{
    MAGMA_ASSERT(buffer->getUsage() & VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
    writeDescriptor(std::move(buffer));
    return *this;
}

StorageBuffer& StorageBuffer::operator=(std::shared_ptr<const magma::StorageBuffer> buffer) noexcept
{
    writeDescriptor(std::move(buffer));
    return *this;
}

DynamicUniformBuffer& DynamicUniformBuffer::operator=(std::shared_ptr<const Buffer> buffer) noexcept
{
    MAGMA_ASSERT(buffer->getUsage() & VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
    writeDescriptor(std::move(buffer));
    return *this;
}

DynamicStorageBuffer& DynamicStorageBuffer::operator=(std::shared_ptr<const magma::StorageBuffer> buffer) noexcept
{
    writeDescriptor(std::move(buffer));
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
    _dirty = true;
    return *this;
}
#endif // VK_NV_ray_tracing
} // namespace reflection
} // namespace magma
