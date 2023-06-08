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
#include "managedDeviceMemory.h"
#include "device.h"
#include "physicalDevice.h"
#include "../allocator/allocator.h"
#include "../misc/extProcAddress.h"
#include "../exceptions/errorResult.h"
#include "../exceptions/notImplemented.h"

namespace magma
{
ManagedDeviceMemory::ManagedDeviceMemory(std::shared_ptr<Device> device, const VkMemoryRequirements& memoryRequirements,
    VkMemoryPropertyFlags flags, float priority_, NonDispatchableHandle object, VkObjectType objectType,
    std::shared_ptr<IAllocator> hostAllocator, std::shared_ptr<IDeviceMemoryAllocator> deviceAllocator_):
    BaseDeviceMemory(std::move(device), memoryRequirements, flags, priority_, 0, std::move(hostAllocator)),
    deviceAllocator(std::move(deviceAllocator_)),
    allocation(deviceAllocator->alloc(memoryRequirements, flags, priority, object, objectType))
{
    onDefragment();
}

ManagedDeviceMemory::~ManagedDeviceMemory()
{
    deviceAllocator->free(allocation);
}

void ManagedDeviceMemory::realloc(VkDeviceSize newSize, float priority,
    NonDispatchableHandle object, VkObjectType objectType)
{
    MAGMA_ASSERT(!mapped());
    if (mapped())
        unmap();
    deviceAllocator->free(allocation);
    allocation = nullptr;
    handle = VK_NULL_HANDLE;
    memoryRequirements.size = newSize;
    allocation = deviceAllocator->alloc(memoryRequirements, flags, clampPriority(priority), object, objectType);
    onDefragment();
}

void ManagedDeviceMemory::bind(NonDispatchableHandle object, VkObjectType objectType,
    VkDeviceSize offset /* 0 */)
{
    VkResult result;
    switch (objectType)
    {
    case VK_OBJECT_TYPE_BUFFER:
    case VK_OBJECT_TYPE_IMAGE:
        result = deviceAllocator->bindMemory(allocation, offset, object, objectType);
        break;
#ifdef VK_NV_ray_tracing
    case VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_NV:
        {
            VkBindAccelerationStructureMemoryInfoNV bindAccelerationStructureMemoryInfo;
            bindAccelerationStructureMemoryInfo.sType = VK_STRUCTURE_TYPE_BIND_ACCELERATION_STRUCTURE_MEMORY_INFO_NV;
            bindAccelerationStructureMemoryInfo.pNext = nullptr;
            bindAccelerationStructureMemoryInfo.accelerationStructure = core::reinterpret<VkAccelerationStructureNV>(object);
            bindAccelerationStructureMemoryInfo.memory = handle;
            bindAccelerationStructureMemoryInfo.memoryOffset = subOffset + offset;
            bindAccelerationStructureMemoryInfo.deviceIndexCount = 0;
            bindAccelerationStructureMemoryInfo.pDeviceIndices = nullptr;
            MAGMA_DEVICE_EXTENSION(vkBindAccelerationStructureMemoryNV);
            result = vkBindAccelerationStructureMemoryNV(MAGMA_HANDLE(device), 1, &bindAccelerationStructureMemoryInfo);
        }
        break;
#endif // VK_NV_ray_tracing
    default:
        MAGMA_THROW_NOT_IMPLEMENTED;
    }
    MAGMA_THROW_FAILURE(result, "failed to bind device memory");
}

void *ManagedDeviceMemory::map(
    VkDeviceSize offset /* 0 */,
    VkDeviceSize size /* VK_WHOLE_SIZE */,
    VkMemoryMapFlags flags /* 0 */) noexcept
{
    MAGMA_ASSERT(hostVisible());
    MAGMA_UNUSED(size);
    MAGMA_UNUSED(flags);
    if (!mapPointer)
    {
        const VkResult result = deviceAllocator->map(allocation, offset, &mapPointer);
        if (result != VK_SUCCESS)
        {   // VK_ERROR_OUT_OF_HOST_MEMORY
            // VK_ERROR_OUT_OF_DEVICE_MEMORY
            // VK_ERROR_MEMORY_MAP_FAILED
            return nullptr;
        }
    }
    return mapPointer;
}

void ManagedDeviceMemory::unmap() noexcept
{
    MAGMA_ASSERT(hostVisible());
    if (mapPointer)
    {
        deviceAllocator->unmap(allocation);
        mapPointer = nullptr;
    }
}

bool ManagedDeviceMemory::flushMappedRange(
    VkDeviceSize offset /* 0 */,
    VkDeviceSize size /* VK_WHOLE_SIZE */) noexcept
{
    const VkResult result = deviceAllocator->flushMappedRange(allocation, offset, size);
    return (VK_SUCCESS == result);
}

bool ManagedDeviceMemory::invalidateMappedRange(
    VkDeviceSize offset /* 0 */,
    VkDeviceSize size /* VK_WHOLE_SIZE */) noexcept
{
    VkResult result = deviceAllocator->invalidateMappedRange(allocation, offset, size);
    return (VK_SUCCESS == result);
}

void ManagedDeviceMemory::onDefragment() noexcept
{
    const MemoryBlockInfo memoryInfo = deviceAllocator->getMemoryBlockInfo(allocation);
    // The following can be changed after call to vmaDefragment()
    // if allocation is passed to the function, or if allocation is lost:
    handle = memoryInfo.deviceMemory;
    subOffset = memoryInfo.offset;
}
} // namespace magma
