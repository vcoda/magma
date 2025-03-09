namespace magma
{
template<class T>
inline lent_ptr<T>::lent_ptr(T *p) noexcept:
    pointer(p)
{}

template<class T>
inline lent_ptr<T>::lent_ptr(std::nullptr_t) noexcept:
    pointer(nullptr)
{}

template<class T> template<class T2>
inline lent_ptr<T>::lent_ptr(const std::shared_ptr<T2>& p) noexcept:
    pointer(p.get()),
    ref(p)
{}

template<class T> template<class T2>
inline lent_ptr<T>::lent_ptr(const std::unique_ptr<T2>& p) noexcept:
    pointer(p.get())
{}

template<class T> template<class T2>
inline lent_ptr<T>::lent_ptr(const std::weak_ptr<T2>& ref) noexcept:
    pointer(nullptr),
    ref(ref)
{}

template<class T> template<class T2>
inline lent_ptr<T>::lent_ptr(const variant_ptr<T2>& p) noexcept:
    pointer(p.get())
{}

template<class T> template<class T2>
inline lent_ptr<T>::lent_ptr(lent_ptr<T2>&& other) noexcept:
    pointer(other.pointer),
    ref(std::move(other.ref))
{
    other.pointer = nullptr;
}

template<class T>
inline bool lent_ptr<T>::shareable() const noexcept
{
    return !ref.expired();
}

template<class T>
inline void lent_ptr<T>::reset() noexcept
{
    pointer = nullptr;
    ref.reset();
}

template<class T> template<class T2>
inline lent_ptr<T>& lent_ptr<T>::operator=(lent_ptr<T2>&& other) noexcept
{
    if (this != &other)
    {
        pointer = other.pointer;
        ref = std::move(other.ref);
        other.pointer = nullptr;
    }
    return *this;
}

template<class T>
inline T *lent_ptr<T>::operator->() const noexcept
{
    MAGMA_ASSERT(pointer);
    return pointer;
}

template<class T>
inline T& lent_ptr<T>::operator*() const noexcept
{
    MAGMA_ASSERT(pointer);
    return *pointer;
}

template<class T>
inline lent_ptr<T>::operator bool() const noexcept
{
    return (pointer != nullptr);
}

template<class T>
inline bool lent_ptr<T>::operator<(const lent_ptr<T>&p) noexcept
{
    return (pointer < p.pointer);
}

template<class T>
inline bool lent_ptr<T>::operator>(const lent_ptr<T>&p) noexcept
{
    return (pointer > p.pointer);
}

template<class T>
inline bool lent_ptr<T>::operator<=(const lent_ptr<T>&p) noexcept
{
    return (pointer <= p.pointer);
}

template<class T>
inline bool lent_ptr<T>::operator>=(const lent_ptr<T>&p) noexcept
{
    return (pointer >= p.pointer);
}

template<class T>
inline bool lent_ptr<T>::operator==(const lent_ptr<T>&p) noexcept
{
    return (pointer == p.pointer);
}

template<class T>
inline bool lent_ptr<T>::operator!=(const lent_ptr<T>&p) noexcept
{
    return (pointer != p.pointer);
}

template<class T>
inline bool lent_ptr<T>::operator<(const T *p) noexcept
{
    return (pointer < p);
}

template<class T>
inline bool lent_ptr<T>::operator>(const T *p) noexcept
{
    return (pointer > p);
}

template<class T>
inline bool lent_ptr<T>::operator<=(const T *p) noexcept
{
    return (pointer <= p);
}

template<class T>
inline bool lent_ptr<T>::operator>=(const T *p) noexcept
{
    return (pointer >= p);
}

template<class T>
inline bool lent_ptr<T>::operator==(const T *p) noexcept
{
    return (pointer == p);
}

template<class T>
inline bool lent_ptr<T>::operator!=(const T *p) noexcept
{
    return (pointer != p);
}

template<class T>
inline bool lent_ptr<T>::operator<(const std::shared_ptr<T>&p) noexcept
{
    return (pointer < p.get());
}

template<class T>
inline bool lent_ptr<T>::operator>(const std::shared_ptr<T>&p) noexcept
{
    return (pointer > p.get());
}

template<class T>
inline bool lent_ptr<T>::operator<=(const std::shared_ptr<T>&p) noexcept
{
    return (pointer <= p.get());
}

template<class T>
inline bool lent_ptr<T>::operator>=(const std::shared_ptr<T>&p) noexcept
{
    return (pointer >= p.get());
}

template<class T>
inline bool lent_ptr<T>::operator==(const std::shared_ptr<T>&p) noexcept
{
    return (pointer == p.get());
}

template<class T>
inline bool lent_ptr<T>::operator!=(const std::shared_ptr<T>&p) noexcept
{
    return (pointer != p.get());
}

template<class T>
inline bool lent_ptr<T>::operator<(const std::unique_ptr<T>&p) noexcept
{
    return (pointer < p.get());
}

template<class T>
inline bool lent_ptr<T>::operator>(const std::unique_ptr<T>&p) noexcept
{
    return (pointer > p.get());
}

template<class T>
inline bool lent_ptr<T>::operator<=(const std::unique_ptr<T>&p) noexcept
{
    return (pointer <= p.get());
}

template<class T>
inline bool lent_ptr<T>::operator>=(const std::unique_ptr<T>&p) noexcept
{
    return (pointer >= p.get());
}

template<class T>
inline bool lent_ptr<T>::operator==(const std::unique_ptr<T>&p) noexcept
{
    return (pointer == p.get());
}

template<class T>
inline bool lent_ptr<T>::operator!=(const std::unique_ptr<T>&p) noexcept
{
    return (pointer != p.get());
}

template<class T>
inline bool lent_ptr<T>::operator<(const variant_ptr<T>&p) noexcept
{
    return (pointer < p.get());
}

template<class T>
inline bool lent_ptr<T>::operator>(const variant_ptr<T>&p) noexcept
{
    return (pointer > p.get());
}

template<class T>
inline bool lent_ptr<T>::operator<=(const variant_ptr<T>&p) noexcept
{
    return (pointer <= p.get());
}

template<class T>
inline bool lent_ptr<T>::operator>=(const variant_ptr<T>&p) noexcept
{
    return (pointer >= p.get());
}

template<class T>
inline bool lent_ptr<T>::operator==(const variant_ptr<T>&p) noexcept
{
    return (pointer == p.get());
}

template<class T>
inline bool lent_ptr<T>::operator!=(const variant_ptr<T>&p) noexcept
{
    return (pointer != p.get());
}
} // namespace magma
