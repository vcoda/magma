namespace magma
{
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
        maxClusterUniqueGeometryCount = std::max(maxClusterUniqueGeometryCount, MAGMA_COUNT(uniqueGeometryIndices));
        // Maximum number of triangles in a cluster
        const uint32_t clusterTriangleCount = MAGMA_COUNT(cluster.indices) / 3;
        maxClusterTriangleCount = std::max(maxClusterTriangleCount, clusterTriangleCount);
        // Maximum number of unique vertices in the cluster's index buffer
        std::unordered_set<Index> uniqueVertexIndices;
        for (Index i: cluster.indices)
            uniqueVertexIndices.insert(i);
        maxClusterVertexCount = std::max(maxClusterVertexCount, MAGMA_COUNT(uniqueVertexIndices));
        maxTotalTriangleCount += clusterTriangleCount;
        maxTotalVertexCount += MAGMA_COUNT(cluster.vertices);
    }
}
} // namespace magma
