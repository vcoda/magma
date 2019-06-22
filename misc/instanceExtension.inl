namespace magma
{
template<typename Func>
inline InstanceExtension<Func>::InstanceExtension(VkInstance instance,
    const char *name) noexcept
{
    if (!_addr)
    {
        MAGMA_ASSERT(instance != VK_NULL_HANDLE);
        MAGMA_ASSERT(name);
        _addr = vkGetInstanceProcAddr(instance, name);
    }
}

template<typename Func>
inline InstanceExtension<Func>::InstanceExtension(VkInstance instance,
    const char *name, const char *extension)
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

template<typename Func>
PFN_vkVoidFunction InstanceExtension<Func>::_addr;
} // namespace magma
