namespace magma
{
template<class Type>
inline NonDispatchable<Type>::NonDispatchable(VkObjectType objectType, std::shared_ptr<IAllocator> allocator) noexcept:
    Object<Type>(objectType, std::move(allocator)),
    DeviceChild(nullptr)
{}

template<class Type>
inline NonDispatchable<Type>::NonDispatchable(VkObjectType objectType, std::shared_ptr<Device> device_,
    std::shared_ptr<IAllocator> allocator) noexcept:
    Object<Type>(objectType, std::move(allocator)),
    DeviceChild(std::move(device_))
{
#if (VK_USE_64_BIT_PTR_DEFINES == 1)
    auto pool = getResourcePool();
    std::lock_guard<std::mutex> lock(pool->getMutex());
    pool->getPool<Type>().insert(this);
#endif // (VK_USE_64_BIT_PTR_DEFINES == 1)
}

template<class Type>
inline NonDispatchable<Type>::~NonDispatchable()
{
#if (VK_USE_64_BIT_PTR_DEFINES == 1)
    if (device)
    {
        auto pool = getResourcePool();
        std::lock_guard<std::mutex> lock(pool->getMutex());
        pool->getPool<Type>().erase(this);
    }
#endif // (VK_USE_64_BIT_PTR_DEFINES == 1)
}

template<class Type>
inline VkObjectType NonDispatchable<Type>::getObjectType() const noexcept
{
    return Object<Type>::getType();
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
} // namespace magma
