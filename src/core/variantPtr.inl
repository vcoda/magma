namespace magma::core
{
template<class Type>
inline variant_ptr<Type>::variant_ptr(std::unique_ptr<Type> ptr) noexcept:
    ptr(std::move(ptr))
{}

template<class Type>
inline variant_ptr<Type>::variant_ptr(std::shared_ptr<Type> ptr) noexcept:
    ptr(std::move(ptr))
{}

template<class Type>
inline Type *variant_ptr<Type>::get() noexcept
{
    try
    {
        auto visitor = [](auto& p) -> Type* { return p.get(); };
        return std::visit(visitor, ptr);
    }
    catch (const std::bad_variant_access&)
    {
        return nullptr;
    }
}

template<class Type>
inline const Type *variant_ptr<Type>::get() const noexcept
{
    try
    {
        auto visitor = [](auto const& p) -> Type* { return p.get(); };
        return std::visit(visitor, ptr);
    }
    catch (const std::bad_variant_access&)
    {
        return nullptr;
    }
}

template<class Type>
inline Type *variant_ptr<Type>::operator->() noexcept
{
    return get();
}

template<class Type>
inline const Type *variant_ptr<Type>::operator->() const noexcept
{
    return get();
}

template<class Type>
inline Type& variant_ptr<Type>::operator*() noexcept
{
    return *get();
}

template<class Type>
inline const Type& variant_ptr<Type>::operator*() const noexcept
{
    return *get();
}

template<class Type>
inline variant_ptr<Type>::operator bool() const
{
    auto visitor = [](auto const& p) -> bool { return p.operator bool(); };
    return std::visit(visitor, ptr);
}
} // namespace magma::core
