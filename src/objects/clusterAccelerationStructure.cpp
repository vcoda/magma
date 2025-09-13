/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2025 Victor Coda.

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
#include "clusterAccelerationStructure.h"
#include "buffer.h"
#include "../misc/extension.h"
#include "../allocator/allocator.h"
#include "../exceptions/errorResult.h"

namespace magma
{
#ifdef VK_NV_cluster_acceleration_structure
ClusterAccelerationStructure::ClusterAccelerationStructure(std::shared_ptr<Device> device,
    VkClusterAccelerationStructureTypeNV type,
    VkBuildAccelerationStructureFlagsKHR buildFlags,
    const VkClusterAccelerationStructureTriangleClusterInputNV& triangleClusters_,
    VkClusterAccelerationStructureOpModeNV opMode):
    type(type),
    opMode(opMode),
    triangleClusters(triangleClusters_)
{
    auto getNativeDevice = [device]() -> VkDevice { return device->getHandle(); };
    VkClusterAccelerationStructureInputInfoNV clusterAccelerationStructureInputInfo;
    clusterAccelerationStructureInputInfo.sType = VK_STRUCTURE_TYPE_CLUSTER_ACCELERATION_STRUCTURE_INPUT_INFO_NV;
    clusterAccelerationStructureInputInfo.pNext = nullptr;
    clusterAccelerationStructureInputInfo.maxAccelerationStructureCount = 0;
	clusterAccelerationStructureInputInfo.flags = buildFlags;
	clusterAccelerationStructureInputInfo.opType = VK_CLUSTER_ACCELERATION_STRUCTURE_OP_TYPE_BUILD_TRIANGLE_CLUSTER_NV;
	clusterAccelerationStructureInputInfo.opMode = opMode;
	clusterAccelerationStructureInputInfo.opInput.pTriangleClusters = &triangleClusters;
    VkAccelerationStructureBuildSizesInfoKHR buildSizesInfo = {VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR};
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkGetClusterAccelerationStructureBuildSizesNV, VK_NV_CLUSTER_ACCELERATION_STRUCTURE_EXTENSION_NAME);
    vkGetClusterAccelerationStructureBuildSizesNV(getNativeDevice(), &clusterAccelerationStructureInputInfo, &buildSizesInfo);
    accelerationStructureSize = buildSizesInfo.accelerationStructureSize;
    updateScratchSize = buildSizesInfo.updateScratchSize;
    buildScratchSize = buildSizesInfo.buildScratchSize;
}
#endif // VK_NV_cluster_acceleration_structure
} // namespace magma
