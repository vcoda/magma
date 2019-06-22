namespace magma
{
template<typename Func>
inline DeviceExtension<Func>::DeviceExtension(VkDevice device,
    const char *name) noexcept
{
    if (!_addr)
    {
        MAGMA_ASSERT(device != VK_NULL_HANDLE);
        MAGMA_ASSERT(name);
        _addr = vkGetDeviceProcAddr(device, name);
    }
}

template<typename Func>
inline DeviceExtension<Func>::DeviceExtension(VkDevice device,
    const char *name, const char *extension)
{
    if (!_addr)
    {
        MAGMA_ASSERT(device != VK_NULL_HANDLE);
        MAGMA_ASSERT(name);
        _addr = vkGetDeviceProcAddr(device, name);
        if (!_addr)
            MAGMA_THROW_NOT_PRESENT(extension);
    }
}

template<typename Func>
PFN_vkVoidFunction DeviceExtension<Func>::_addr;
} // namespace magma
