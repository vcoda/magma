namespace magma
{
template<class Type>
inline Dispatchable<Type>::Dispatchable(VkObjectType objectType, Type handle) noexcept:
    Object<Type>(objectType, handle, nullptr)
{}

template<class Type>
inline Dispatchable<Type>::Dispatchable(VkObjectType objectType, std::shared_ptr<IAllocator> allocator, Type handle) noexcept:
    Object<Type>(objectType, handle, std::move(allocator))
{}

template<class Type>
inline VkObjectType Dispatchable<Type>::getObjectType() const noexcept
{
    return Object<Type>::getType();
}

template<class Type>
inline uint64_t Dispatchable<Type>::getObjectHandle() const noexcept
{
    return reinterpret_cast<uint64_t>(Object<Type>::handle);
}
} // namespace magma
