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
#include "descriptorSet.h"
#include "descriptorSetLayout.h"
#include "descriptorPool.h"
#include "device.h"
#include "buffer.h"
#include "bufferView.h"
#include "image.h"
#include "imageView.h"
#include "sampler.h"
#include "accelerationStructure.h"
#include "../exceptions/errorResult.h"

namespace magma
{
DescriptorSet::DescriptorSet(std::shared_ptr<DescriptorPool> descriptorPool,
    uint32_t setIndex, std::shared_ptr<DescriptorSetLayout> setLayout,
    uint32_t maxDescriptorWrites /* 16 */):
    BaseDescriptorSet(std::move(descriptorPool), setIndex, std::move(setLayout))
{
    const VkDescriptorSetLayout dereferencedSetLayouts[1] = {*setLayout};
    // Allocate descriptor set
    VkDescriptorSetAllocateInfo allocInfo;
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.pNext = nullptr;
    allocInfo.descriptorPool = *this->descriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = dereferencedSetLayouts;
    const VkResult result = vkAllocateDescriptorSets(MAGMA_HANDLE(device), &allocInfo, &handle);
    MAGMA_THROW_FAILURE(result, "failed to allocate descriptor set");
    // Make sure that pointer adresses will be preserved after push_back()
    bufferDescriptors.reserve(maxDescriptorWrites);
    imageDescriptors.reserve(maxDescriptorWrites);
    samplerDescriptors.reserve(maxDescriptorWrites);
    bufferViews.reserve(maxDescriptorWrites);
#ifdef VK_EXT_inline_uniform_block
    inlineUniformBlockDescriptors.reserve(maxDescriptorWrites);
    inlineUniformBlockAllocator = std::make_unique<core::LinearAllocator>(MAGMA_MAX_INLINE_UNIFORM_BLOCK_POOL_SIZE);
#endif
#ifdef VK_NV_ray_tracing
    accelerationDescriptors.reserve(maxDescriptorWrites);
    accelerationStructures.reserve(maxDescriptorWrites);
#endif
    descriptorWrites.reserve(maxDescriptorWrites);
    this->handle = handle;
}

DescriptorSet::~DescriptorSet()
{
    vkFreeDescriptorSets(MAGMA_HANDLE(device), *descriptorPool, 1, &handle);
    release(); // Take care if update() has not been called
}

void DescriptorSet::writeDescriptor(uint32_t index, std::shared_ptr<const Buffer> buffer)
{
    const DescriptorSetLayout::Binding& binding = setLayout->getBinding(index);
    MAGMA_ASSERT((VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER == binding.descriptorType) ||
                 (VK_DESCRIPTOR_TYPE_STORAGE_BUFFER == binding.descriptorType) ||
                 (VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC == binding.descriptorType) ||
                 (VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC == binding.descriptorType));
    MAGMA_ASSERT(1 == binding.descriptorCount);
    MAGMA_ASSERT(bufferDescriptors.capacity() - bufferDescriptors.size() >= 1);
    bufferDescriptors.push_back(buffer->getDescriptor());
    VkWriteDescriptorSet descriptorWrite;
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.pNext = nullptr;
    descriptorWrite.dstSet = handle;
    descriptorWrite.dstBinding = binding.binding;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorCount = binding.descriptorCount;
    descriptorWrite.descriptorType = binding.descriptorType;
    descriptorWrite.pImageInfo = nullptr;
    descriptorWrite.pBufferInfo = &bufferDescriptors.back();
    descriptorWrite.pTexelBufferView = nullptr;
    descriptorWrites.push_back(descriptorWrite);
}

void DescriptorSet::writeDescriptor(uint32_t index, std::shared_ptr<const ImageView> imageView)
{
    const DescriptorSetLayout::Binding& binding = setLayout->getBinding(index);
    MAGMA_ASSERT((VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE == binding.descriptorType) ||
                 (VK_DESCRIPTOR_TYPE_STORAGE_IMAGE == binding.descriptorType) ||
                 (VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT == binding.descriptorType));
    MAGMA_ASSERT(1 == binding.descriptorCount);
    MAGMA_ASSERT(imageDescriptors.capacity() - imageDescriptors.size() >= 1);
    imageDescriptors.push_back(imageView->getDescriptor(nullptr));
    VkWriteDescriptorSet descriptorWrite;
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.pNext = nullptr;
    descriptorWrite.dstSet = handle;
    descriptorWrite.dstBinding = binding.binding;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorCount = binding.descriptorCount;
    descriptorWrite.descriptorType = binding.descriptorType;
    descriptorWrite.pImageInfo = &imageDescriptors.back();
    descriptorWrite.pBufferInfo = nullptr;
    descriptorWrite.pTexelBufferView = nullptr;
    descriptorWrites.push_back(descriptorWrite);
}

void DescriptorSet::writeDescriptor(uint32_t index, std::shared_ptr<const Sampler> sampler)
{
    const DescriptorSetLayout::Binding& binding = setLayout->getBinding(index);
    MAGMA_ASSERT(VK_DESCRIPTOR_TYPE_SAMPLER == binding.descriptorType);
    MAGMA_ASSERT(1 == binding.descriptorCount);
    MAGMA_ASSERT(sampler);
    MAGMA_ASSERT(samplerDescriptors.capacity() - samplerDescriptors.size() >= 1);
    VkDescriptorImageInfo descriptor;
    descriptor.sampler = *sampler;
    descriptor.imageView = VK_NULL_HANDLE;
    descriptor.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    samplerDescriptors.push_back(descriptor);
    VkWriteDescriptorSet descriptorWrite;
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.pNext = nullptr;
    descriptorWrite.dstSet = handle;
    descriptorWrite.dstBinding = binding.binding;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorCount = binding.descriptorCount;
    descriptorWrite.descriptorType = binding.descriptorType;
    descriptorWrite.pImageInfo = &samplerDescriptors.back();
    descriptorWrite.pBufferInfo = nullptr;
    descriptorWrite.pTexelBufferView = nullptr;
    descriptorWrites.push_back(descriptorWrite);
}

void DescriptorSet::writeDescriptor(uint32_t index, std::shared_ptr<const ImageView> imageView, std::shared_ptr<const Sampler> sampler)
{
    const DescriptorSetLayout::Binding& binding = setLayout->getBinding(index);
    MAGMA_ASSERT(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER == binding.descriptorType);
    MAGMA_ASSERT(1 == binding.descriptorCount);
    MAGMA_ASSERT(imageView->getImage()->getUsage() & VK_IMAGE_USAGE_SAMPLED_BIT);
    MAGMA_ASSERT(sampler);
    MAGMA_ASSERT(imageDescriptors.capacity() - imageDescriptors.size() >= 1);
    imageDescriptors.push_back(imageView->getDescriptor(std::move(sampler)));
    VkWriteDescriptorSet descriptorWrite;
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.pNext = nullptr;
    descriptorWrite.dstSet = handle;
    descriptorWrite.dstBinding = binding.binding;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorCount = binding.descriptorCount;
    descriptorWrite.descriptorType = binding.descriptorType;
    descriptorWrite.pImageInfo = &imageDescriptors.back();
    descriptorWrite.pBufferInfo = nullptr;
    descriptorWrite.pTexelBufferView = nullptr;
    descriptorWrites.push_back(descriptorWrite);
}

void DescriptorSet::writeDescriptor(uint32_t index, std::shared_ptr<const BufferView> bufferView)
{
    const DescriptorSetLayout::Binding& binding = setLayout->getBinding(index);
    MAGMA_ASSERT((VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER == binding.descriptorType) ||
                 (VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER == binding.descriptorType));
    MAGMA_ASSERT(1 == binding.descriptorCount);
    MAGMA_ASSERT(bufferViews.capacity() - bufferViews.size() >= 1);
    bufferViews.push_back(*bufferView);
    VkWriteDescriptorSet descriptorWrite;
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.pNext = nullptr;
    descriptorWrite.dstSet = handle;
    descriptorWrite.dstBinding = binding.binding;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorCount = binding.descriptorCount;
    descriptorWrite.descriptorType = binding.descriptorType;
    descriptorWrite.pImageInfo = nullptr;
    descriptorWrite.pBufferInfo = nullptr;
    descriptorWrite.pTexelBufferView = &bufferViews.back();
    descriptorWrites.push_back(descriptorWrite);
}

#ifdef VK_NV_ray_tracing
void DescriptorSet::writeDescriptor(uint32_t index, std::shared_ptr<const AccelerationStructure> accelerationStructure)
{
    const DescriptorSetLayout::Binding& binding = setLayout->getBinding(index);
    MAGMA_ASSERT(VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_NV == binding.descriptorType);
    MAGMA_ASSERT(accelerationStructures.capacity() - accelerationStructures.size() >= 1);
    MAGMA_ASSERT(1 == binding.descriptorCount);
    accelerationStructures.push_back(*accelerationStructure);
    VkWriteDescriptorSetAccelerationStructureNV descriptor;
    descriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_NV;
    descriptor.pNext = nullptr;
    descriptor.accelerationStructureCount = binding.descriptorCount;
    descriptor.pAccelerationStructures = &accelerationStructures.back();
    accelerationDescriptors.push_back(descriptor);
    VkWriteDescriptorSet descriptorWrite;
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.pNext = &accelerationDescriptors.back();
    descriptorWrite.dstSet = handle;
    descriptorWrite.dstBinding = binding.binding;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorCount = descriptor.accelerationStructureCount;
    descriptorWrite.descriptorType = binding.descriptorType;
    descriptorWrite.pImageInfo = nullptr;
    descriptorWrite.pBufferInfo = nullptr;
    descriptorWrite.pTexelBufferView = nullptr;
    descriptorWrites.push_back(descriptorWrite);
}
#endif // VK_NV_ray_tracing

void DescriptorSet::writeDescriptorArray(uint32_t index, const std::vector<std::shared_ptr<const Buffer>>& bufferArray)
{
    const DescriptorSetLayout::Binding& binding = setLayout->getBinding(index);
    MAGMA_ASSERT((VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER == binding.descriptorType) ||
                 (VK_DESCRIPTOR_TYPE_STORAGE_BUFFER == binding.descriptorType) ||
                 (VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC == binding.descriptorType) ||
                 (VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC == binding.descriptorType));
    MAGMA_ASSERT(binding.descriptorCount > 1);
    MAGMA_ASSERT(binding.descriptorCount <= bufferArray.size());
    VkDescriptorBufferInfo *bufferInfo = new VkDescriptorBufferInfo[bufferArray.size()]; // Deleted in release()
    for (size_t i = 0, n = bufferArray.size(); i < n; ++i)
        bufferInfo[i] = bufferArray[i]->getDescriptor();
    VkWriteDescriptorSet descriptorWrite;
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.pNext = nullptr;
    descriptorWrite.dstSet = handle;
    descriptorWrite.dstBinding = binding.binding;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorCount = binding.descriptorCount;
    descriptorWrite.descriptorType = binding.descriptorType;
    descriptorWrite.pImageInfo = nullptr;
    descriptorWrite.pBufferInfo = bufferInfo;
    descriptorWrite.pTexelBufferView = nullptr;
    descriptorWrites.push_back(descriptorWrite);
}

void DescriptorSet::writeDescriptorArray(uint32_t index, const std::vector<std::shared_ptr<const ImageView>>& imageViewArray)
{
    const DescriptorSetLayout::Binding& binding = setLayout->getBinding(index);
    MAGMA_ASSERT((VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE == binding.descriptorType) ||
                 (VK_DESCRIPTOR_TYPE_STORAGE_IMAGE == binding.descriptorType));
    MAGMA_ASSERT(binding.descriptorCount > 1);
    MAGMA_ASSERT(binding.descriptorCount <= imageViewArray.size());
    VkDescriptorImageInfo *imageInfo = new VkDescriptorImageInfo[imageViewArray.size()]; // Deleted in release()
    for (size_t i = 0, n = imageViewArray.size(); i < n; ++i)
    {
        MAGMA_ASSERT(imageViewArray[i]->getImage()->getUsage() & VK_IMAGE_USAGE_SAMPLED_BIT);
        imageInfo[i] = imageViewArray[i]->getDescriptor(nullptr);
    }
    VkWriteDescriptorSet descriptorWrite;
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.pNext = nullptr;
    descriptorWrite.dstSet = handle;
    descriptorWrite.dstBinding = binding.binding;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorCount = binding.descriptorCount;
    descriptorWrite.descriptorType = binding.descriptorType;
    descriptorWrite.pImageInfo = imageInfo;
    descriptorWrite.pBufferInfo = nullptr;
    descriptorWrite.pTexelBufferView = nullptr;
    descriptorWrites.push_back(descriptorWrite);
}

void DescriptorSet::writeDescriptorArray(uint32_t index, const std::vector<std::shared_ptr<const ImageView>>& imageViewArray,
    const std::vector<std::shared_ptr<const Sampler>>& samplerArray)
{
    const DescriptorSetLayout::Binding& binding = setLayout->getBinding(index);
    MAGMA_ASSERT(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER == binding.descriptorType);
    MAGMA_ASSERT(binding.descriptorCount > 1);
    MAGMA_ASSERT(binding.descriptorCount <= imageViewArray.size());
    MAGMA_ASSERT(imageViewArray.size() <= samplerArray.size());
    VkDescriptorImageInfo *imageInfo = new VkDescriptorImageInfo[imageViewArray.size()]; // Deleted in release()
    for (size_t i = 0, n = imageViewArray.size(); i < n; ++i)
    {
        MAGMA_ASSERT(imageViewArray[i]->getImage()->getUsage() & (samplerArray[i] ? VK_IMAGE_USAGE_SAMPLED_BIT : VK_IMAGE_USAGE_STORAGE_BIT));
        imageInfo[i] = imageViewArray[i]->getDescriptor(samplerArray[i]);
    }
    VkWriteDescriptorSet descriptorWrite;
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.pNext = nullptr;
    descriptorWrite.dstSet = handle;
    descriptorWrite.dstBinding = binding.binding;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorCount = binding.descriptorCount;
    descriptorWrite.descriptorType = binding.descriptorType;
    descriptorWrite.pImageInfo = imageInfo;
    descriptorWrite.pBufferInfo = nullptr;
    descriptorWrite.pTexelBufferView = nullptr;
    descriptorWrites.push_back(descriptorWrite);
}

void DescriptorSet::writeDescriptorArray(uint32_t index, const std::vector<std::shared_ptr<const BufferView>>& bufferViewArray)
{
    const DescriptorSetLayout::Binding& binding = setLayout->getBinding(index);
    MAGMA_ASSERT((VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER == binding.descriptorType) ||
                 (VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER == binding.descriptorType));
    MAGMA_ASSERT(binding.descriptorCount > 1);
    MAGMA_ASSERT(binding.descriptorCount <= bufferViewArray.size());
    VkBufferView *texelBufferView = new VkBufferView[bufferViewArray.size()]; // Deleted in release()
    for (size_t i = 0, n = bufferViewArray.size(); i < n; ++i)
        texelBufferView[i] = *bufferViewArray[i];
    VkWriteDescriptorSet descriptorWrite;
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.pNext = nullptr;
    descriptorWrite.dstSet = handle;
    descriptorWrite.dstBinding = binding.binding;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorCount = binding.descriptorCount;
    descriptorWrite.descriptorType = binding.descriptorType;
    descriptorWrite.pImageInfo = nullptr;
    descriptorWrite.pBufferInfo = nullptr;
    descriptorWrite.pTexelBufferView = texelBufferView;
    descriptorWrites.push_back(descriptorWrite);
}

bool DescriptorSet::dirty() const noexcept
{
    return !descriptorWrites.empty();
}

void DescriptorSet::update()
{
    device->updateDescriptorSets(descriptorWrites);
    release();
}

void DescriptorSet::release()
{   // This shouldn't free actual memory (otherwise reserve() should be called again)
    bufferDescriptors.clear();
    imageDescriptors.clear();
    samplerDescriptors.clear();
    bufferViews.clear();
#ifdef VK_EXT_inline_uniform_block
    inlineUniformBlockDescriptors.clear();
    inlineUniformBlockAllocator->reset();
#endif
#ifdef VK_NV_ray_tracing
    accelerationDescriptors.clear();
    accelerationStructures.clear();
#endif  
    for (auto& descriptor : descriptorWrites)
    {
        if (descriptor.descriptorCount > 1) // Is array?
        {   // Free memory allocated in writeDescriptorArray()
            delete[] descriptor.pImageInfo;
            delete[] descriptor.pBufferInfo;
            delete[] descriptor.pTexelBufferView;
        }
    }
    descriptorWrites.clear();
    descriptorWrites.shrink_to_fit(); // Free memory
}
} // namespace magma
