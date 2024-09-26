namespace magma
{
template<typename Pointer>
inline TBufferView<Pointer>::TBufferView(Pointer buffer, VkFormat format,
    VkDeviceSize offset /* 0 */,
    VkDeviceSize range /* VK_WHOLE_SIZE */,
    const StructureChain& extendedInfo /* default */):
    BufferView(buffer.get(), format, offset, range, extendedInfo),
    buffer(std::move(buffer))
{}
} // namespace magma
