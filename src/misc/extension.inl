namespace magma
{
template<class Fn>
inline Extension<Fn>::Extension(PFN_vkVoidFunction procAddr) noexcept:
    procAddr(reinterpret_cast<Fn>(procAddr))
{}

template<class Fn>
inline void Extension<Fn>::requireProcAddress(const char *extensionName, bool device) const
{
    if (!procAddr)
    {
    #ifndef MAGMA_NO_EXCEPTIONS
        throw exception::UnsupportedExtension(extensionName, device);
    #else
        std::cout << "unsupported " << (device ? "device" : "instance") << " extension: "
            << extensionName << std::endl;
        MAGMA_ASSERT(procAddr);
        abort();
    #endif // !MAGMA_NO_EXCEPTIONS
    }
}

template<class Fn>
inline InstanceExtension<Fn>::InstanceExtension(VkInstance instance, const char *name) noexcept:
    Extension<Fn>(vkGetInstanceProcAddr(instance, name))
{}

template<class Fn>
inline InstanceExtension<Fn>::InstanceExtension(VkInstance instance, const char *name, const char *extensionName):
    InstanceExtension(instance, name)
{
    Extension<Fn>::requireProcAddress(extensionName, false);
}

template<class Fn>
inline DeviceExtension<Fn>::DeviceExtension(VkDevice device, const char *name) noexcept:
    Extension<Fn>(vkGetDeviceProcAddr(device, name))
{}

template<class Fn>
inline DeviceExtension<Fn>::DeviceExtension(VkDevice device, const char *name, const char *extensionName):
    DeviceExtension(device, name)
{
    Extension<Fn>::requireProcAddress(extensionName, true);
}
} // namespace magma
