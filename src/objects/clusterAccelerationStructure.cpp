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
#include "storageBuffer.h"
#include "../raytracing/accelerationStructureTriangleCluster.h"
#include "../misc/extension.h"
#include "../allocator/allocator.h"
#include "../exceptions/errorResult.h"

namespace magma
{
#ifdef VK_NV_cluster_acceleration_structure
ClusterAccelerationStructure::ClusterAccelerationStructure(std::shared_ptr<Device> device,
    VkClusterAccelerationStructureTypeNV type,
    VkBuildAccelerationStructureFlagsKHR buildFlags,
    const VkClusterAccelerationStructureTriangleClusterInputNV& triangleClusters,
    uint32_t maxAccelerationStructureCount,
    uint32_t maxTotalClusterCount, // ???
    uint32_t maxClusterCountPerAccelerationStructure,
    VkClusterAccelerationStructureOpModeNV opMode,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Sharing& sharing /* default */):
    type(type),
    opMode(opMode),
    maxAccelerationStructureCount(maxAccelerationStructureCount),
    triangleClustersInput(triangleClusters)
{
    auto getNativeDevice = [device]() -> VkDevice { return device->getHandle(); };
    // Parameters to build a regular or templated cluster acceleration structure
    VkClusterAccelerationStructureInputInfoNV triangleClustersInfo, clustersBottomLevelInfo, moveObjectsInfo;
    triangleClustersInfo.sType = VK_STRUCTURE_TYPE_CLUSTER_ACCELERATION_STRUCTURE_INPUT_INFO_NV;
    triangleClustersInfo.pNext = nullptr;
    triangleClustersInfo.maxAccelerationStructureCount = maxAccelerationStructureCount;
	triangleClustersInfo.flags = buildFlags;
	triangleClustersInfo.opType = VK_CLUSTER_ACCELERATION_STRUCTURE_OP_TYPE_BUILD_TRIANGLE_CLUSTER_NV;
	triangleClustersInfo.opMode = opMode;
	triangleClustersInfo.opInput.pTriangleClusters = &triangleClustersInput;
    // Parameters to build multiple bottom level acceleration structures from multiple cluster level acceleration structures
    clustersBottomLevelInput.sType = VK_STRUCTURE_TYPE_CLUSTER_ACCELERATION_STRUCTURE_CLUSTERS_BOTTOM_LEVEL_INPUT_NV;
    clustersBottomLevelInput.pNext = nullptr;
    clustersBottomLevelInput.maxTotalClusterCount = maxTotalClusterCount;
    clustersBottomLevelInput.maxClusterCountPerAccelerationStructure = maxClusterCountPerAccelerationStructure;
    clustersBottomLevelInfo.sType = VK_STRUCTURE_TYPE_CLUSTER_ACCELERATION_STRUCTURE_INPUT_INFO_NV;
    clustersBottomLevelInfo.pNext = nullptr;
    clustersBottomLevelInfo.maxAccelerationStructureCount = 0; // accelInfo.maxAccelerationStructureCount = meshes.size();
    clustersBottomLevelInfo.flags = buildFlags;
    clustersBottomLevelInfo.opType = VK_CLUSTER_ACCELERATION_STRUCTURE_OP_TYPE_BUILD_CLUSTERS_BOTTOM_LEVEL_NV;
	clustersBottomLevelInfo.opMode = opMode;
    clustersBottomLevelInfo.opInput.pClustersBottomLevel = &clustersBottomLevelInput;
    // Upper threshold on the number of bytes moved and the type of acceleration structure being moved
    moveObjectsInput.sType = VK_STRUCTURE_TYPE_CLUSTER_ACCELERATION_STRUCTURE_MOVE_OBJECTS_INPUT_NV;
    moveObjectsInput.pNext = nullptr;
    moveObjectsInput.type = VK_CLUSTER_ACCELERATION_STRUCTURE_TYPE_TRIANGLE_CLUSTER_NV;
    moveObjectsInput.noMoveOverlap = VK_FALSE;
    moveObjectsInput.maxMovedBytes = 0; // Assigned later
    moveObjectsInfo.sType = VK_STRUCTURE_TYPE_CLUSTER_ACCELERATION_STRUCTURE_INPUT_INFO_NV;
    moveObjectsInfo.pNext = nullptr;
    moveObjectsInfo.maxAccelerationStructureCount = triangleClustersInfo.maxAccelerationStructureCount;
    moveObjectsInfo.flags = buildFlags;
    moveObjectsInfo.opType = VK_CLUSTER_ACCELERATION_STRUCTURE_OP_TYPE_MOVE_OBJECTS_NV;
	moveObjectsInfo.opMode = opMode;
    moveObjectsInfo.opInput.pMoveObjects = &moveObjectsInput;
    VkAccelerationStructureBuildSizesInfoKHR triangleClustersBuildSizes = {VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR};
    VkAccelerationStructureBuildSizesInfoKHR clustersBottomLevelBuildSizes = {VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR};
    VkAccelerationStructureBuildSizesInfoKHR moveObjectsBuildSizes = {VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR};
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkGetClusterAccelerationStructureBuildSizesNV, VK_NV_CLUSTER_ACCELERATION_STRUCTURE_EXTENSION_NAME);
    vkGetClusterAccelerationStructureBuildSizesNV(getNativeDevice(), &triangleClustersInfo, &triangleClustersBuildSizes);
	vkGetClusterAccelerationStructureBuildSizesNV(getNativeDevice(), &clustersBottomLevelInfo, &clustersBottomLevelBuildSizes);
    moveObjectsInput.maxMovedBytes = triangleClustersBuildSizes.accelerationStructureSize;
    vkGetClusterAccelerationStructureBuildSizesNV(getNativeDevice(), &moveObjectsInfo, &moveObjectsBuildSizes);
    auto clusterAccelerationStructureBuffer = std::make_unique<AccelerationStructureStorageBuffer>(device,
        triangleClustersBuildSizes.accelerationStructureSize, VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR,
        allocator, Buffer::Initializer(), sharing);
    VkDeviceSize maxAddressesBufferSize = maxAccelerationStructureCount * 16; // TODO: correct size
    Buffer::Initializer initializer;
    initializer.deviceAddress = VK_TRUE;
    addressesBuffer = std::make_unique<DynamicStorageBuffer>(device, maxAddressesBufferSize, false, allocator, nullptr, initializer, sharing);
    sizesBuffer = std::make_unique<DynamicStorageBuffer>(device, maxAccelerationStructureCount * sizeof(VkDeviceSize), false, allocator, nullptr, initializer, sharing);
    const VkDeviceSize maxInfosBufferSize = triangleClustersInfo.maxAccelerationStructureCount * getClusterAccelerationStructureMaxSize();
    infosBuffer = std::make_unique<DynamicStorageBuffer>(device, maxInfosBufferSize, false, allocator, nullptr, initializer, sharing);
}
#endif // VK_NV_cluster_acceleration_structure
} // namespace magma
