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
#include "dynamicUniformBuffer.h"
#include "device.h"
#include "physicalDevice.h"

namespace magma
{
BaseDynamicUniformBuffer::BaseDynamicUniformBuffer(std::shared_ptr<Device> device,
    std::size_t typeSize, uint32_t arraySize, VkMemoryPropertyFlags memoryFlags,
    const Initializer& optional, const Sharing& sharing, std::shared_ptr<Allocator> allocator,
    bool mappedPersistently):
    BaseUniformBuffer(std::move(device),
        [&]() -> VkDeviceSize
        {   // Calculate the buffer size taking into account alignment of each element
            const std::shared_ptr<PhysicalDevice>& physicalDevice = device->getPhysicalDevice();
            const VkPhysicalDeviceProperties& properties = physicalDevice->getProperties();
            const VkPhysicalDeviceLimits& limits = properties.limits;
            alignment = core::alignUp(static_cast<VkDeviceSize>(typeSize), limits.minUniformBufferOffsetAlignment);
            return arraySize * alignment;
        }(),
        typeSize, arraySize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, memoryFlags,
        optional, sharing, std::move(allocator), mappedPersistently)
{}

VkDescriptorBufferInfo BaseDynamicUniformBuffer::getDescriptor() const noexcept
{
    VkDescriptorBufferInfo descriptorBufferInfo;
    descriptorBufferInfo.buffer = handle;
    // For UNIFORM_BUFFER_DYNAMIC and STORAGE_BUFFER_DYNAMIC descriptor types,
    // offset is the base offset from which the dynamic offset is applied
    // and range is the static size used for all dynamic offsets.
    descriptorBufferInfo.offset = 0;
    descriptorBufferInfo.range = getTypeSize();
    return descriptorBufferInfo;
}
} // namespace magma
