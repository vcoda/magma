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
#include "buffer.h"

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
    geometry.triangles.vertexData = *vertexData;
    geometry.triangles.vertexOffset = vertexOffset;
    geometry.triangles.vertexCount = vertexCount;
    geometry.triangles.vertexStride = vertexStride;
    geometry.triangles.vertexFormat = vertexFormat;
    geometry.triangles.indexData = *indexData;
    geometry.triangles.indexOffset = indexOffset;
    geometry.triangles.indexCount = indexCount;
    geometry.triangles.indexType = indexType;
    geometry.triangles.transformData = *transformData;
    geometry.triangles.transformOffset = transformOffset;
    memset(&geometry.aabbs, 0, sizeof(VkGeometryAABBNVX));
}

GeometryBVH::GeometryBVH(std::shared_ptr<Buffer> aabbData, uint32_t numAABBs, uint32_t stride, VkDeviceSize offset,
    VkGeometryFlagsNVX flags /* 0 */):
    Geometry(VK_GEOMETRY_TYPE_AABBS_NVX, flags)
{
    geometry.aabbs.aabbData = *aabbData;
    geometry.aabbs.numAABBs = numAABBs;
    geometry.aabbs.stride = stride;
    geometry.aabbs.offset = offset;
    memset(&geometry.triangles, 0, sizeof(VkGeometryTrianglesNVX));
}
} // namespace magma