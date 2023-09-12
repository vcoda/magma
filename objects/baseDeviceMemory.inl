namespace magma
{
inline bool BaseDeviceMemory::local() const noexcept
{
    return flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
}

inline bool BaseDeviceMemory::staged() const noexcept
{   // On discrete NVIDIA and AMD GPUs there are around 256 MiB of
    // DEVICE_LOCAL + HOST_VISIBLE memory pool. This 256MiB limit
    // correlates with the 256MiB PCIE-specified BAR-size limit
    // that defines the size of the 256MiB aperture/window of VRAM
    // that the host can access.
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

#ifdef VK_AMD_device_coherent_memory
inline bool BaseDeviceMemory::deviceHostCoherent() const noexcept
{   // For any memory allocated with both the HOST_COHERENT and
    // the DEVICE_COHERENT flags, host or device accesses also
    // perform automatic memory domain transfer operations,
    // such that writes are always automatically available and
    // visible to both host and device memory domains.
    constexpr VkMemoryPropertyFlags coherentFlags =
        VK_MEMORY_PROPERTY_HOST_COHERENT_BIT |
        VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD;
    return (flags & coherentFlags) == coherentFlags;
}
#endif // VK_AMD_device_coherent_memory

inline bool BaseDeviceMemory::binded() const noexcept
{
#ifdef VK_NULL_HANDLE
    return binding != VK_NULL_HANDLE;
#else
    return binding != 0ull;
#endif
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
