namespace magma
{
template<class T>
inline variant_ptr<T>::variant_ptr(std::nullptr_t) noexcept
{}

template<class T>
inline variant_ptr<T>::variant_ptr(std::unique_ptr<T> p) noexcept:
    var(std::move(p))
{}

template<class T>
inline variant_ptr<T>::variant_ptr(std::shared_ptr<T> p) noexcept:
    var(std::move(p))
{}

template<class T>
inline variant_ptr<T>::variant_ptr(variant_ptr<T>&& other) noexcept:
    var(std::move(other.var))
{}

template<class T>
inline T *variant_ptr<T>::get() const noexcept
{
    auto visitor = [](auto& p) -> T* { return p.get(); };
    try
    {
        return std::visit(visitor, var);
    }
    catch (const std::bad_variant_access&)
    {
        return nullptr;
    }
}

template<class T>
inline variant_ptr<T>& variant_ptr<T>::operator=(variant_ptr<T>&& other) noexcept
{
    if (this != &other)
        var = std::move(other.var);
    return *this;
}

template<class T>
inline T *variant_ptr<T>::operator->() const noexcept
{
    T *pointer = get();
    MAGMA_ASSERT(pointer);
    return pointer;
}

template<class T>
inline T& variant_ptr<T>::operator*() const noexcept
{
    T *pointer = get();
    MAGMA_ASSERT(pointer);
    return *pointer;
}

template<class T>
inline variant_ptr<T>::operator bool() const noexcept
{
    if (var.index() == std::variant_npos)
        return false;
    T *pointer = get();
    return (pointer != nullptr);
}

template<class T>
inline bool variant_ptr<T>::operator<(const variant_ptr<T>& p) noexcept
{
    return (get() < p.get());
}

template<class T>
inline bool variant_ptr<T>::operator>(const variant_ptr<T>& p) noexcept
{
    return (get() > p.get());
}

template<class T>
inline bool variant_ptr<T>::operator<=(const variant_ptr<T>& p) noexcept
{
    return (get() <= p.get());
}

template<class T>
inline bool variant_ptr<T>::operator>=(const variant_ptr<T>& p) noexcept
{
    return (get() >= p.get());
}

template<class T>
inline bool variant_ptr<T>::operator==(const variant_ptr<T>& p) noexcept
{
    return (get() == p.get());
}

template<class T>
inline bool variant_ptr<T>::operator!=(const variant_ptr<T>& p) noexcept
{
    return (get() != p.get());
}

template<class T>
inline bool variant_ptr<T>::operator<(const T *p) noexcept
{
    return (get() < p);
}

template<class T>
inline bool variant_ptr<T>::operator>(const T *p) noexcept
{
    return (get() > p);
}

template<class T>
inline bool variant_ptr<T>::operator<=(const T *p) noexcept
{
    return (get() <= p);
}

template<class T>
inline bool variant_ptr<T>::operator>=(const T *p) noexcept
{
    return (get() >= p);
}

template<class T>
inline bool variant_ptr<T>::operator==(const T *p) noexcept
{
    return (get() == p);
}

template<class T>
inline bool variant_ptr<T>::operator!=(const T *p) noexcept
{
    return (get() != p);
}

template<class T>
inline bool variant_ptr<T>::operator<(const std::shared_ptr<T>& p) noexcept
{
    return (get() < p.get());
}

template<class T>
inline bool variant_ptr<T>::operator>(const std::shared_ptr<T>& p) noexcept
{
    return (get() > p.get());
}

template<class T>
inline bool variant_ptr<T>::operator<=(const std::shared_ptr<T>& p) noexcept
{
    return (get() <= p.get());
}

template<class T>
inline bool variant_ptr<T>::operator>=(const std::shared_ptr<T>& p) noexcept
{
    return (get() >= p.get());
}

template<class T>
inline bool variant_ptr<T>::operator==(const std::shared_ptr<T>& p) noexcept
{
    return (get() == p.get());
}

template<class T>
inline bool variant_ptr<T>::operator!=(const std::shared_ptr<T>& p) noexcept
{
    return (get() != p.get());
}

template<class T>
inline bool variant_ptr<T>::operator<(const std::unique_ptr<T>& p) noexcept
{
    return (get() < p.get());
}

template<class T>
inline bool variant_ptr<T>::operator>(const std::unique_ptr<T>& p) noexcept
{
    return (get() > p.get());
}

template<class T>
inline bool variant_ptr<T>::operator<=(const std::unique_ptr<T>& p) noexcept
{
    return (get() <= p.get());
}

template<class T>
inline bool variant_ptr<T>::operator>=(const std::unique_ptr<T>& p) noexcept
{
    return (get() >= p.get());
}

template<class T>
inline bool variant_ptr<T>::operator==(const std::unique_ptr<T>& p) noexcept
{
    return (get() == p.get());
}

template<class T>
inline bool variant_ptr<T>::operator!=(const std::unique_ptr<T>& p) noexcept
{
    return (get() != p.get());
}
} // namespace magma
