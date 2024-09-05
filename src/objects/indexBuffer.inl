namespace magma
{
template<class T>
constexpr VkIndexType getIndexType() noexcept
{
    static_assert(
    #if defined(VK_KHR_index_type_uint8) || defined(VK_EXT_index_type_uint8)
        std::is_same<T, uint8_t>::value ||
    #endif
        std::is_same<T, uint16_t>::value ||
        std::is_same<T, uint32_t>::value,
    #if defined(VK_KHR_index_type_uint8) || defined(VK_EXT_index_type_uint8)
        "elements of index buffer should be of unsigned char, short or int types");
    #else
        "elements of index buffer should be of unsigned short or int types");
    #endif
    const VkIndexType indexType =
    #if defined(VK_KHR_index_type_uint8) || defined(VK_EXT_index_type_uint8)
        std::is_same<T, uint8_t>::value ? VK_INDEX_TYPE_UINT8_EXT :
    #endif
        std::is_same<T, uint16_t>::value ? VK_INDEX_TYPE_UINT16 : VK_INDEX_TYPE_UINT32;
    return indexType;
}
} // namespace magma
