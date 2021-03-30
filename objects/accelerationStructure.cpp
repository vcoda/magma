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
#include "../helpers/stackArray.h"
#include "../exceptions/errorResult.h"

namespace magma
{
#ifdef VK_NV_ray_tracing
AccelerationStructure::AccelerationStructure(std::shared_ptr<Device> device, VkAccelerationStructureTypeNV type,
    uint32_t instanceCount, const std::list<Geometry>& geometries, VkBuildAccelerationStructureFlagsNV flags,
    VkDeviceSize compactedSize, std::shared_ptr<IAllocator> allocator):
    NonDispatchableResource(VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_NV, compactedSize, std::move(device), std::move(allocator)),
    type(type)
{
    MAGMA_ASSERT(geometries.size() > 0);
    info.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_INFO_NV;
    info.pNext = nullptr;
    info.type = type;
    info.flags = flags;
    info.instanceCount = instanceCount;
    info.geometryCount = MAGMA_COUNT(geometries);
    info.pGeometries = new VkGeometryNV[info.geometryCount];
    uint32_t i = 0;
    for (const auto& geometry : geometries)
        (const_cast<VkGeometryNV *>(info.pGeometries))[i++] = geometry;
    VkAccelerationStructureCreateInfoNV createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_NV;
    createInfo.pNext = nullptr;
    createInfo.compactedSize = compactedSize;
    createInfo.info = info;
    MAGMA_DEVICE_EXTENSION(vkCreateAccelerationStructureNV, VK_NV_RAY_TRACING_EXTENSION_NAME);
    const VkResult create = vkCreateAccelerationStructureNV(MAGMA_HANDLE(device), &createInfo, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create acceleration structure");
    const VkMemoryRequirements2 memRequirements = getMemoryRequirements(VK_ACCELERATION_STRUCTURE_MEMORY_REQUIREMENTS_TYPE_OBJECT_NV);
    std::shared_ptr<DeviceMemory> memory(std::make_shared<DeviceMemory>(this->device, 
        memRequirements.memoryRequirements.size, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT));
    bindMemory(std::move(memory));
}

AccelerationStructure::~AccelerationStructure()
{
    MAGMA_DEVICE_EXTENSION(vkDestroyAccelerationStructureNV, VK_NV_RAY_TRACING_EXTENSION_NAME);
    vkDestroyAccelerationStructureNV(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(allocator));
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
    this->offset = offset;
    this->memory = std::move(memory);
}

VkMemoryRequirements2 AccelerationStructure::getMemoryRequirements(VkAccelerationStructureMemoryRequirementsTypeNV type) const
{
    VkAccelerationStructureMemoryRequirementsInfoNV info;
    info.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_MEMORY_REQUIREMENTS_INFO_NV;
    info.pNext = nullptr;
    info.type = type;
    info.accelerationStructure = handle;
    VkMemoryRequirements2 memRequirements = {};
    memRequirements.sType = VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2;
    MAGMA_DEVICE_EXTENSION(vkGetAccelerationStructureMemoryRequirementsNV, VK_NV_RAY_TRACING_EXTENSION_NAME);
    vkGetAccelerationStructureMemoryRequirementsNV(MAGMA_HANDLE(device), &info, &memRequirements);
    return memRequirements;
}

uint64_t AccelerationStructure::getStructureHandle() const
{
    uint64_t structureHandle;
    MAGMA_DEVICE_EXTENSION(vkGetAccelerationStructureHandleNV, VK_NV_RAY_TRACING_EXTENSION_NAME);
    const VkResult get = vkGetAccelerationStructureHandleNV(MAGMA_HANDLE(device), handle, sizeof(structureHandle), &structureHandle);
    MAGMA_THROW_FAILURE(get, "failed to get acceleration structure handle");
    return structureHandle;
}
#endif // VK_NV_ray_tracing
} // namespace magma
