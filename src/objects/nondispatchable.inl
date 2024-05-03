namespace magma
{
// The following Vulkan objects are created before logical device is initialized,
// so they call constructor w/o <device> parameter:
// VK_OBJECT_TYPE_INSTANCE
// VK_OBJECT_TYPE_SURFACE_KHR
// VK_OBJECT_TYPE_PHYSICAL_DEVICE
// VK_OBJECT_TYPE_DEVICE
// VK_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT
// VK_OBJECT_TYPE_DEBUG_UTILS_MESSENGER_EXT
template<class Type>
inline NonDispatchable<Type>::NonDispatchable(VkObjectType objectType, std::shared_ptr<IAllocator> allocator) noexcept:
    Object<Type>(objectType, std::move(allocator))
{}

template<class Type>
inline NonDispatchable<Type>::NonDispatchable(VkObjectType objectType, std::shared_ptr<Device> device_,
    std::shared_ptr<IAllocator> allocator) noexcept:
    Object<Type>(objectType, std::move(allocator)),
    DebugUtilsObject(std::move(device_))
{
#if (VK_USE_64_BIT_PTR_DEFINES == 1)
    std::shared_ptr<ResourcePool> pool = DeviceResourcePool::getPool(device);
    if (pool)
        pool->getPool<NonDispatchable<Type>>().insert(this);
#endif // (VK_USE_64_BIT_PTR_DEFINES == 1)
}

template<class Type>
inline NonDispatchable<Type>::~NonDispatchable()
{
#if (VK_USE_64_BIT_PTR_DEFINES == 1)
    std::shared_ptr<ResourcePool> pool = DeviceResourcePool::getPool(device);
    if (pool)
        pool->getPool<NonDispatchable<Type>>().erase(this);
#endif // (VK_USE_64_BIT_PTR_DEFINES == 1)
}

template<class Type>
inline uint64_t NonDispatchable<Type>::getObjectHandle() const noexcept
{
#if (VK_USE_64_BIT_PTR_DEFINES == 1)
    return reinterpret_cast<uint64_t>(Object<Type>::handle);
#else
    return Object<Type>::handle;
#endif // (VK_USE_64_BIT_PTR_DEFINES == 1)
}

template<class Type>
inline void NonDispatchable<Type>::setPrivateData(uint64_t data)
{
    if (!pimpl)
        pimpl = std::make_unique<NonDispatchableImpl>();
    pimpl->setPrivateData(this, device, data);
}

template<class Type>
inline uint64_t NonDispatchable<Type>::getPrivateData() const noexcept
{
    if (pimpl)
        return pimpl->getPrivateData(this, device);
    return 0ull;
}

template<class Type>
inline void NonDispatchable<Type>::setDebugName(const char *name)
{
    DebugUtilsObject::setDebugName(this, name);
}

template<class Type>
inline void NonDispatchable<Type>::setDebugTag(uint64_t tagName, std::size_t tagSize, const void *tag)
{
    DebugUtilsObject::setDebugTag(this, tagName, tagSize, tag);
}

template<class Type>
template<class Tag>
inline void NonDispatchable<Type>::setDebugTag(uint64_t tagName, const Tag& tag)
{
    DebugUtilsObject::setDebugTag(this, tagName, sizeof(Tag), &tag);
}
} // namespace magma
