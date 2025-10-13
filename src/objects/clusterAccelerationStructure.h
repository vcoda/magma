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
#include "../allocator/allocator.h"

namespace magma
{
    class Device;
    class Buffer;

    /* https://docs.vulkan.org/features/latest/features/proposals/VK_NV_cluster_acceleration_structure.html
       https://github.com/KhronosGroup/Vulkan-Docs/blob/main/proposals/VK_NV_cluster_acceleration_structure.adoc */

    /* Base cluster acceleration structure. */

#ifdef VK_NV_cluster_acceleration_structure
    class ClusterAccelerationStructure : public IClass
    {
    public:
        ~ClusterAccelerationStructure();
        VkClusterAccelerationStructureTypeNV getType() const noexcept { return type; }
        VkClusterAccelerationStructureOpModeNV getOpMode() const noexcept { return opMode; }
        VkBuildAccelerationStructureFlagsKHR getBuildFlags() const noexcept { return buildFlags; }
        uint32_t getMaxAccelerationStructureCount() const noexcept { return maxAccelerationStructureCount; }
        VkDeviceSize getSize() const noexcept { return size; }
        VkDeviceSize getBuildScratchSize() const noexcept { return buildScratchSize; }
        VkDeviceSize getUpdateScratchSize() const noexcept { return updateScratchSize; }
        const std::unique_ptr<Buffer>& getImplicitData() const noexcept { return implicitData; }
        const std::unique_ptr<Buffer>& getAddresses() const noexcept { return addressesBuffer; }
        const std::unique_ptr<Buffer>& getSizes() const noexcept { return sizesBuffer; }
        const std::shared_ptr<Buffer>& getInfosArray() const noexcept { return infosArray; }
        virtual VkClusterAccelerationStructureOpInputNV getOpInput() const noexcept = 0;

    protected:
        ClusterAccelerationStructure(std::shared_ptr<Device> device,
            VkClusterAccelerationStructureTypeNV type,
            const void *opInput,
            uint32_t maxAccelerationStructureCount,
            VkClusterAccelerationStructureOpModeNV opMode,
            VkBuildAccelerationStructureFlagsKHR buildFlags,
            std::shared_ptr<Allocator> allocator,
            const Sharing& sharing,
            const StructureChain& extendedInfo);

        const VkClusterAccelerationStructureTypeNV type;
        const VkClusterAccelerationStructureOpModeNV opMode;
        const VkBuildAccelerationStructureFlagsKHR buildFlags;
        const uint32_t maxAccelerationStructureCount;
        VkDeviceSize size;
        VkDeviceSize buildScratchSize;
        VkDeviceSize updateScratchSize;
        std::unique_ptr<Buffer> implicitData;
        std::unique_ptr<Buffer> addressesBuffer;
        std::unique_ptr<Buffer> sizesBuffer;
        std::shared_ptr<Buffer> infosArray;
    };

    /* Cluster Bottom Level Acceleration Structure (CBLAS), constructed
       from references to CLAS structures.

       The Cluster BLAS serves as an alternative to the traditional BLAS.
       The goal is for applications to organize mesh geometry into CLAS
       primitives before creating the Cluster BLAS.

       Usage example:

        struct ClusterizedMesh
        {
            struct Cluster
            {
                ...
            } clusters[N];
            int clusterCount;
        };

        list<ClusterizedMesh> meshes;
        int maxTotalClusterCount = 0;
        int maxClusterCountPerAccelerationStructure = 0;
        int maxAccelerationStructureCount = 0;
        for (auto& mesh: meshes)
        {
            maxTotalClusterCount += mesh.clusterCount;
            maxClusterCountPerAccelerationStructure = std::max(
                maxClusterCountPerAccelerationStructure, mesh.clusterCount);
            ++maxAccelerationStructureCount;
        } */

    class BottomLevelClusterAccelerationStructure : public ClusterAccelerationStructure
    {
    public:
        explicit BottomLevelClusterAccelerationStructure(std::shared_ptr<Buffer> buildBottomLevelInfos,
            uint32_t maxTotalClusterCount,
            uint32_t maxClusterCountPerAccelerationStructure,
            uint32_t maxAccelerationStructureCount,
            VkClusterAccelerationStructureOpModeNV opMode,
            VkBuildAccelerationStructureFlagsKHR buildFlags,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Sharing& sharing = Sharing(),
            const StructureChain& extendedInfo = StructureChain());
        uint32_t getMaxTotalClusterCount() const noexcept { return clustersBottomLevel.maxTotalClusterCount; }
        uint32_t getMaxClusterCountPerAccelerationStructure() const noexcept { return clustersBottomLevel.maxClusterCountPerAccelerationStructure; }
        VkClusterAccelerationStructureOpInputNV getOpInput() const noexcept override;

