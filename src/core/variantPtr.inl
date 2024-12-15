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

} // namespace magma::core
