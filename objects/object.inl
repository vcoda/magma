namespace magma
{
inline Object::Object(std::shared_ptr<Device> device, std::shared_ptr<IAllocator> hostAllocator) noexcept:
    device(std::move(device)),
    hostAllocator(std::move(hostAllocator))
{}

#ifdef MAGMA_DEBUG
template<class Type>
inline void Object::setDebugTag(uint64_t tagName, const Type& tag)
{
    setDebugTag(tagName, sizeof(Type), &tag);
}
#endif // MAGMA_DEBUG

// The following Vulkan objects are created before logical device is initialized,
// so they call constructor w/o <device> parameter:
// VK_OBJECT_TYPE_INSTANCE
// VK_OBJECT_TYPE_SURFACE_KHR
// VK_OBJECT_TYPE_PHYSICAL_DEVICE
// VK_OBJECT_TYPE_DEVICE
// VK_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT
// VK_OBJECT_TYPE_DEBUG_UTILS_MESSENGER_EXT
template<class Type>
inline TObject<Type>::TObject(VkObjectType objectType, std::shared_ptr<IAllocator> hostAllocator) noexcept:
    Object(nullptr, std::move(hostAllocator)),
#if !defined(MAGMA_X64)
    objectType(objectType),
#endif
    handle(VK_NULL_HANDLE)
{
    MAGMA_UNUSED(objectType);
}

template<class Type>
inline TObject<Type>::TObject(VkObjectType objectType, std::shared_ptr<Device> device_, std::shared_ptr<IAllocator> hostAllocator):
    Object(std::move(device_), std::move(hostAllocator)),
#if !defined(MAGMA_X64)
    objectType(objectType),
#endif
    handle(VK_NULL_HANDLE)
{
    MAGMA_UNUSED(objectType);
    if (!device)
        throw std::invalid_argument("invalid device argument");
}

template<class Type>
inline TObject<Type>::TObject(VkObjectType objectType, Type handle, std::shared_ptr<Device> device_, std::shared_ptr<IAllocator> hostAllocator):
    Object(std::move(device_), std::move(hostAllocator)),
#if !defined(MAGMA_X64)
    objectType(objectType),
#endif
    handle(handle)
{
    MAGMA_UNUSED(objectType);
    if (!device)
        throw std::invalid_argument("invalid device argument");
}

template<class Type>
inline VkObjectType TObject<Type>::getObjectType() const noexcept
{
#ifdef MAGMA_X64
    return ObjectType<Type>::getObjectType();
#else
    return objectType;
#endif
}
} // namespace magma
