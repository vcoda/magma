namespace magma
{
template<typename IndexType>
inline IndexBuffer::IndexBuffer(std::shared_ptr<Device> device,
    const std::vector<IndexType>& indices,
    VkBufferCreateFlags flags /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    CopyMemoryFunction copyFn /* nullptr */):
    IndexBuffer(device, indices.data(), static_cast<VkDeviceSize>(sizeof(IndexType) * indices.size()),
        sizeof(IndexType) == sizeof(uint16_t) ? VK_INDEX_TYPE_UINT16 : VK_INDEX_TYPE_UINT32, flags,
        std::move(allocator), std::move(copyFn))
{}

template<typename IndexType>
inline IndexBuffer::IndexBuffer(std::shared_ptr<CommandBuffer> copyCmdBuffer,
    const std::vector<IndexType>& indices,
    VkBufferCreateFlags flags /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    CopyMemoryFunction copyFn /* nullptr */):
    IndexBuffer(copyCmdBuffer, indices.data(), static_cast<VkDeviceSize>(sizeof(IndexType) * indices.size()),
        sizeof(IndexType) == sizeof(uint16_t) ? VK_INDEX_TYPE_UINT16 : VK_INDEX_TYPE_UINT32, flags,
        std::move(allocator), std::move(copyFn))
{}

inline uint32_t IndexBuffer::getIndexCount() const noexcept
{
    if (VK_INDEX_TYPE_UINT16 == indexType)
        return static_cast<uint32_t>(size / sizeof(uint16_t));
    return static_cast<uint32_t>(size / sizeof(uint32_t));
}
} // namespace magma
