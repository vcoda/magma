namespace magma
{
template<typename Type>
inline SrcTransferBuffer::SrcTransferBuffer(std::shared_ptr<Device> device,
    const std::vector<Type>& data,
    VkBufferCreateFlags flags /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    CopyMemoryFunction copyFn /* nullptr */):
    SrcTransferBuffer(device, data.data(), static_cast<VkDeviceSize>(data.size() * sizeof(Type)), flags,
        std::move(allocator), std::move(copyFn))
{}
} // namespace magma
