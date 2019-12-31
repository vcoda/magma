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
#pragma once

namespace magma
{
    class Buffer;
    class VertexBuffer;
    class IndexBuffer;

    class Geometry : public VkGeometryNV
    {
    public:
        bool isTriangles() const noexcept
            { return VK_GEOMETRY_TYPE_TRIANGLES_NV == geometryType; }
        bool isBVH() const noexcept
            { return VK_GEOMETRY_TYPE_AABBS_NV == geometryType; }

    protected:
        explicit Geometry(VkGeometryTypeNV geometryType,
            VkGeometryFlagsNV flags) noexcept;
    };

    class GeometryTriangles : public Geometry
    {
    public:
        explicit GeometryTriangles(std::shared_ptr<const Buffer> vertexData,
            VkDeviceSize vertexOffset,
            uint32_t vertexCount,
            VkDeviceSize vertexStride,
            VkFormat vertexFormat,
            std::shared_ptr<const Buffer> indexData,
            VkDeviceSize indexOffset,
            uint32_t indexCount,
            VkIndexType indexType,
            std::shared_ptr<const Buffer> transformData,
            VkDeviceSize transformOffset,
            VkGeometryFlagsNV flags = 0) noexcept;
        explicit GeometryTriangles(std::shared_ptr<const VertexBuffer> vertexData,
            VkDeviceSize vertexStride,
            VkFormat vertexFormat,
            std::shared_ptr<const IndexBuffer> indexData,
            std::shared_ptr<const Buffer> transformData,
            VkDeviceSize vertexOffset = 0,
            VkDeviceSize indexOffset = 0,
            VkDeviceSize transformOffset = 0,
            VkGeometryFlagsNV flags = 0) noexcept;
    };

    class GeometryBVH : public Geometry
    {
    public:
        explicit GeometryBVH(std::shared_ptr<const Buffer> aabbData,
            uint32_t numAABBs,
            uint32_t stride,
            VkDeviceSize offset,
            VkGeometryFlagsNV flags = 0) noexcept;
    };
} // namespace magma
