namespace magma
{
inline AccelerationStructureGeometry::AccelerationStructureGeometry(VkGeometryTypeKHR geometryType_) noexcept
{
    sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
    pNext = nullptr;
    geometryType = geometryType_;
    flags = 0;
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
    geometry.triangles.transformData = address(transform);
    geometry.triangles.indexType = indexType;
    geometry.triangles.indexData.deviceAddress = indices->getDeviceAddress();
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
}

template<class Buffer>
inline AccelerationStructureGeometryAabbs::AccelerationStructureGeometryAabbs(const Buffer& aabbs,
    VkGeometryFlagsKHR flags_ /* 0 */, VkDeviceSize stride /* sizeof(VkAabbPositionsKHR) */) noexcept:
    AccelerationStructureGeometryAabbs()
{
    geometry.aabbs.data.deviceAddress = aabbs->getDeviceAddress();
    geometry.aabbs.stride = stride;
    flags = flags_;
    primitiveCount = aabbs->getSize() / stride;
}

inline AccelerationStructureGeometryInstances::AccelerationStructureGeometryInstances() noexcept:
    AccelerationStructureGeometry(VK_GEOMETRY_TYPE_INSTANCES_KHR)
{
    geometry.instances.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR;
    geometry.instances.pNext = nullptr;
    geometry.instances.arrayOfPointers = VK_FALSE;
    geometry.instances.data.deviceAddress = MAGMA_NULL;
}

template<class Buffer>
inline AccelerationStructureGeometryInstances::AccelerationStructureGeometryInstances(const Buffer& instances, VkGeometryFlagsKHR flags_ /* 0 */) noexcept:
    AccelerationStructureGeometryInstances()
{
    geometry.instances.data.deviceAddress = instances->getDeviceAddress();
    flags = flags_;
    primitiveCount = instances->getInstanceCount();
}
}// namespace magma
