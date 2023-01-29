namespace magma
{
template<class Type>
inline void Object::setDebugTag(uint64_t tagName, const Type& tag)
{
    setDebugTag(tagName, sizeof(Type), &tag);
}

template<class Type>
inline ObjectT<Type>::ObjectT(VkObjectType objectType, std::shared_ptr<Device> device, std::shared_ptr<IAllocator> hostAllocator) noexcept:
    Object(std::move(device), std::move(hostAllocator)),
#if !defined(MAGMA_X64)
    objectType(objectType),
#endif
    handle(VK_NULL_HANDLE)
{
    MAGMA_UNUSED(objectType);
}

template<class Type>
inline ObjectT<Type>::ObjectT(VkObjectType objectType, Type handle, std::shared_ptr<Device> device, std::shared_ptr<IAllocator> hostAllocator) noexcept:
    Object(std::move(device), std::move(hostAllocator)),
#if !defined(MAGMA_X64)
    objectType(objectType),
#endif
    handle(handle)
{
    MAGMA_UNUSED(objectType);
}

template<class Type>
inline VkObjectType ObjectT<Type>::getObjectType() const noexcept
{
#ifdef MAGMA_X64
    return ObjectType<Type>::getObjectType();
#else
    return objectType;
#endif
}
} // namespace magma
