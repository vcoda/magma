namespace magma
{
inline bool DeviceMemory::local() const noexcept
{
    return flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
}

inline bool DeviceMemory::pinned() const noexcept
{
    // Pinned memory is virtual memory pages that are specially marked so that they cannot be paged out.
    // Higher bandwidth is possible between the host and the device when using page-locked (or "pinned") memory.
    // On discrete AMD GPU there is around 256 MiB of DEVICE_LOCAL + HOST_VISIBLE memory pool.
    return flags & (VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
}

inline bool DeviceMemory::hostVisible() const noexcept
{
    return flags & (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

inline bool DeviceMemory::hostCached() const noexcept
{
    return flags & (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT);
}

inline bool DeviceMemory::hostMapped() const noexcept
{
    return mapped;
}

template<typename Type>
inline Type *DeviceMemory::map(VkDeviceSize offset /* 0 */,
    VkDeviceSize size /* VK_WHOLE_SIZE */,
    VkMemoryMapFlags flags /* 0 */) noexcept
{
    MAGMA_ASSERT(sizeof(Type) <= memoryRequirements.size);
    if (sizeof(Type) > memoryRequirements.size)
        return nullptr;
    if ((size != VK_WHOLE_SIZE) && (sizeof(Type) > size))
        return nullptr;
    void *data = map(offset, size, flags);
    return reinterpret_cast<Type *>(data);
}
} // namespace magma
