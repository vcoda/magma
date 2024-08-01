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
#include "bottomLevelStructure.h"
#include "deferredOperation.h"
#include "../raytracing/accelerationStructureGeometry.h"
#include "../misc/extension.h"

namespace magma
{
#ifdef VK_KHR_acceleration_structure
BottomLevelAccelerationStructure::BottomLevelAccelerationStructure(std::shared_ptr<Device> device,
    const std::forward_list<AccelerationStructureGeometry>& geometries,
    VkAccelerationStructureBuildTypeKHR buildType, VkBuildAccelerationStructureFlagsKHR buildFlags,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    VkAccelerationStructureCreateFlagsKHR flags /* 0 */,
    const StructureChain& extendedInfo /* default */):
    AccelerationStructure(std::move(device), VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR,
        flags, buildType, buildFlags, geometries, std::move(allocator), extendedInfo)
{}

uint64_t BottomLevelAccelerationStructure::getReference() const noexcept
{
    if (VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR == buildType)
        return getDeviceAddress();
    return getObjectHandle();
}

void BottomLevelAccelerationStructure::build(const std::forward_list<AccelerationStructureGeometry>& geometries,
    void *scratchBuffer, std::shared_ptr<DeferredOperation> deferredOperation /* nullptr */)
{
    std::vector<VkAccelerationStructureBuildRangeInfoKHR> buildRanges;
    for (auto const& geometry: geometries)
        buildRanges.push_back({geometry.primitiveCount});
    build(geometries, buildRanges, scratchBuffer, std::move(deferredOperation));
}

void BottomLevelAccelerationStructure::build(const std::forward_list<AccelerationStructureGeometry>& geometries,
    const std::vector<VkAccelerationStructureBuildRangeInfoKHR>& buildRanges, void *scratchBuffer,
    std::shared_ptr<DeferredOperation> deferredOperation /* nullptr */)
{
    const VkResult result = rebuild(VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR,
        geometries, buildRanges, scratchBuffer, std::move(deferredOperation));
    MAGMA_HANDLE_RESULT(result, "failed to build bottom-level acceleration structure");
}

void BottomLevelAccelerationStructure::update(const std::forward_list<AccelerationStructureGeometry>& geometries,
    void *scratchBuffer, std::shared_ptr<DeferredOperation> deferredOperation /* nullptr */)
{
    std::vector<VkAccelerationStructureBuildRangeInfoKHR> buildRanges;
    for (auto const& geometry: geometries)
        buildRanges.push_back({geometry.primitiveCount});
    update(geometries, buildRanges, scratchBuffer, std::move(deferredOperation));
}

void BottomLevelAccelerationStructure::update(const std::forward_list<AccelerationStructureGeometry>& geometries,
    const std::vector<VkAccelerationStructureBuildRangeInfoKHR>& buildRanges, void *scratchBuffer,
    std::shared_ptr<DeferredOperation> deferredOperation /* nullptr */)
{
    const VkResult result = rebuild(VK_BUILD_ACCELERATION_STRUCTURE_MODE_UPDATE_KHR,
        geometries, buildRanges, scratchBuffer, std::move(deferredOperation));
    MAGMA_HANDLE_RESULT(result, "failed to update bottom-level acceleration structure");
}

VkResult BottomLevelAccelerationStructure::rebuild(VkBuildAccelerationStructureModeKHR mode,
    const std::forward_list<AccelerationStructureGeometry>& geometries,
    const std::vector<VkAccelerationStructureBuildRangeInfoKHR>& buildRanges,
    void *scratchBuffer, std::shared_ptr<DeferredOperation> deferredOperation)
{
    const uint32_t geometryCount = (uint32_t)std::distance(geometries.begin(), geometries.end());
    MAGMA_STACK_ARRAY(const VkAccelerationStructureGeometryKHR *, geometryPointers, geometryCount);
    for (auto const& geometry: geometries)
        geometryPointers.put(&geometry);
    VkAccelerationStructureBuildGeometryInfoKHR buildGeometryInfo;
    buildGeometryInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
    buildGeometryInfo.pNext = nullptr;
    buildGeometryInfo.type = VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR,
    buildGeometryInfo.flags = getBuildFlags();
    buildGeometryInfo.mode = mode;
    if (VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR == mode)
        buildGeometryInfo.srcAccelerationStructure = VK_NULL_HANDLE;
    else // VK_BUILD_ACCELERATION_STRUCTURE_MODE_UPDATE_KHR
        buildGeometryInfo.srcAccelerationStructure = handle;
    buildGeometryInfo.dstAccelerationStructure = handle;
    buildGeometryInfo.geometryCount = geometryCount;
    buildGeometryInfo.pGeometries = nullptr;
    buildGeometryInfo.ppGeometries = geometryPointers;
    buildGeometryInfo.scratchData.hostAddress = scratchBuffer;
    MAGMA_ASSERT(buildRanges.size() >= geometryCount);
    const VkAccelerationStructureBuildRangeInfoKHR *buildRangeInfos = buildRanges.data();
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkBuildAccelerationStructuresKHR, VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME);
    return vkBuildAccelerationStructuresKHR(getNativeDevice(), MAGMA_OPTIONAL_HANDLE(deferredOperation),
        1, &buildGeometryInfo, &buildRangeInfos);
}
#endif // VK_KHR_acceleration_structure
} // namespace magma
