namespace magma
{
template<typename Type>
inline StorageBuffer::StorageBuffer(std::shared_ptr<CommandBuffer> copyCmd, const std::vector<Type>& data,
    VkBufferCreateFlags flags /* 0 */,
    const Sharing& sharing /* default */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    CopyMemoryFunction copyFn /* nullptr */):
    StorageBuffer(std::move(copyCmd), data.data(), static_cast<VkDeviceSize>(sizeof(Type) * data.size()),
        flags, sharing, std::move(allocator), std::move(copyFn))
{}
} // namespace magma
