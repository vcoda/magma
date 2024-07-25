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
#include "topLevelStructure.h"
#include "deferredOperation.h"
#include "../raytracing/accelerationStructureGeometry.h"
#include "../misc/extension.h"

namespace magma
{
#ifdef VK_KHR_acceleration_structure
TopLevelAccelerationStructure::TopLevelAccelerationStructure(std::shared_ptr<Device> device,
    const AccelerationStructureGeometryInstances& instances,
    VkAccelerationStructureBuildTypeKHR buildType, VkBuildAccelerationStructureFlagsKHR buildFlags,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    VkAccelerationStructureCreateFlagsKHR flags /* 0 */,
    const StructureChain& extendedInfo /* default */):
    AccelerationStructure(std::move(device), VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR,
        flags, buildType, buildFlags, {instances}, std::move(allocator), extendedInfo)
{}

void TopLevelAccelerationStructure::build(const AccelerationStructureGeometryInstances& instances, void *scratchBuffer,
    std::shared_ptr<DeferredOperation> deferredOperation /* nullptr */)
{
    const VkResult result = rebuild(VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR,
        instances, scratchBuffer, std::move(deferredOperation));
    MAGMA_HANDLE_RESULT(result, "failed to build top-level acceleration structure");
}

void TopLevelAccelerationStructure::update(const AccelerationStructureGeometryInstances& instances, void *scratchBuffer,
    std::shared_ptr<DeferredOperation> deferredOperation /* nullptr */)
{
    const VkResult result = rebuild(VK_BUILD_ACCELERATION_STRUCTURE_MODE_UPDATE_KHR,
        instances, scratchBuffer, std::move(deferredOperation));
    MAGMA_HANDLE_RESULT(result, "failed to update top-level acceleration structure");
}

VkResult TopLevelAccelerationStructure::rebuild(VkBuildAccelerationStructureModeKHR mode,
    const AccelerationStructureGeometryInstances& instances, void *scratchBuffer,
    std::shared_ptr<DeferredOperation> deferredOperation)
{
    VkAccelerationStructureBuildGeometryInfoKHR buildGeometryInfo;
    buildGeometryInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
    buildGeometryInfo.pNext = nullptr;
    buildGeometryInfo.type = VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR;
    buildGeometryInfo.flags = getBuildFlags();
    buildGeometryInfo.mode = mode;
    if (VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR == mode)
        buildGeometryInfo.srcAccelerationStructure = VK_NULL_HANDLE;
    else // VK_BUILD_ACCELERATION_STRUCTURE_MODE_UPDATE_KHR
        buildGeometryInfo.srcAccelerationStructure = handle;
    buildGeometryInfo.dstAccelerationStructure = handle;
    buildGeometryInfo.geometryCount = 1; // If type is VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR, geometryCount must be 1
    buildGeometryInfo.pGeometries = &instances;
    buildGeometryInfo.ppGeometries = nullptr;
    buildGeometryInfo.scratchData.hostAddress = scratchBuffer;
    const VkAccelerationStructureBuildRangeInfoKHR buildRangeInfo = {instances.primitiveCount};
    const VkAccelerationStructureBuildRangeInfoKHR *buildRangeInfos = &buildRangeInfo;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkBuildAccelerationStructuresKHR, VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME);
    return vkBuildAccelerationStructuresKHR(getNativeDevice(), MAGMA_OPTIONAL_HANDLE(deferredOperation),
        1, &buildGeometryInfo, &buildRangeInfos);
}
#endif // VK_KHR_acceleration_structure
} // namespace magma
