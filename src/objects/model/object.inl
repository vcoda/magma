namespace magma
{
template<class Type>
inline Object<Type>::Object(VkObjectType objectType, Type handle, std::shared_ptr<IAllocator> allocator) noexcept:
#if (VK_USE_64_BIT_PTR_DEFINES == 0)
    objectType(objectType),
#endif
    handle(handle),
    hostAllocator(std::move(allocator))
{
    MAGMA_UNUSED(objectType);
}

template<class Type>
inline VkObjectType Object<Type>::getType() const noexcept
{
#if (VK_USE_64_BIT_PTR_DEFINES == 1)
    return ObjectType<Type>::getType();
#else
    return objectType;
#endif
}
} // namespace magma
