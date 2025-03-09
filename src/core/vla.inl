namespace magma::core
{
template<class T>
VariableLengthArray<T>::VariableLengthArray(void *ptr, std::size_t len) noexcept:
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
VariableLengthArray<T>::~VariableLengthArray()
{
    if constexpr (std::is_destructible<T>::value)
    {
        for (T *p = begin(); p != end(); ++p)
            p->~T();
    }
    MAGMA_STACK_FREE(array);
}

template<class T>
void VariableLengthArray<T>::put(const T& element) noexcept
{
    MAGMA_ASSERT(cnt < len);
    array[cnt++] = element;
}
} // namespace magma::core

#define MAGMA_VLA(Type, var, count)\
    MAGMA_ASSERT(sizeof(Type) * count <= MAGMA_MAX_STACK_SIZE);\
    magma::core::VariableLengthArray<Type> var(MAGMA_STACK_ALLOC(sizeof(Type) * count), count)
