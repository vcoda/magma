namespace magma
{
template<typename Type>
inline SrcTransferBuffer::SrcTransferBuffer(std::shared_ptr<Device> device, const std::vector<Type>& data,
    VkBufferCreateFlags flags /* 0 */,
    const Sharing& sharing /* default */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    CopyMemoryFunction copyFn /* nullptr */):
    SrcTransferBuffer(std::move(device),
        static_cast<VkDeviceSize>(data.size() * sizeof(Type)),
        data.data(),
        flags, sharing, std::move(allocator), std::move(copyFn))
{}
} // namespace magma
