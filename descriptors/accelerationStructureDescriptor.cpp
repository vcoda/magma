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
#include "descriptor.h"
#include "../objects/accelerationStructure.h"

namespace magma
{
namespace descriptor
{
#if defined(VK_KHR_acceleration_structure) || defined(VK_NV_ray_tracing)
AccelerationStructure::AccelerationStructure(uint32_t binding) noexcept:
#ifdef VK_KHR_acceleration_structure
    Descriptor(VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR, 1, binding),
#else
    Descriptor(VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_NV, 1, binding),
#endif
    handle(VK_NULL_HANDLE)
{
#ifdef VK_KHR_acceleration_structure
    writeDescriptorSetAccelerationStructure.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_KHR;
#else
    writeDescriptorSetAccelerationStructure.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_NV;
#endif
    writeDescriptorSetAccelerationStructure.pNext = nullptr;
    writeDescriptorSetAccelerationStructure.accelerationStructureCount = 1;
    writeDescriptorSetAccelerationStructure.pAccelerationStructures = &handle;
}

void AccelerationStructure::getWriteDescriptor(VkDescriptorSet dstSet,
    VkWriteDescriptorSet& writeDescriptorSet) const noexcept
{
    writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet.pNext = &writeDescriptorSetAccelerationStructure;
    writeDescriptorSet.dstSet = dstSet;
    writeDescriptorSet.dstBinding = binding.binding;
    writeDescriptorSet.dstArrayElement = 0;
    writeDescriptorSet.descriptorCount = binding.descriptorCount;
    writeDescriptorSet.descriptorType = binding.descriptorType;
    writeDescriptorSet.pImageInfo = nullptr;
    writeDescriptorSet.pBufferInfo = nullptr;
    writeDescriptorSet.pTexelBufferView = nullptr;
    updated = false;
}

AccelerationStructure& AccelerationStructure::operator=(std::shared_ptr<const magma::AccelerationStructure> accelerationStructure) noexcept
{
#ifdef VK_KHR_acceleration_structure
    // TODO: refactor this when VK_KHR_acceleration_structure support will be implemented!
    VkAccelerationStructureKHR handleKHR;
    #ifdef MAGMA_X64
    handleKHR = reinterpret_cast<VkAccelerationStructureKHR>((VkAccelerationStructureNV)*accelerationStructure);
    #else
    handleKHR = static_cast<VkAccelerationStructureKHR>((VkAccelerationStructureNV)*accelerationStructure);
    #endif
    if (handleKHR != handle)
    {
        handle = handleKHR;
        updated = true;
    }
#else
    if (handle != *accelerationStructure)
    {
        handle = *accelerationStructure;
        updated = true;
    }
#endif
    return *this;
}
#endif // VK_KHR_acceleration_structure || VK_NV_ray_tracing
} // namespace descriptor
} // namespace magma
