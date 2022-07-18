namespace magma
{
template<typename Type>
inline SrcTransferBuffer::SrcTransferBuffer(std::shared_ptr<Device> device, const std::vector<Type>& data,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    VkBufferCreateFlags flags /* 0 */,
    float memoryPriority /* 0.f */,
    const Sharing& sharing /* default */,
    CopyMemoryFunction copyFn /* nullptr */):
    SrcTransferBuffer(std::move(device),
        static_cast<VkDeviceSize>(data.size() * sizeof(Type)), data.data(),
        std::move(allocator), flags, memoryPriority, sharing, std::move(copyFn))
{}
} // namespace magma
