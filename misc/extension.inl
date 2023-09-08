namespace magma
{
template<class Fn>
inline Extension<Fn>::Extension(PFN_vkVoidFunction procAddr) noexcept:
    procAddr(reinterpret_cast<Fn>(procAddr))
{}

template<class Fn>
inline void Extension<Fn>::verify(const char *extensionName, bool device) const
{
    if (!procAddr)
    {
    #ifndef MAGMA_NO_EXCEPTIONS
        throw exception::UnsupportedExtension(extensionName, device);
    #else
        std::cout << "unsupported " << (device ? "device" : "instance") << " extension: "
            << extensionName << std::endl;
        abort();
    #endif // !MAGMA_NO_EXCEPTIONS
    }
}
} // namespace magma
