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
#include "device.h"
#include "physicalDevice.h"
#include "storageBuffer.h"
#include "../misc/extension.h"
#include "../misc/mapBuffer.h"
#include "../allocator/allocator.h"
#include "../exceptions/errorResult.h"

namespace magma
{
#ifdef VK_NV_cluster_acceleration_structure
ClusterAccelerationStructure::ClusterAccelerationStructure(std::shared_ptr<Device> device_,
    VkClusterAccelerationStructureTypeNV type, const void *opInput, uint32_t maxAccelerationStructureCount,
    VkClusterAccelerationStructureOpModeNV opMode, VkBuildAccelerationStructureFlagsKHR buildFlags,
    std::shared_ptr<Allocator> allocator, const Sharing& sharing, const StructureChain& extendedInfo):
    device(std::move(device_)),
    type(type),
    opMode(opMode),
    buildFlags(buildFlags),
    maxAccelerationStructureCount(maxAccelerationStructureCount),
    size(0ull),
    buildScratchSize(0ull),
    updateScratchSize(0ull)
{
    VkClusterAccelerationStructureInputInfoNV clusterAccelerationStructureInputInfo;
    clusterAccelerationStructureInputInfo.sType = VK_STRUCTURE_TYPE_CLUSTER_ACCELERATION_STRUCTURE_INPUT_INFO_NV;
    clusterAccelerationStructureInputInfo.pNext = (void *)extendedInfo.headNode();
    clusterAccelerationStructureInputInfo.maxAccelerationStructureCount = maxAccelerationStructureCount;
    clusterAccelerationStructureInputInfo.flags = buildFlags;
	clusterAccelerationStructureInputInfo.opMode = opMode;
    switch (type)
    {
    case VK_CLUSTER_ACCELERATION_STRUCTURE_TYPE_CLUSTERS_BOTTOM_LEVEL_NV:
        clusterAccelerationStructureInputInfo.opType = VK_CLUSTER_ACCELERATION_STRUCTURE_OP_TYPE_BUILD_CLUSTERS_BOTTOM_LEVEL_NV;
        clusterAccelerationStructureInputInfo.opInput.pClustersBottomLevel = (VkClusterAccelerationStructureClustersBottomLevelInputNV *)opInput;
        break;
    case VK_CLUSTER_ACCELERATION_STRUCTURE_TYPE_TRIANGLE_CLUSTER_NV:
        clusterAccelerationStructureInputInfo.opType = VK_CLUSTER_ACCELERATION_STRUCTURE_OP_TYPE_BUILD_TRIANGLE_CLUSTER_NV;
        clusterAccelerationStructureInputInfo.opInput.pTriangleClusters = (VkClusterAccelerationStructureTriangleClusterInputNV *)opInput;
        break;
    case VK_CLUSTER_ACCELERATION_STRUCTURE_TYPE_TRIANGLE_CLUSTER_TEMPLATE_NV:
        clusterAccelerationStructureInputInfo.opType = VK_CLUSTER_ACCELERATION_STRUCTURE_OP_TYPE_BUILD_TRIANGLE_CLUSTER_TEMPLATE_NV;
        clusterAccelerationStructureInputInfo.opInput.pTriangleClusters = (VkClusterAccelerationStructureTriangleClusterInputNV *)opInput;
        break;
    }
    auto getNativeDevice = [this]() -> VkDevice { return device->getHandle(); };
    VkAccelerationStructureBuildSizesInfoKHR buildSizesInfo = {VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR};
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkGetClusterAccelerationStructureBuildSizesNV, VK_NV_CLUSTER_ACCELERATION_STRUCTURE_EXTENSION_NAME);
	vkGetClusterAccelerationStructureBuildSizesNV(getNativeDevice(), &clusterAccelerationStructureInputInfo, &buildSizesInfo);
    if (VK_CLUSTER_ACCELERATION_STRUCTURE_OP_MODE_IMPLICIT_DESTINATIONS_NV == opMode)
    {   // Allocate storage for implicit data
        implicitData = std::make_unique<AccelerationStructureStorageBuffer>(device, buildSizesInfo.accelerationStructureSize,
            VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR, std::move(allocator), Buffer::Initializer(), sharing);
    }
    Buffer::Initializer initializer;
    initializer.deviceAddress = VK_TRUE; // Buffer::getDeviceAddress() should succeed
    const VkDeviceSize addressesBufferSize = maxAccelerationStructureCount * sizeof(VkDeviceAddress);
    addressesBuffer = std::make_unique<HostStorageBuffer>(device, addressesBufferSize, allocator, initializer, sharing);
    const VkDeviceSize sizesBufferSize = maxAccelerationStructureCount * sizeof(uint32_t);
    sizesBuffer = std::make_unique<HostStorageBuffer>(device, sizesBufferSize, allocator, initializer, sharing);
    size = buildSizesInfo.accelerationStructureSize;
    buildScratchSize = buildSizesInfo.buildScratchSize;
    updateScratchSize = buildSizesInfo.updateScratchSize;
}

