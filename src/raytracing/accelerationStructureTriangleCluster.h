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
#ifdef VK_NV_cluster_acceleration_structure

// Defined in VkClusterAccelerationStructureBuildTriangleClusterInfoNV
#define MAGMA_NUM_CLUSTER_BITS 9

#define MAGMA_MAX_CLUSTER_TRIANGLE_COUNT (1 << MAGMA_NUM_CLUSTER_BITS)
#define MAGMA_MAX_CLUSTER_VERTEX_COUNT (1 << MAGMA_NUM_CLUSTER_BITS)

namespace magma
{
    /* Cluster geometry. */

    template<class Vertex, class Index>
    struct Cluster
    {
        static_assert(std::is_same<Index, uint8_t>::value || std::is_same<Index, uint16_t>::value || std::is_same<Index, uint32_t>::value,
            "index should be of unsigned char, short or int type");

        uint32_t id = 0;
        uint32_t vertexCount = 0;
        std::vector<Index> indices;
        std::vector<uint32_t> geometryIndices = {0};
        VkClusterAccelerationStructureClusterFlagsNV flags = 0;
        VkClusterAccelerationStructureGeometryFlagsNV geometryFlags = 0;
        uint64_t vertexBufferOffset = 0;
        uint64_t indexBufferOffset = 0;
        uint64_t geometryIndexAndFlagsBufferOffset = 0;

        uint32_t findBaseGeometryIndex() const noexcept;
        constexpr VkClusterAccelerationStructureIndexFormatFlagBitsNV getIndexFormat() const noexcept;
    };

    /* Describes a cluster acceleration structure. */

    struct AccelerationStructureTriangleCluster : VkClusterAccelerationStructureTriangleClusterInputNV
    {
        constexpr AccelerationStructureTriangleCluster(VkFormat vertexFormat = VK_FORMAT_UNDEFINED) noexcept;
        template<class Vertex, class Index>
        AccelerationStructureTriangleCluster(VkFormat vertexFormat,
            const std::forward_list<Cluster<Vertex, Index>>& clusters);
    };

    /* Describes build operation for a cluster acceleration structure. */

    struct AccelerationStructureBuildTriangleCluster : VkClusterAccelerationStructureBuildTriangleClusterInfoNV
    {
        constexpr AccelerationStructureBuildTriangleCluster(uint32_t clusterID = 0) noexcept;
        template<class Vertex, class Index>
        AccelerationStructureBuildTriangleCluster(const Cluster<Vertex, Index>& cluster) noexcept;
    };

    /* Instead of true device addresses of vertex/index buffers we store
       offsets in the VkClusterAccelerationStructureBuildTriangleClusterInfoNV.
       When memory is allocated and data is placed in the buffers, we get
       real device addresses and fixup pointers in the triangle clusters. */

    void fixupTriangleClustersBufferAddresses(std::vector<VkClusterAccelerationStructureBuildTriangleClusterInfoNV>& triangleClusters,
        const Buffer *vertexBuffer,
        const Buffer *indexBuffer,
        const Buffer *geometryIndexAndFlagsBuffer = nullptr) noexcept;

    /* Structure size is dependent on VkClusterAccelerationStructureInputInfoNV::opType. */

    constexpr std::size_t getClusterAccelerationStructureSize(VkClusterAccelerationStructureOpTypeNV opType) noexcept;
    constexpr std::size_t getClusterAccelerationStructureMaxSize() noexcept;
} // namespace magma

#include "accelerationStructureTriangleCluster.inl"
#endif // VK_KHR_acceleration_structure
