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
#ifdef VK_KHR_acceleration_structure
#include "../objects/accelerationStructureInputBuffer.h"
#include "../objects/accelerationStructureInstanceBuffer.h"
#include "../misc/format.h"
#include "address.h"

namespace magma
{
    /* Geometries to be built into an acceleration structure. */

    struct AccelerationStructureGeometry : VkAccelerationStructureGeometryKHR
    {
        constexpr AccelerationStructureGeometry(VkGeometryTypeKHR geometryType,
            VkGeometryFlagsKHR flags = 0) noexcept;

        uint32_t primitiveCount = 0;
    };

    /* Triangle geometry in a bottom-level acceleration structure. */

    struct AccelerationStructureTriangles : AccelerationStructureGeometry
    {
        explicit AccelerationStructureTriangles(VkFormat vertexFormat,
            const void *vertices,
            uint32_t maxVertex,
            const void *transform = nullptr,
            VkGeometryFlagsKHR flags = 0) noexcept;
        explicit AccelerationStructureTriangles(VkFormat vertexFormat,
            const Buffer *vertices,
            const Buffer *transform = nullptr,
            VkGeometryFlagsKHR flags = 0) noexcept;
    };

    /* Indexed triangle geometry in a bottom-level acceleration structure. */

    struct AccelerationStructureIndexedTriangles : AccelerationStructureGeometry
    {
        explicit AccelerationStructureIndexedTriangles(VkFormat vertexFormat,
            const void *vertices,
            uint32_t maxVertex,
            VkIndexType indexType,
            const void *indices,
            uint32_t indexCount,
            const void *transform = nullptr,
            VkGeometryFlagsKHR flags = 0) noexcept;
        explicit AccelerationStructureIndexedTriangles(VkFormat vertexFormat,
            const Buffer *vertices,
            VkIndexType indexType,
            const Buffer *indices,
            const Buffer *transform = nullptr,
            VkGeometryFlagsKHR flags = 0) noexcept;
        size_t getIndexSize() const noexcept;
    };

    /* Axis-aligned bounding box. */

    struct Aabb : VkAabbPositionsKHR
    {
        Aabb(): VkAabbPositionsKHR{} {}
        Aabb(float minX, float minY, float minZ,
            float maxX, float maxY, float maxZ) noexcept;
        Aabb(const float min[3], const float max[3]) noexcept;
        Aabb(const float bounds[6]) noexcept;
        template<class Vector>
        Aabb(const Vector& min, const Vector& max) noexcept;
        void translate(float x, float y, float z) noexcept;
        // An inactive AABB is one for which the minimum X coordinate is NaN
        void deactivate() noexcept { minX = std::nanf(""); }
        bool inactive() const noexcept { return std::isnan(minX); }
    };

    /* Axis-aligned bounding box geometry in a bottom-level acceleration structure. */

    struct AccelerationStructureAabbs : AccelerationStructureGeometry
    {
        AccelerationStructureAabbs() noexcept;
        explicit AccelerationStructureAabbs(const Aabb& aabb,
            VkGeometryFlagsKHR flags = 0) noexcept;
        explicit AccelerationStructureAabbs(const std::vector<Aabb>& aabbs,
            VkGeometryFlagsKHR flags = 0) noexcept;
        explicit AccelerationStructureAabbs(const Buffer *aabbs,
            VkGeometryFlagsKHR flags = 0) noexcept;
    };

    /* Geometry consisting of instances of other acceleration structures. */

    struct AccelerationStructureInstances : AccelerationStructureGeometry
    {
        AccelerationStructureInstances() noexcept;
        explicit AccelerationStructureInstances(uint32_t instanceCount,
            const void *instances,
            VkGeometryFlagsKHR flags = 0) noexcept;
        template<class Instance>
        explicit AccelerationStructureInstances(const AccelerationStructureInstanceBuffer<Instance> *instances,
            VkGeometryFlagsKHR flags = 0) noexcept;
    };
} // namespace magma

#include "accelerationStructureGeometry.inl"
#endif // VK_KHR_acceleration_structure
