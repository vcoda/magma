namespace magma
{
template<typename Function>
MAGMA_INLINE InstanceExtension<Function>::InstanceExtension(VkInstance instance, const char *name) noexcept
{
    if (!_addr)
    {
        MAGMA_ASSERT(instance != VK_NULL_HANDLE);
        MAGMA_ASSERT(name);
        _addr = vkGetInstanceProcAddr(instance, name);
    }
}

template<typename Function>
MAGMA_INLINE InstanceExtension<Function>::InstanceExtension(VkInstance instance, const char *name, const char *extension)
{
    if (!_addr)
    {
        MAGMA_ASSERT(instance != VK_NULL_HANDLE);
        MAGMA_ASSERT(name);
        _addr = vkGetInstanceProcAddr(instance, name);
        if (!_addr)
            MAGMA_THROW_NOT_PRESENT(extension);
    }
}

template<typename Function>
PFN_vkVoidFunction InstanceExtension<Function>::_addr;
} // namespace magma
