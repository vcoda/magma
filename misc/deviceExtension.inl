namespace magma
{
template<typename Func>
inline DeviceExtension<Func>::DeviceExtension(VkDevice device, const char *name) noexcept
{
    if (!pfn)
    {
        MAGMA_ASSERT(device != VK_NULL_HANDLE);
        MAGMA_ASSERT(name);
        pfn = vkGetDeviceProcAddr(device, name);
    }
}

template<typename Func>
inline DeviceExtension<Func>::DeviceExtension(VkDevice device, const char *name, const char *extension,
    const exception::source_location& location)
{
    if (!pfn)
    {
        MAGMA_ASSERT(device != VK_NULL_HANDLE);
        MAGMA_ASSERT(name);
        pfn = vkGetDeviceProcAddr(device, name);
        if (!pfn)
            throw exception::UnsupportedDeviceExtension(extension, location);
    }
}

template<typename Func>
PFN_vkVoidFunction DeviceExtension<Func>::pfn;
} // namespace magma
