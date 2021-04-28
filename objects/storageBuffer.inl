namespace magma
{
template<typename Type>
inline StorageBuffer::StorageBuffer(std::shared_ptr<CommandBuffer> cmdBuffer, const std::vector<Type>& data,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    VkBufferCreateFlags flags /* 0 */,
    const Sharing& sharing /* default */,
    CopyMemoryFunction copyFn /* nullptr */):
    StorageBuffer(std::move(cmdBuffer), data.data(), static_cast<VkDeviceSize>(sizeof(Type) * data.size()),
        std::move(allocator), flags, sharing, std::move(copyFn))
{}
} // namespace magma
