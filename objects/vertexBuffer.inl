namespace magma
{
template<typename VertexType>
MAGMA_INLINE VertexBuffer::VertexBuffer(std::shared_ptr<Device> device,
    const std::vector<VertexType>& vertices,
    VkBufferCreateFlags flags /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    CopyMemoryFunction copyFn /* nullptr */):
    VertexBuffer(std::move(device), vertices.data(), static_cast<VkDeviceSize>(sizeof(VertexType) * vertices.size()), flags,
        std::move(allocator), std::move(copyFn))
{
    vertexCount = static_cast<uint32_t>(vertices.size());
}

template<typename VertexType>
MAGMA_INLINE VertexBuffer::VertexBuffer(std::shared_ptr<CommandBuffer> copyCmdBuffer,
    const std::vector<VertexType>& vertices,
    VkBufferCreateFlags flags /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    CopyMemoryFunction copyFn /* nullptr */):
    VertexBuffer(copyCmdBuffer, vertices.data(), static_cast<VkDeviceSize>(sizeof(VertexType) * vertices.size()), flags,
        std::move(allocator), std::move(copyFn))
{
    vertexCount = static_cast<uint32_t>(vertices.size());
}

MAGMA_INLINE void VertexBuffer::setVertexCount(uint32_t count) noexcept
{
    vertexCount = count;
}

MAGMA_INLINE uint32_t VertexBuffer::getVertexCount() const noexcept
{
    MAGMA_ASSERT(vertexCount > 0);
    return vertexCount;
}
} // namespace magma
