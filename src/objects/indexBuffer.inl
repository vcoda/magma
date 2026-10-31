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
    std::is_same_v<T, uint8_t> ||
#endif
    std::is_same_v<T, uint16_t> || std::is_same_v<T, uint32_t>,
#if defined(VK_KHR_index_type_uint8) || defined(VK_EXT_index_type_uint8)
    "index should be of unsigned char, short or int type");
#else
    "index should be of unsigned short or int type");
#endif
    if constexpr (std::is_same_v<T, uint16_t>) return VK_INDEX_TYPE_UINT16;
#if defined(VK_KHR_index_type_uint8)
    if constexpr (std::is_same_v<T, uint8_t>) return VK_INDEX_TYPE_UINT8_KHR;
#elif defined(VK_EXT_index_type_uint8)
    if constexpr (std::is_same_v<T, uint8_t>) return VK_INDEX_TYPE_UINT8_EXT;
#endif
    return VK_INDEX_TYPE_UINT32;
}
} // namespace magma
