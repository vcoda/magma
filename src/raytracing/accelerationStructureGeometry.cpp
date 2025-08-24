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
#include "pch.h"
#pragma hdrstop
#include "accelerationStructureGeometry.h"

namespace magma
{
#ifdef VK_KHR_acceleration_structure
AccelerationStructureTriangles::AccelerationStructureTriangles(
    VkFormat vertexFormat, const void *vertices, uint32_t maxVertex,
    std::size_t vertexStride /* 0 */,
    const void *transform /* nullptr */,
    VkGeometryFlagsKHR flags /* 0 */) noexcept:
    AccelerationStructureGeometry(VK_GEOMETRY_TYPE_TRIANGLES_KHR, flags)
{
    MAGMA_ASSERT(vertices);
    MAGMA_ASSERT(maxVertex);
    if (!vertexStride)
        vertexStride = Format(vertexFormat).size();
    geometry.triangles.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR;
    geometry.triangles.pNext = nullptr;
    geometry.triangles.vertexFormat = vertexFormat;
    geometry.triangles.vertexData = address(vertices);
    geometry.triangles.vertexStride = static_cast<VkDeviceSize>(vertexStride);
    geometry.triangles.maxVertex = maxVertex;
    geometry.triangles.indexType = VK_INDEX_TYPE_NONE_KHR;
    geometry.triangles.indexData.deviceAddress = MAGMA_NULL;
    geometry.triangles.transformData.hostAddress = transform;
    primitiveCount = (geometry.triangles.maxVertex + 1) / 3;
    MAGMA_ASSERT(primitiveCount);
}

AccelerationStructureTriangles::AccelerationStructureTriangles(
    VkFormat vertexFormat, const Buffer *vertices,
    std::size_t vertexStride /* 0 */,
    const Buffer *transform /* nullptr */,
    VkGeometryFlagsKHR flags /* 0 */) noexcept:
    AccelerationStructureGeometry(VK_GEOMETRY_TYPE_TRIANGLES_KHR, flags)
{
    MAGMA_ASSERT(vertices);
    if (!vertexStride)
        vertexStride = Format(vertexFormat).size();
    MAGMA_ASSERT(vertices->getSize() % vertexStride == 0);
    geometry.triangles.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR;
    geometry.triangles.pNext = nullptr;
    geometry.triangles.vertexFormat = vertexFormat;
    geometry.triangles.vertexData = address(vertices);
    geometry.triangles.vertexStride = static_cast<VkDeviceSize>(vertexStride);
    geometry.triangles.maxVertex = static_cast<uint32_t>(vertices->getSize() / geometry.triangles.vertexStride) - 1;
    geometry.triangles.indexType = VK_INDEX_TYPE_NONE_KHR;
    geometry.triangles.indexData.deviceAddress = MAGMA_NULL;
    geometry.triangles.transformData = address(transform);
    primitiveCount = (geometry.triangles.maxVertex + 1) / 3;
    MAGMA_ASSERT(primitiveCount);
}

AccelerationStructureIndexedTriangles::AccelerationStructureIndexedTriangles(
    VkFormat vertexFormat, const void *vertices, uint32_t maxVertex,
    VkIndexType indexType, const void *indices, uint32_t indexCount,
    std::size_t vertexStride /* 0 */,
    const void *transform /* nullptr */,
    VkGeometryFlagsKHR flags /* 0 */) noexcept:
    AccelerationStructureGeometry(VK_GEOMETRY_TYPE_TRIANGLES_KHR, flags)
{
    MAGMA_ASSERT(vertices);
    MAGMA_ASSERT(indices);
    MAGMA_ASSERT(indexCount);
    MAGMA_ASSERT(indexCount % 3 == 0);
    if (!vertexStride)
        vertexStride = Format(vertexFormat).size();
    geometry.triangles.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR;
    geometry.triangles.pNext = nullptr;
    geometry.triangles.vertexFormat = vertexFormat;
    geometry.triangles.vertexData = address(vertices);
    geometry.triangles.vertexStride = static_cast<VkDeviceSize>(vertexStride);
    geometry.triangles.maxVertex = maxVertex;
    geometry.triangles.indexType = indexType;
    geometry.triangles.indexData = address(indices);
    geometry.triangles.transformData.hostAddress = transform;
    primitiveCount = indexCount / 3;
    MAGMA_ASSERT(primitiveCount);
}

AccelerationStructureIndexedTriangles::AccelerationStructureIndexedTriangles(
    VkFormat vertexFormat, const Buffer *vertices, VkIndexType indexType, const Buffer *indices,
    std::size_t vertexStride /* 0 */,
    const Buffer *transform /* nullptr */,
    VkGeometryFlagsKHR flags /* 0 */) noexcept:
    AccelerationStructureGeometry(VK_GEOMETRY_TYPE_TRIANGLES_KHR, flags)
{
    MAGMA_ASSERT(vertices);
    MAGMA_ASSERT(indices);
    if (!vertexStride)
        vertexStride = Format(vertexFormat).size();
    MAGMA_ASSERT(vertices->getSize() % vertexStride == 0);
    geometry.triangles.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR;
    geometry.triangles.pNext = nullptr;
    geometry.triangles.vertexFormat = vertexFormat;
    geometry.triangles.vertexData = address(vertices);
    geometry.triangles.vertexStride = static_cast<VkDeviceSize>(vertexStride);
    geometry.triangles.maxVertex = static_cast<uint32_t>(vertices->getSize() / geometry.triangles.vertexStride) - 1;
    geometry.triangles.indexType = indexType;
    geometry.triangles.indexData = address(indices);
    geometry.triangles.transformData = address(transform);
    const size_t indexSize = getIndexSize();
    MAGMA_ASSERT(indices->getSize() % indexSize == 0);
    const VkDeviceSize indexCount = indices->getSize() / indexSize;
    MAGMA_ASSERT(indexCount % 3 == 0);
    primitiveCount = static_cast<uint32_t>(indexCount / 3);
    MAGMA_ASSERT(primitiveCount);
}

size_t AccelerationStructureIndexedTriangles::getIndexSize() const noexcept
{
    switch (geometry.triangles.indexType)
    {
    case VK_INDEX_TYPE_NONE_KHR:
        return 0;
    case VK_INDEX_TYPE_UINT16:
        return sizeof(uint16_t);
    case VK_INDEX_TYPE_UINT32:
        return sizeof(uint32_t);
#if defined(VK_KHR_index_type_uint8)
    case VK_INDEX_TYPE_UINT8_KHR:
        return sizeof(uint8_t);
#elif defined(VK_EXT_index_type_uint8)
    case VK_INDEX_TYPE_UINT8_EXT:
        return sizeof(uint8_t);
#endif // VK_EXT_index_type_uint8
    default:
        MAGMA_FAILURE("invalid index type");
        return 0;
    }
}
#endif // VK_KHR_acceleration_structure
} // namesapce magma
