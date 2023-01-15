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
    updated(false)
{}

VkWriteDescriptorSet DescriptorSetLayoutBinding::getWriteDescriptorSet(VkDescriptorSet dstSet) const noexcept
{
	VkWriteDescriptorSet writeDescriptorSet;
    writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet.pNext = nullptr;
    writeDescriptorSet.dstSet = dstSet;
    writeDescriptorSet.dstBinding = binding;
    writeDescriptorSet.dstArrayElement = 0;
    writeDescriptorSet.descriptorCount = descriptorCount;
    writeDescriptorSet.descriptorType = descriptorType;
	switch (descriptorType)
	{
    case VK_DESCRIPTOR_TYPE_SAMPLER:
    case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
    case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
    case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
	case VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT:
		writeDescriptorSet.pImageInfo = &imageDescriptor;
        writeDescriptorSet.pBufferInfo = nullptr;
        writeDescriptorSet.pTexelBufferView = nullptr;
		break;
    case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
    case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:
		writeDescriptorSet.pImageInfo = nullptr;
        writeDescriptorSet.pBufferInfo = nullptr;
        writeDescriptorSet.pTexelBufferView = &texelBufferView;
		break;
    case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
    case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
    case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
    case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC:
        writeDescriptorSet.pImageInfo = nullptr;
        writeDescriptorSet.pBufferInfo = &bufferDescriptor;
        writeDescriptorSet.pTexelBufferView = nullptr;
		break;
	default:
		writeDescriptorSet.pImageInfo = nullptr;
        writeDescriptorSet.pBufferInfo = nullptr;
        writeDescriptorSet.pTexelBufferView = nullptr;
		MAGMA_ASSERT(false);
	}
	updated = false;
	return writeDescriptorSet;
}

#ifdef VK_NV_ray_tracing
inline VkWriteDescriptorSet AccelerationStructure::getWriteDescriptorSet(VkDescriptorSet dstSet) const noexcept
{
	VkWriteDescriptorSet writeDescriptorSet;
    writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet.pNext = &writeDescriptorSetAccelerationStructure;
    writeDescriptorSet.dstSet = dstSet;
    writeDescriptorSet.dstBinding = binding;
    writeDescriptorSet.dstArrayElement = 0;
    writeDescriptorSet.descriptorCount = descriptorCount;
    writeDescriptorSet.descriptorType = descriptorType;
	writeDescriptorSet.pImageInfo = nullptr;
	writeDescriptorSet.pBufferInfo = nullptr;
	writeDescriptorSet.pTexelBufferView = nullptr;
	updated = false;
	return writeDescriptorSet;
}

AccelerationStructure& AccelerationStructure::operator=(std::shared_ptr<const magma::AccelerationStructure> accelerationStructure_) noexcept
{
    accelerationStructure = *accelerationStructure_;
    writeDescriptorSetAccelerationStructure.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_NV;
    writeDescriptorSetAccelerationStructure.pNext = nullptr;
    writeDescriptorSetAccelerationStructure.accelerationStructureCount = 1;
    writeDescriptorSetAccelerationStructure.pAccelerationStructures = &accelerationStructure;
    updated = true;
    return *this;
}
#endif // VK_NV_ray_tracing
} // namespace binding
} // namespace magma
