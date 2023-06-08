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
#include "deviceMemory.h"
#include "device.h"
#include "physicalDevice.h"
#include "../allocator/allocator.h"
#include "../misc/extProcAddress.h"
#include "../exceptions/errorResult.h"
#include "../exceptions/notImplemented.h"

namespace magma
{
std::atomic<uint32_t> DeviceMemory::allocationCount;

DeviceMemory::DeviceMemory(std::shared_ptr<Device> device_,
    const VkMemoryRequirements& memoryRequirements, VkMemoryPropertyFlags flags, float priority_,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    BaseDeviceMemory(std::move(device_), memoryRequirements, flags, priority_, 0, std::move(allocator))
{
    VkMemoryAllocateInfo memoryAllocateInfo;
    memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocateInfo.pNext = nullptr;
    memoryAllocateInfo.allocationSize = memoryRequirements.size;
    memoryAllocateInfo.memoryTypeIndex = findTypeIndex(flags);
#ifdef VK_EXT_memory_priority
    VkMemoryPriorityAllocateInfoEXT memoryPriorityAllocateInfo;
    if (device->extensionEnabled(VK_EXT_MEMORY_PRIORITY_EXTENSION_NAME))
    {
        memoryAllocateInfo.pNext = &memoryPriorityAllocateInfo;
        memoryPriorityAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_PRIORITY_ALLOCATE_INFO_EXT;
        memoryPriorityAllocateInfo.pNext = nullptr;
        memoryPriorityAllocateInfo.priority = priority;
    }
#endif // VK_EXT_memory_priority
    const VkResult result = vkAllocateMemory(MAGMA_HANDLE(device), &memoryAllocateInfo,
        MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to allocate device memory");
    ++allocationCount;
}

#ifdef VK_KHR_device_group
DeviceMemory::DeviceMemory(std::shared_ptr<Device> device_, uint32_t deviceMask,
    const VkMemoryRequirements& memoryRequirements, VkMemoryPropertyFlags flags, float priority_,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    BaseDeviceMemory(std::move(device_), memoryRequirements, flags, priority_, deviceMask, std::move(allocator))
{
    VkMemoryAllocateInfo memoryAllocateInfo;
    VkMemoryAllocateFlagsInfoKHR memoryAllocateFlagsInfo;
    memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocateInfo.pNext = &memoryAllocateFlagsInfo;
    memoryAllocateInfo.allocationSize = memoryRequirements.size;
    memoryAllocateInfo.memoryTypeIndex = findTypeIndex(flags);
    memoryAllocateFlagsInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO_KHR;
    memoryAllocateFlagsInfo.pNext = nullptr;
    memoryAllocateFlagsInfo.flags = VK_MEMORY_ALLOCATE_DEVICE_MASK_BIT_KHR;
    memoryAllocateFlagsInfo.deviceMask = deviceMask;
#ifdef VK_EXT_memory_priority
    VkMemoryPriorityAllocateInfoEXT memoryPriorityAllocateInfo;
    if (device->extensionEnabled(VK_EXT_MEMORY_PRIORITY_EXTENSION_NAME))
    {
        memoryAllocateFlagsInfo.pNext = &memoryPriorityAllocateInfo;
        memoryPriorityAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_PRIORITY_ALLOCATE_INFO_EXT;
        memoryPriorityAllocateInfo.pNext = nullptr;
        memoryPriorityAllocateInfo.priority = priority;
    }
#endif // VK_EXT_memory_priority
    const VkResult result = vkAllocateMemory(MAGMA_HANDLE(device), &memoryAllocateInfo,
        MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to allocate device memory within device group");
    ++allocationCount;
}
#endif // VK_KHR_device_group

DeviceMemory::~DeviceMemory()
{
    MAGMA_ASSERT(!mapped());
    vkFreeMemory(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}

void DeviceMemory::setPriority(float priority) noexcept
{
    MAGMA_UNUSED(priority);
#ifdef VK_EXT_pageable_device_local_memory
    MAGMA_DEVICE_EXTENSION(vkSetDeviceMemoryPriorityEXT);
    if (vkSetDeviceMemoryPriorityEXT)
        vkSetDeviceMemoryPriorityEXT(MAGMA_HANDLE(device), handle, updatePriority(priority));
#endif // VK_EXT_pageable_device_local_memory
}

void DeviceMemory::realloc(VkDeviceSize newSize, float priority,
    NonDispatchableHandle /* unused */,
    VkObjectType /* unused */)
{
    MAGMA_ASSERT(!mapped());
    if (mapped())
        unmap();
    vkFreeMemory(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
    handle = VK_NULL_HANDLE;
    --allocationCount;
    // Fill new allocation description
    VkMemoryAllocateInfo memoryAllocateInfo;
    memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocateInfo.pNext = nullptr;
    memoryAllocateInfo.allocationSize = newSize;
    memoryAllocateInfo.memoryTypeIndex = findTypeIndex(flags);
#ifdef VK_KHR_device_group
    VkMemoryAllocateFlagsInfoKHR memoryAllocateFlagsInfo;
    if (deviceMask && device->extensionEnabled(VK_KHR_DEVICE_GROUP_EXTENSION_NAME))
    {
        memoryAllocateInfo.pNext = &memoryAllocateFlagsInfo;
        memoryAllocateFlagsInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO_KHR;
        memoryAllocateFlagsInfo.pNext = nullptr;
        memoryAllocateFlagsInfo.flags = VK_MEMORY_ALLOCATE_DEVICE_MASK_BIT_KHR;
        memoryAllocateFlagsInfo.deviceMask = deviceMask;
    }
#endif // VK_KHR_device_group
#ifdef VK_EXT_memory_priority
    VkMemoryPriorityAllocateInfoEXT memoryPriorityAllocateInfo;
    if (device->extensionEnabled(VK_EXT_MEMORY_PRIORITY_EXTENSION_NAME))
    {
    #ifdef VK_KHR_device_group
        if (memoryAllocateInfo.pNext)
            memoryAllocateFlagsInfo.pNext = &memoryPriorityAllocateInfo;
        else
    #endif // VK_KHR_device_group
        if (!memoryAllocateInfo.pNext)
            memoryAllocateInfo.pNext = &memoryPriorityAllocateInfo;
        memoryPriorityAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_PRIORITY_ALLOCATE_INFO_EXT;
        memoryPriorityAllocateInfo.pNext = nullptr;
        memoryPriorityAllocateInfo.priority = updatePriority(priority);
    }
#endif // VK_EXT_memory_priority
    const VkResult result = vkAllocateMemory(MAGMA_HANDLE(device), &memoryAllocateInfo,
        MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to reallocate device memory");
    memoryRequirements.size = newSize;
    ++allocationCount;
}

void DeviceMemory::bind(NonDispatchableHandle object, VkObjectType objectType,
    VkDeviceSize offset /* 0 */)
{
    VkResult result;
    switch (objectType)
    {
    case VK_OBJECT_TYPE_BUFFER:
        result = vkBindBufferMemory(MAGMA_HANDLE(device), core::reinterpret<VkBuffer>(object), handle, offset);
        break;
    case VK_OBJECT_TYPE_IMAGE:
        result = vkBindImageMemory(MAGMA_HANDLE(device), core::reinterpret<VkImage>(object), handle, offset);
        break;
#ifdef VK_NV_ray_tracing
    case VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_NV:
        {
            VkBindAccelerationStructureMemoryInfoNV bindAccelerationStructureMemoryInfo;
            bindAccelerationStructureMemoryInfo.sType = VK_STRUCTURE_TYPE_BIND_ACCELERATION_STRUCTURE_MEMORY_INFO_NV;
            bindAccelerationStructureMemoryInfo.pNext = nullptr;
            bindAccelerationStructureMemoryInfo.accelerationStructure = core::reinterpret<VkAccelerationStructureNV>(object);
            bindAccelerationStructureMemoryInfo.memory = handle;
            bindAccelerationStructureMemoryInfo.memoryOffset = offset;
            bindAccelerationStructureMemoryInfo.deviceIndexCount = 0;
            bindAccelerationStructureMemoryInfo.pDeviceIndices = nullptr;
            MAGMA_DEVICE_EXTENSION(vkBindAccelerationStructureMemoryNV);
            result = vkBindAccelerationStructureMemoryNV(MAGMA_HANDLE(device), 1, &bindAccelerationStructureMemoryInfo);
        }
        break;
#endif // VK_NV_ray_tracing
    default:
        MAGMA_THROW_NOT_IMPLEMENTED;
    };
    MAGMA_THROW_FAILURE(result, "failed to bind device memory");
}

void *DeviceMemory::map(
    VkDeviceSize offset /* 0 */,
    VkDeviceSize size /* VK_WHOLE_SIZE */,
    VkMemoryMapFlags flags /* 0 */) noexcept
{
    MAGMA_ASSERT(hostVisible());
    if (!mapPointer)
    {
        const VkResult result = vkMapMemory(MAGMA_HANDLE(device), handle, offset, size, flags, &mapPointer);
        if (result != VK_SUCCESS)
        {   // VK_ERROR_OUT_OF_HOST_MEMORY
            // VK_ERROR_OUT_OF_DEVICE_MEMORY
            // VK_ERROR_MEMORY_MAP_FAILED
            return nullptr;
        }
    }
    return mapPointer;
}

void DeviceMemory::unmap() noexcept
{
    MAGMA_ASSERT(hostVisible());
    if (mapPointer)
    {
        vkUnmapMemory(MAGMA_HANDLE(device), handle);
        mapPointer = nullptr;
    }
}

bool DeviceMemory::flushMappedRange(
    VkDeviceSize offset /* 0 */,
    VkDeviceSize size /* VK_WHOLE_SIZE */) noexcept
{
    VkMappedMemoryRange memoryRange;
    memoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    memoryRange.pNext = nullptr;
    memoryRange.memory = handle;
    memoryRange.offset = offset;
    memoryRange.size = size;
    const VkResult result = vkFlushMappedMemoryRanges(MAGMA_HANDLE(device), 1, &memoryRange);
    return (VK_SUCCESS == result);
}

bool DeviceMemory::invalidateMappedRange(
    VkDeviceSize offset /* 0 */,
    VkDeviceSize size /* VK_WHOLE_SIZE */) noexcept
{
    VkMappedMemoryRange memoryRange;
    memoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    memoryRange.pNext = nullptr;
    memoryRange.memory = handle;
    memoryRange.offset = offset;
    memoryRange.size = size;
    const VkResult result = vkInvalidateMappedMemoryRanges(MAGMA_HANDLE(device), 1, &memoryRange);
    return (VK_SUCCESS == result);
}

void DeviceMemory::onDefragment() noexcept
{
    // Do nothing with direct memory allocation
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
