namespace magma::core
{
template<class T>
constexpr bool powerOfTwo(T x) noexcept
{
    static_assert(std::is_integral<T>::value,
        "powerOfTwo() requires an integral type");
    return (x > 0) && (x & (x - 1)) == 0;
}

template<class T>
constexpr T alignUp(T value, T alignment) noexcept
{
    static_assert(std::is_integral<T>::value,
        "alignUp() requires an integral type");
    return (value + alignment - 1) & ~(alignment - 1);
}

template<class T>
constexpr T alignDown(T value, T alignment) noexcept
{
    static_assert(std::is_integral<T>::value,
        "alignDown() requires an integral type");
    return value & ~(alignment - 1);
}

template<class T>
constexpr T roundUp(T value, T multiple) noexcept
{
    static_assert(std::is_integral<T>::value,
        "roundUp() requires an integral type");
    if (T(0) == multiple)
        return multiple;
    if (T(1) == multiple)
        return value;
    return ((value + multiple - T(1)) / multiple) * multiple;
}

template<class T>
inline uint32_t countof(const T& container) noexcept
{
    return static_cast<uint32_t>(container.size());
}

template<class T>
inline typename T::NativeHandle dereference(T *p) noexcept
{
    return p ? p->getHandle() : VK_NULL_HANDLE;
}

template<class VkObject>
inline VkObject reinterpret(NonDispatchableHandle handle) noexcept
{
    if (!handle)
        return VK_NULL_HANDLE;
#if (VK_USE_64_BIT_PTR_DEFINES == 1)
    return reinterpret_cast<VkObject>(handle);
#else
    return static_cast<VkObject>(handle);
#endif
}

template<class T1, class T2, class L>
inline void foreach(T1& a, T2& b, L&& fn)
{
    auto i = a.begin(), ie = a.end();
    auto j = b.begin(), je = b.end();
    while ((i != ie) && (j != je))
    {
        fn(*i, *j);
        ++i; ++j;
    }
}
} // namespace magma::core
