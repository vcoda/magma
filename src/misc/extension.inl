namespace magma
{
template<class Fn, bool Instance>
inline void ProcAddress<Fn, Instance>::checkAddress(const char *extensionName) const
{
    if (!addr)
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
    ProcAddress<Fn, true>(vkGetInstanceProcAddr(instance, procName))
{
    MAGMA_ASSERT(instance);
}

template<class Fn>
inline InstanceExtension<Fn>::InstanceExtension(VkInstance instance, const char *procName, const char *extensionName):
    InstanceExtension(instance, procName)
{
    ProcAddress<Fn, true>::checkAddress(extensionName);
}

template<class Fn>
inline DeviceExtension<Fn>::DeviceExtension(VkDevice device, const char *procName) noexcept:
    ProcAddress<Fn, false>(vkGetDeviceProcAddr(device, procName))
{
    MAGMA_ASSERT(device);
}

template<class Fn>
inline DeviceExtension<Fn>::DeviceExtension(VkDevice device, const char *procName, const char *extensionName):
    DeviceExtension(device, procName)
{
    ProcAddress<Fn, false>::checkAddress(extensionName);
}
} // namespace magma
