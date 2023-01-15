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
#include "bindingArray.h"

namespace magma
{
namespace binding
{
VkWriteDescriptorSet SamplerArray::getWriteDescriptorSet(VkDescriptorSet dstSet) const noexcept
{
	VkWriteDescriptorSet writeDescriptorSet;
    writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet.pNext = nullptr;
    writeDescriptorSet.dstSet = dstSet;
    writeDescriptorSet.dstBinding = binding;
    writeDescriptorSet.dstArrayElement = 0;
    writeDescriptorSet.descriptorCount = descriptorCount;
    writeDescriptorSet.descriptorType = descriptorType;
	writeDescriptorSet.pImageInfo = imageDescriptors.data();
    writeDescriptorSet.pBufferInfo = nullptr;
    writeDescriptorSet.pTexelBufferView = nullptr;
	updated = false;
	return writeDescriptorSet;
}

VkWriteDescriptorSet CombinedImageSamplerArray::getWriteDescriptorSet(VkDescriptorSet dstSet) const noexcept
{
	VkWriteDescriptorSet writeDescriptorSet;
    writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet.pNext = nullptr;
    writeDescriptorSet.dstSet = dstSet;
    writeDescriptorSet.dstBinding = binding;
    writeDescriptorSet.dstArrayElement = 0;
    writeDescriptorSet.descriptorCount = descriptorCount;
    writeDescriptorSet.descriptorType = descriptorType;
	writeDescriptorSet.pImageInfo = imageDescriptors.data();
    writeDescriptorSet.pBufferInfo = nullptr;
    writeDescriptorSet.pTexelBufferView = nullptr;
	updated = false;
	return writeDescriptorSet;
}

VkWriteDescriptorSet StorageBufferArray::getWriteDescriptorSet(VkDescriptorSet dstSet) const noexcept
{
	VkWriteDescriptorSet writeDescriptorSet;
    writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet.pNext = nullptr;
    writeDescriptorSet.dstSet = dstSet;
    writeDescriptorSet.dstBinding = binding;
    writeDescriptorSet.dstArrayElement = 0;
    writeDescriptorSet.descriptorCount = descriptorCount;
    writeDescriptorSet.descriptorType = descriptorType;
	writeDescriptorSet.pImageInfo = nullptr;
    writeDescriptorSet.pBufferInfo = bufferDescriptors.data();
    writeDescriptorSet.pTexelBufferView = nullptr;
	updated = false;
	return writeDescriptorSet;
}
} // namespace binding
} // namespace magma
