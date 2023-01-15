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
#include "../objects/storageBuffer.h"

namespace magma
{
namespace binding
{
DescriptorSetLayoutBindingArray::DescriptorSetLayoutBindingArray(VkDescriptorType descriptorType, uint32_t binding) noexcept:
    DescriptorSetLayoutBinding(descriptorType, /* TBD */ 0, binding)
{}

CombinedImageSamplerArray::DescriptorImageInfo& CombinedImageSamplerArray::operator[](uint32_t index)
{
    if (index >= descriptorCount)
    {
        descriptorCount = std::max(descriptorCount, index + 1);
        if (descriptorCount > imageDescriptors.size())
            imageDescriptors.resize(descriptorCount);
        descriptorWrite.descriptorCount = descriptorCount;
        descriptorWrite.pImageInfo = imageDescriptors.data();
    }
    changed = true;
    return imageDescriptors[index];
}

StorageBufferArray::DescriptorBufferInfo& StorageBufferArray::operator[](uint32_t index)
{
    if (index >= descriptorCount)
    {
        descriptorCount = std::max(descriptorCount, index + 1);
        if (descriptorCount > bufferDescriptors.size())
            bufferDescriptors.resize(descriptorCount);
        descriptorWrite.descriptorCount = descriptorCount;
        descriptorWrite.pBufferInfo = bufferDescriptors.data();
    }
    changed = true;
    return bufferDescriptors[index];
}
} // namespace binding
} // namespace magma
