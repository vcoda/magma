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
#include "baseDeviceMemory.h"
#include "device.h"
#include "physicalDevice.h"
#include "../exceptions/exception.h"

namespace magma
{
BaseDeviceMemory::BaseDeviceMemory(std::shared_ptr<Device> device,
    const VkMemoryRequirements& memoryRequirements, VkMemoryPropertyFlags propertyFlags,
    std::shared_ptr<IAllocator> allocator, const StructureChain& extendedInfo) noexcept:
    NonDispatchable(VK_OBJECT_TYPE_DEVICE_MEMORY, std::move(device), std::move(allocator)),
    memoryRequirements(memoryRequirements),
    memoryType(findMemoryType(propertyFlags)),
    deviceMask(0),
    priority(MemoryPriorityDefault),
    binding(VK_NULL_HANDLE),
    mapPointer(nullptr),
    mapOffset(0),
    mapSize(0),
    mapFlags(0)
{
    flags.deviceLocal = MAGMA_BITWISE_AND(memoryType.propertyFlags, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    flags.hostVisible = MAGMA_BITWISE_AND(memoryType.propertyFlags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
    flags.hostCoherent = MAGMA_BITWISE_AND(memoryType.propertyFlags, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    flags.hostCached = MAGMA_BITWISE_AND(memoryType.propertyFlags,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT);
    flags.lazilyAllocated = MAGMA_BITWISE_AND(memoryType.propertyFlags, VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT);
#ifdef VK_AMD_device_coherent_memory
    // For any memory allocated with both the HOST_COHERENT and
    // the DEVICE_COHERENT flags, host or device accesses also
    // perform automatic memory domain transfer operations,
    // such that writes are always automatically available and
    // visible to both host and device memory domains.
    flags.deviceHostCoherent = MAGMA_BITWISE_AND(memoryType.propertyFlags,
        VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD);
    flags.deviceUncached = MAGMA_BITWISE_AND(memoryType.propertyFlags, VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD);
#else
    flags.deviceHostCoherent = VK_FALSE;
    flags.deviceUncached = VK_FALSE;
#endif // VK_AMD_device_coherent_memory
    // On discrete NVIDIA and AMD GPUs there are around 256 MiB of
    // DEVICE_LOCAL + HOST_VISIBLE memory pool. This 256MiB limit
    // correlates with the 256MiB PCIE-specified BAR-size limit
    // that defines the size of the 256MiB aperture/window of VRAM
    // that the host can access.
    flags.staged = flags.deviceLocal && flags.hostVisible;
    if (!extendedInfo.empty())
    {
    #ifdef VK_KHR_device_group
        const VkMemoryAllocateFlagsInfoKHR *memoryAllocateFlagsInfo = extendedInfo.findNode<VkMemoryAllocateFlagsInfoKHR>();
        if (memoryAllocateFlagsInfo)
        {
            if (memoryAllocateFlagsInfo->flags & VK_MEMORY_ALLOCATE_DEVICE_MASK_BIT_KHR)
                deviceMask = memoryAllocateFlagsInfo->deviceMask;
        }
    #endif // VK_KHR_device_group
    #ifdef VK_EXT_memory_priority
        const VkMemoryPriorityAllocateInfoEXT *memoryPriorityAllocateInfo = extendedInfo.findNode<VkMemoryPriorityAllocateInfoEXT>();
        if (memoryPriorityAllocateInfo)
            clampPriority(memoryPriorityAllocateInfo->priority);
    #endif // VK_EXT_memory_priority
    }
}

VkMemoryType BaseDeviceMemory::findMemoryType(VkMemoryPropertyFlags propertyFlags) const
{
    const VkPhysicalDeviceMemoryProperties properties = device->getPhysicalDevice()->getMemoryProperties();
    auto memoryTypeIndex = findTypeIndex(propertyFlags);
    return properties.memoryTypes[memoryTypeIndex.value()];
}

std::optional<uint32_t> BaseDeviceMemory::findTypeIndex(VkMemoryPropertyFlags propertyFlags) const noexcept
{
    const VkPhysicalDeviceMemoryProperties properties = device->getPhysicalDevice()->getMemoryProperties();
    for (uint32_t memoryTypeIndex = 0; memoryTypeIndex < properties.memoryTypeCount; ++memoryTypeIndex)
    {   // Try to find exact match
        const VkMemoryType& memoryType = properties.memoryTypes[memoryTypeIndex];
        if (memoryType.propertyFlags == propertyFlags)
            return memoryTypeIndex;
    }
    for (uint32_t memoryTypeIndex = 0; memoryTypeIndex < properties.memoryTypeCount; ++memoryTypeIndex)
    {   // Try to find any suitable memory type
        const VkMemoryType& memoryType = properties.memoryTypes[memoryTypeIndex];
        if ((memoryType.propertyFlags & propertyFlags) == propertyFlags)
            return memoryTypeIndex;
    }
    return std::nullopt;
}

float BaseDeviceMemory::clampPriority(float value) noexcept
{
    priority = std::clamp(value, MemoryPriorityLowest, MemoryPriorityHighest);
    return priority;
}
} // namespace magma
