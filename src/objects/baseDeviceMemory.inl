namespace magma
{
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
    priority = std::max(MAGMA_MEMORY_PRIORITY_LOWEST, value);
    priority = std::min(MAGMA_MEMORY_PRIORITY_HIGHEST, priority);
    return priority;
}
} // namespace magma
