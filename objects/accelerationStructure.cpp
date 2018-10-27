/*
Magma - C++1x interface over Khronos Vulkan API.
Copyright (C) 2018 Victor Coda.

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
#include "accelerationStructure.h"
#include "device.h"
#include "deviceMemory.h"
#include "../allocator/allocator.h"
#include "../misc/geometry.h"
#include "../misc/deviceExtension.h"
#include "../helpers/stackArray.h"

namespace magma
{
AccelerationStructure::AccelerationStructure(std::shared_ptr<Device> device, VkAccelerationStructureTypeNVX type,
    uint32_t instanceCount, const std::list<Geometry>& geometries, VkBuildAccelerationStructureFlagsNVX flags,
    VkDeviceSize compactedSize, std::shared_ptr<IAllocator> allocator):
    NonDispatchable(VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_NVX, std::move(device), std::move(allocator))
{
    VkAccelerationStructureCreateInfoNVX info;
    info.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_NVX;
    info.pNext = nullptr;
    info.type = type;
    info.flags = flags;
    info.compactedSize = compactedSize;
    info.instanceCount = instanceCount;
    info.geometryCount = MAGMA_COUNT(geometries);
    MAGMA_STACK_ARRAY(VkGeometryNVX, dereferencedGeometries, geometries.size());
    for (const auto& geometry : geometries)
        dereferencedGeometries.put(geometry);
    info.pGeometries = dereferencedGeometries;
    MAGMA_DEVICE_EXTENSION(vkCreateAccelerationStructureNVX, VK_NVX_RAYTRACING_EXTENSION_NAME);
    const VkResult create = vkCreateAccelerationStructureNVX(MAGMA_HANDLE(device), &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create acceleration structure");
}

AccelerationStructure::~AccelerationStructure()
{
    MAGMA_DEVICE_EXTENSION(vkDestroyAccelerationStructureNVX, VK_NVX_RAYTRACING_EXTENSION_NAME);
    vkDestroyAccelerationStructureNVX(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(allocator));
}

void AccelerationStructure::bindMemory(std::shared_ptr<DeviceMemory> memory, const std::vector<uint32_t>& deviceIndices,
    VkDeviceSize offset /* 0 */)
{
    VkBindAccelerationStructureMemoryInfoNVX info;
    info.sType = VK_STRUCTURE_TYPE_BIND_ACCELERATION_STRUCTURE_MEMORY_INFO_NVX;
    info.pNext = nullptr;
    info.accelerationStructure = handle;
    info.memory = *memory;
    info.memoryOffset = offset;
    info.deviceIndexCount = MAGMA_COUNT(deviceIndices);
    info.pDeviceIndices = nullptr;
    MAGMA_DEVICE_EXTENSION(vkBindAccelerationStructureMemoryNVX, VK_NVX_RAYTRACING_EXTENSION_NAME);
    const VkResult bind = vkBindAccelerationStructureMemoryNVX(MAGMA_HANDLE(device), 1, &info);
    MAGMA_THROW_FAILURE(bind, "failed to bind acceleration structure memory");
    this->memory = std::move(memory);
}

VkMemoryRequirements2 AccelerationStructure::getMemoryRequirements() const
{
    VkAccelerationStructureMemoryRequirementsInfoNVX info;
    info.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_MEMORY_REQUIREMENTS_INFO_NVX;
    info.pNext = nullptr;
    info.accelerationStructure = handle;
    VkMemoryRequirements2 memoryRequirements;
    MAGMA_DEVICE_EXTENSION(vkGetAccelerationStructureMemoryRequirementsNVX, VK_NVX_RAYTRACING_EXTENSION_NAME);
    vkGetAccelerationStructureMemoryRequirementsNVX(MAGMA_HANDLE(device), &info, &memoryRequirements);
    return memoryRequirements;
}

VkMemoryRequirements2 AccelerationStructure::getScratchMemoryRequirements() const
{
    VkAccelerationStructureMemoryRequirementsInfoNVX info;
    info.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_MEMORY_REQUIREMENTS_INFO_NVX;
    info.pNext = nullptr;
    info.accelerationStructure = handle;
    VkMemoryRequirements2 memoryRequirements;
    MAGMA_DEVICE_EXTENSION(vkGetAccelerationStructureScratchMemoryRequirementsNVX, VK_NVX_RAYTRACING_EXTENSION_NAME);
    vkGetAccelerationStructureScratchMemoryRequirementsNVX(MAGMA_HANDLE(device), &info, &memoryRequirements);
    return memoryRequirements;
}

uint64_t AccelerationStructure::getStructureHandle() const
{
    uint64_t structureHandle;
    MAGMA_DEVICE_EXTENSION(vkGetAccelerationStructureHandleNVX, VK_NVX_RAYTRACING_EXTENSION_NAME);
    const VkResult get = vkGetAccelerationStructureHandleNVX(MAGMA_HANDLE(device), handle, sizeof(structureHandle), &structureHandle);
    MAGMA_THROW_FAILURE(get, "failed to get acceleration structure handle");
    return structureHandle;
}
} // namespace magma