ClusterAccelerationStructure::~ClusterAccelerationStructure() {}

BottomLevelClusterAccelerationStructure::BottomLevelClusterAccelerationStructure(std::shared_ptr<Device> device, std::shared_ptr<Buffer> buildBottomLevelInfos,
    uint32_t maxTotalClusterCount, uint32_t maxClusterCountPerAccelerationStructure, uint32_t maxAccelerationStructureCount,
    VkClusterAccelerationStructureOpModeNV opMode, VkBuildAccelerationStructureFlagsKHR buildFlags,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Sharing& sharing /* default */,
    const StructureChain& extendedInfo /* default */):
    ClusterAccelerationStructure(std::move(device), VK_CLUSTER_ACCELERATION_STRUCTURE_TYPE_CLUSTERS_BOTTOM_LEVEL_NV,
        [maxTotalClusterCount, maxClusterCountPerAccelerationStructure]() -> void*
        {   // TODO: check lifetime
            auto clustersBottomLevel = stackalloc(VkClusterAccelerationStructureClustersBottomLevelInputNV, 1);
            clustersBottomLevel->sType = VK_STRUCTURE_TYPE_CLUSTER_ACCELERATION_STRUCTURE_CLUSTERS_BOTTOM_LEVEL_INPUT_NV;
            clustersBottomLevel->pNext = nullptr;
            clustersBottomLevel->maxTotalClusterCount = maxTotalClusterCount;
            clustersBottomLevel->maxClusterCountPerAccelerationStructure = maxClusterCountPerAccelerationStructure;
            return clustersBottomLevel;
        }(),
        maxAccelerationStructureCount, opMode, buildFlags, std::move(allocator), sharing, extendedInfo)
{
    MAGMA_ASSERT(buildBottomLevelInfos->getSize() >= maxAccelerationStructureCount * sizeof(VkClusterAccelerationStructureBuildClustersBottomLevelInfoNV));
    infosArray = std::move(buildBottomLevelInfos);
    clustersBottomLevel.sType = VK_STRUCTURE_TYPE_CLUSTER_ACCELERATION_STRUCTURE_CLUSTERS_BOTTOM_LEVEL_INPUT_NV;
    clustersBottomLevel.pNext = nullptr;
    clustersBottomLevel.maxTotalClusterCount = maxTotalClusterCount;
    clustersBottomLevel.maxClusterCountPerAccelerationStructure = maxClusterCountPerAccelerationStructure;
}

VkClusterAccelerationStructureOpInputNV BottomLevelClusterAccelerationStructure::getOpInput() const noexcept
{
    VkClusterAccelerationStructureOpInputNV opInput;
    opInput.pClustersBottomLevel = (VkClusterAccelerationStructureClustersBottomLevelInputNV *)&clustersBottomLevel;
    return opInput;
}

