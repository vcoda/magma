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
    /* Meshlet geometry. */

    template<class Vertex, class Index>
    struct Cluster
    {
        std::vector<Vertex> vertices;
        std::vector<Index> indices;
        std::vector<uint32_t> geometryIndices = {0};
    };

    /* Parameters describing a cluster acceleration structure. */

    struct AccelerationStructureTriangleClusterInput : VkClusterAccelerationStructureTriangleClusterInputNV
    {
        constexpr AccelerationStructureTriangleClusterInput(VkFormat vertexFormat = VK_FORMAT_UNDEFINED) noexcept;
        template<class Vertex, class Index>
        AccelerationStructureTriangleClusterInput(VkFormat vertexFormat,
            const std::list<Cluster<Vertex, Index>>& clusters);
    };
} // namespace magma

#include "accelerationStructureTriangleCluster.inl"
#endif // VK_KHR_acceleration_structure
