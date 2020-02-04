/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2019 Victor Coda.

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
#include "../misc/exception.h"

namespace magma
{
DeviceMemory::DeviceMemory(std::shared_ptr<Device> device, VkDeviceSize size, VkMemoryPropertyFlags flags,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    NonDispatchable(VK_OBJECT_TYPE_DEVICE_MEMORY, std::move(device), std::move(allocator)),
    size(size),
    flags(flags)
{
    VkMemoryAllocateInfo info;
    info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    info.pNext = nullptr;
    info.allocationSize = size;
    info.memoryTypeIndex = getTypeIndex(flags);
    const VkResult allocate = vkAllocateMemory(MAGMA_HANDLE(device), &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(allocate, "failed to allocate memory");
}

DeviceMemory::DeviceMemory(std::shared_ptr<Device> device, VkDeviceSize size, VkMemoryPropertyFlags flags, uint32_t deviceMask,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    NonDispatchable(VK_OBJECT_TYPE_DEVICE_MEMORY, std::move(device), std::move(allocator)),
    size(size)
{
    VkMemoryAllocateFlagsInfo flagsInfo;
    flagsInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO;
    flagsInfo.pNext = nullptr;
    flagsInfo.flags = VK_MEMORY_ALLOCATE_DEVICE_MASK_BIT;
    flagsInfo.deviceMask = deviceMask;
    VkMemoryAllocateInfo info;
    info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    info.pNext = &flagsInfo;
    info.allocationSize = size;
    info.memoryTypeIndex = getTypeIndex(flags);
    const VkResult allocate = vkAllocateMemory(MAGMA_HANDLE(device), &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(allocate, "failed to allocate memory");
}

DeviceMemory::~DeviceMemory()
{
    vkFreeMemory(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(allocator));
}

void *DeviceMemory::map(
    VkDeviceSize offset /* 0 */,
    VkDeviceSize size /* VK_WHOLE_SIZE */,
    VkMemoryMapFlags flags /* 0 */) noexcept
{
    void *data;
    const VkResult map = vkMapMemory(MAGMA_HANDLE(device), handle, offset, size, flags, &data);
    return (VK_SUCCESS == map) ? data : nullptr;
}

void DeviceMemory::unmap() noexcept
{
    vkUnmapMemory(*device, handle);
}

bool DeviceMemory::flushMappedRange(
    VkDeviceSize offset /* 0 */,
    VkDeviceSize size /* VK_WHOLE_SIZE */) noexcept
{
    VkMappedMemoryRange memRange;
    memRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    memRange.pNext = nullptr;
    memRange.memory = handle;
    memRange.offset = offset;
    memRange.size = size;
    const VkResult flush = vkFlushMappedMemoryRanges(MAGMA_HANDLE(device), 1, &memRange);
    return (VK_SUCCESS == flush);
}

bool DeviceMemory::invalidateMappedRange(
    VkDeviceSize offset /* 0 */,
    VkDeviceSize size /* VK_WHOLE_SIZE */) noexcept
{
    VkMappedMemoryRange memRange;
    memRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    memRange.pNext = nullptr;
    memRange.memory = handle;
    memRange.offset = offset;
    memRange.size = size;
    const VkResult invalidate = vkInvalidateMappedMemoryRanges(MAGMA_HANDLE(device), 1, &memRange);
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
