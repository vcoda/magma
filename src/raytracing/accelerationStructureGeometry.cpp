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
#include "pch.h"
#pragma hdrstop
#include "accelerationStructureGeometry.h"

namespace magma
{
#ifdef VK_KHR_acceleration_structure
AccelerationStructureGeometryTriangles::AccelerationStructureGeometryTriangles() noexcept:
    AccelerationStructureGeometry(VK_GEOMETRY_TYPE_TRIANGLES_KHR)
{
    geometry.triangles.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR;
    geometry.triangles.pNext = nullptr;
    geometry.triangles.vertexFormat = VK_FORMAT_UNDEFINED;
    geometry.triangles.vertexData.deviceAddress = MAGMA_NULL;
    geometry.triangles.vertexStride = 0;
    geometry.triangles.maxVertex = 0;
    geometry.triangles.indexType = VK_INDEX_TYPE_NONE_KHR,
    geometry.triangles.indexData.deviceAddress = MAGMA_NULL;
    geometry.triangles.transformData.deviceAddress = MAGMA_NULL;
}

size_t AccelerationStructureGeometryTriangles::getIndexSize() const noexcept
{
    switch (geometry.triangles.indexType)
    {
    case VK_INDEX_TYPE_NONE_KHR:
        return 0;
    case VK_INDEX_TYPE_UINT16:
        return sizeof(uint16_t);
    case VK_INDEX_TYPE_UINT32:
        return sizeof(uint32_t);
#ifdef VK_KHR_index_type_uint8
    case VK_INDEX_TYPE_UINT8_KHR:
        return sizeof(uint8_t);
#endif // VK_KHR_index_type_uint8
#ifdef VK_EXT_index_type_uint8
    case VK_INDEX_TYPE_UINT8_EXT:
        return sizeof(uint8_t);
#endif // VK_EXT_index_type_uint8
    default:
        MAGMA_ASSERT(false);
        return 0;
    }
}
#endif // VK_KHR_acceleration_structure
} // namesapce magma
