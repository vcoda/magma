/*
Magma - C++1x interface over Khronos Vulkan API.
Copyright (C) 2018 Victor Coda.

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
#include <cstring>
#include "geometry.h"
#include "../objects/vertexBuffer.h"
#include "../objects/indexBuffer.h"

namespace magma
{
Geometry::Geometry(VkGeometryTypeNVX geometryType, VkGeometryFlagsNVX flags) noexcept
{
    sType = (VkStructureType)VK_STRUCTURE_TYPE_GEOMETRY_NVX;
    pNext = nullptr;
    this->geometryType = geometryType;
    this->flags = flags;
}

GeometryTriangles::GeometryTriangles(std::shared_ptr<Buffer> vertexData, VkDeviceSize vertexOffset, uint32_t vertexCount, VkDeviceSize vertexStride, VkFormat vertexFormat,
    std::shared_ptr<Buffer> indexData, VkDeviceSize indexOffset, uint32_t indexCount, VkIndexType indexType,
    std::shared_ptr<Buffer> transformData, VkDeviceSize transformOffset,
    VkGeometryFlagsNVX flags /* 0 */) noexcept:
    Geometry(VK_GEOMETRY_TYPE_TRIANGLES_NVX, flags)
{
    geometry.triangles.sType = (VkStructureType)VK_STRUCTURE_TYPE_GEOMETRY_TRIANGLES_NVX;
    geometry.triangles.pNext = nullptr;
    geometry.triangles.vertexData = MAGMA_OPTIONAL_HANDLE(vertexData);
    geometry.triangles.vertexOffset = vertexOffset;
    geometry.triangles.vertexCount = vertexCount;
    geometry.triangles.vertexStride = vertexStride;
    geometry.triangles.vertexFormat = vertexFormat;
    geometry.triangles.indexData = MAGMA_OPTIONAL_HANDLE(indexData);
    geometry.triangles.indexOffset = indexOffset;
    geometry.triangles.indexCount = indexCount;
    geometry.triangles.indexType = indexType;
    geometry.triangles.transformData = MAGMA_OPTIONAL_HANDLE(transformData);
    geometry.triangles.transformOffset = transformOffset;
    memset(&geometry.aabbs, 0, sizeof(VkGeometryAABBNVX));
}

GeometryTriangles::GeometryTriangles(std::shared_ptr<VertexBuffer> vertexData, VkDeviceSize vertexStride, VkFormat vertexFormat,
    std::shared_ptr<IndexBuffer> indexData, std::shared_ptr<Buffer> transformData,
    VkDeviceSize vertexOffset /* 0 */,
    VkDeviceSize indexOffset /* 0 */,
    VkDeviceSize transformOffset /* 0 */,
    VkGeometryFlagsNVX flags /* 0 */) noexcept:
    Geometry(VK_GEOMETRY_TYPE_TRIANGLES_NVX, flags)
{
    geometry.triangles.sType = (VkStructureType)VK_STRUCTURE_TYPE_GEOMETRY_TRIANGLES_NVX;
    geometry.triangles.pNext = nullptr;
    geometry.triangles.vertexData = MAGMA_OPTIONAL_HANDLE(vertexData);
    geometry.triangles.vertexOffset = vertexOffset;
    geometry.triangles.vertexCount = vertexData->getVertexCount();
    geometry.triangles.vertexStride = vertexStride;
    geometry.triangles.vertexFormat = vertexFormat;
    geometry.triangles.indexData = MAGMA_OPTIONAL_HANDLE(indexData);
    geometry.triangles.indexOffset = indexOffset;
    geometry.triangles.indexCount = indexData->getIndexCount();
    geometry.triangles.indexType = indexData->getIndexType();
    geometry.triangles.transformData = MAGMA_OPTIONAL_HANDLE(transformData);
    geometry.triangles.transformOffset = transformOffset;
    memset(&geometry.aabbs, 0, sizeof(VkGeometryAABBNVX));
}

GeometryBVH::GeometryBVH(std::shared_ptr<Buffer> aabbData, uint32_t numAABBs, uint32_t stride, VkDeviceSize offset,
    VkGeometryFlagsNVX flags /* 0 */) noexcept:
    Geometry(VK_GEOMETRY_TYPE_AABBS_NVX, flags)
{
    geometry.aabbs.sType = (VkStructureType)VK_STRUCTURE_TYPE_GEOMETRY_AABB_NVX;
    geometry.aabbs.pNext = nullptr;
    geometry.aabbs.aabbData = MAGMA_OPTIONAL_HANDLE(aabbData);
    geometry.aabbs.numAABBs = numAABBs;
    geometry.aabbs.stride = stride;
    geometry.aabbs.offset = offset;
    memset(&geometry.triangles, 0, sizeof(VkGeometryTrianglesNVX));
}
} // namespace magma
