namespace magma
{
constexpr AccelerationStructureGeometryTriangles::AccelerationStructureGeometryTriangles() noexcept:
    VkAccelerationStructureGeometryTrianglesDataKHR{
        VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR,
        nullptr, // pNext
        VK_FORMAT_UNDEFINED,
        MAGMA_NULL, // vertexData
        0, // vertexStride
        0, // maxVertex
        VK_INDEX_TYPE_NONE_KHR,
        MAGMA_NULL, // indexData
        MAGMA_NULL // transformData
    },
    primitiveCount(0)
{}

template<class InputBuffer>
inline AccelerationStructureGeometryTriangles::AccelerationStructureGeometryTriangles(
    VkFormat vertexFormat, const InputBuffer& vertices,
    const InputBuffer& transform /* nullptr */,
    uint32_t transformIndex /* 0 */) noexcept:
    VkAccelerationStructureGeometryTrianglesDataKHR{
        VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR,
        nullptr, // pNext
        vertexFormat,
        address(vertices),
        VkDeviceSize(Format(vertexFormat).size()), // vertexStride
        uint32_t(vertices->getSize() / vertexStride - 1), // maxVertex
        VK_INDEX_TYPE_NONE_KHR,
        MAGMA_NULL, // indexData
        address(transform, sizeof(VkTransformMatrixKHR) * transformIndex)
    },
    primitiveCount((maxVertex + 1) / 3)
{}

template<class InputBuffer>
inline AccelerationStructureGeometryTriangles::AccelerationStructureGeometryTriangles(
    VkFormat vertexFormat, const InputBuffer& vertices,
    VkIndexType indexType, const InputBuffer& indices,
    const InputBuffer& transform /* nullptr */,
    uint32_t transformIndex /* 0 */) noexcept:
    VkAccelerationStructureGeometryTrianglesDataKHR{
        VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR,
        nullptr, // pNext
        vertexFormat,
        address(vertices),
        VkDeviceSize(Format(vertexFormat).size()), // vertexStride
        uint32_t(vertices->getSize() / vertexStride - 1), // maxVertex
        indexType,
        address(indices),
        address(transform, sizeof(VkTransformMatrixKHR) * transformIndex)
    }
{
    const size_t indexSize = getIndexSize();
    MAGMA_ASSERT(indices->getSize() % indexSize == 0);
    const VkDeviceSize indexCount = indices->getSize() / indexSize;
    MAGMA_ASSERT(indexCount % 3 == 0);
    primitiveCount = static_cast<uint32_t>(indexCount / 3);
}

inline size_t AccelerationStructureGeometryTriangles::getIndexSize() const noexcept
{
    switch (indexType)
    {
    case VK_INDEX_TYPE_UINT16: return sizeof(uint16_t);
    case VK_INDEX_TYPE_UINT32: return sizeof(uint32_t);
#ifdef VK_KHR_index_type_uint8
    case VK_INDEX_TYPE_UINT8_KHR: return sizeof(uint8_t);
#endif
    case VK_INDEX_TYPE_NONE_KHR: return 0;
    }
    MAGMA_ASSERT(false);
    return 0;
}

inline AccelerationStructureGeometryAabbs::AccelerationStructureGeometryAabbs() noexcept:
    VkAccelerationStructureGeometryAabbsDataKHR{
        VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_AABBS_DATA_KHR,
        nullptr, // pNext
        MAGMA_NULL, // data
        0 // stride
    },
    primitiveCount(0)
{}

template<class InputBuffer>
inline AccelerationStructureGeometryAabbs::AccelerationStructureGeometryAabbs(const InputBuffer& aabbsData,
    VkDeviceSize stride /* sizeof(VkAabbPositionsKHR) */) noexcept:
    VkAccelerationStructureGeometryAabbsDataKHR{
        VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_AABBS_DATA_KHR,
        nullptr, // pNext
        address(aabbsData),
        stride // The stride must be a multiple of 8
    },
    primitiveCount(uint32_t(aabbsData->getSize() / sizeof(VkAabbPositionsKHR)))
{}

inline AccelerationStructureGeometryInstances::AccelerationStructureGeometryInstances() noexcept:
    VkAccelerationStructureGeometryInstancesDataKHR{
        VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR,
        nullptr, // pNext
        VK_FALSE, // arrayOfPointers;
        MAGMA_NULL // data
    },
    primitiveCount(0)
{}

template<class InputBuffer>
inline AccelerationStructureGeometryInstances::AccelerationStructureGeometryInstances(const InputBuffer& instances) noexcept:
    VkAccelerationStructureGeometryInstancesDataKHR{
        VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR,
        nullptr, // pNext
        VK_FALSE,
        address(instances)
    },
    primitiveCount(instances->getInstanceCount())
{}

inline AccelerationStructureGeometry::AccelerationStructureGeometry() noexcept:
    VkAccelerationStructureGeometryKHR{
        VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR,
        nullptr, // pNext
        VK_GEOMETRY_TYPE_MAX_ENUM_KHR,
        VkAccelerationStructureGeometryDataKHR{},
        0 // flags
    },
    primitiveCount(0)
{}

inline AccelerationStructureGeometry::AccelerationStructureGeometry(const AccelerationStructureGeometryTriangles& triangles,
    VkGeometryFlagsKHR flags /* VK_GEOMETRY_OPAQUE_BIT_KHR */,
    const StructureChain& extendedInfo /* default */) noexcept:
    VkAccelerationStructureGeometryKHR{
        VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR,
        extendedInfo.headNode(),
        VK_GEOMETRY_TYPE_TRIANGLES_KHR,
        VkAccelerationStructureGeometryDataKHR{},
        flags
    },
    primitiveCount(triangles.primitiveCount)
{
    geometry.triangles = triangles;
}

inline AccelerationStructureGeometry::AccelerationStructureGeometry(const AccelerationStructureGeometryAabbs& aabbs,
    VkGeometryFlagsKHR flags /* VK_GEOMETRY_OPAQUE_BIT_KHR */,
    const StructureChain& extendedInfo /* default */) noexcept:
    VkAccelerationStructureGeometryKHR{
        VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR,
        extendedInfo.headNode(),
        VK_GEOMETRY_TYPE_AABBS_KHR,
        VkAccelerationStructureGeometryDataKHR{},
        flags
    },
    primitiveCount(aabbs.primitiveCount)
{
    geometry.aabbs = aabbs;
}

inline AccelerationStructureGeometry::AccelerationStructureGeometry(const AccelerationStructureGeometryInstances& instances,
    VkGeometryFlagsKHR flags /* VK_GEOMETRY_OPAQUE_BIT_KHR */,
    const StructureChain& extendedInfo /* default */) noexcept:
    VkAccelerationStructureGeometryKHR{
        VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR,
        extendedInfo.headNode(),
        VK_GEOMETRY_TYPE_INSTANCES_KHR,
        VkAccelerationStructureGeometryDataKHR{},
        flags
    },
    primitiveCount(instances.primitiveCount)
{
    geometry.instances = instances;
}
} // namespace magma
