/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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
    /* Structure specifying geometries to be built into an acceleration structure. */

    struct AccelerationStructureGeometry : VkAccelerationStructureGeometryKHR
    {
        AccelerationStructureGeometry(VkGeometryTypeKHR geometryType,
            VkGeometryFlagsKHR flags = 0) noexcept;
        uint32_t primitiveCount;
    };

    /* Triangle geometry in a bottom-level acceleration structure. */

    struct AccelerationStructureGeometryTriangles : AccelerationStructureGeometry
    {
        AccelerationStructureGeometryTriangles() noexcept;
        explicit AccelerationStructureGeometryTriangles(VkFormat vertexFormat,
            const void *vertices,
            uint32_t maxVertex,
            const void *transform = nullptr,
            VkGeometryFlagsKHR flags = 0) noexcept;
        explicit AccelerationStructureGeometryTriangles(VkFormat vertexFormat,
            const void *vertices,
            uint32_t maxVertex,
            VkIndexType indexType,
            const void *indices,
            uint32_t indexCount,
            const void *transform = nullptr,
            VkGeometryFlagsKHR flags = 0) noexcept;
        template<class Buffer>
        explicit AccelerationStructureGeometryTriangles(VkFormat vertexFormat,
            const Buffer& vertices,
            const Buffer& transform = nullptr,
            VkGeometryFlagsKHR flags = 0) noexcept;
        template<class Buffer>
        explicit AccelerationStructureGeometryTriangles(VkFormat vertexFormat,
            const Buffer& vertices,
            VkIndexType indexType,
            const Buffer& indices,
            const Buffer& transform = nullptr,
            VkGeometryFlagsKHR flags = 0) noexcept;
        size_t getIndexSize() const noexcept;
    };

    /* Axis-aligned bounding box geometry in a bottom-level acceleration structure. */

    struct AccelerationStructureGeometryAabbs : AccelerationStructureGeometry
    {
        AccelerationStructureGeometryAabbs() noexcept;
        template<class Buffer>
        explicit AccelerationStructureGeometryAabbs(const Buffer& aabbs,
            VkGeometryFlagsKHR flags = 0) noexcept;
    };

    /* Geometry consisting of instances of other acceleration structures. */

    struct AccelerationStructureGeometryInstances : AccelerationStructureGeometry
    {
        AccelerationStructureGeometryInstances() noexcept;
        explicit AccelerationStructureGeometryInstances(uint32_t instanceCount,
            const void *instances,
            VkGeometryFlagsKHR flags = 0) noexcept;
        template<class Buffer>
        explicit AccelerationStructureGeometryInstances(const Buffer& instances,
            VkGeometryFlagsKHR flags = 0) noexcept;
    };
} // namespace magma

#include "accelerationStructureGeometry.inl"
#endif // VK_KHR_acceleration_structure
