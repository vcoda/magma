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
    class Device;
    class Buffer;

    /* https://docs.vulkan.org/features/latest/features/proposals/VK_NV_cluster_acceleration_structure.html
       https://github.com/KhronosGroup/Vulkan-Docs/blob/main/proposals/VK_NV_cluster_acceleration_structure.adoc */

#ifdef VK_NV_cluster_acceleration_structure

    /* A CLAS is an intermediate acceleration structure created from triangles,
       which can then be used to build Cluster BLAS. The Cluster BLAS serves
       as an alternative to the traditional BLAS. The goal is for applications
       to organize mesh geometry into CLAS primitives before creating the
       Cluster BLAS. To optimize trace performance, geometry should be grouped
       into CLAS based on spatial proximity. */

    class ClusterAccelerationStructure : public IClass
    {
    public:
        ~ClusterAccelerationStructure();
        VkClusterAccelerationStructureTypeNV getType() const noexcept { return type; }
        VkClusterAccelerationStructureOpModeNV getOpMode() const noexcept { return opMode; }
        VkBuildAccelerationStructureFlagsKHR getBuildFlags() const noexcept { return buildFlags; }
        uint32_t getMaxAccelerationStructureCount() const noexcept { return maxAccelerationStructureCount; }
        const std::unique_ptr<Buffer>& getImplicitData() const noexcept { return implicitData; }
        virtual VkClusterAccelerationStructureOpInputNV getOpInput() const noexcept = 0;

    protected:
        ClusterAccelerationStructure(std::shared_ptr<Device> device,
            VkClusterAccelerationStructureTypeNV type,
            VkClusterAccelerationStructureOpModeNV opMode,
            VkBuildAccelerationStructureFlagsKHR buildFlags,
            uint32_t maxAccelerationStructureCount,
            const void *opInput,
            std::shared_ptr<Allocator> allocator,
            const Sharing& sharing,
            const StructureChain& extendedInfo);

        std::shared_ptr<Device> device;
        const VkClusterAccelerationStructureTypeNV type;
        const VkClusterAccelerationStructureOpModeNV opMode;
        const VkBuildAccelerationStructureFlagsKHR buildFlags;
        const uint32_t maxAccelerationStructureCount;
        std::unique_ptr<Buffer> implicitData;
    };

    /* A bottom level cluster acceleration structure.
       Used to build multiple bottom level acceleration structures
       from multiple cluster level acceleration structures.

       Typical calculation of input parameters may be done like this:

        constexpr int MaxClustersPerMesh = 100;

       Typical calculation of input parameters may be done like this:

        struct ClusterizedMesh
        {
            struct Cluster
            {
                uint32_t vertexCount, indexCount;
                ...
            } clusters[N];
            uint32_t clusterCount;
        };

        std::list<ClusterizedMesh> meshes;
        uint32_t maxTotalClusterCount = 0;
        uint32_t maxClusterCountPerAccelerationStructure = 0;
        uint32_t maxAccelerationStructureCount = 0;

        for (auto& mesh: meshes)
        {
            maxTotalClusterCount += mesh.clusterCount;
            maxClusterCountPerAccelerationStructure = std::max(maxClusterCountPerAccelerationStructure, mesh.clusterCount);
            ++maxAccelerationStructureCount;
        } */

    class BottomLevelClusterAcccelerationStructure : public ClusterAccelerationStructure
    {
    public:
        explicit BottomLevelClusterAcccelerationStructure(std::shared_ptr<Device> device,
            VkClusterAccelerationStructureOpModeNV opMode,
            VkBuildAccelerationStructureFlagsKHR buildFlags,
            uint32_t maxTotalClusterCount,
            uint32_t maxClusterCountPerAccelerationStructure,
            uint32_t maxAccelerationStructureCount,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Sharing& sharing = Sharing(),
            const StructureChain& extendedInfo = StructureChain());
        uint32_t getMaxTotalClusterCount() const noexcept { return clustersBottomLevel.maxTotalClusterCount; }
        uint32_t getMaxClusterCountPerAccelerationStructure() const noexcept { return clustersBottomLevel.maxClusterCountPerAccelerationStructure; }
        VkClusterAccelerationStructureOpInputNV getOpInput() const noexcept override;

    private:
        VkClusterAccelerationStructureClustersBottomLevelInputNV clustersBottomLevel;
    };

    /* A cluster acceleration structure. */

    class TriangleClusterAccelerationStructure : public ClusterAccelerationStructure
    {
    public:
        explicit TriangleClusterAccelerationStructure(std::shared_ptr<Device> device,
            VkClusterAccelerationStructureOpModeNV opMode,
            VkBuildAccelerationStructureFlagsKHR buildFlags,
            const VkClusterAccelerationStructureTriangleClusterInputNV& triangleClusters,
            uint32_t maxAccelerationStructureCount,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Sharing& sharing = Sharing(),
            const StructureChain& extendedInfo = StructureChain());
        VkClusterAccelerationStructureOpInputNV getOpInput() const noexcept override;

    private:
        const VkClusterAccelerationStructureTriangleClusterInputNV triangleClusters;
    };

    /* A cluster acceleration structure template. */

    class TriangleClusterAccelerationStructureTemplate : public ClusterAccelerationStructure
    {
    public:
        explicit TriangleClusterAccelerationStructureTemplate(std::shared_ptr<Device> device,
            VkClusterAccelerationStructureOpModeNV opMode,
            VkBuildAccelerationStructureFlagsKHR buildFlags,
            const VkClusterAccelerationStructureTriangleClusterInputNV& triangleClustersTemplate,
            uint32_t maxAccelerationStructureCount,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Sharing& sharing = Sharing(),
            const StructureChain& extendedInfo = StructureChain());
        VkClusterAccelerationStructureOpInputNV getOpInput() const noexcept override;

    private:
        const VkClusterAccelerationStructureTriangleClusterInputNV triangleClusters;
    };
#endif // VK_NV_cluster_acceleration_structure
} // namespace magma
