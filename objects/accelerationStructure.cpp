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
#include "accelerationStructure.h"
#include "device.h"
#include "deviceMemory.h"
#include "../allocator/allocator.h"
#include "../misc/geometry.h"
#include "../misc/deviceExtension.h"
#include "../exceptions/errorResult.h"

namespace magma
{
#ifdef VK_NV_ray_tracing
AccelerationStructure::AccelerationStructure(std::shared_ptr<Device> device, VkAccelerationStructureTypeNV type,
    uint32_t instanceCount, const std::list<Geometry>& geometries, VkBuildAccelerationStructureFlagsNV flags,
    VkDeviceSize compactedSize, std::shared_ptr<Allocator> allocator):
    NonDispatchableResource(VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_NV, device, Sharing(), allocator)
{
    info.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_INFO_NV;
    info.pNext = nullptr;
    info.type = type;
    info.flags = flags;
    info.instanceCount = instanceCount;
    info.geometryCount = MAGMA_COUNT(geometries);
    if (0 == info.geometryCount)
        info.pGeometries = nullptr;
    else
    {
        VkGeometryNV *pGeometries = new VkGeometryNV[info.geometryCount];
        info.pGeometries = pGeometries;
        for (const auto& geometry : geometries)
            *pGeometries++ = geometry;
    }
    VkAccelerationStructureCreateInfoNV createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_NV;
    createInfo.pNext = nullptr;
    createInfo.compactedSize = compactedSize;
    createInfo.info = info;
    MAGMA_DEVICE_EXTENSION(vkCreateAccelerationStructureNV, VK_NV_RAY_TRACING_EXTENSION_NAME);
    const VkResult create = vkCreateAccelerationStructureNV(MAGMA_HANDLE(device), &createInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create acceleration structure");
    const VkMemoryRequirements memoryRequirements = getObjectMemoryRequirements();
    std::shared_ptr<DeviceMemory> memory = std::make_shared<DeviceMemory>(
        std::move(device),
        memoryRequirements, 
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        &handle,
        VK_OBJECT_TYPE_BUFFER,
        std::move(allocator));
    bindMemory(std::move(memory));
}

AccelerationStructure::~AccelerationStructure()
{
    MAGMA_DEVICE_EXTENSION(vkDestroyAccelerationStructureNV, VK_NV_RAY_TRACING_EXTENSION_NAME);
    vkDestroyAccelerationStructureNV(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
    delete[] info.pGeometries;
}

void AccelerationStructure::bindMemory(std::shared_ptr<DeviceMemory> memory,
    const std::vector<uint32_t>& deviceIndices /* {} */,
    VkDeviceSize offset /* 0 */)
{
    VkBindAccelerationStructureMemoryInfoNV info;
    info.sType = VK_STRUCTURE_TYPE_BIND_ACCELERATION_STRUCTURE_MEMORY_INFO_NV;
    info.pNext = nullptr;
    info.accelerationStructure = handle;
    info.memory = *memory;
    info.memoryOffset = offset;
    info.deviceIndexCount = MAGMA_COUNT(deviceIndices);
    info.pDeviceIndices = deviceIndices.data();
    MAGMA_DEVICE_EXTENSION(vkBindAccelerationStructureMemoryNV, VK_NV_RAY_TRACING_EXTENSION_NAME);
    const VkResult bind = vkBindAccelerationStructureMemoryNV(MAGMA_HANDLE(device), 1, &info);
    MAGMA_THROW_FAILURE(bind, "failed to bind acceleration structure memory");
    this->size = memory->getSize();
    this->offset = offset;
    this->memory = std::move(memory);
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
    MAGMA_DEVICE_EXTENSION(vkGetAccelerationStructureHandleNV, VK_NV_RAY_TRACING_EXTENSION_NAME);
    const VkResult get = vkGetAccelerationStructureHandleNV(MAGMA_HANDLE(device), handle, sizeof(uint64_t), &refHandle);
    MAGMA_THROW_FAILURE(get, "failed to get acceleration structure handle");
    return refHandle;
}

VkMemoryRequirements2 AccelerationStructure::getMemoryRequirements(VkAccelerationStructureMemoryRequirementsTypeNV type) const
{
    VkAccelerationStructureMemoryRequirementsInfoNV info;
    info.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_MEMORY_REQUIREMENTS_INFO_NV;
    info.pNext = nullptr;
    info.type = type;
    info.accelerationStructure = handle;
    VkMemoryRequirements2 memoryRequirements = {};
    memoryRequirements.sType = VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2;
    MAGMA_DEVICE_EXTENSION(vkGetAccelerationStructureMemoryRequirementsNV, VK_NV_RAY_TRACING_EXTENSION_NAME);
    vkGetAccelerationStructureMemoryRequirementsNV(MAGMA_HANDLE(device), &info, &memoryRequirements);
    return memoryRequirements;
}
#endif // VK_NV_ray_tracing
} // namespace magma
