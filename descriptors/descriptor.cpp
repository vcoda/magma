/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

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
#include "descriptor.h"
#include "../objects/buffer.h"
#include "../objects/bufferView.h"
#include "../objects/image.h"
#include "../objects/imageView.h"

namespace magma
{
namespace descriptor
{
Descriptor::Descriptor(VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t binding_) noexcept:
    imageType(VK_IMAGE_TYPE_MAX_ENUM),
    updated(false)
{
    binding.binding = binding_;
    binding.descriptorType = descriptorType;
    binding.descriptorCount = descriptorCount;
    binding.stageFlags = 0;
    binding.pImmutableSamplers = nullptr;
}

void Descriptor::write(VkDescriptorSet dstSet, VkWriteDescriptorSet& writeDescriptorSet) const noexcept
{
    writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet.pNext = nullptr;
    writeDescriptorSet.dstSet = dstSet;
    writeDescriptorSet.dstBinding = binding.binding;
    writeDescriptorSet.dstArrayElement = 0;
    writeDescriptorSet.descriptorCount = binding.descriptorCount;
    writeDescriptorSet.descriptorType = binding.descriptorType;
    switch (binding.descriptorType)
    {
    case VK_DESCRIPTOR_TYPE_SAMPLER:
    case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
        MAGMA_ASSERT(imageDescriptor.sampler || binding.pImmutableSamplers);
    case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
    case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
    case VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT:
        MAGMA_ASSERT(imageDescriptor.imageView != VK_NULL_HANDLE);
        writeDescriptorSet.pImageInfo = &imageDescriptor;
        writeDescriptorSet.pBufferInfo = nullptr;
        writeDescriptorSet.pTexelBufferView = nullptr;
        break;
    case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
    case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:
        MAGMA_ASSERT(texelBufferView != VK_NULL_HANDLE);
        writeDescriptorSet.pImageInfo = nullptr;
        writeDescriptorSet.pBufferInfo = nullptr;
        writeDescriptorSet.pTexelBufferView = &texelBufferView;
        break;
    case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
    case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
    case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
    case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC:
        MAGMA_ASSERT(bufferDescriptor.buffer != VK_NULL_HANDLE);
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
}

void Descriptor::updateImageView(std::shared_ptr<const ImageView> imageView, VkImageUsageFlags requiredUsage) noexcept
{
    MAGMA_UNUSED(requiredUsage);
    MAGMA_ASSERT(imageView);
    MAGMA_ASSERT(imageView->getImage()->getUsage() & requiredUsage);
    if (imageDescriptor.imageView != *imageView)
    {
        imageDescriptor = imageView->getDescriptor(nullptr);
        imageType = imageView->getImage()->getType();
        updated = true;
    }
}

void Descriptor::updateBufferView(std::shared_ptr<const BufferView> bufferView, VkBufferUsageFlags requiredUsage) noexcept
{
    MAGMA_UNUSED(requiredUsage);
    MAGMA_ASSERT(bufferView);
    MAGMA_ASSERT(bufferView->getBuffer()->getUsage() & requiredUsage);
    if (texelBufferView != *bufferView)
    {
        texelBufferView = *bufferView;
        updated = true;
    }
}

void Descriptor::updateBuffer(std::shared_ptr<const Buffer> buffer, VkBufferUsageFlags requiredUsage) noexcept
{
    MAGMA_UNUSED(requiredUsage);
    MAGMA_ASSERT(buffer);
    MAGMA_ASSERT(buffer->getUsage() & requiredUsage);
    if (bufferDescriptor.buffer != *buffer)
    {   // TODO: offset, range?
        bufferDescriptor = buffer->getDescriptor();
        updated = true;
    }
}
} // namespace descriptor
} // namespace magma
