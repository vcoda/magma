/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

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
#include "geometry.h"
#include "../objects/vertexBuffer.h"
#include "../objects/indexBuffer.h"

namespace magma
{
#ifdef VK_NV_ray_tracing
Geometry::Geometry(VkGeometryTypeNV geometryType, VkGeometryFlagsNV flags) noexcept
{
    sType = VK_STRUCTURE_TYPE_GEOMETRY_NV;
    pNext = nullptr;
    this->geometryType = geometryType;
    this->flags = flags;
}

GeometryTriangles::GeometryTriangles(std::shared_ptr<const Buffer> vertexData, VkDeviceSize vertexOffset, uint32_t vertexCount, VkDeviceSize vertexStride, VkFormat vertexFormat,
    std::shared_ptr<const Buffer> indexData, VkDeviceSize indexOffset, uint32_t indexCount, VkIndexType indexType,
    std::shared_ptr<const Buffer> transformData, VkDeviceSize transformOffset, VkGeometryFlagsNV flags /* 0 */) noexcept:
    Geometry(VK_GEOMETRY_TYPE_TRIANGLES_NV, flags)
{
    geometry.triangles.sType = VK_STRUCTURE_TYPE_GEOMETRY_TRIANGLES_NV;
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
    geometry.aabbs.sType = VK_STRUCTURE_TYPE_GEOMETRY_AABB_NV;
    geometry.aabbs.pNext = nullptr;
    geometry.aabbs.aabbData = VK_NULL_HANDLE;
    geometry.aabbs.numAABBs = 0;
    geometry.aabbs.stride = 0;
    geometry.aabbs.offset = 0;
}

GeometryTriangles::GeometryTriangles(std::shared_ptr<const BaseVertexBuffer> vertexData, VkDeviceSize vertexStride, VkFormat vertexFormat,
    std::shared_ptr<const BaseIndexBuffer> indexData, std::shared_ptr<const Buffer> transformData /* nullptr */,
    VkDeviceSize vertexOffset /* 0 */, VkDeviceSize indexOffset /* 0 */, VkDeviceSize transformOffset /* 0 */, VkGeometryFlagsNV flags /* 0 */) noexcept:
    GeometryTriangles(vertexData,
        vertexOffset,
        vertexData->getVertexCount(),
        vertexStride,
        vertexFormat,
        indexData,
        indexOffset,
        indexData->getIndexCount(),
        indexData->getIndexType(),
        std::move(transformData),
        transformOffset,
        flags)
{}

GeometryTriangles::GeometryTriangles(std::shared_ptr<const BaseVertexBuffer> vertexData, const VkVertexInputAttributeDescription& attribute, VkDeviceSize vertexStride,
    std::shared_ptr<const BaseIndexBuffer> indexData, std::shared_ptr<const Buffer> transformData /* nullptr */,
    VkDeviceSize vertexOffset /* 0 */, VkDeviceSize indexOffset /* 0 */, VkDeviceSize transformOffset /* 0 */, VkGeometryFlagsNV flags /* 0 */) noexcept:
    GeometryTriangles(vertexData,
        vertexOffset + attribute.offset,
        vertexData->getVertexCount(),
        vertexStride,
        attribute.format,
        indexData,
        indexOffset,
        indexData->getIndexCount(),
        indexData->getIndexType(),
        std::move(transformData),
        transformOffset,
        flags)
{}

GeometryAABBs::GeometryAABBs(std::shared_ptr<const Buffer> aabbData, uint32_t numAABBs, uint32_t stride, VkDeviceSize offset,
    VkGeometryFlagsNV flags /* 0 */) noexcept:
    Geometry(VK_GEOMETRY_TYPE_AABBS_NV, flags)
{
    geometry.aabbs.sType = VK_STRUCTURE_TYPE_GEOMETRY_AABB_NV;
    geometry.aabbs.pNext = nullptr;
    geometry.aabbs.aabbData = MAGMA_OPTIONAL_HANDLE(aabbData);
    geometry.aabbs.numAABBs = numAABBs;
    geometry.aabbs.stride = stride;
    geometry.aabbs.offset = offset;
    core::zeroMemory(geometry.triangles);
}
#endif // VK_NV_ray_tracing
} // namespace magma
