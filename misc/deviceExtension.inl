namespace magma
{
template<typename Function>
MAGMA_INLINE DeviceExtension<Function>::DeviceExtension(VkDevice device, const char *name) noexcept
{
    if (!_addr)
    {
        MAGMA_ASSERT(device != VK_NULL_HANDLE);
        MAGMA_ASSERT(name);
        _addr = vkGetDeviceProcAddr(device, name);
    }
}

template<typename Function>
MAGMA_INLINE DeviceExtension<Function>::DeviceExtension(VkDevice device, const char *name, const char *extension)
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

template<typename Function>
PFN_vkVoidFunction DeviceExtension<Function>::_addr;
} // namespace magma
