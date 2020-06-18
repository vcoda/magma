namespace magma
{
inline VkIndexType BaseIndexBuffer::getIndexType() const noexcept
{
    return indexType;
}

inline uint32_t BaseIndexBuffer::getIndexCount() const noexcept
{
    if (VK_INDEX_TYPE_UINT16 == indexType)
        return static_cast<uint32_t>(size / sizeof(uint16_t));
    return static_cast<uint32_t>(size / sizeof(uint32_t));
}

template<typename IndexType, uint32_t indexArraySize>
inline IndexBuffer::IndexBuffer(std::shared_ptr<CommandBuffer> cmdBuffer, const IndexType (&indices)[indexArraySize],
    VkBufferCreateFlags flags /* 0 */,
    const Sharing& sharing /* default */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    CopyMemoryFunction copyFn /* nullptr */):
    IndexBuffer(std::move(cmdBuffer), static_cast<VkDeviceSize>(sizeof(IndexType) * indexArraySize), indices,
        std::is_same<IndexType, uint16_t>::value ? VK_INDEX_TYPE_UINT16 : VK_INDEX_TYPE_UINT32,
        flags, sharing, std::move(allocator), std::move(copyFn))
{
    static_assert(std::is_same<IndexType, uint16_t>::value ||
        std::is_same<IndexType, uint32_t>::value,
        "element type of index buffer should be unsigned short or unsigned int");
}

template<typename IndexType>
inline IndexBuffer::IndexBuffer(std::shared_ptr<CommandBuffer> cmdBuffer, const std::vector<IndexType>& indices,
    VkBufferCreateFlags flags /* 0 */,
    const Sharing& sharing /* default */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    CopyMemoryFunction copyFn /* nullptr */):
    IndexBuffer(std::move(cmdBuffer), static_cast<VkDeviceSize>(sizeof(IndexType) * indices.size()), indices.data(),
        std::is_same<IndexType, uint16_t>::value ? VK_INDEX_TYPE_UINT16 : VK_INDEX_TYPE_UINT32,
        flags, sharing, std::move(allocator), std::move(copyFn))
{
    static_assert(std::is_same<IndexType, uint16_t>::value ||
        std::is_same<IndexType, uint32_t>::value,
        "element type of index buffer should be unsigned short or unsigned int");
}
} // namespace magma
