namespace magma
{
template<typename Type>
inline void Object::setDebugTag(uint64_t tagName, const Type& tag)
{
    setDebugTag(tagName, sizeof(Type), &tag);
}

template<typename Type>
inline ObjectT<Type>::ObjectT(VkObjectType objectType, std::shared_ptr<Device> device, std::shared_ptr<IAllocator> hostAllocator) noexcept:
    Object(std::move(device), std::move(hostAllocator))
#if !defined(MAGMA_X64)
    ,
    // Additional storage is required under x86 target
    // as Vulkan non-dispatchable handles are defined as uint64_t
    // and thus cannot be used in custom template specialization.
    objectType(objectType)
#endif
{
    MAGMA_UNUSED(objectType);
}

template<typename Type>
inline VkObjectType ObjectT<Type>::getObjectType() const noexcept
{
#ifdef MAGMA_X64
    return ObjectType<Type>::getObjectType();
#else
    return objectType;
#endif
}
} // namespace magma
