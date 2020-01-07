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
inline InstanceExtension<Func>::InstanceExtension(VkInstance instance, const char *name, const char *extension)
{
    if (!pfn)
    {
        MAGMA_ASSERT(instance != VK_NULL_HANDLE);
        MAGMA_ASSERT(name);
        pfn = vkGetInstanceProcAddr(instance, name);
        MAGMA_THROW_UNSUPPORTED_EXTENSION(pfn, extension);
    }
}

template<typename Func>
PFN_vkVoidFunction InstanceExtension<Func>::pfn;
} // namespace magma
