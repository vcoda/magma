namespace magma
{
inline AccelerationStructureGeometry::AccelerationStructureGeometry(VkGeometryTypeKHR geometryType_, VkGeometryFlagsKHR flags_ /* 0 */) noexcept
{
    sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
    pNext = nullptr;
    geometryType = geometryType_;
    flags = flags_;
}

inline AccelerationStructureGeometryTriangles::AccelerationStructureGeometryTriangles(VkFormat vertexFormat, const void *vertices,
    uint32_t maxVertex, const void *transform /* nullptr */, VkGeometryFlagsKHR flags_ /* 0 */) noexcept:
    AccelerationStructureGeometryTriangles()
{
    MAGMA_ASSERT(vertexFormat);
    MAGMA_ASSERT(vertices);
    MAGMA_ASSERT(maxVertex);
    geometry.triangles.vertexFormat = vertexFormat;
    geometry.triangles.vertexData = address(vertices);
    geometry.triangles.vertexStride = static_cast<VkDeviceSize>(Format(vertexFormat).size());
    geometry.triangles.maxVertex = maxVertex;
    geometry.triangles.transformData.hostAddress = transform;
    flags = flags_;
    primitiveCount = (geometry.triangles.maxVertex + 1) / 3;
    MAGMA_ASSERT(primitiveCount);
}

inline AccelerationStructureGeometryTriangles::AccelerationStructureGeometryTriangles(VkFormat vertexFormat, const void *vertices,
    uint32_t maxVertex, VkIndexType indexType, const void *indices, uint32_t indexCount,
    const void *transform /* nullptr */, VkGeometryFlagsKHR flags_ /* 0 */) noexcept:
    AccelerationStructureGeometryTriangles()
{
    MAGMA_ASSERT(vertexFormat);
    MAGMA_ASSERT(vertices);
    MAGMA_ASSERT(maxVertex);
    MAGMA_ASSERT(indices);
    MAGMA_ASSERT(indexCount);
    geometry.triangles.vertexFormat = vertexFormat;
    geometry.triangles.vertexData = address(vertices);
    geometry.triangles.vertexStride = static_cast<VkDeviceSize>(Format(vertexFormat).size());
    geometry.triangles.maxVertex = maxVertex;
    geometry.triangles.indexType = indexType;
    geometry.triangles.indexData = address(indices);
    geometry.triangles.transformData.hostAddress = transform;
    flags = flags_;
    MAGMA_ASSERT(indexCount % 3 == 0);
    primitiveCount = indexCount / 3;
    MAGMA_ASSERT(primitiveCount);
}

inline AccelerationStructureGeometryTriangles::AccelerationStructureGeometryTriangles(VkFormat vertexFormat, const Buffer *vertices,
    const Buffer *transform /* nullptr */, VkGeometryFlagsKHR flags_ /* 0 */) noexcept:
    AccelerationStructureGeometryTriangles()
{
    MAGMA_ASSERT(vertexFormat);
    geometry.triangles.vertexFormat = vertexFormat;
    geometry.triangles.vertexData = address(vertices);
    geometry.triangles.vertexStride = static_cast<VkDeviceSize>(Format(vertexFormat).size());
    geometry.triangles.maxVertex = static_cast<uint32_t>(vertices->getSize() / geometry.triangles.vertexStride) - 1;
    geometry.triangles.transformData = address(transform);
    flags = flags_;
    primitiveCount = (geometry.triangles.maxVertex + 1) / 3;
    MAGMA_ASSERT(primitiveCount);
}

inline AccelerationStructureGeometryTriangles::AccelerationStructureGeometryTriangles(VkFormat vertexFormat, const Buffer *vertices,
    VkIndexType indexType, const Buffer *indices, const Buffer *transform /* nullptr */, VkGeometryFlagsKHR flags_ /* 0 */) noexcept:
    AccelerationStructureGeometryTriangles()
{
    MAGMA_ASSERT(vertexFormat);
    geometry.triangles.vertexFormat = vertexFormat;
    geometry.triangles.vertexData = address(vertices);
    geometry.triangles.vertexStride = static_cast<VkDeviceSize>(Format(vertexFormat).size()),
    geometry.triangles.maxVertex = static_cast<uint32_t>(vertices->getSize() / geometry.triangles.vertexStride) - 1;
    geometry.triangles.indexType = indexType;
    geometry.triangles.indexData = address(indices);
    geometry.triangles.transformData = address(transform);
    flags = flags_;
    const size_t indexSize = getIndexSize();
    MAGMA_ASSERT(indices->getSize() % indexSize == 0);
    const VkDeviceSize indexCount = indices->getSize() / indexSize;
    MAGMA_ASSERT(indexCount % 3 == 0);
    primitiveCount = static_cast<uint32_t>(indexCount / 3);
    MAGMA_ASSERT(primitiveCount);
}

inline Aabb::Aabb(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) noexcept:
    VkAabbPositionsKHR{
        minX, minY, minZ,
        maxX, maxY, maxZ
    }
{}

inline Aabb::Aabb(const float min[3], const float max[3]) noexcept:
    VkAabbPositionsKHR{
        min[0], min[1], min[2],
        max[0], max[1], max[2]
    }
{}

inline Aabb::Aabb(const float bounds[6]) noexcept:
    VkAabbPositionsKHR{
        bounds[0],
        bounds[1],
        bounds[2],
        bounds[3],
        bounds[4],
        bounds[5]
    }
{}

inline void Aabb::translate(float x, float y, float z) noexcept
{
    minX += x;
    minY += y;
    minZ += z;
    maxX += x;
    maxY += y;
    maxZ += z;
}

template<class Vector>
inline Aabb::Aabb(const Vector& min, const Vector& max) noexcept:
    VkAabbPositionsKHR{
        min.x, min.y, min.z,
        max.x, max.y, max.z,
    }
{}

inline AccelerationStructureGeometryAabbs::AccelerationStructureGeometryAabbs() noexcept:
    AccelerationStructureGeometry(VK_GEOMETRY_TYPE_AABBS_KHR)
{
    geometry.aabbs.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_AABBS_DATA_KHR;
    geometry.aabbs.pNext = nullptr;
    geometry.aabbs.data.deviceAddress = MAGMA_NULL;
    geometry.aabbs.stride = 0;
    primitiveCount = 0;
}

inline AccelerationStructureGeometryAabbs::AccelerationStructureGeometryAabbs(const Aabb& aabb, VkGeometryFlagsKHR flags /* 0 */) noexcept:
    AccelerationStructureGeometry(VK_GEOMETRY_TYPE_AABBS_KHR, flags)
{
    geometry.aabbs.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_AABBS_DATA_KHR;
    geometry.aabbs.pNext = nullptr;
    geometry.aabbs.data.hostAddress = &aabb;
    geometry.aabbs.stride = sizeof(Aabb);
    primitiveCount = 1;
}

inline AccelerationStructureGeometryAabbs::AccelerationStructureGeometryAabbs(const std::vector<Aabb>& aabbs, VkGeometryFlagsKHR flags /* 0 */) noexcept:
    AccelerationStructureGeometry(VK_GEOMETRY_TYPE_AABBS_KHR, flags)
{
    geometry.aabbs.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_AABBS_DATA_KHR;
    geometry.aabbs.pNext = nullptr;
    geometry.aabbs.data = address(aabbs.data());
    geometry.aabbs.stride = sizeof(Aabb);
    primitiveCount = core::countof(aabbs);
    MAGMA_ASSERT(primitiveCount);
}

inline AccelerationStructureGeometryAabbs::AccelerationStructureGeometryAabbs(const Buffer *aabbs, VkGeometryFlagsKHR flags /* 0 */) noexcept:
    AccelerationStructureGeometry(VK_GEOMETRY_TYPE_AABBS_KHR, flags)
{
    geometry.aabbs.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_AABBS_DATA_KHR;
    geometry.aabbs.pNext = nullptr;
    geometry.aabbs.data = address(aabbs);
    geometry.aabbs.stride = sizeof(VkAabbPositionsKHR);
    MAGMA_ASSERT(aabbs->getSize() % geometry.aabbs.stride == 0);
    primitiveCount = static_cast<uint32_t>(aabbs->getSize() / geometry.aabbs.stride);
    MAGMA_ASSERT(primitiveCount);
}

inline AccelerationStructureGeometryInstances::AccelerationStructureGeometryInstances() noexcept:
    AccelerationStructureGeometry(VK_GEOMETRY_TYPE_INSTANCES_KHR)
{
    geometry.instances.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR;
    geometry.instances.pNext = nullptr;
    geometry.instances.arrayOfPointers = VK_FALSE;
    geometry.instances.data.deviceAddress = MAGMA_NULL;
    primitiveCount = 0;
}

inline AccelerationStructureGeometryInstances::AccelerationStructureGeometryInstances(uint32_t instanceCount, const void *instances, VkGeometryFlagsKHR flags /* 0 */) noexcept:
    AccelerationStructureGeometry(VK_GEOMETRY_TYPE_INSTANCES_KHR, flags)
{
    MAGMA_ASSERT(instanceCount);
    MAGMA_ASSERT(instances);
    geometry.instances.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR;
    geometry.instances.pNext = nullptr;
    geometry.instances.arrayOfPointers = VK_FALSE;
    geometry.instances.data = address(instances);
    primitiveCount = instanceCount;
}

template<class Instance>
inline AccelerationStructureGeometryInstances::AccelerationStructureGeometryInstances(const AccelerationStructureInstanceBuffer<Instance> *instances, VkGeometryFlagsKHR flags /* 0 */) noexcept:
    AccelerationStructureGeometry(VK_GEOMETRY_TYPE_INSTANCES_KHR, flags)
{
    MAGMA_ASSERT(instances->getInstanceCount());
    geometry.instances.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR;
    geometry.instances.pNext = nullptr;
    geometry.instances.arrayOfPointers = VK_FALSE;
    geometry.instances.data = address(instances);
    primitiveCount = instances->getInstanceCount();
}
} // namespace magma
