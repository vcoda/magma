namespace magma
{
inline void BaseVertexBuffer::setVertexCount(uint32_t count) noexcept
{
    MAGMA_ASSERT(count > 0);
    vertexCount = count;
}

inline uint32_t BaseVertexBuffer::getVertexCount() const noexcept
{
    MAGMA_ASSERT(vertexCount > 0);
    return vertexCount;
}

template<typename VertexType, uint32_t vertexArraySize>
inline VertexBuffer::VertexBuffer(std::shared_ptr<CommandBuffer> copyCmd, const VertexType (&vertices)[vertexArraySize],
    VkBufferCreateFlags flags /* 0 */,
    const Sharing& sharing /* default */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    CopyMemoryFunction copyFn /* nullptr */):
    VertexBuffer(std::move(copyCmd), static_cast<VkDeviceSize>(sizeof(VertexType) * vertexArraySize), vertices,
        flags, sharing, std::move(allocator), std::move(copyFn))
{
    vertexCount = vertexArraySize;
}

template<typename VertexType>
inline VertexBuffer::VertexBuffer(std::shared_ptr<CommandBuffer> copyCmd, const std::vector<VertexType>& vertices,
    VkBufferCreateFlags flags /* 0 */,
    const Sharing& sharing /* default */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    CopyMemoryFunction copyFn /* nullptr */):
    VertexBuffer(std::move(copyCmd), static_cast<VkDeviceSize>(sizeof(VertexType) * vertices.size()), vertices.data(),
        flags, sharing, std::move(allocator), std::move(copyFn))
{
    vertexCount = MAGMA_COUNT(vertices);
}
} // namespace magma
