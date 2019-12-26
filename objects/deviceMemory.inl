namespace magma
{
inline bool DeviceMemory::deviceLocal() const noexcept
{
    return flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
}

inline bool DeviceMemory::hostVisible() const noexcept
{
    return flags & (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
        VK_MEMORY_PROPERTY_HOST_COHERENT_BIT |
        VK_MEMORY_PROPERTY_HOST_CACHED_BIT);
}

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
