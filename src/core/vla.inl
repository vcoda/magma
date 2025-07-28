namespace magma::core
{
template<class T>
inline VariableLengthArray<T>::VariableLengthArray(void *ptr, std::size_t len) noexcept:
    array(reinterpret_cast<T *>(ptr)),
    len(static_cast<uint32_t>(len)),
    cnt(0)
{
    if constexpr (std::is_default_constructible<T>::value)
    {
        for (T *p = begin(); p != end(); ++p)
            new(p) T();
    }
}

template<class T>
inline VariableLengthArray<T>::~VariableLengthArray()
{
    if constexpr (std::is_destructible<T>::value)
    {
        for (T *p = begin(); p != end(); ++p)
            p->~T();
    }
#if defined(_MSC_VER) || defined(__MINGW32__)
    _freea(array);
#endif
}

template<class T>
inline void VariableLengthArray<T>::put(const T& element) noexcept
{
    MAGMA_ASSERT(cnt < len);
    array[cnt++] = element;
}
} // namespace magma::core

#define MAGMA_VLA(Type, var, count)\
    MAGMA_ASSERT(sizeof(Type) * count <= magma::core::VariableLengthArray<Type>::MaxSize);\
    magma::core::VariableLengthArray<Type> var(count ? MAGMA_ALLOCA(sizeof(Type) * count) : nullptr, count)
