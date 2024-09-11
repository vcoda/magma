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
    geometry.triangles.vertexFormat = vertexFormat;
    geometry.triangles.vertexData.hostAddress = vertices;
    geometry.triangles.vertexStride = (VkDeviceSize)Format(vertexFormat).size(),
    geometry.triangles.maxVertex = maxVertex;
    geometry.triangles.transformData.hostAddress = transform;
    flags = flags_;
    primitiveCount = (geometry.triangles.maxVertex + 1) / 3;
}

inline AccelerationStructureGeometryTriangles::AccelerationStructureGeometryTriangles(VkFormat vertexFormat, const void *vertices,
    uint32_t maxVertex, VkIndexType indexType, const void *indices, uint32_t indexCount,
    const void *transform /* nullptr */, VkGeometryFlagsKHR flags_ /* 0 */) noexcept:
    AccelerationStructureGeometryTriangles()
{
    geometry.triangles.vertexFormat = vertexFormat;
    geometry.triangles.vertexData.hostAddress = vertices;
    geometry.triangles.vertexStride = (VkDeviceSize)Format(vertexFormat).size(),
    geometry.triangles.maxVertex = maxVertex;
    geometry.triangles.indexType = indexType;
    geometry.triangles.indexData.hostAddress = indices;
    geometry.triangles.transformData.hostAddress = transform;
    flags = flags_;
    MAGMA_ASSERT(indexCount % 3 == 0);
    primitiveCount = indexCount / 3;
}

template<class Buffer>
inline AccelerationStructureGeometryTriangles::AccelerationStructureGeometryTriangles(VkFormat vertexFormat, const Buffer& vertices,
    const Buffer& transform /* nullptr */, VkGeometryFlagsKHR flags_ /* 0 */) noexcept:
    AccelerationStructureGeometryTriangles()
{
    geometry.triangles.vertexFormat = vertexFormat;
    geometry.triangles.vertexData.deviceAddress = vertices->getDeviceAddress();
    geometry.triangles.vertexStride = (VkDeviceSize)Format(vertexFormat).size(),
    geometry.triangles.maxVertex = uint32_t(vertices->getSize() / geometry.triangles.vertexStride - 1),
    geometry.triangles.transformData = address(transform);
    flags = flags_;
    primitiveCount = (geometry.triangles.maxVertex + 1) / 3;
}

template<class Buffer>
inline AccelerationStructureGeometryTriangles::AccelerationStructureGeometryTriangles(VkFormat vertexFormat, const Buffer& vertices,
    VkIndexType indexType, const Buffer& indices, const Buffer& transform /* nullptr */, VkGeometryFlagsKHR flags_ /* 0 */) noexcept:
    AccelerationStructureGeometryTriangles()
{
    geometry.triangles.vertexFormat = vertexFormat;
    geometry.triangles.vertexData.deviceAddress = vertices->getDeviceAddress();
    geometry.triangles.vertexStride = (VkDeviceSize)Format(vertexFormat).size(),
    geometry.triangles.maxVertex = uint32_t(vertices->getSize() / geometry.triangles.vertexStride - 1),
    geometry.triangles.indexType = indexType;
    geometry.triangles.indexData.deviceAddress = indices->getDeviceAddress();
    geometry.triangles.transformData = address(transform);
    flags = flags_;
    const size_t indexSize = getIndexSize();
    MAGMA_ASSERT(indices->getSize() % indexSize == 0);
    const VkDeviceSize indexCount = indices->getSize() / indexSize;
    MAGMA_ASSERT(indexCount % 3 == 0);
    primitiveCount = static_cast<uint32_t>(indexCount / 3);
}

inline AccelerationStructureGeometryAabbs::AccelerationStructureGeometryAabbs() noexcept:
    AccelerationStructureGeometry(VK_GEOMETRY_TYPE_AABBS_KHR)
{
    geometry.aabbs.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_AABBS_DATA_KHR;
    geometry.aabbs.pNext = nullptr;
    geometry.aabbs.data.deviceAddress = MAGMA_NULL;
    geometry.aabbs.stride = 0;
    primitiveCount = 0;
}

template<class Buffer>
inline AccelerationStructureGeometryAabbs::AccelerationStructureGeometryAabbs(const Buffer& aabbs, VkGeometryFlagsKHR flags /* 0 */) noexcept:
    AccelerationStructureGeometry(VK_GEOMETRY_TYPE_AABBS_KHR, flags)
{
    geometry.aabbs.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_AABBS_DATA_KHR;
    geometry.aabbs.pNext = nullptr;
    geometry.aabbs.data.deviceAddress = aabbs->getDeviceAddress();
    geometry.aabbs.stride = sizeof(VkAabbPositionsKHR);
    MAGMA_ASSERT(aabbs->getSize() % geometry.aabbs.stride == 0);
    primitiveCount = static_cast<uint32_t>(aabbs->getSize() / geometry.aabbs.stride);
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
    geometry.instances.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR;
    geometry.instances.pNext = nullptr;
    geometry.instances.arrayOfPointers = VK_FALSE;
    geometry.instances.data.hostAddress = instances;
    primitiveCount = instanceCount;
}

template<class Buffer>
inline AccelerationStructureGeometryInstances::AccelerationStructureGeometryInstances(const Buffer& instances, VkGeometryFlagsKHR flags /* 0 */) noexcept:
    AccelerationStructureGeometry(VK_GEOMETRY_TYPE_INSTANCES_KHR, flags)
{
    geometry.instances.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR;
    geometry.instances.pNext = nullptr;
    geometry.instances.arrayOfPointers = VK_FALSE;
    geometry.instances.data.deviceAddress = instances->getDeviceAddress();
    primitiveCount = instances->getInstanceCount();
}
} // namespace magma
