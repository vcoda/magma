namespace magma
{
template<typename Type>
MAGMA_INLINE StorageBuffer::StorageBuffer(std::shared_ptr<CommandBuffer> copyCmdBuffer,
    const std::vector<Type>& data,
    VkBufferCreateFlags flags /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    CopyMemoryFunction copyFn /* nullptr */):
    StorageBuffer(copyCmdBuffer, data.data(), static_cast<VkDeviceSize>(sizeof(Type) * data.size()), flags,
        std::move(allocator), std::move(copyFn))
{}
} // namespace magma
