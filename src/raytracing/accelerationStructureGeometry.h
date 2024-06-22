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
    /* Triangle geometry in a bottom-level acceleration structure. */

    struct AccelerationStructureGeometryTriangles : VkAccelerationStructureGeometryTrianglesDataKHR
    {
        constexpr AccelerationStructureGeometryTriangles() noexcept;
        template<class InputBuffer>
        explicit AccelerationStructureGeometryTriangles(VkFormat vertexFormat,
            const InputBuffer& vertices,
            const InputBuffer& transform = nullptr) noexcept;
        template<class InputBuffer>
        explicit AccelerationStructureGeometryTriangles(VkFormat vertexFormat,
            const InputBuffer& vertices,
            VkIndexType indexType,
            const InputBuffer& indices,
            const InputBuffer& transform = nullptr) noexcept;
        size_t getIndexSize() const noexcept;

        uint32_t primitiveCount;
    };

    /* Axis-aligned bounding box geometry in a bottom-level acceleration structure. */

    struct AccelerationStructureGeometryAabbs : VkAccelerationStructureGeometryAabbsDataKHR
    {
        AccelerationStructureGeometryAabbs() noexcept;
        template<class InputBuffer>
        explicit AccelerationStructureGeometryAabbs(const InputBuffer& aabbsData,
            VkDeviceSize stride = sizeof(VkAabbPositionsKHR)) noexcept;

        uint32_t primitiveCount;
    };

    /* Geometry consisting of instances of other acceleration structures. */

    struct AccelerationStructureGeometryInstances : VkAccelerationStructureGeometryInstancesDataKHR
    {
        AccelerationStructureGeometryInstances() noexcept;
        template<class InputBuffer>
        explicit AccelerationStructureGeometryInstances(const InputBuffer& instances) noexcept;

        uint32_t primitiveCount;
    };

    /* Geometries to be built into an acceleration structure. */

    struct AccelerationStructureGeometry : VkAccelerationStructureGeometryKHR
    {
        AccelerationStructureGeometry() noexcept;
        explicit AccelerationStructureGeometry(const AccelerationStructureGeometryTriangles& triangles,
            VkGeometryFlagsKHR flags = VK_GEOMETRY_OPAQUE_BIT_KHR,
            const StructureChain& extendedInfo = StructureChain()) noexcept;
        explicit AccelerationStructureGeometry(const AccelerationStructureGeometryAabbs& aabbs,
            VkGeometryFlagsKHR flags = 0,
            const StructureChain& extendedInfo = StructureChain()) noexcept;
        explicit AccelerationStructureGeometry(const AccelerationStructureGeometryInstances& instances,
            VkGeometryFlagsKHR flags = 0,
            const StructureChain& extendedInfo = StructureChain()) noexcept;

        uint32_t primitiveCount;
    };
} // namespace magma

#include "accelerationStructureGeometry.inl"
#endif // VK_KHR_acceleration_structure
