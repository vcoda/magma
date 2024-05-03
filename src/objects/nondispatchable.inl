namespace magma
{
template<class Type>
inline NonDispatchable<Type>::NonDispatchable(VkObjectType objectType, std::shared_ptr<IAllocator> allocator) noexcept:
    Object<Type>(objectType, std::move(allocator)),
    NonDispatchableImpl(nullptr)
{}

template<class Type>
inline NonDispatchable<Type>::NonDispatchable(VkObjectType objectType, std::shared_ptr<Device> device_,
    std::shared_ptr<IAllocator> allocator) noexcept:
    Object<Type>(objectType, std::move(allocator)),
    NonDispatchableImpl(std::move(device_))
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
    NonDispatchableImpl::setPrivateData(this, data);
}

template<class Type>
inline uint64_t NonDispatchable<Type>::getPrivateData() const noexcept
{
    return NonDispatchableImpl::getPrivateData(this);
}

template<class Type>
inline void NonDispatchable<Type>::setDebugName(const char *name)
{
    NonDispatchableImpl::setDebugName(this, name);
}

template<class Type>
inline void NonDispatchable<Type>::setDebugTag(uint64_t tagName, std::size_t tagSize, const void *tag)
{
    NonDispatchableImpl::setDebugTag(this, tagName, tagSize, tag);
}

template<class Type>
template<class Tag>
inline void NonDispatchable<Type>::setDebugTag(uint64_t tagName, const Tag& tag)
{
    NonDispatchableImpl::setDebugTag(this, tagName, sizeof(Tag), &tag);
}
} // namespace magma
