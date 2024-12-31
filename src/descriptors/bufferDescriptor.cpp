/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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
#include "bufferDescriptor.h"
#include "../objects/buffer.h"
#include "../objects/bufferView.h"

namespace magma::descriptor
{
void BufferDescriptor::write(VkDescriptorSet dstSet, VkWriteDescriptorSet& writeDescriptorSet) const noexcept
{
    writeDescriptor(dstSet, writeDescriptorSet);
    writeDescriptorSet.pImageInfo = nullptr;
    writeDescriptorSet.pBufferInfo = &descriptor;
    writeDescriptorSet.pTexelBufferView = nullptr;
    dirty = false;
}

void BufferDescriptor::update(lent_ptr<const Buffer> buffer, VkBufferUsageFlags usage) noexcept
{
    MAGMA_UNUSED(usage);
    MAGMA_ASSERT(buffer);
    MAGMA_ASSERT(MAGMA_BITWISE_AND(buffer->getUsage(), usage));
    if (descriptor.buffer != *buffer)
    {   // TODO: offset, range?
        descriptor = buffer->getDescriptor();
        dirty = true;
    }
}

void TexelBufferDescriptor::write(VkDescriptorSet dstSet, VkWriteDescriptorSet& writeDescriptorSet) const noexcept
{
    writeDescriptor(dstSet, writeDescriptorSet);
    writeDescriptorSet.pImageInfo = nullptr;
    writeDescriptorSet.pBufferInfo = nullptr;
    writeDescriptorSet.pTexelBufferView = &descriptor;
    dirty = false;
}

void TexelBufferDescriptor::update(lent_ptr<const BufferView> bufferView, VkBufferUsageFlags usage) noexcept
{
    MAGMA_UNUSED(usage);
    MAGMA_ASSERT(bufferView);
    MAGMA_ASSERT(MAGMA_BITWISE_AND(bufferView->getBuffer()->getUsage(), usage));
    if (descriptor != *bufferView)
    {
        descriptor = *bufferView;
        dirty = true;
    }
}
} // namespace magma::descriptor
