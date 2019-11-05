namespace magma
{
template<typename IndexType>
inline IndexBuffer::IndexBuffer(std::shared_ptr<Device> device, const std::vector<IndexType>& indices,
    VkBufferCreateFlags flags /* 0 */,
    const ResourceSharing& sharing /* default */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    CopyMemoryFunction copyFn /* nullptr */):
    Buffer(std::move(device), 0, VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        flags, sharing, std::move(allocator), 0),
    indexType(VK_INDEX_TYPE_MAX_ENUM)
{
    static_assert(std::is_same<IndexType, uint16_t>::value ||
        std::is_same<IndexType, uint32_t>::value,
        "element type of index buffer should be unsigned short or unsigned int");
}

template<>
inline IndexBuffer::IndexBuffer(std::shared_ptr<Device> device, const std::vector<uint16_t>& indices,
    VkBufferCreateFlags flags /* 0 */,
    const ResourceSharing& sharing /* default */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    CopyMemoryFunction copyFn /* nullptr */):
    IndexBuffer(device, indices.data(), static_cast<VkDeviceSize>(sizeof(uint16_t) * indices.size()), VK_INDEX_TYPE_UINT16,
        flags, sharing, std::move(allocator), std::move(copyFn))
{}

template<>
inline IndexBuffer::IndexBuffer(std::shared_ptr<Device> device, const std::vector<uint32_t>& indices,
    VkBufferCreateFlags flags /* 0 */,
    const ResourceSharing& sharing /* default */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    CopyMemoryFunction copyFn /* nullptr */):
    IndexBuffer(device, indices.data(), static_cast<VkDeviceSize>(sizeof(uint32_t) * indices.size()), VK_INDEX_TYPE_UINT32,
        flags, sharing, std::move(allocator), std::move(copyFn))
{}

template<typename IndexType>
inline IndexBuffer::IndexBuffer(std::shared_ptr<CommandBuffer> copyCmdBuffer, const std::vector<IndexType>& indices,
    VkBufferCreateFlags flags /* 0 */,
    const ResourceSharing& sharing /* default */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    CopyMemoryFunction copyFn /* nullptr */):
    Buffer(std::move(device), 0, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, flags, sharing, std::move(allocator), 0),
    indexType(VK_INDEX_TYPE_MAX_ENUM)
{
    static_assert(std::is_same<IndexType, uint16_t>::value ||
        std::is_same<IndexType, uint32_t>::value,
        "element type of index buffer should be unsigned short or unsigned int");
}

template<>
inline IndexBuffer::IndexBuffer(std::shared_ptr<CommandBuffer> copyCmdBuffer, const std::vector<uint16_t>& indices,
    VkBufferCreateFlags flags /* 0 */,
    const ResourceSharing& sharing /* default */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    CopyMemoryFunction copyFn /* nullptr */):
    IndexBuffer(copyCmdBuffer, indices.data(), static_cast<VkDeviceSize>(sizeof(uint16_t) * indices.size()), VK_INDEX_TYPE_UINT16,
        flags, sharing, std::move(allocator), std::move(copyFn))
{}

template<>
inline IndexBuffer::IndexBuffer(std::shared_ptr<CommandBuffer> copyCmdBuffer, const std::vector<uint32_t>& indices,
    VkBufferCreateFlags flags /* 0 */,
    const ResourceSharing& sharing /* default */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    CopyMemoryFunction copyFn /* nullptr */):
    IndexBuffer(copyCmdBuffer, indices.data(), static_cast<VkDeviceSize>(sizeof(uint32_t) * indices.size()), VK_INDEX_TYPE_UINT32,
        flags, sharing, std::move(allocator), std::move(copyFn))
{}

inline uint32_t IndexBuffer::getIndexCount() const noexcept
{
    if (VK_INDEX_TYPE_UINT16 == indexType)
        return static_cast<uint32_t>(size / sizeof(uint16_t));
    return static_cast<uint32_t>(size / sizeof(uint32_t));
}
} // namespace magma
