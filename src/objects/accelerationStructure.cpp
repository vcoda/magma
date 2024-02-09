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
#include "accelerationStructure.h"
#include "device.h"
#include "deviceMemory.h"
#include "managedDeviceMemory.h"
#include "../allocator/allocator.h"
#include "../misc/geometry.h"
#include "../misc/extension.h"
#include "../exceptions/errorResult.h"

namespace magma
{
#ifdef VK_NV_ray_tracing
AccelerationStructure::AccelerationStructure(std::shared_ptr<Device> device_, VkAccelerationStructureTypeNV type,
    uint32_t instanceCount, const std::list<Geometry>& geometries, VkBuildAccelerationStructureFlagsNV flags,
    VkDeviceSize compactedSize, float memoryPriority, std::shared_ptr<Allocator> allocator):
    NonDispatchableResource(VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_NV, std::move(device), Sharing(), allocator)
{
    MAGMA_UNUSED(memoryPriority);
    VkAccelerationStructureCreateInfoNV accelerationStructureInfo;
    accelerationStructureInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_NV;
    accelerationStructureInfo.pNext = nullptr;
    accelerationStructureInfo.compactedSize = compactedSize;
    accelerationStructureInfo.info.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_INFO_NV;
    accelerationStructureInfo.info.pNext = nullptr;
    accelerationStructureInfo.info.type = type;
    accelerationStructureInfo.info.flags = flags;
    accelerationStructureInfo.info.instanceCount = instanceCount;
    accelerationStructureInfo.info.geometryCount = MAGMA_COUNT(geometries);
    if (0 == accelerationStructureInfo.info.geometryCount)
        accelerationStructureInfo.info.pGeometries = nullptr;
    else
    {
        VkGeometryNV *pGeometries = MAGMA_NEW VkGeometryNV[accelerationStructureInfo.info.geometryCount];
        accelerationStructureInfo.info.pGeometries = pGeometries;
        for (const auto& geometry : geometries)
            *pGeometries++ = geometry;
    }
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkCreateAccelerationStructureNV, VK_NV_RAY_TRACING_EXTENSION_NAME);
    const VkResult result = vkCreateAccelerationStructureNV(MAGMA_HANDLE(device), &accelerationStructureInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create acceleration structure");
    // Allocate acceleration structure memory
    StructureChain extendedMemoryInfo;
    const VkMemoryRequirements memoryRequirements = getObjectMemoryRequirements();
#ifdef VK_EXT_memory_priority
    if (device->extensionEnabled(VK_EXT_MEMORY_PRIORITY_EXTENSION_NAME))
    {
        VkMemoryPriorityAllocateInfoEXT memoryPriorityAllocateInfo;
        memoryPriorityAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_PRIORITY_ALLOCATE_INFO_EXT;
        memoryPriorityAllocateInfo.pNext = nullptr;
        memoryPriorityAllocateInfo.priority = memoryPriority;
        extendedMemoryInfo.addNode(memoryPriorityAllocateInfo);
    }
#endif // VK_EXT_memory_priority
    std::shared_ptr<IDeviceMemory> memory;
    if (MAGMA_DEVICE_ALLOCATOR(allocator))
    {
        memory = std::make_shared<ManagedDeviceMemory>(device,
            VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_NV, handle,
            memoryRequirements, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            MAGMA_HOST_ALLOCATOR(allocator),
            MAGMA_DEVICE_ALLOCATOR(allocator),
            extendedMemoryInfo);
    }
    else
    {
        memory = std::make_shared<DeviceMemory>(device,
            memoryRequirements, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            MAGMA_HOST_ALLOCATOR(allocator),
            extendedMemoryInfo);
    }
    bindMemory(std::move(memory));
    this->accelerationStructureInfo = accelerationStructureInfo.info;
}

AccelerationStructure::~AccelerationStructure()
{
    MAGMA_DEVICE_EXTENSION(vkDestroyAccelerationStructureNV);
    vkDestroyAccelerationStructureNV(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
    delete[] accelerationStructureInfo.pGeometries;
}

void AccelerationStructure::bindMemory(std::shared_ptr<IDeviceMemory> memory_,
    VkDeviceSize offset_ /* 0 */)
{
    memory_->bind(handle, VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_NV, offset_);
    memory = std::move(memory_);
    offset = offset_;
    size = memory->getSize();
}

#ifdef VK_KHR_device_group
void AccelerationStructure::bindMemoryDeviceGroup(std::shared_ptr<IDeviceMemory> memory_,
    const std::vector<uint32_t>& deviceIndices /* {} */,
    const std::vector<VkRect2D>& /* splitInstanceBindRegions */,
    VkDeviceSize offset_ /* 0 */)
{
    memory_->bindDeviceGroup(handle, VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_NV, deviceIndices, {/* unused */}, offset_);
    memory = std::move(memory_);
    offset = offset_;
    size = memory->getSize();
}
#endif // VK_KHR_device_group

void AccelerationStructure::onDefragment()
{
    // Currently there is no allocator that deal with it
}

VkMemoryRequirements AccelerationStructure::getObjectMemoryRequirements() const
{
    return getMemoryRequirements(VK_ACCELERATION_STRUCTURE_MEMORY_REQUIREMENTS_TYPE_OBJECT_NV).memoryRequirements;
}

VkMemoryRequirements AccelerationStructure::getBuildScratchMemoryRequirements() const
{
    return getMemoryRequirements(VK_ACCELERATION_STRUCTURE_MEMORY_REQUIREMENTS_TYPE_BUILD_SCRATCH_NV).memoryRequirements;
}

VkMemoryRequirements AccelerationStructure::getUpdateScratchMemoryRequirements() const
{
    return getMemoryRequirements(VK_ACCELERATION_STRUCTURE_MEMORY_REQUIREMENTS_TYPE_UPDATE_SCRATCH_NV).memoryRequirements;
}

uint64_t AccelerationStructure::getReferenceHandle() const
{
    uint64_t refHandle;
    MAGMA_DEVICE_EXTENSION(vkGetAccelerationStructureHandleNV);
    const VkResult result = vkGetAccelerationStructureHandleNV(MAGMA_HANDLE(device), handle, sizeof(uint64_t), &refHandle);
    MAGMA_HANDLE_RESULT(result, "failed to get acceleration structure handle");
    return refHandle;
}

VkMemoryRequirements2 AccelerationStructure::getMemoryRequirements(VkAccelerationStructureMemoryRequirementsTypeNV type) const
{
    VkMemoryRequirements2 memoryRequirements = {};
    VkAccelerationStructureMemoryRequirementsInfoNV memoryRequirementsInfo;
    memoryRequirementsInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_MEMORY_REQUIREMENTS_INFO_NV;
    memoryRequirementsInfo.pNext = nullptr;
    memoryRequirementsInfo.type = type;
    memoryRequirementsInfo.accelerationStructure = handle;
    memoryRequirements.sType = VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2;
    MAGMA_DEVICE_EXTENSION(vkGetAccelerationStructureMemoryRequirementsNV);
    vkGetAccelerationStructureMemoryRequirementsNV(MAGMA_HANDLE(device), &memoryRequirementsInfo, &memoryRequirements);
    return memoryRequirements;
}
#endif // VK_NV_ray_tracing
} // namespace magma