    private:
        VkClusterAccelerationStructureClustersBottomLevelInputNV clustersBottomLevel;
    };

    /* A CLAS is an intermediate acceleration structure created from
       triangles, which can then be used to build Cluster BLAS.
       To optimize trace performance, geometry should be grouped into
       CLAS based on spatial proximity.

       A CLAS behaves similarly to a BLAS in many respects but has the
       following differences:

        * Triangle and Vertex Limits: A CLAS can contain up to a small
          number of triangles and vertices.
        * TLAS Integration: CLAS cannot be directly included in a TLAS.
          Instead, they are referenced as part of a Cluster BLAS, which
          can be traced.
        * Geometry Indices: Indices in a CLAS can be specified per primitive
          that are local to the CLAS and may be non-consecutive.
        * ClusterID: A CLAS can be assigned a user-defined 32-bit
          ClusterID, which can be accessed from a hit shader.
        * Vertex positions in a CLAS can be quantized for better storage
          by implicitly zeroing a variable number of floating point
          mantissa bits.

       Usage example:

        list<ClusterizedMesh> meshes;
        list<magma::Cluster<Pos4Half, byte>> clusters;
        int maxClusterAccelerationStructureCount = 0;
        for (auto& mesh: meshes)
        {
            for (auto& meshCluster: mesh.clusters)
            {
                magma::Cluster<Pos4Half, byte> cluster = toMagma(meshCluster);
                clusters.push_front(cluster);
                ++maxClusterAccelerationStructureCount;
            }
        }
        magma::TriangleClusterAccelerationStructureInput triangleClusters(VK_FORMAT_R16G16B16A16_SFLOAT, clusters);
    */

    class TriangleClusterAccelerationStructure : public ClusterAccelerationStructure
    {
    public:
        explicit TriangleClusterAccelerationStructure(std::shared_ptr<Buffer> buildClusterInfos,
            const VkClusterAccelerationStructureTriangleClusterInputNV& triangleClusters,
            uint32_t maxClusterAccelerationStructureCount,
            VkClusterAccelerationStructureOpModeNV opMode,
            VkBuildAccelerationStructureFlagsKHR buildFlags,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Sharing& sharing = Sharing(),
            const StructureChain& extendedInfo = StructureChain());
        VkClusterAccelerationStructureOpInputNV getOpInput() const noexcept override;

    private:
        const VkClusterAccelerationStructureTriangleClusterInputNV triangleClusters;
    };

    /* A partially constructed CLAS which can be instantiated to multiple
       cluster level acceleration structures.

       Cluster Templates are designed to efficiently instantiate CLAS
       in memory. During the CLAS instantiation process from a Cluster
       Template, the actual vertex positions are provided, and the
       ClusterID as well as the geometry index can be offset uniformly.
       Cluster Templates perform as much pre-computation as possible that
       is independent of final vertex positions, enabling reuse when
       generating multiple CLAS instances. A Cluster Template is a
       partially constructed CLAS with the following distinctions:

        * It does not store or require vertex position data, however
          it can use it to guide the spatial relationship among triangles.
        * Its size is smaller due to the absence of position information.
        * It cannot be used for tracing or as a basis for building other
          acceleration structures.
        * Bounding box information can be used in combination with the
          ability to zero some of the floating point mantissa bits, to
          optimize the storage of the actual vertices at instantiation.
        * It retains non-positional properties similar to a CLAS, which
          are inherited when the CLAS is instantiated. */

    class TriangleClusterAccelerationStructureTemplate : public ClusterAccelerationStructure
    {
    public:
        explicit TriangleClusterAccelerationStructureTemplate(std::shared_ptr<Buffer> buildClusterTemplateInfos,
            const VkClusterAccelerationStructureTriangleClusterInputNV& triangleClustersTemplate,
            uint32_t maxClusterAccelerationStructureCount,
            VkClusterAccelerationStructureOpModeNV opMode,
            VkBuildAccelerationStructureFlagsKHR buildFlags,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Sharing& sharing = Sharing(),
            const StructureChain& extendedInfo = StructureChain());
        VkClusterAccelerationStructureOpInputNV getOpInput() const noexcept override;

    private:
        const VkClusterAccelerationStructureTriangleClusterInputNV triangleClustersTemplate;
    };

    VkDeviceSize calculateClusterCompactBufferSize(lent_ptr<Buffer> sizesBuffer,
        uint32_t accelerationStructureCount);
#endif // VK_NV_cluster_acceleration_structure
} // namespace magma
