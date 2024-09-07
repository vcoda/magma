namespace magma::helpers
{
template<class Vertex, std::size_t Size>
inline std::shared_ptr<VertexBuffer> makeVertexBuffer(const Vertex (&vertices)[Size],
    std::shared_ptr<magma::CommandBuffer> cmdBuffer,
    std::shared_ptr<magma::Allocator> allocator /* nullptr */)
{
    static_assert(Size > 0, "invalid vertex array size");
    auto vertexBuffer = std::make_shared<VertexBuffer>(std::move(cmdBuffer),
        Size * sizeof(Vertex), vertices, std::move(allocator));
    vertexBuffer->setVertexCount(static_cast<uint32_t>(Size));
    return vertexBuffer;
}

template<class Vertex>
inline std::shared_ptr<VertexBuffer> makeVertexBuffer(const std::vector<Vertex>& vertices,
    std::shared_ptr<magma::CommandBuffer> cmdBuffer,
    std::shared_ptr<magma::Allocator> allocator /* nullptr */)
{
    MAGMA_ASSERT(!vertices.empty());
    auto vertexBuffer = std::make_shared<VertexBuffer>(std::move(cmdBuffer),
        vertices.size() * sizeof(Vertex), vertices.data, std::move(allocator));
    vertexBuffer->setVertexCount(MAGMA_COUNT(vertices));
    return vertexBuffer;
}

template<class Index, std::size_t Size>
inline std::shared_ptr<IndexBuffer> makeIndexBuffer(const Index (&indices)[Size],
    std::shared_ptr<magma::CommandBuffer> cmdBuffer,
    std::shared_ptr<magma::Allocator> allocator /* nullptr */)
{
    static_assert(Size > 0, "invalid index array size");
    auto indexBuffer = std::make_shared<IndexBuffer>(std::move(cmdBuffer),
        getIndexType<Index>(), Size * sizeof(Index), indices, std::move(allocator));
    MAGMA_ASSERT(indexBuffer->getIndexCount() == static_cast<uint32_t>(Size));
    return indexBuffer;
}

template<class Index>
inline std::shared_ptr<IndexBuffer> makeIndexBuffer(const std::vector<Index>& indices,
    std::shared_ptr<magma::CommandBuffer> cmdBuffer,
    std::shared_ptr<magma::Allocator> allocator /* nullptr */)
{
    MAGMA_ASSERT(!indices.empty());
    auto indexBuffer = std::make_shared<IndexBuffer>(std::move(cmdBuffer),
        getIndexType<Index>(), indices.size() * sizeof(Index), indices.data(), std::move(allocator));
    MAGMA_ASSERT(indexBuffer->getIndexCount() == MAGMA_COUNT(indices));
    return indexBuffer;
}
} // namespace magma::helpers