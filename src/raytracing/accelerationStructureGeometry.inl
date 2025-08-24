namespace magma
{
constexpr AccelerationStructureGeometry::AccelerationStructureGeometry() noexcept:
    VkAccelerationStructureGeometryKHR{
        VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR,
        nullptr, // pNext
        VK_GEOMETRY_TYPE_MAX_ENUM_KHR,
        VkAccelerationStructureGeometryDataKHR{}, // geometry
        0, // flags
    }
{}

constexpr AccelerationStructureGeometry::AccelerationStructureGeometry(const VkGeometryTypeKHR geometryType, const VkGeometryFlagsKHR flags /* 0 */) noexcept:
    VkAccelerationStructureGeometryKHR{
        VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR,
        nullptr, // pNext
        geometryType,
        VkAccelerationStructureGeometryDataKHR{}, // geometry
        flags
    }
{}

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

inline AccelerationStructureAabbs::AccelerationStructureAabbs(const Aabb& aabb, VkGeometryFlagsKHR flags /* 0 */) noexcept:
    AccelerationStructureGeometry(VK_GEOMETRY_TYPE_AABBS_KHR, flags)
{
    geometry.aabbs.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_AABBS_DATA_KHR;
    geometry.aabbs.pNext = nullptr;
    geometry.aabbs.data.hostAddress = &aabb;
    geometry.aabbs.stride = sizeof(Aabb);
    primitiveCount = 1;
}

inline AccelerationStructureAabbs::AccelerationStructureAabbs(const std::vector<Aabb>& aabbs, VkGeometryFlagsKHR flags /* 0 */) noexcept:
    AccelerationStructureGeometry(VK_GEOMETRY_TYPE_AABBS_KHR, flags)
{
    geometry.aabbs.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_AABBS_DATA_KHR;
    geometry.aabbs.pNext = nullptr;
    geometry.aabbs.data = address(aabbs.data());
    geometry.aabbs.stride = sizeof(Aabb);
    primitiveCount = core::countof(aabbs);
    MAGMA_ASSERT(primitiveCount);
}

inline AccelerationStructureAabbs::AccelerationStructureAabbs(const Buffer *aabbs, VkGeometryFlagsKHR flags /* 0 */) noexcept:
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

inline AccelerationStructureInstances::AccelerationStructureInstances(uint32_t instanceCount, const void *instances, VkGeometryFlagsKHR flags /* 0 */) noexcept:
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
inline AccelerationStructureInstances::AccelerationStructureInstances(const AccelerationStructureInstanceBuffer<Instance> *instances, VkGeometryFlagsKHR flags /* 0 */) noexcept:
    AccelerationStructureGeometry(VK_GEOMETRY_TYPE_INSTANCES_KHR, flags)
{
    MAGMA_ASSERT(instances);
    MAGMA_ASSERT(instances->getInstanceCount());
    geometry.instances.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR;
    geometry.instances.pNext = nullptr;
    geometry.instances.arrayOfPointers = VK_FALSE;
    geometry.instances.data = address(instances);
    primitiveCount = instances->getInstanceCount();
}
} // namespace magma
