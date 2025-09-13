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

constexpr AccelerationStructureTriangleClusterInput::AccelerationStructureTriangleClusterInput(VkFormat vertexFormat /* VK_FORMAT_UNDEFINED */) noexcept:
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
inline AccelerationStructureTriangleClusterInput::AccelerationStructureTriangleClusterInput(VkFormat vertexFormat, const std::list<Cluster<Vertex, Index>>& clusters):
    AccelerationStructureTriangleClusterInput(vertexFormat)
{
    for (auto const& cluster: clusters)
    {
        MAGMA_ASSERT(!cluster.vertices.empty());
        MAGMA_ASSERT(!cluster.indices.empty());
        MAGMA_ASSERT(cluster.indices.size() % 3 == 0);
        std::unordered_set<uint32_t> uniqueGeometryIndices;
        for (uint32_t geometryIndex: cluster.geometryIndices)
        {   // Maximum geometry index value for any constructed geometry
            maxGeometryIndexValue = std::max(maxGeometryIndexValue, geometryIndex);
            uniqueGeometryIndices.insert(geometryIndex);
        }
        // Maximum number of unique values of the geometry index for each cluster
        maxClusterUniqueGeometryCount = std::max(maxClusterUniqueGeometryCount, core::countof(uniqueGeometryIndices));
        // Maximum number of triangles in a cluster
        const uint32_t clusterTriangleCount = core::countof(cluster.indices) / 3;
        maxClusterTriangleCount = std::max(maxClusterTriangleCount, clusterTriangleCount);
        // Maximum number of unique vertices in the cluster's index buffer
        std::unordered_set<Index> uniqueVertexIndices;
        for (Index i: cluster.indices)
            uniqueVertexIndices.insert(i);
        maxClusterVertexCount = std::max(maxClusterVertexCount, core::countof(uniqueVertexIndices));
        maxTotalTriangleCount += clusterTriangleCount;
        maxTotalVertexCount += core::countof(cluster.vertices);
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
    MAGMA_ASSERT(!cluster.vertices.empty());
    MAGMA_ASSERT(!cluster.indices.empty());
    MAGMA_ASSERT(cluster.indices.size() % 3 == 0);
    const uint32_t clusterTriangleCount = core::countof(cluster.indices) / 3;
    MAGMA_ASSERT(clusterTriangleCount <= MAGMA_MAX_CLUSTER_TRIANGLE_COUNT); // max 9 bits
    triangleCount = clusterTriangleCount;
    const uint32_t clusterVertexCount = core::countof(cluster.vertices);
    MAGMA_ASSERT(clusterVertexCount <= MAGMA_MAX_CLUSTER_VERTEX_COUNT); // max 9 bits
    vertexCount = clusterVertexCount;
    indexType = cluster.getIndexFormat();
    indexBufferStride = sizeof(Index);
    vertexBufferStride = sizeof(Vertex);
    baseGeometryIndexAndGeometryFlags.geometryIndex = cluster.findMinGeometryIndex();
    baseGeometryIndexAndGeometryFlags.reserved = 0;
    baseGeometryIndexAndGeometryFlags.geometryFlags = cluster.geometryFlags;
    indexBuffer = (VkDeviceAddress)cluster.indexBufferOffset;
    vertexBuffer = (VkDeviceAddress)cluster.vertexBufferOffset;
    geometryIndexAndFlagsBuffer = (VkDeviceAddress)cluster.propertyBufferOffset;
}
} // namespace magma
