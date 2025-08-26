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
#include "managedDeviceMemory.h"
#include "device.h"
#include "physicalDevice.h"
#include "../allocator/allocator.h"
#include "../misc/extension.h"
#include "../exceptions/errorResult.h"

namespace magma
{
ManagedDeviceMemory::ManagedDeviceMemory(std::shared_ptr<Device> device,
    VkObjectType objectType, NonDispatchableHandle object,
    const VkMemoryRequirements& memoryRequirements, VkMemoryPropertyFlags flags,
    std::shared_ptr<IAllocator> hostAllocator, std::shared_ptr<IDeviceMemoryAllocator> deviceAllocator_,
    const StructureChain& extendedInfo):
    BaseDeviceMemory(std::move(device), memoryRequirements, flags, std::move(hostAllocator), extendedInfo),
    objectType(objectType),
    deviceAllocator(std::move(deviceAllocator_)),
    allocation(deviceAllocator->allocate(objectType, object, memoryRequirements, flags, extendedInfo)),
    subOffset(deviceAllocator->getMemoryBlockInfo(allocation).offset)
{}

ManagedDeviceMemory::~ManagedDeviceMemory()
{
    if (mapPointer)
        deviceAllocator->unmap(allocation);
    deviceAllocator->free(allocation);
}

void ManagedDeviceMemory::realloc(NonDispatchableHandle object,
    const VkMemoryRequirements& memoryRequirements_,
    const StructureChain& extendedInfo /* default */)
{
    MAGMA_ASSERT(!mapped());
    if (mapPointer)
        deviceAllocator->unmap(allocation);
    deviceAllocator->free(allocation);
    allocation = nullptr;
    subOffset = 0ull;
    memoryRequirements = memoryRequirements_;
    allocation = deviceAllocator->allocate(objectType, object, memoryRequirements, memoryType.propertyFlags, extendedInfo);
    subOffset = deviceAllocator->getMemoryBlockInfo(allocation).offset;
    VkResult result = VK_ERROR_MEMORY_MAP_FAILED;
    if (mapPersistent)
        result = deviceAllocator->map(allocation, mapOffset, &mapPointer);
    if (result != VK_SUCCESS)
    {
        mapPointer = nullptr;
        mapOffset = mapSize = mapFlags = 0;
        mapPersistent = false;
    }
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
            const MemoryBlockInfo memoryInfo = deviceAllocator->getMemoryBlockInfo(allocation);
            VkBindAccelerationStructureMemoryInfoNV bindAccelerationStructureMemoryInfo;
            bindAccelerationStructureMemoryInfo.sType = VK_STRUCTURE_TYPE_BIND_ACCELERATION_STRUCTURE_MEMORY_INFO_NV;
            bindAccelerationStructureMemoryInfo.pNext = nullptr;
            bindAccelerationStructureMemoryInfo.accelerationStructure = core::reinterpret<VkAccelerationStructureNV>(object);
            bindAccelerationStructureMemoryInfo.memory = memoryInfo.deviceMemory;
            bindAccelerationStructureMemoryInfo.memoryOffset = memoryInfo.offset + offset;
            bindAccelerationStructureMemoryInfo.deviceIndexCount = 0;
            bindAccelerationStructureMemoryInfo.pDeviceIndices = nullptr;
            MAGMA_REQUIRED_DEVICE_EXTENSION(vkBindAccelerationStructureMemoryNV, VK_NV_RAY_TRACING_EXTENSION_NAME);
            result = vkBindAccelerationStructureMemoryNV(getNativeDevice(), 1, &bindAccelerationStructureMemoryInfo);
        }
        break;
#endif // VK_NV_ray_tracing
    default:
        MAGMA_FAILURE("unknown type of binded object");
        result = VK_ERROR_UNKNOWN;
    }
    MAGMA_HANDLE_RESULT(result, "failed to bind device memory");
    if (VK_SUCCESS == result)
        binding = object;
}

#ifdef VK_KHR_device_group
void ManagedDeviceMemory::bindDeviceGroup(NonDispatchableHandle object, VkObjectType objectType,
    const std::vector<uint32_t>& deviceIndices,
    const std::vector<VkRect2D>& splitInstanceBindRegions /* empty */,
    VkDeviceSize offset /* 0 */)
{
    MAGMA_UNUSED(object);
    MAGMA_UNUSED(objectType);
    MAGMA_UNUSED(deviceIndices);
    MAGMA_UNUSED(splitInstanceBindRegions);
    MAGMA_UNUSED(offset);
    MAGMA_ERROR("VMA allocator can't allocate memory across the subdevices");
}
#endif // VK_KHR_device_group

void *ManagedDeviceMemory::map(
    VkDeviceSize offset /* 0 */,
    VkDeviceSize size /* VK_WHOLE_SIZE */,
    VkMemoryMapFlags /* mmapFlags = 0 */,
    bool persistently /* false */) noexcept
{
    MAGMA_ASSERT(flags.hostVisible);
    if (mapPersistent)
        return mapPointer;
    if (!mapPointer)
    {
        const VkResult result = deviceAllocator->map(allocation, offset, &mapPointer);
        if (result != VK_SUCCESS)
        {   // VK_ERROR_OUT_OF_HOST_MEMORY
            // VK_ERROR_OUT_OF_DEVICE_MEMORY
            // VK_ERROR_MEMORY_MAP_FAILED
            return nullptr;
        }
        mapOffset = offset;
        mapSize = (VK_WHOLE_SIZE == size) ? getSize() : size;
    }
    if (offset != mapOffset)
    {   // Offset inside mapped sub-allocation
        const ptrdiff_t ptrdiff = static_cast<ptrdiff_t>(offset - mapOffset);
        mapPointer = reinterpret_cast<uint8_t *>(mapPointer) + ptrdiff;
        mapOffset = offset;
        mapPersistent = persistently;
    }
    return mapPointer;
}

void ManagedDeviceMemory::unmap() noexcept
{
    MAGMA_ASSERT(flags.hostVisible);
    if (mapPointer && !mapPersistent)
    {
        deviceAllocator->unmap(allocation);
        mapPointer = nullptr;
        mapOffset = 0;
        mapSize = 0;
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
    const VkResult result = deviceAllocator->invalidateMappedRange(allocation, offset, size);
    return (VK_SUCCESS == result);
}

VkDeviceSize ManagedDeviceMemory::getCommitment() noexcept
{   // With VMA we can't get commitment as VkDeviceMemory may be shared
    // between many blocks, so just return the size of sub-allocation.
    return deviceAllocator->getMemoryBlockInfo(allocation).size;
}

void ManagedDeviceMemory::onDefragment() noexcept
{   // The following can be changed after call to vmaDefragment()
    // if allocation is passed to the function, or if allocation is lost:
    subOffset = deviceAllocator->getMemoryBlockInfo(allocation).offset;
}
} // namespace magma
