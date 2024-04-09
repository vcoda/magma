namespace magma
{
template<class Fn, bool instance>
inline Extension<Fn, instance>::Extension(PFN_vkVoidFunction procAddr) noexcept:
    procAddr(reinterpret_cast<Fn>(procAddr))
{}

template<class Fn, bool instance>
inline void Extension<Fn, instance>::requireProcAddress(const char *extensionName) const
{
    if (!procAddr)
    {
    #ifndef MAGMA_NO_EXCEPTIONS
        throw exception::UnsupportedExtension(extensionName, instance);
    #else
        MAGMA_HANDLE_RESULT(VK_ERROR_EXTENSION_NOT_PRESENT, extensionName);
    #endif // !MAGMA_NO_EXCEPTIONS
    }
}

template<class Fn>
inline InstanceExtension<Fn>::InstanceExtension(VkInstance instance, const char *name) noexcept:
    Extension<Fn, true>(vkGetInstanceProcAddr(instance, name))
{}

template<class Fn>
inline InstanceExtension<Fn>::InstanceExtension(VkInstance instance, const char *name, const char *extensionName):
    InstanceExtension(instance, name)
{
    Extension<Fn, true>::requireProcAddress(extensionName);
}

template<class Fn>
inline DeviceExtension<Fn>::DeviceExtension(VkDevice device, const char *name) noexcept:
    Extension<Fn, false>(vkGetDeviceProcAddr(device, name))
{}

template<class Fn>
inline DeviceExtension<Fn>::DeviceExtension(VkDevice device, const char *name, const char *extensionName):
    DeviceExtension(device, name)
{
    Extension<Fn, false>::requireProcAddress(extensionName);
}
} // namespace magma
