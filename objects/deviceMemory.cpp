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
#include "deviceMemory.h"
#include "device.h"
#include "physicalDevice.h"
#include "../allocator/allocator.h"
#include "../exceptions/errorResult.h"

namespace magma
{
DeviceMemory::DeviceMemory(std::shared_ptr<Device> device,
    const VkMemoryRequirements& memoryRequirements, VkMemoryPropertyFlags flags,
    std::shared_ptr<Allocator> allocator /* nullptr */):
    NonDispatchable(VK_OBJECT_TYPE_DEVICE_MEMORY, std::move(device), allocator ? allocator->getHostAllocator() : nullptr),
    memoryRequirements(memoryRequirements),
    flags(flags),
    deviceAllocator(allocator->getDeviceAllocator()),
    memory(nullptr),
    mapped(false)
{
    if (deviceAllocator)
    {
        memory = deviceAllocator->alloc(memoryRequirements, flags);
        handle = deviceAllocator->getMemoryHandle(memory);
    }
    else
    {
        VkMemoryAllocateInfo info;
        info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        info.pNext = nullptr;
        info.allocationSize = memoryRequirements.size;
        info.memoryTypeIndex = getTypeIndex(flags);
        const VkResult allocate = vkAllocateMemory(MAGMA_HANDLE(device), &info, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
        MAGMA_THROW_FAILURE(allocate, "failed to allocate memory");
    }
}

#ifdef VK_KHR_device_group
DeviceMemory::DeviceMemory(std::shared_ptr<Device> device, uint32_t deviceMask,
    const VkMemoryRequirements& memoryRequirements, VkMemoryPropertyFlags flags,
    std::shared_ptr<Allocator> allocator /* nullptr */):
    NonDispatchable(VK_OBJECT_TYPE_DEVICE_MEMORY, std::move(device), allocator->getHostAllocator()),
    memoryRequirements(memoryRequirements),
    flags(flags),
    deviceAllocator(),
    memory(nullptr),
    mapped(false)
{
    VkMemoryAllocateFlagsInfoKHR flagsInfo;
    flagsInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO_KHR;
    flagsInfo.pNext = nullptr;
    flagsInfo.flags = VK_MEMORY_ALLOCATE_DEVICE_MASK_BIT_KHR;
    flagsInfo.deviceMask = deviceMask;
    VkMemoryAllocateInfo info;
    info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    info.pNext = &flagsInfo;
    info.allocationSize = memoryRequirements.size;
    info.memoryTypeIndex = getTypeIndex(flags);
    const VkResult allocate = vkAllocateMemory(MAGMA_HANDLE(device), &info, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(allocate, "failed to allocate memory");
}
#endif // VK_KHR_device_group

DeviceMemory::~DeviceMemory()
{
    MAGMA_ASSERT(!mapped);
    if (memory)
        deviceAllocator->free(memory);
    else
        vkFreeMemory(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}

void *DeviceMemory::map(
    VkDeviceSize offset /* 0 */,
    VkDeviceSize size /* VK_WHOLE_SIZE */,
    VkMemoryMapFlags flags /* 0 */) noexcept
{
    void *data = nullptr;
    VkResult map;
    if (memory)
        map = deviceAllocator->map(memory, offset, &data);
    else
        map = vkMapMemory(MAGMA_HANDLE(device), handle, offset, size, flags, &data);
    mapped = (VK_SUCCESS == map);
    return data;
}

void DeviceMemory::unmap() noexcept
{
    MAGMA_ASSERT(mapped);
    if (memory)
        deviceAllocator->unmap(memory);
    else
        vkUnmapMemory(*device, handle);
    mapped = false;
}

bool DeviceMemory::flushMappedRange(
    VkDeviceSize offset /* 0 */,
    VkDeviceSize size /* VK_WHOLE_SIZE */) noexcept
{
    VkResult flush;
    if (memory)
        flush = deviceAllocator->flushMappedRange(memory, offset, size);
    else
    {
        VkMappedMemoryRange memoryRange;
        memoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        memoryRange.pNext = nullptr;
        memoryRange.memory = handle;
        memoryRange.offset = offset;
        memoryRange.size = size;
        flush = vkFlushMappedMemoryRanges(MAGMA_HANDLE(device), 1, &memoryRange);
    }
    return (VK_SUCCESS == flush);
}

bool DeviceMemory::invalidateMappedRange(
    VkDeviceSize offset /* 0 */,
    VkDeviceSize size /* VK_WHOLE_SIZE */) noexcept
{
    VkResult invalidate;
    if (memory)
        invalidate = deviceAllocator->invalidateMappedRange(memory, offset, size);
    else
    {
        VkMappedMemoryRange memoryRange;
        memoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        memoryRange.pNext = nullptr;
        memoryRange.memory = handle;
        memoryRange.offset = offset;
        memoryRange.size = size;
        invalidate = vkInvalidateMappedMemoryRanges(MAGMA_HANDLE(device), 1, &memoryRange);
    }
    return (VK_SUCCESS == invalidate);
}

uint32_t DeviceMemory::getTypeIndex(VkMemoryPropertyFlags flags) const
{
    const VkPhysicalDeviceMemoryProperties& properties = this->device->getPhysicalDevice()->getMemoryProperties();
    for (uint32_t memTypeIndex = 0; memTypeIndex < properties.memoryTypeCount; ++memTypeIndex)
    {   // Try exact match
        const VkMemoryType& memType = properties.memoryTypes[memTypeIndex];
        if (memType.propertyFlags == flags)
            return memTypeIndex;
    }
    for (uint32_t memTypeIndex = 0; memTypeIndex < properties.memoryTypeCount; ++memTypeIndex)
    {   // Try any suitable memory type
        const VkMemoryType& memType = properties.memoryTypes[memTypeIndex];
        if ((memType.propertyFlags & flags) == flags)
            return memTypeIndex;
    }
    MAGMA_THROW("failed to find suitable memory type");
}
} // namespace magma
