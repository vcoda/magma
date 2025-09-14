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
#pragma once
#include "model/nondispatchable.h"
#include "../misc/sharing.h"

namespace magma
{
    class Buffer;

#ifdef VK_NV_cluster_acceleration_structure
    class ClusterAccelerationStructure
    {
    public:
        explicit ClusterAccelerationStructure(std::shared_ptr<Device> device,
            VkClusterAccelerationStructureTypeNV type,
            VkBuildAccelerationStructureFlagsKHR buildFlags,
            const VkClusterAccelerationStructureTriangleClusterInputNV& triangleClusters,
            uint32_t maxAccelerationStructureCount,
            uint32_t maxTotalClusterCount,
            uint32_t maxClusterCountPerAccelerationStructure,
            VkClusterAccelerationStructureOpModeNV opMode,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Sharing& sharing = Sharing());
        VkClusterAccelerationStructureTypeNV getType() const noexcept { return type; }
        VkClusterAccelerationStructureOpModeNV getOpMode() const noexcept { return opMode; }
        uint32_t getMaxAccelerationStructureCount() const noexcept { return maxAccelerationStructureCount; }
        const std::unique_ptr<Buffer>& getClusterAccelerationStructureBuffer() const noexcept { return clusterAccelerationStructureBuffer; }
        const std::unique_ptr<Buffer>& getAddressesBuffer() const noexcept { return addressesBuffer; }
        const std::unique_ptr<Buffer>& getSizesBuffer() const noexcept { return sizesBuffer; }
        const std::unique_ptr<Buffer>& getInfosBuffer() const noexcept { return infosBuffer; }

        VkClusterAccelerationStructureTriangleClusterInputNV *getTriangleClusters() noexcept { return &triangleClustersInput; }
        VkClusterAccelerationStructureClustersBottomLevelInputNV *getClustersBottomLevel() noexcept { return &clustersBottomLevelInput; }

    private:
        const VkClusterAccelerationStructureTypeNV type;
        const VkClusterAccelerationStructureOpModeNV opMode;
        const uint32_t maxAccelerationStructureCount;
        VkClusterAccelerationStructureTriangleClusterInputNV triangleClustersInput;
        VkClusterAccelerationStructureClustersBottomLevelInputNV clustersBottomLevelInput;
        VkClusterAccelerationStructureMoveObjectsInputNV moveObjectsInput;
        std::unique_ptr<Buffer> clusterAccelerationStructureBuffer;
        std::unique_ptr<Buffer> addressesBuffer;
        std::unique_ptr<Buffer> sizesBuffer;
        std::unique_ptr<Buffer> infosBuffer;
    };
#endif // VK_NV_cluster_acceleration_structure
} // namespace magma
