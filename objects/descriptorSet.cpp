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
#include "sampler.h"
#include "../misc/format.h"

namespace magma
{
DescriptorSet::DescriptorSet(VkDescriptorSet handle,
    std::shared_ptr<Device> device,
    std::shared_ptr<DescriptorPool> pool,
    std::shared_ptr<DescriptorSetLayout> layout) noexcept:
    NonDispatchable(VK_OBJECT_TYPE_DESCRIPTOR_SET, std::move(device), nullptr),
    pool(std::move(pool)),
    layout(std::move(layout))
{
    this->handle = handle;
}

void DescriptorSet::update(uint32_t index, std::shared_ptr<const Buffer> buffer) noexcept
{
    const DescriptorSetLayout::Binding& binding = layout->getBinding(index);
    MAGMA_ASSERT(1 == binding.descriptorCount);
    const VkDescriptorBufferInfo info = buffer->getDescriptor();
    VkWriteDescriptorSet descriptorWrite;
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.pNext = nullptr;
    descriptorWrite.dstSet = handle;
    descriptorWrite.dstBinding = binding.binding;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorCount = binding.descriptorCount;
    descriptorWrite.descriptorType = binding.descriptorType;
    descriptorWrite.pImageInfo = nullptr;
    descriptorWrite.pBufferInfo = &info;
    descriptorWrite.pTexelBufferView = nullptr;
    vkUpdateDescriptorSets(MAGMA_HANDLE(device), 1, &descriptorWrite, 0, nullptr);
}

void DescriptorSet::update(uint32_t index, std::shared_ptr<const ImageView> imageView, std::shared_ptr<const Sampler> sampler) noexcept
{
    const DescriptorSetLayout::Binding& binding = layout->getBinding(index);
    const Format format(imageView->getImage()->getFormat());
    MAGMA_ASSERT(1 == binding.descriptorCount);
    VkDescriptorImageInfo info;
    info.sampler = *sampler;
    info.imageView = *imageView;
#ifdef VK_KHR_separate_depth_stencil_layouts
    if (format.depth()) // Read-only image in a shader where only the depth aspect is accessed
        info.imageLayout = VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL_KHR;
    else if (format.stencil()) // Read-only image in a shader where only the stencil aspect is accessed
        info.imageLayout = VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL_KHR;
    else if (format.depthStencil()) // Read-only image in a shader where both depth and stencil is accessed
        info.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
#else
    if (format.depth() || format.stencil() || format.depthStencil())
        info.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
#endif // VK_KHR_separate_depth_stencil_layouts
    else // Read-only image in a shader
        info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    VkWriteDescriptorSet descriptorWrite;
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.pNext = nullptr;
    descriptorWrite.dstSet = handle;
    descriptorWrite.dstBinding = binding.binding;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorCount = binding.descriptorCount;
    descriptorWrite.descriptorType = binding.descriptorType;
    descriptorWrite.pImageInfo = &info;
    descriptorWrite.pBufferInfo = nullptr;
    descriptorWrite.pTexelBufferView = nullptr;
    vkUpdateDescriptorSets(MAGMA_HANDLE(device), 1, &descriptorWrite, 0, nullptr);
}

void DescriptorSet::update(uint32_t index, std::shared_ptr<const BufferView> texelBufferView) noexcept
{
    const DescriptorSetLayout::Binding& binding = layout->getBinding(index);
    MAGMA_ASSERT(1 == binding.descriptorCount);
    const VkBufferView bufferViews[1] = {*texelBufferView};
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
    descriptorWrite.pTexelBufferView = bufferViews;
    vkUpdateDescriptorSets(MAGMA_HANDLE(device), 1, &descriptorWrite, 0, nullptr);
}
} // namespace magma
