namespace magma
{
template<typename VertexType>
inline VertexBuffer::VertexBuffer(std::shared_ptr<Device> device,
    const std::vector<VertexType>& vertices,
    VkBufferCreateFlags flags /* 0 */,
    const Sharing& sharing /* default */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    CopyMemoryFunction copyFn /* nullptr */):
    VertexBuffer(std::move(device), vertices.data(), static_cast<VkDeviceSize>(sizeof(VertexType) * vertices.size()),
        flags, sharing, std::move(allocator), std::move(copyFn))
{
    vertexCount = static_cast<uint32_t>(vertices.size());
}

template<typename VertexType>
inline VertexBuffer::VertexBuffer(std::shared_ptr<CommandBuffer> copyCmdBuffer,
    const std::vector<VertexType>& vertices,
    VkBufferCreateFlags flags /* 0 */,
    const Sharing& sharing /* default */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    CopyMemoryFunction copyFn /* nullptr */):
    VertexBuffer(std::move(copyCmdBuffer), vertices.data(), static_cast<VkDeviceSize>(sizeof(VertexType) * vertices.size()),
        flags, sharing, std::move(allocator), std::move(copyFn))
{
    vertexCount = static_cast<uint32_t>(vertices.size());
}

inline uint32_t VertexBuffer::getVertexCount() const noexcept
{
    MAGMA_ASSERT(vertexCount > 0);
    return vertexCount;
}
} // namespace magma
