namespace magma
{
template<class Vertex, class Index>
inline uint32_t Cluster<Vertex, Index>::findMinGeometryIndex() const noexcept
{
    if (!geometryIndices.empty())
        return *std::min_element(geometryIndices.begin(), geometryIndices.end());
    return 0;
}

template<class Vertex, class Index>
constexpr VkClusterAccelerationStructureIndexFormatFlagBitsNV Cluster<Vertex, Index>::getIndexFormat() const noexcept
{
    if constexpr (std::is_same<Index, uint8_t>::value)
        return VK_CLUSTER_ACCELERATION_STRUCTURE_INDEX_FORMAT_8BIT_NV;
    else if constexpr (std::is_same<Index, uint16_t>::value)
        return VK_CLUSTER_ACCELERATION_STRUCTURE_INDEX_FORMAT_16BIT_NV;
    else // if constexpr (std::is_same<Index, uint32_t>::value)
        return VK_CLUSTER_ACCELERATION_STRUCTURE_INDEX_FORMAT_32BIT_NV;
}

constexpr AccelerationStructureTriangleCluster::AccelerationStructureTriangleCluster(VkFormat vertexFormat /* VK_FORMAT_UNDEFINED */) noexcept:
    VkClusterAccelerationStructureTriangleClusterInputNV{
        VK_STRUCTURE_TYPE_CLUSTER_ACCELERATION_STRUCTURE_TRIANGLE_CLUSTER_INPUT_NV,
        nullptr, // pNext
        vertexFormat,
        0, // maxGeometryIndexValue
        1, // maxClusterUniqueGeometryCount
        0, // maxClusterTriangleCount
        0, // maxClusterVertexCount
        0, // maxTotalTriangleCount
        0, // maxTotalVertexCount
        0  // minPositionTruncateBitCount
    }
{}

template<class Vertex, class Index>
inline AccelerationStructureTriangleCluster::AccelerationStructureTriangleCluster(VkFormat vertexFormat, const std::forward_list<Cluster<Vertex, Index>>& clusters):
    AccelerationStructureTriangleCluster(vertexFormat)
{
    for (auto const& cluster: clusters)
    {
        MAGMA_ASSERT(cluster.vertexCount <= MAGMA_MAX_CLUSTER_VERTEX_COUNT); // max 9 bits
        MAGMA_ASSERT(!cluster.indices.empty());
        MAGMA_ASSERT(cluster.indices.size() % 3 == 0);
        std::unordered_set<uint32_t> uniqueGeometryIndices;
        uniqueGeometryIndices.reserve(cluster.geometryIndices.size());
        for (uint32_t geometryIndex: cluster.geometryIndices)
        {   // Maximum geometry index value for any constructed geometry
            maxGeometryIndexValue = std::max(maxGeometryIndexValue, geometryIndex);
            uniqueGeometryIndices.insert(geometryIndex);
        }
        // Maximum number of unique values of the geometry index for each cluster
        maxClusterUniqueGeometryCount = std::max(maxClusterUniqueGeometryCount, core::countof(uniqueGeometryIndices));
        // Maximum number of triangles in a cluster
        const uint32_t clusterTriangleCount = core::countof(cluster.indices) / 3;
        MAGMA_ASSERT(clusterTriangleCount <= MAGMA_MAX_CLUSTER_TRIANGLE_COUNT);
        maxClusterTriangleCount = std::max(maxClusterTriangleCount, clusterTriangleCount);
        // Maximum number of unique vertices in the cluster's index buffer
        std::unordered_set<Index> uniqueVertexIndices;
        uniqueVertexIndices.reserve(cluster.indices.size());
        for (Index i: cluster.indices)
            uniqueVertexIndices.insert(i);
        maxClusterVertexCount = std::max(maxClusterVertexCount, core::countof(uniqueVertexIndices));
        maxTotalTriangleCount += clusterTriangleCount;
        maxTotalVertexCount += cluster.vertexCount;
    }
}

constexpr AccelerationStructureBuildTriangleCluster::AccelerationStructureBuildTriangleCluster(uint32_t clusterID /* 0 */) noexcept:
    VkClusterAccelerationStructureBuildTriangleClusterInfoNV{
        clusterID,
        0, // clusterFlags
        0, // triangleCount
        0, // vertexCount
        0, // positionTruncateBitCount
        0, // indexType
        0, // opacityMicromapIndexType
        {0, 0, 0}, // baseGeometryIndexAndGeometryFlags
        0, // indexBufferStride
        0, // vertexBufferStride
        0, // geometryIndexAndFlagsBufferStride
        0, // opacityMicromapIndexBufferStride
        MAGMA_NULL, // indexBuffer
        MAGMA_NULL, // vertexBuffer
        MAGMA_NULL, // geometryIndexAndFlagsBuffer
        MAGMA_NULL, // opacityMicromapArray
        MAGMA_NULL  // opacityMicromapIndexBuffer
    }
{}

template<class Vertex, class Index>
AccelerationStructureBuildTriangleCluster::AccelerationStructureBuildTriangleCluster(const Cluster<Vertex, Index>& cluster, uint32_t clusterID) noexcept:
    AccelerationStructureBuildTriangleCluster(clusterID)
{
    MAGMA_ASSERT(!cluster.indices.empty());
    MAGMA_ASSERT(cluster.indices.size() % 3 == 0);
    const uint32_t clusterTriangleCount = core::countof(cluster.indices) / 3;
    MAGMA_ASSERT(clusterTriangleCount <= MAGMA_MAX_CLUSTER_TRIANGLE_COUNT); // max 9 bits
    triangleCount = clusterTriangleCount;
    MAGMA_ASSERT(cluster.vertexCount <= MAGMA_MAX_CLUSTER_VERTEX_COUNT); // max 9 bits
    vertexCount = cluster.vertexCount;
    indexType = cluster.getIndexFormat();
    indexBufferStride = sizeof(Index);
    vertexBufferStride = sizeof(Vertex);
    baseGeometryIndexAndGeometryFlags.geometryIndex = cluster.findMinGeometryIndex();
    baseGeometryIndexAndGeometryFlags.reserved = 0;
    baseGeometryIndexAndGeometryFlags.geometryFlags = cluster.geometryFlags;
    indexBuffer = (VkDeviceAddress)cluster.indexBufferOffset;
    vertexBuffer = (VkDeviceAddress)cluster.vertexBufferOffset;
    geometryIndexAndFlagsBuffer = (VkDeviceAddress)cluster.geometryIndexAndFlagsBufferOffset;
}

inline void fixupTriangleClustersBufferAddresses(std::vector<VkClusterAccelerationStructureBuildTriangleClusterInfoNV>& triangleClusters,
    const Buffer *vertexBuffer, const Buffer *indexBuffer, const Buffer *geometryIndexAndFlagsBuffer /* nullptr */) noexcept
{
    const VkDeviceAddress vertexBufferDeviceAddress = vertexBuffer->getDeviceAddress();
    const VkDeviceAddress indexBufferDeviceAddress = indexBuffer->getDeviceAddress();
    const VkDeviceAddress geometryIndexAndFlagsBufferDeviceAddress = geometryIndexAndFlagsBuffer ? geometryIndexAndFlagsBuffer->getDeviceAddress() : MAGMA_NULL;
    for (auto& cluster: triangleClusters)
    {
        cluster.vertexBuffer += vertexBufferDeviceAddress;
        cluster.indexBuffer += indexBufferDeviceAddress;
        cluster.geometryIndexAndFlagsBuffer += geometryIndexAndFlagsBufferDeviceAddress;
    }
}

constexpr std::size_t getClusterAccelerationStructureSize(VkClusterAccelerationStructureOpTypeNV opType) noexcept
{
    switch (opType)
    {
    case VK_CLUSTER_ACCELERATION_STRUCTURE_OP_TYPE_MOVE_OBJECTS_NV:
        return sizeof(VkClusterAccelerationStructureMoveObjectsInfoNV);
    case VK_CLUSTER_ACCELERATION_STRUCTURE_OP_TYPE_BUILD_CLUSTERS_BOTTOM_LEVEL_NV:
        return sizeof(VkClusterAccelerationStructureBuildClustersBottomLevelInfoNV);
    case VK_CLUSTER_ACCELERATION_STRUCTURE_OP_TYPE_BUILD_TRIANGLE_CLUSTER_NV:
        return sizeof(VkClusterAccelerationStructureBuildTriangleClusterInfoNV);
    case VK_CLUSTER_ACCELERATION_STRUCTURE_OP_TYPE_BUILD_TRIANGLE_CLUSTER_TEMPLATE_NV:
        return sizeof(VkClusterAccelerationStructureBuildTriangleClusterTemplateInfoNV);
    case VK_CLUSTER_ACCELERATION_STRUCTURE_OP_TYPE_INSTANTIATE_TRIANGLE_CLUSTER_NV:
        return sizeof(VkClusterAccelerationStructureInstantiateClusterInfoNV);
    case VK_CLUSTER_ACCELERATION_STRUCTURE_OP_TYPE_GET_CLUSTER_TEMPLATE_INDICES_NV:
        return sizeof(VkClusterAccelerationStructureGetTemplateIndicesInfoNV);
    default:
        return 0;
    }
}

constexpr std::size_t getClusterAccelerationStructureMaxSize() noexcept
{
    constexpr std::array<std::size_t, 6> sizes = {
        sizeof(VkClusterAccelerationStructureMoveObjectsInfoNV),
        sizeof(VkClusterAccelerationStructureBuildClustersBottomLevelInfoNV),
        sizeof(VkClusterAccelerationStructureBuildTriangleClusterInfoNV),
        sizeof(VkClusterAccelerationStructureBuildTriangleClusterTemplateInfoNV),
        sizeof(VkClusterAccelerationStructureInstantiateClusterInfoNV),
        sizeof(VkClusterAccelerationStructureGetTemplateIndicesInfoNV)
    };
    return *std::max_element(sizes.begin(), sizes.end());
}
} // namespace magma
