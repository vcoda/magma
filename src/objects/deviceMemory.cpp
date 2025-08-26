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
#include "deviceMemory.h"
#include "device.h"
#include "physicalDevice.h"
#include "../platform/androidHardwareBuffer.h"
#include "../allocator/allocator.h"
#include "../misc/extension.h"
#include "../exceptions/errorResult.h"

namespace magma
{
std::atomic<uint32_t> DeviceMemory::allocationCount;

DeviceMemory::DeviceMemory(std::shared_ptr<Device> device,
    const VkMemoryRequirements& memoryRequirements, VkMemoryPropertyFlags flags,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    const StructureChain& extendedInfo /* default */):
    BaseDeviceMemory(std::move(device), memoryRequirements, flags, std::move(allocator), extendedInfo)
{
    VkMemoryAllocateInfo memoryAllocateInfo;
    memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocateInfo.pNext = extendedInfo.headNode();
    memoryAllocateInfo.allocationSize = memoryRequirements.size;
    memoryAllocateInfo.memoryTypeIndex = findTypeIndex(flags).value();
    const VkResult result = vkAllocateMemory(getNativeDevice(), &memoryAllocateInfo,
        MAGMA_OPTIONAL(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to allocate device memory");
    ++allocationCount;
}

#ifdef VK_ANDROID_external_memory_android_hardware_buffer
DeviceMemory::DeviceMemory(std::shared_ptr<Device> device,
    lent_ptr<AndroidHardwareBuffer> hardwareBuffer,
    VkMemoryPropertyFlags flags,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    const StructureChain& extendedInfo /* default */):
    BaseDeviceMemory(std::move(device), hardwareBuffer->getMemoryRequirements(), flags, std::move(allocator), extendedInfo)
{
    VkMemoryAllocateInfo memoryAllocateInfo;
    VkImportAndroidHardwareBufferInfoANDROID importAndroidHardwareBufferInfo;
    memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocateInfo.pNext = &importAndroidHardwareBufferInfo;
    memoryAllocateInfo.allocationSize = memoryRequirements.size;
    memoryAllocateInfo.memoryTypeIndex = findTypeIndex(flags).value();
    importAndroidHardwareBufferInfo.sType = VK_STRUCTURE_TYPE_IMPORT_ANDROID_HARDWARE_BUFFER_INFO_ANDROID;
    importAndroidHardwareBufferInfo.pNext = extendedInfo.headNode();
    importAndroidHardwareBufferInfo.buffer = hardwareBuffer->getBuffer();
    const VkResult result = vkAllocateMemory(getNativeDevice(), &memoryAllocateInfo,
        MAGMA_OPTIONAL(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to import memory from android hardware buffer");
    ++allocationCount;
}
#endif // VK_ANDROID_external_memory_android_hardware_buffer

DeviceMemory::~DeviceMemory()
{
    if (mapPointer)
        vkUnmapMemory(getNativeDevice(), handle);
    vkFreeMemory(getNativeDevice(), handle, MAGMA_OPTIONAL(hostAllocator));
    --allocationCount;
}

void DeviceMemory::setPriority(float priority) noexcept
{
    MAGMA_UNUSED(priority);
#ifdef VK_EXT_pageable_device_local_memory
    MAGMA_DEVICE_EXTENSION(vkSetDeviceMemoryPriorityEXT);
    if (vkSetDeviceMemoryPriorityEXT)
        vkSetDeviceMemoryPriorityEXT(getNativeDevice(), handle, clampPriority(priority));
#endif // VK_EXT_pageable_device_local_memory
}

#ifdef VK_ANDROID_external_memory_android_hardware_buffer
AHardwareBuffer* DeviceMemory::getHardwareBuffer() const noexcept
{
    VkMemoryGetAndroidHardwareBufferInfoANDROID androidHardwareBufferInfo;
    androidHardwareBufferInfo.sType = VK_STRUCTURE_TYPE_MEMORY_GET_ANDROID_HARDWARE_BUFFER_INFO_ANDROID;
    androidHardwareBufferInfo.pNext = nullptr;
    androidHardwareBufferInfo.memory = handle;
    MAGMA_DEVICE_EXTENSION(vkGetMemoryAndroidHardwareBufferANDROID);
    if (vkGetMemoryAndroidHardwareBufferANDROID)
    {   // A new reference acquired in addition to the reference held by the VkDeviceMemory
        AHardwareBuffer *buffer = nullptr;
        const VkResult result = vkGetMemoryAndroidHardwareBufferANDROID(getNativeDevice(),
            &androidHardwareBufferInfo, &buffer);
        if (MAGMA_SUCCEEDED(result))
            return buffer;
    }
    return nullptr;
}
#endif // VK_ANDROID_external_memory_android_hardware_buffer

void DeviceMemory::realloc(NonDispatchableHandle /* unused */,
    const VkMemoryRequirements& memoryRequirements_,
    const StructureChain& extendedInfo /* default */)
{
    if (mapPointer)
        vkUnmapMemory(getNativeDevice(), handle);
    vkFreeMemory(getNativeDevice(), handle, MAGMA_OPTIONAL(hostAllocator));
    handle = VK_NULL_HANDLE;
    --allocationCount;
    memoryRequirements = memoryRequirements_;
    VkMemoryAllocateInfo memoryAllocateInfo;
    memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocateInfo.pNext = extendedInfo.headNode();
    memoryAllocateInfo.allocationSize = memoryRequirements.size;
    memoryAllocateInfo.memoryTypeIndex = findTypeIndex(memoryType.propertyFlags).value();
    VkResult result = vkAllocateMemory(getNativeDevice(), &memoryAllocateInfo,
        MAGMA_OPTIONAL(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to reallocate device memory");
    ++allocationCount;
    result = VK_ERROR_MEMORY_MAP_FAILED;
    if (mapPersistent)
    {   // Try remap the memory within new allocation
        mapSize = std::min(mapSize, getSize());
        result = vkMapMemory(getNativeDevice(), handle, mapOffset, mapSize, mapFlags, &mapPointer);
    }
    if (result != VK_SUCCESS)
    {
        mapPointer = nullptr;
        mapOffset = mapSize = mapFlags = 0;
        mapPersistent = false;
    }
}

void DeviceMemory::bind(NonDispatchableHandle object, VkObjectType objectType,
    VkDeviceSize offset /* 0 */)
{
    VkResult result;
    switch (objectType)
    {
    case VK_OBJECT_TYPE_BUFFER:
        result = vkBindBufferMemory(getNativeDevice(), core::reinterpret<VkBuffer>(object), handle, offset);
        break;
    case VK_OBJECT_TYPE_IMAGE:
        result = vkBindImageMemory(getNativeDevice(), core::reinterpret<VkImage>(object), handle, offset);
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
            MAGMA_REQUIRED_DEVICE_EXTENSION(vkBindAccelerationStructureMemoryNV, VK_NV_RAY_TRACING_EXTENSION_NAME);
            result = vkBindAccelerationStructureMemoryNV(getNativeDevice(), 1, &bindAccelerationStructureMemoryInfo);
        }
        break;
#endif // VK_NV_ray_tracing
    default:
        MAGMA_FAILURE("unknown object type");
        result = VK_ERROR_UNKNOWN;
    };
    MAGMA_HANDLE_RESULT(result, "failed to bind device memory");
    if (VK_SUCCESS == result)
        binding = object;
}

#ifdef VK_KHR_device_group
void DeviceMemory::bindDeviceGroup(NonDispatchableHandle object, VkObjectType objectType,
    const std::vector<uint32_t>& deviceIndices,
    const std::vector<VkRect2D>& splitInstanceBindRegions /* empty */,
    VkDeviceSize offset /* 0 */)
{
    VkResult result = VK_SUCCESS;
    if (VK_OBJECT_TYPE_BUFFER == objectType)
    {
        VkBindBufferMemoryInfoKHR bindBufferMemoryInfo;
        VkBindBufferMemoryDeviceGroupInfoKHR bindBufferMemoryDeviceGroupInfo;
        bindBufferMemoryInfo.sType = VK_STRUCTURE_TYPE_BIND_BUFFER_MEMORY_INFO_KHR;
        bindBufferMemoryInfo.pNext = &bindBufferMemoryDeviceGroupInfo;
        bindBufferMemoryInfo.buffer = core::reinterpret<VkBuffer>(object);
        bindBufferMemoryInfo.memory = handle;
        bindBufferMemoryInfo.memoryOffset = offset;
        bindBufferMemoryDeviceGroupInfo.sType = VK_STRUCTURE_TYPE_BIND_BUFFER_MEMORY_DEVICE_GROUP_INFO_KHR;
        bindBufferMemoryDeviceGroupInfo.pNext = nullptr;
        bindBufferMemoryDeviceGroupInfo.deviceIndexCount = core::countof(deviceIndices);
        bindBufferMemoryDeviceGroupInfo.pDeviceIndices = deviceIndices.data();
        MAGMA_REQUIRED_DEVICE_EXTENSION(vkBindBufferMemory2KHR, VK_KHR_BIND_MEMORY_2_EXTENSION_NAME);
        result = vkBindBufferMemory2KHR(getNativeDevice(), 1, &bindBufferMemoryInfo);
    }
    else if (VK_OBJECT_TYPE_IMAGE == objectType)
    {
        VkBindImageMemoryInfoKHR bindImageMemoryInfo;
        VkBindImageMemoryDeviceGroupInfoKHR bindImageMemoryDeviceGroupInfo;
        bindImageMemoryInfo.sType = VK_STRUCTURE_TYPE_BIND_IMAGE_MEMORY_INFO_KHR;
        bindImageMemoryInfo.pNext = &bindImageMemoryDeviceGroupInfo;
        bindImageMemoryInfo.image = core::reinterpret<VkImage>(object);
        bindImageMemoryInfo.memory = handle;
        bindImageMemoryInfo.memoryOffset = offset;
        bindImageMemoryDeviceGroupInfo.sType = VK_STRUCTURE_TYPE_BIND_IMAGE_MEMORY_DEVICE_GROUP_INFO_KHR;
        bindImageMemoryDeviceGroupInfo.pNext = nullptr;
        bindImageMemoryDeviceGroupInfo.deviceIndexCount = core::countof(deviceIndices);
        bindImageMemoryDeviceGroupInfo.pDeviceIndices = deviceIndices.data();
        bindImageMemoryDeviceGroupInfo.splitInstanceBindRegionCount = core::countof(splitInstanceBindRegions);
        bindImageMemoryDeviceGroupInfo.pSplitInstanceBindRegions = splitInstanceBindRegions.data();
        MAGMA_REQUIRED_DEVICE_EXTENSION(vkBindImageMemory2KHR, VK_KHR_BIND_MEMORY_2_EXTENSION_NAME);
        result = vkBindImageMemory2KHR(getNativeDevice(), 1, &bindImageMemoryInfo);
    }
#ifdef VK_NV_ray_tracing
    else if (VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_NV == objectType)
    {
        VkBindAccelerationStructureMemoryInfoNV bindAccelerationStructureMemoryInfo;
        bindAccelerationStructureMemoryInfo.sType = VK_STRUCTURE_TYPE_BIND_ACCELERATION_STRUCTURE_MEMORY_INFO_NV;
        bindAccelerationStructureMemoryInfo.pNext = nullptr;
        bindAccelerationStructureMemoryInfo.accelerationStructure = core::reinterpret<VkAccelerationStructureNV>(object);
        bindAccelerationStructureMemoryInfo.memory = handle;
        bindAccelerationStructureMemoryInfo.memoryOffset = offset;
        bindAccelerationStructureMemoryInfo.deviceIndexCount = core::countof(deviceIndices);
        bindAccelerationStructureMemoryInfo.pDeviceIndices = deviceIndices.data();
        MAGMA_REQUIRED_DEVICE_EXTENSION(vkBindAccelerationStructureMemoryNV, VK_NV_RAY_TRACING_EXTENSION_NAME);
        result = vkBindAccelerationStructureMemoryNV(getNativeDevice(), 1, &bindAccelerationStructureMemoryInfo);
    }
#endif // VK_NV_ray_tracing
    else
    {
        MAGMA_FAILURE("unknown type of binded object");
    }
    MAGMA_HANDLE_RESULT(result, "failed to bind device memory across the subdevices");
    if (VK_SUCCESS == result)
        binding = object;
}
#endif // VK_KHR_device_group

void *DeviceMemory::map(
    VkDeviceSize offset /* 0 */,
    VkDeviceSize size /* VK_WHOLE_SIZE */,
    VkMemoryMapFlags mmapFlags /* 0 */,
    bool persistently /* false */) noexcept
{
    MAGMA_ASSERT(flags.hostVisible);
    if (mapPersistent)
        return mapPointer;
    if (!mapPointer || (mapOffset != offset) ||
        mapSize < ((VK_WHOLE_SIZE == size) ? getSize() : size) ||
        mapFlags != mmapFlags)
    {
        unmap();
        const VkResult result = vkMapMemory(getNativeDevice(), handle, offset, size, mmapFlags, &mapPointer);
        if (result != VK_SUCCESS)
        {   // VK_ERROR_OUT_OF_HOST_MEMORY
            // VK_ERROR_OUT_OF_DEVICE_MEMORY
            // VK_ERROR_MEMORY_MAP_FAILED
            return nullptr;
        }
        mapOffset = offset;
        mapSize = (VK_WHOLE_SIZE == size) ? getSize() : size;
        mapFlags = mmapFlags;
        mapPersistent = persistently;
    }
    return mapPointer;
}

void DeviceMemory::unmap() noexcept
{
    MAGMA_ASSERT(flags.hostVisible);
    if (mapPointer && !mapPersistent)
    {
        vkUnmapMemory(getNativeDevice(), handle);
        mapPointer = nullptr;
        mapOffset = 0;
        mapSize = 0;
        mapFlags = 0;
    }
}

bool DeviceMemory::flushMappedRange(
    VkDeviceSize offset /* 0 */,
    VkDeviceSize size /* VK_WHOLE_SIZE */) noexcept
{
    MAGMA_ASSERT(!flags.hostCoherent);
    VkMappedMemoryRange memoryRange;
    memoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    memoryRange.pNext = nullptr;
    memoryRange.memory = handle;
    memoryRange.offset = offset;
    memoryRange.size = size;
    const VkResult result = vkFlushMappedMemoryRanges(getNativeDevice(), 1, &memoryRange);
    return (VK_SUCCESS == result);
}

bool DeviceMemory::invalidateMappedRange(
    VkDeviceSize offset /* 0 */,
    VkDeviceSize size /* VK_WHOLE_SIZE */) noexcept
{
    MAGMA_ASSERT(!flags.hostCoherent);
    VkMappedMemoryRange memoryRange;
    memoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    memoryRange.pNext = nullptr;
    memoryRange.memory = handle;
    memoryRange.offset = offset;
    memoryRange.size = size;
    const VkResult result = vkInvalidateMappedMemoryRanges(getNativeDevice(), 1, &memoryRange);
    return (VK_SUCCESS == result);
}

VkDeviceSize DeviceMemory::getCommitment() noexcept
{
    MAGMA_ASSERT(getPropertyFlags() & VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT);
    VkDeviceSize commitedMemoryInBytes = 0ull;
    vkGetDeviceMemoryCommitment(getNativeDevice(), handle, &commitedMemoryInBytes);
    return commitedMemoryInBytes;
}
} // namespace magma