TriangleClusterAccelerationStructure::TriangleClusterAccelerationStructure(std::shared_ptr<Device> device, std::shared_ptr<Buffer> buildClusterInfos,
    const VkClusterAccelerationStructureTriangleClusterInputNV& triangleClusters_, uint32_t maxClusterAccelerationStructureCount,
    VkClusterAccelerationStructureOpModeNV opMode, VkBuildAccelerationStructureFlagsKHR buildFlags,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Sharing& sharing /* default */,
    const StructureChain& extendedInfo /* default */):
    ClusterAccelerationStructure(std::move(device), VK_CLUSTER_ACCELERATION_STRUCTURE_TYPE_TRIANGLE_CLUSTER_NV,
         &triangleClusters_, maxClusterAccelerationStructureCount, opMode, buildFlags, std::move(allocator), sharing, extendedInfo),
    triangleClusters(triangleClusters_)
{
    MAGMA_ASSERT(buildClusterInfos->getSize() >= maxAccelerationStructureCount * sizeof(VkClusterAccelerationStructureBuildTriangleClusterInfoNV));
    infosArray = std::move(buildClusterInfos);
}

VkClusterAccelerationStructureOpInputNV TriangleClusterAccelerationStructure::getOpInput() const noexcept
{
    VkClusterAccelerationStructureOpInputNV opInput;
    opInput.pTriangleClusters = (VkClusterAccelerationStructureTriangleClusterInputNV *)&triangleClusters;
    return opInput;
}

TriangleClusterAccelerationStructureTemplate::TriangleClusterAccelerationStructureTemplate(std::shared_ptr<Device> device, std::shared_ptr<Buffer> buildClusterTemplateInfos,
    const VkClusterAccelerationStructureTriangleClusterInputNV& triangleClustersTemplate_, uint32_t maxClusterAccelerationStructureCount,
    VkClusterAccelerationStructureOpModeNV opMode, VkBuildAccelerationStructureFlagsKHR buildFlags,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Sharing& sharing /* default */,
    const StructureChain& extendedInfo /* default */):
    ClusterAccelerationStructure(std::move(device), VK_CLUSTER_ACCELERATION_STRUCTURE_TYPE_TRIANGLE_CLUSTER_TEMPLATE_NV,
         &triangleClustersTemplate_, maxClusterAccelerationStructureCount, opMode, buildFlags, std::move(allocator), sharing, extendedInfo),
    triangleClustersTemplate(triangleClustersTemplate_)
{
    MAGMA_ASSERT(buildClusterTemplateInfos->getSize() >= maxAccelerationStructureCount * sizeof(VkClusterAccelerationStructureBuildTriangleClusterTemplateInfoNV));
    infosArray = std::move(buildClusterTemplateInfos);
}

VkClusterAccelerationStructureOpInputNV TriangleClusterAccelerationStructureTemplate::getOpInput() const noexcept
{
    VkClusterAccelerationStructureOpInputNV opInput;
    opInput.pTriangleClusters = (VkClusterAccelerationStructureTriangleClusterInputNV *)&triangleClustersTemplate;
    return opInput;
}

VkDeviceSize calculateClusterCompactBufferSize(lent_ptr<Buffer> dstSizesArray, uint32_t maxAccelerationStructureCount)
{
    std::shared_ptr<PhysicalDevice> physicalDevice = dstSizesArray->getDevice()->getPhysicalDevice();
    const VkPhysicalDeviceClusterAccelerationStructurePropertiesNV clusterAccelerationStructureProperties = physicalDevice->getClusterAccelerationStructureProperties();
    VkDeviceSize compactBufferSize = 0ull;
    map<uint32_t>(std::move(dstSizesArray),
        [&](const uint32_t *clasSizes)
        {
            for (uint32_t i = 0; i < maxAccelerationStructureCount; ++i)
            {
                const uint32_t size = clasSizes[i];
                compactBufferSize += core::alignUp(size, clusterAccelerationStructureProperties.clusterByteAlignment);
            }
        });
    return compactBufferSize;
}

#endif // VK_NV_cluster_acceleration_structure
} // namespace magma
