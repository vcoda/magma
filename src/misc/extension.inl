namespace magma
{
template<class Fn, bool Instance>
inline void Extension<Fn, Instance>::checkProcAddress(const char *extensionName) const
{
    if (!procAddr)
    {
    #ifndef MAGMA_NO_EXCEPTIONS
        throw exception::ExtensionNotPresent(extensionName);
    #else
        MAGMA_HANDLE_RESULT(VK_ERROR_EXTENSION_NOT_PRESENT, extensionName);
    #endif
    }
}

template<class Fn>
inline InstanceExtension<Fn>::InstanceExtension(VkInstance instance, const char *procName) noexcept:
    Extension<Fn, true>(vkGetInstanceProcAddr(instance, procName))
{}

template<class Fn>
inline InstanceExtension<Fn>::InstanceExtension(VkInstance instance, const char *procName, const char *extensionName):
    InstanceExtension(instance, procName)
{
    Extension<Fn, true>::checkProcAddress(extensionName);
}

template<class Fn>
inline DeviceExtension<Fn>::DeviceExtension(VkDevice device, const char *procName) noexcept:
    Extension<Fn, false>(vkGetDeviceProcAddr(device, procName))
{}

template<class Fn>
inline DeviceExtension<Fn>::DeviceExtension(VkDevice device, const char *procName, const char *extensionName):
    DeviceExtension(device, procName)
{
    Extension<Fn, false>::checkProcAddress(extensionName);
}
} // namespace magma
