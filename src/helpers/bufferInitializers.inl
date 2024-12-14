namespace magma::helpers
{
template<class Vertex, std::size_t Size>
inline std::shared_ptr<VertexBuffer> makeVertexBuffer(const Vertex (&vertices)[Size],
    lent_ptr<CommandBuffer> cmdBuffer,
    std::shared_ptr<Allocator> allocator /* nullptr */)
{
    static_assert(Size > 0, "invalid vertex array size");
    auto vertexBuffer = std::make_shared<VertexBuffer>(cmdBuffer,
        Size * sizeof(Vertex), vertices, std::move(allocator));
    vertexBuffer->setVertexCount(static_cast<uint32_t>(Size));
    return vertexBuffer;
}

template<class Vertex>
inline std::shared_ptr<VertexBuffer> makeVertexBuffer(const std::vector<Vertex>& vertices,
    lent_ptr<CommandBuffer> cmdBuffer,
    std::shared_ptr<Allocator> allocator /* nullptr */)
{
    MAGMA_ASSERT(!vertices.empty());
    auto vertexBuffer = std::make_shared<VertexBuffer>(cmdBuffer,
        vertices.size() * sizeof(Vertex), vertices.data(), std::move(allocator));
    vertexBuffer->setVertexCount(core::countof(vertices));
    return vertexBuffer;
}

template<class Index, std::size_t Size>
inline std::shared_ptr<IndexBuffer> makeIndexBuffer(const Index (&indices)[Size],
    lent_ptr<CommandBuffer> cmdBuffer,
    std::shared_ptr<Allocator> allocator /* nullptr */)
{
    static_assert(Size > 0, "invalid index array size");
    auto indexBuffer = std::make_shared<IndexBuffer>(cmdBuffer,
        getIndexType<Index>(), Size * sizeof(Index), indices, std::move(allocator));
    MAGMA_ASSERT(indexBuffer->getIndexCount() == static_cast<uint32_t>(Size));
    return indexBuffer;
}

template<class Index>
inline std::shared_ptr<IndexBuffer> makeIndexBuffer(const std::vector<Index>& indices,
    lent_ptr<CommandBuffer> cmdBuffer,
    std::shared_ptr<Allocator> allocator /* nullptr */)
{
    MAGMA_ASSERT(!indices.empty());
    auto indexBuffer = std::make_shared<IndexBuffer>(cmdBuffer,
        getIndexType<Index>(), indices.size() * sizeof(Index), indices.data(), std::move(allocator));
    MAGMA_ASSERT(indexBuffer->getIndexCount() == core::countof(indices));
    return indexBuffer;
}

template<class Type, std::size_t Size>
inline std::shared_ptr<StorageBuffer> makeStorageBuffer(const Type (&data)[Size],
    lent_ptr<CommandBuffer> cmdBuffer,
    std::shared_ptr<Allocator> allocator /* nullptr */)
{
    static_assert(Size > 0, "invalid array size");
    auto storageBuffer = std::make_shared<StorageBuffer>(cmdBuffer,
        Size * sizeof(Type), data, std::move(allocator));
    return storageBuffer;
}

template<class Type>
inline std::shared_ptr<StorageBuffer> makeStorageBuffer(const std::vector<Type>& data,
    lent_ptr<CommandBuffer> cmdBuffer,
    std::shared_ptr<Allocator> allocator /* nullptr */)
{
    MAGMA_ASSERT(!data.empty());
    auto storageBuffer = std::make_shared<StorageBuffer>(cmdBuffer,
        data.size() * sizeof(Type), data.data(), std::move(allocator));
    return storageBuffer;
}

#ifdef VK_KHR_acceleration_structure
template<class Type>
inline std::unique_ptr<AccelerationStructureInputBuffer> makeInputBuffer(const Type& element,
    lent_ptr<CommandBuffer> cmdBuffer,
    std::shared_ptr<Allocator> allocator /* nullptr */)
{
    return std::make_unique<AccelerationStructureInputBuffer>(cmdBuffer,
        sizeof(Type), &element, std::move(allocator));
}

template<class Type, std::size_t Size>
inline std::unique_ptr<AccelerationStructureInputBuffer> makeInputBuffer(const Type (&elements)[Size],
    lent_ptr<CommandBuffer> cmdBuffer,
    std::shared_ptr<Allocator> allocator /* nullptr */)
{
    static_assert(Size > 0, "invalid array size");
    return std::make_unique<AccelerationStructureInputBuffer>(cmdBuffer,
        Size * sizeof(Type), elements, std::move(allocator));
}

template<class Type>
inline std::unique_ptr<AccelerationStructureInputBuffer> makeInputBuffer(const std::vector<Type>& elements,
    lent_ptr<CommandBuffer> cmdBuffer,
    std::shared_ptr<Allocator> allocator /* nullptr */)
{
    MAGMA_ASSERT(!elements.empty());
    return std::make_unique<AccelerationStructureInputBuffer>(cmdBuffer,
        elements.size() * sizeof(Type), elements.data(), std::move(allocator));
}
#endif //  VK_KHR_acceleration_structure
} // namespace magma::helpers
