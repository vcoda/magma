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
#include "bindingArray.h"

namespace magma
{
namespace binding
{
DescriptorSetLayoutBindingArray::DescriptorSetLayoutBindingArray(VkDescriptorType descriptorType, uint32_t binding) noexcept:
    DescriptorSetLayoutBinding(descriptorType, 0, binding)
{}

DescriptorSetLayoutBindingArray::~DescriptorSetLayoutBindingArray()
{
    if (descriptorWrite.pImageInfo)
        delete[] descriptorWrite.pImageInfo;
    else if (descriptorWrite.pBufferInfo)
        delete[] descriptorWrite.pBufferInfo;
    else if (descriptorWrite.pTexelBufferView)
        delete[] descriptorWrite.pTexelBufferView;
}

StorageBufferArray& StorageBufferArray::operator=(std::initializer_list<std::shared_ptr<const magma::StorageBuffer>> bufferArray) noexcept
{
    VkDescriptorBufferInfo *bufferInfo;
    if (!descriptorWrite.pBufferInfo)
        bufferInfo = new(std::nothrow) VkDescriptorBufferInfo[bufferArray.size()];
    else
    {
        if (bufferArray.size() <= descriptorCount)
            bufferInfo = const_cast<VkDescriptorBufferInfo *>(descriptorWrite.pBufferInfo);
        else
        {   // Reallocate
            delete[] descriptorWrite.pBufferInfo;
            bufferInfo = new(std::nothrow) VkDescriptorBufferInfo[bufferArray.size()];
        }
    }
    descriptorCount = 0;
    if (bufferInfo)
    {
        for (auto& buffer : bufferArray)
            bufferInfo[descriptorCount++] = buffer->getDescriptor();
    }
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.pNext = nullptr;
    descriptorWrite.dstSet = VK_NULL_HANDLE;
    descriptorWrite.dstBinding = binding;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorCount = descriptorCount;
    descriptorWrite.descriptorType = descriptorType;
    descriptorWrite.pImageInfo = nullptr;
    descriptorWrite.pBufferInfo = bufferInfo;
    descriptorWrite.pTexelBufferView = nullptr;
    changed = true;
    return *this;
}
} // namespace binding
} // namespace magma
