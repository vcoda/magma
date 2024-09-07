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
#include "accelerationStructure.h"
#include "../objects/accelerationStructure.h"

namespace magma::descriptor
{
#ifdef VK_KHR_acceleration_structure
AccelerationStructure::AccelerationStructure(uint32_t binding) noexcept:
    DescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR, 1, binding),
    descriptor{
        VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_KHR,
        nullptr, // pNext
        1, // accelerationStructureCount
        nullptr // pAccelerationStructures
    }
{}

void AccelerationStructure::write(VkDescriptorSet dstSet, VkWriteDescriptorSet& writeDescriptorSet) const noexcept
{
    writeDescriptor(dstSet, writeDescriptorSet);
    writeDescriptorSet.pNext = &descriptor;
    writeDescriptorSet.pImageInfo = nullptr;
    writeDescriptorSet.pBufferInfo = nullptr;
    writeDescriptorSet.pTexelBufferView = nullptr;
    dirty = false;
}

AccelerationStructure& AccelerationStructure::operator=(std::shared_ptr<const magma::AccelerationStructure> accelerationStructure) noexcept
{
    const VkAccelerationStructureKHR *handleAddress = accelerationStructure->getHandleAddress();
    if (descriptor.pAccelerationStructures != handleAddress)
    {
        descriptor.pAccelerationStructures = handleAddress;
        dirty = true;
    }
    return *this;
}
#endif // VK_KHR_acceleration_structure
} // namespace magma::descriptor
