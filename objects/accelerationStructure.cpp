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
#include "../allocator/allocator.h"
#include "../misc/deviceExtension.h"
#include "../helpers/stackArray.h"

namespace magma
{
AccelerationStructure::AccelerationStructure(std::shared_ptr<Device> device,
    VkDeviceSize compactedSize,
    uint32_t instanceCount,
    const std::vector<std::shared_ptr<Geometry>>& geometries,
    VkBuildAccelerationStructureFlagsNVX flags /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    NonDispatchable((VkDebugReportObjectTypeEXT)VK_DEBUG_REPORT_OBJECT_TYPE_ACCELERATION_STRUCTURE_NVX_EXT, std::move(device), std::move(allocator))
{
    VkAccelerationStructureCreateInfoNVX info;
    info.sType = (VkStructureType)VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_NVX;
    info.pNext = nullptr;
    info.type = VK_ACCELERATION_STRUCTURE_TYPE_UNKNOWN;
    info.flags = flags;
    info.compactedSize = compactedSize;
    info.instanceCount = instanceCount;
    info.geometryCount = MAGMA_COUNT(geometries);
    info.pGeometries = nullptr; // TODO
    MAGMA_DEVICE_EXTENSION(vkCreateAccelerationStructureNVX, VK_NVX_RAYTRACING_INFO_EXTENSION_NAME);
    const VkResult create = vkCreateAccelerationStructureNVX(MAGMA_HANDLE(device), &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create acceleration structure");
}

AccelerationStructure::~AccelerationStructure()
{
    MAGMA_DEVICE_EXTENSION(vkDestroyAccelerationStructureNVX, VK_NVX_RAYTRACING_INFO_EXTENSION_NAME);
    vkDestroyAccelerationStructureNVX(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(allocator));
}
} // namespace magma
