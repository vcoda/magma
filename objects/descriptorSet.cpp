/*
Magma - C++1x interface over Khronos Vulkan API.
Copyright (C) 2018 Victor Coda.

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
#include "descriptorSet.h"
#include "device.h"

namespace magma
{
DescriptorSet::DescriptorSet(VkDescriptorSet handle, std::shared_ptr<const Device> device):
    NonDispatchable(VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_EXT, handle, device)
{}

void DescriptorSet::update(uint32_t binding, const Descriptor& descriptor, const VkDescriptorBufferInfo& info) noexcept
{
    VkWriteDescriptorSet descriptorWrite;
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.pNext = nullptr;
    descriptorWrite.dstSet = handle;
    descriptorWrite.dstBinding = binding;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorCount = descriptor.descriptorCount;
    descriptorWrite.descriptorType = descriptor.type;
    descriptorWrite.pImageInfo = nullptr;
    descriptorWrite.pBufferInfo = &info;
    descriptorWrite.pTexelBufferView = nullptr;
    vkUpdateDescriptorSets(*device, 1, &descriptorWrite, 0, nullptr);
}
} // namespace magma
