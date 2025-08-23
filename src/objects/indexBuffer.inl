namespace magma
{
template<class IndexType>
template<std::size_t IndexCount>
inline TIndexBuffer<IndexType>::TIndexBuffer(lent_ptr<CommandBuffer> cmdBuffer, const IndexType (&indices)[IndexCount],
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* default */,
    CopyMemoryFn copyMem /* nullptr */):
    IndexBuffer(std::move(cmdBuffer), magma::getIndexType<IndexType>(), IndexCount * sizeof(IndexType), indices,
        std::move(allocator), optional, sharing, std::move(copyMem))
{}

template<class T>
constexpr VkIndexType getIndexType() noexcept
{
    static_assert(
    #if defined(VK_KHR_index_type_uint8) || defined(VK_EXT_index_type_uint8)
        std::is_same<T, uint8_t>::value ||
    #endif
        std::is_same<T, uint16_t>::value || std::is_same<T, uint32_t>::value,
    #if defined(VK_KHR_index_type_uint8) || defined(VK_EXT_index_type_uint8)
        "index should be of unsigned char, short or int type");
    #else
        "index should be of unsigned short or int type");
    #endif
#if defined(VK_KHR_index_type_uint8) || defined(VK_EXT_index_type_uint8)
    if constexpr (std::is_same<T, uint8_t>::value)
    #ifdef VK_KHR_index_type_uint8
        return VK_INDEX_TYPE_UINT8_KHR;
    #else
        return VK_INDEX_TYPE_UINT8_EXT;
    #endif
#endif // VK_KHR_index_type_uint8 || VK_EXT_index_type_uint8
    if constexpr (std::is_same<T, uint16_t>::value)
        return VK_INDEX_TYPE_UINT16;
    return VK_INDEX_TYPE_UINT32;
}
} // namespace magma
