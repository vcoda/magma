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
    return (priority = std::max(0.f, std::min(value, 1.f)));
}
} // namespace magma
