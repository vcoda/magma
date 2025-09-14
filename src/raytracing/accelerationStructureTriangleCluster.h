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

#define MAGMA_MAX_CLUSTER_TRIANGLE_COUNT 511
#define MAGMA_MAX_CLUSTER_VERTEX_COUNT 511

namespace magma
{
    /* Cluster geometry. */

    template<class Vertex, class Index>
    struct Cluster
    {
        static_assert(std::is_same<Index, uint8_t>::value || std::is_same<Index, uint16_t>::value || std::is_same<Index, uint32_t>::value,
            "index should be of unsigned char, short or int type");

        std::vector<Vertex> vertices;
        std::vector<Index> indices;
        std::vector<uint32_t> geometryIndices = {0};
        VkClusterAccelerationStructureGeometryFlagsNV geometryFlags = 0;
        VkDeviceSize vertexBufferOffset = 0;
        VkDeviceSize indexBufferOffset = 0;
        VkDeviceSize propertyBufferOffset = 0;

        uint32_t findMinGeometryIndex() const noexcept;
        constexpr VkClusterAccelerationStructureIndexFormatFlagBitsNV getIndexFormat() const noexcept;
    };

    /* Describes a cluster acceleration structure. */

    struct AccelerationStructureTriangleCluster : VkClusterAccelerationStructureTriangleClusterInputNV
    {
        constexpr AccelerationStructureTriangleCluster(VkFormat vertexFormat = VK_FORMAT_UNDEFINED) noexcept;
        template<class Vertex, class Index>
        AccelerationStructureTriangleCluster(VkFormat vertexFormat,
            const std::list<Cluster<Vertex, Index>>& clusters);
    };

    /* Describes build operation for a cluster acceleration structure. */

    struct AccelerationStructureBuildTriangleCluster : VkClusterAccelerationStructureBuildTriangleClusterInfoNV
    {
        constexpr AccelerationStructureBuildTriangleCluster(uint32_t clusterID = 0) noexcept;
        template<class Vertex, class Index>
        AccelerationStructureBuildTriangleCluster(const Cluster<Vertex, Index>& cluster,
            uint32_t clusterID) noexcept;
    };

    /* Structure size is dependent on VkClusterAccelerationStructureInputInfoNV::opType. */

    constexpr std::size_t getClusterAccelerationStructureSize(VkClusterAccelerationStructureOpTypeNV opType) noexcept;
    constexpr std::size_t getClusterAccelerationStructureMaxSize() noexcept;
} // namespace magma

#include "accelerationStructureTriangleCluster.inl"
#endif // VK_KHR_acceleration_structure
