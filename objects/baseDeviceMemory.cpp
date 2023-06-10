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
#include "baseDeviceMemory.h"
#include "device.h"
#include "physicalDevice.h"
#include "../exceptions/exception.h"

namespace magma
{
BaseDeviceMemory::BaseDeviceMemory(std::shared_ptr<Device> device,
    const VkMemoryRequirements& memoryRequirements, VkMemoryPropertyFlags flags,
    std::shared_ptr<IAllocator> allocator, const StructureChain& extendedInfo):
    NonDispatchable(VK_OBJECT_TYPE_DEVICE_MEMORY, std::move(device), std::move(allocator)),
    memoryRequirements(memoryRequirements),
    flags(flags),
    deviceMask(0),
    priority(MAGMA_DEFAULT_MEMORY_PRIORITY),
    mapPointer(nullptr)
{
    if (!extendedInfo.empty())
    {
    #ifdef VK_KHR_device_group
        VkMemoryAllocateFlagsInfoKHR *memoryAllocateFlagsInfo = extendedInfo.findNode<VkMemoryAllocateFlagsInfoKHR>();
        if (memoryAllocateFlagsInfo)
        {
            if (memoryAllocateFlagsInfo->flags & VK_MEMORY_ALLOCATE_DEVICE_MASK_BIT_KHR)
                deviceMask = memoryAllocateFlagsInfo->deviceMask;
        }
    #endif // VK_KHR_device_group
    #ifdef VK_EXT_memory_priority
        VkMemoryPriorityAllocateInfoEXT *memoryPriorityAllocateInfo = extendedInfo.findNode<VkMemoryPriorityAllocateInfoEXT>();
        if (memoryPriorityAllocateInfo)
            clampPriority(memoryPriorityAllocateInfo->priority);
    #endif // VK_EXT_memory_priority
    }
}

uint32_t BaseDeviceMemory::findTypeIndex(VkMemoryPropertyFlags flags) const
{
    const VkPhysicalDeviceMemoryProperties& properties = device->getPhysicalDevice()->getMemoryProperties();
    for (uint32_t memTypeIndex = 0; memTypeIndex < properties.memoryTypeCount; ++memTypeIndex)
    {   // Try to find exact match
        const VkMemoryType& memType = properties.memoryTypes[memTypeIndex];
        if (memType.propertyFlags == flags)
            return memTypeIndex;
    }
    for (uint32_t memTypeIndex = 0; memTypeIndex < properties.memoryTypeCount; ++memTypeIndex)
    {   // Try to find any suitable memory type
        const VkMemoryType& memType = properties.memoryTypes[memTypeIndex];
        if ((memType.propertyFlags & flags) == flags)
            return memTypeIndex;
    }
    MAGMA_THROW("failed to find suitable memory type");
}
} // namespace magma
