namespace magma
{
template<typename Func>
inline InstanceExtension<Func>::InstanceExtension(VkInstance instance, const char *name) noexcept
{
    if (!pfn)
    {
        MAGMA_ASSERT(instance != VK_NULL_HANDLE);
        MAGMA_ASSERT(name);
        pfn = vkGetInstanceProcAddr(instance, name);
    }
}

template<typename Func>
inline InstanceExtension<Func>::InstanceExtension(VkInstance instance, const char *name,
    const char *extension, const char *file, long line)
{
    if (!pfn)
    {
        MAGMA_ASSERT(instance != VK_NULL_HANDLE);
        MAGMA_ASSERT(name);
        pfn = vkGetInstanceProcAddr(instance, name);
        if (!pfn)
            throw exception::UnsupportedInstanceExtension(extension, file, line);
    }
}

template<typename Func>
PFN_vkVoidFunction InstanceExtension<Func>::pfn;
} // namespace magma
