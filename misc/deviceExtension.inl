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
inline DeviceExtension<Func>::DeviceExtension(VkDevice device, const char *name,
    const char *extension, const char *file, long line)
{
    if (!pfn)
    {
        MAGMA_ASSERT(device != VK_NULL_HANDLE);
        MAGMA_ASSERT(name);
        pfn = vkGetDeviceProcAddr(device, name);
        if (!pfn)
            throw exception::UnsupportedDeviceExtension(extension, file, line);
    }
}

template<typename Func>
PFN_vkVoidFunction DeviceExtension<Func>::pfn;
} // namespace magma
