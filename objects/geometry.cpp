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
#include "geometry.h"
#include "vertexBuffer.h"
#include "indexBuffer.h"

namespace magma
{
Geometry::Geometry(VkGeometryTypeNVX geometryType, VkGeometryFlagsNVX flags)
{
    sType = (VkStructureType)0;
    pNext = nullptr;
    this->geometryType = geometryType;
    this->flags = flags;
}

GeometryTriangles::GeometryTriangles(std::shared_ptr<Buffer> vertexData, VkDeviceSize vertexOffset, uint32_t vertexCount, VkDeviceSize vertexStride, VkFormat vertexFormat,
    std::shared_ptr<Buffer> indexData, VkDeviceSize indexOffset, uint32_t indexCount, VkIndexType indexType,
    std::shared_ptr<Buffer> transformData, VkDeviceSize transformOffset,
    VkGeometryFlagsNVX flags /* 0 */):
    Geometry(VK_GEOMETRY_TYPE_TRIANGLES_NVX, flags)
{
    VkGeometryTrianglesNVX& triangles = geometry.triangles;
    triangles.vertexData = *vertexData;
    triangles.vertexOffset = vertexOffset;
    triangles.vertexCount = vertexCount;
    triangles.vertexStride = vertexStride;
    triangles.vertexFormat = vertexFormat;
    triangles.indexData = *indexData;
    triangles.indexOffset = indexOffset;
    triangles.indexCount = indexCount;
    triangles.indexType = indexType;
    triangles.transformData = *transformData;
    triangles.transformOffset = transformOffset;
    memset(&geometry.aabbs, 0, sizeof(VkGeometryAABBNVX));
}

GeometryTriangles::GeometryTriangles(std::shared_ptr<VertexBuffer> vertexData, VkDeviceSize vertexStride, VkFormat vertexFormat,
    std::shared_ptr<IndexBuffer> indexData, std::shared_ptr<Buffer> transformData,
    VkDeviceSize vertexOffset /* 0 */,
    VkDeviceSize transformOffset /* 0 */,
    VkDeviceSize indexOffset /* 0 */,
    VkGeometryFlagsNVX flags /* 0 */):
    Geometry(VK_GEOMETRY_TYPE_TRIANGLES_NVX, flags)
{
    VkGeometryTrianglesNVX& triangles = geometry.triangles;
    triangles.vertexData = *vertexData;
    triangles.vertexOffset = vertexOffset;
    triangles.vertexCount = vertexData->getVertexCount();
    triangles.vertexStride = vertexStride;
    triangles.vertexFormat = vertexFormat;
    triangles.indexData = *indexData;
    triangles.indexOffset = indexOffset;
    triangles.indexCount = indexData->getIndexCount();
    triangles.indexType = indexData->getIndexType();
    triangles.transformData = *transformData;
    triangles.transformOffset = transformOffset;
    memset(&geometry.aabbs, 0, sizeof(VkGeometryAABBNVX));
}

GeometryBVH::GeometryBVH(std::shared_ptr<Buffer> aabbData, uint32_t numAABBs, uint32_t stride, VkDeviceSize offset,
    VkGeometryFlagsNVX flags /* 0 */):
    Geometry(VK_GEOMETRY_TYPE_AABBS_NVX, flags)
{
    VkGeometryAABBNVX& aabbs = geometry.aabbs;
    aabbs.aabbData = *aabbData;
    aabbs.numAABBs = numAABBs;
    aabbs.stride = stride;
    aabbs.offset = offset;
    memset(&geometry.triangles, 0, sizeof(VkGeometryTrianglesNVX));
}
} // namespace magma