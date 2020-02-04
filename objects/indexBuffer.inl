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

template<typename IndexType>
inline IndexBuffer::IndexBuffer(std::shared_ptr<CommandBuffer> copyCmd, const std::vector<IndexType>& indices,
    VkBufferCreateFlags flags /* 0 */,
    const Sharing& sharing /* default */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    CopyMemoryFunction copyFn /* nullptr */):
    IndexBuffer(std::move(copyCmd), 0, indices.data(), VK_INDEX_TYPE_MAX_ENUM, flags, sharing, std::move(allocator), std::move(copyFn)),
{
    static_assert(std::is_same<IndexType, uint16_t>::value ||
        std::is_same<IndexType, uint32_t>::value,
        "element type of index buffer should be unsigned short or unsigned int");
}

template<> // Custom specialization for short indices
inline IndexBuffer::IndexBuffer(std::shared_ptr<CommandBuffer> copyCmd, const std::vector<uint16_t>& indices,
    VkBufferCreateFlags flags /* 0 */,
    const Sharing& sharing /* default */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    CopyMemoryFunction copyFn /* nullptr */):
    IndexBuffer(std::move(copyCmd), static_cast<VkDeviceSize>(sizeof(uint16_t) * indices.size()),
        indices.data(), VK_INDEX_TYPE_UINT16, flags, sharing, std::move(allocator), std::move(copyFn))
{}

template<> // Custom specialization for long indices
inline IndexBuffer::IndexBuffer(std::shared_ptr<CommandBuffer> copyCmd, const std::vector<uint32_t>& indices,
    VkBufferCreateFlags flags /* 0 */,
    const Sharing& sharing /* default */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    CopyMemoryFunction copyFn /* nullptr */):
    IndexBuffer(std::move(copyCmd), static_cast<VkDeviceSize>(sizeof(uint32_t) * indices.size()),
        indices.data(), VK_INDEX_TYPE_UINT32, flags, sharing, std::move(allocator), std::move(copyFn))
{}
} // namespace magma
