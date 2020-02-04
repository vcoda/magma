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

template<typename VertexType>
inline VertexBuffer::VertexBuffer(std::shared_ptr<CommandBuffer> copyCmd, const std::vector<VertexType>& vertices,
    VkBufferCreateFlags flags /* 0 */,
    const Sharing& sharing /* default */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    CopyMemoryFunction copyFn /* nullptr */):
    BaseVertexBuffer(std::move(copyCmd), vertices.data(), static_cast<VkDeviceSize>(sizeof(VertexType) * vertices.size()),
        flags, sharing, std::move(allocator), std::move(copyFn))
{
    vertexCount = MAGMA_COUNT(vertices);
}
} // namespace magma
