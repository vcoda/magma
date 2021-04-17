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
#include "device.h"
#include "buffer.h"
#include "bufferView.h"
#include "image.h"
#include "imageView.h"
#include "accelerationStructure.h"

namespace magma
{
DescriptorSet::DescriptorSet(VkDescriptorSet handle,
    std::shared_ptr<Device> device,
    std::shared_ptr<DescriptorPool> pool,
    std::shared_ptr<DescriptorSetLayout> layout,
    const std::size_t maxDescriptors /* 16 */):
    NonDispatchable(VK_OBJECT_TYPE_DESCRIPTOR_SET, std::move(device), nullptr),
    pool(std::move(pool)),
    layout(std::move(layout))
{   // Make sure that pointer adresses will be preserved after push_back()
    bufferDescriptors.reserve(maxDescriptors);
    imageDescriptors.reserve(maxDescriptors);
    bufferViews.reserve(maxDescriptors);
#ifdef VK_NV_ray_tracing
    accelerationDescriptors.reserve(maxDescriptors);
    accelerationStructures.reserve(maxDescriptors);
#endif
    this->handle = handle;
}

DescriptorSet::~DescriptorSet()
{
    release(); // Take care if update() has not been called
}

void DescriptorSet::writeDescriptor(uint32_t index, std::shared_ptr<const Buffer> buffer)
{
    MAGMA_ASSERT(bufferDescriptors.capacity() - bufferDescriptors.size() >= 1);
    const DescriptorSetLayout::Binding& binding = layout->getBinding(index);
    MAGMA_ASSERT(1 == binding.descriptorCount);
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

void DescriptorSet::writeDescriptor(uint32_t index, std::shared_ptr<const ImageView> imageView, std::shared_ptr<const Sampler> sampler)
{
    MAGMA_ASSERT(imageDescriptors.capacity() - imageDescriptors.size() >= 1);
    MAGMA_ASSERT(imageView->getImage()->getUsage() & (sampler ? VK_IMAGE_USAGE_SAMPLED_BIT : VK_IMAGE_USAGE_STORAGE_BIT));
    const DescriptorSetLayout::Binding& binding = layout->getBinding(index);
    MAGMA_ASSERT(1 == binding.descriptorCount);
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
    MAGMA_ASSERT(bufferViews.capacity() - bufferViews.size() >= 1);
    const DescriptorSetLayout::Binding& binding = layout->getBinding(index);
    MAGMA_ASSERT(1 == binding.descriptorCount);
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
    MAGMA_ASSERT(accelerationStructures.capacity() - accelerationStructures.size() >= 1);
    const DescriptorSetLayout::Binding& binding = layout->getBinding(index);
    MAGMA_ASSERT(1 == binding.descriptorCount);
    accelerationStructures.push_back(*accelerationStructure);
    VkWriteDescriptorSetAccelerationStructureNV descriptor;
    descriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_NV;
    descriptor.pNext = nullptr;
    descriptor.accelerationStructureCount = 1;
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
    const DescriptorSetLayout::Binding& binding = layout->getBinding(index);
    MAGMA_ASSERT(binding.descriptorCount > 1);
    MAGMA_ASSERT(binding.descriptorCount <= bufferArray.size());
    VkDescriptorBufferInfo *bufferInfo = new VkDescriptorBufferInfo[bufferArray.size()];
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

void DescriptorSet::writeDescriptorArray(uint32_t index, const std::vector<std::shared_ptr<const ImageView>>& imageViewArray,
    const std::vector<std::shared_ptr<const Sampler>>& samplerArray)
{
    MAGMA_ASSERT(imageViewArray.size() <= samplerArray.size());
    const DescriptorSetLayout::Binding& binding = layout->getBinding(index);
    MAGMA_ASSERT(binding.descriptorCount > 1);
    MAGMA_ASSERT(binding.descriptorCount <= imageViewArray.size());
    VkDescriptorImageInfo *imageInfo = new VkDescriptorImageInfo[imageViewArray.size()];
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
    const DescriptorSetLayout::Binding& binding = layout->getBinding(index);
    MAGMA_ASSERT(binding.descriptorCount > 1);
    MAGMA_ASSERT(binding.descriptorCount <= bufferViewArray.size());
    VkBufferView *texelBufferView = new VkBufferView[bufferViewArray.size()];
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

void DescriptorSet::update()
{
    if (!descriptorWrites.empty())
    {
        device->updateDescriptorSets(descriptorWrites);
        release();
    }
}

void DescriptorSet::release()
{   // This shouldn't free actual memory (otherwise reserve() should be called again)
    bufferDescriptors.clear();
    imageDescriptors.clear();
    bufferViews.clear();
#ifdef VK_NV_ray_tracing
    accelerationDescriptors.clear();
    accelerationStructures.clear();
#endif  
    for (auto& descriptor : descriptorWrites)
    {
        if (descriptor.descriptorCount > 0)
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
