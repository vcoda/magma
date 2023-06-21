namespace magma
{
inline bool BaseDeviceMemory::local() const noexcept
{
    return flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
}

// Pinned memory is virtual memory pages that are specially marked so that they cannot be paged out.
// Higher bandwidth is possible between the host and the device when using page-locked (or "pinned") memory.
// On discrete AMD GPU there is around 256 MiB of DEVICE_LOCAL + HOST_VISIBLE memory pool.
inline bool BaseDeviceMemory::pinned() const noexcept
{
    return local() && hostVisible();
}

inline bool BaseDeviceMemory::hostVisible() const noexcept
{
    constexpr VkMemoryPropertyFlags hostVisibleFlags =
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
        VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    return (flags & hostVisibleFlags) == hostVisibleFlags;
}

inline bool BaseDeviceMemory::hostCached() const noexcept
{
    return hostVisible() && (flags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT);
}

inline bool BaseDeviceMemory::binded() const noexcept
{
    return binding != VK_NULL_HANDLE;
}

inline bool BaseDeviceMemory::mapped() const noexcept
{
    return mapPointer != nullptr;
}

inline float BaseDeviceMemory::clampPriority(float value) noexcept
{
    return (priority = std::max(0.f, std::min(value, 1.f)));
}
} // namespace magma
