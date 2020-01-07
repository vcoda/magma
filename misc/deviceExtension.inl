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
inline DeviceExtension<Func>::DeviceExtension(VkDevice device, const char *name, const char *extension)
{
    if (!pfn)
    {
        MAGMA_ASSERT(device != VK_NULL_HANDLE);
        MAGMA_ASSERT(name);
        pfn = vkGetDeviceProcAddr(device, name);
        MAGMA_THROW_UNSUPPORTED_EXTENSION(pfn, extension);
    }
}

template<typename Func>
PFN_vkVoidFunction DeviceExtension<Func>::pfn;
} // namespace magma
