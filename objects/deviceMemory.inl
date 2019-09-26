namespace magma
{
template<typename Type>
inline Type *DeviceMemory::map(VkDeviceSize offset /* 0 */,
    VkDeviceSize size /* VK_WHOLE_SIZE */,
    VkMemoryMapFlags flags /* 0 */) noexcept
{
    MAGMA_ASSERT(sizeof(Type) <= this->size);
    if (sizeof(Type) > this->size)
        return nullptr;
    if ((size != VK_WHOLE_SIZE) && (sizeof(Type) > size))
        return nullptr;
    void *data = map(offset, size, flags);
    return reinterpret_cast<Type *>(data);
}
} // namespace magma
