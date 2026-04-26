namespace magma::descriptor
{
template<class Descriptor>
inline DescriptorPool<Descriptor>::DescriptorPool():
    lock(std::make_shared<core::Spinlock>())
{
    descriptors.reserve(16);
    assigned.reserve(16);
}

template<class Descriptor>
inline void DescriptorPool<Descriptor>::reserve(std::size_t capacity)
{
    std::lock_guard<core::Spinlock> guard(*lock);
    descriptors.reserve(capacity);
    assigned.reserve(capacity);
}

template<class Descriptor>
inline BindlessHandle DescriptorPool<Descriptor>::insert(const Descriptor& descriptor)
{
    std::lock_guard<core::Spinlock> guard(*lock);
    BindlessHandle handle;
    if (unusedHandles.empty())
    {
        handle = (BindlessHandle)descriptors.size();
        descriptors.push_back(descriptor);
        assigned.push_back(true);
    }
    else
    {
        handle = unusedHandles.back();
        unusedHandles.pop_back();
        descriptors[handle] = descriptor;
        assigned[handle] = true;
    }
    return handle;
}

template<class Descriptor>
inline bool DescriptorPool<Descriptor>::erase(BindlessHandle handle)
{
    std::lock_guard<core::Spinlock> guard(*lock);
    if (handle >= assigned.size() || !assigned[handle])
        return false;
    descriptors[handle] = Descriptor{};
    assigned[handle] = false;
    unusedHandles.push_back(handle);
    return true;
}

template<class Descriptor>
inline void DescriptorPool<Descriptor>::clear() noexcept
{
    std::lock_guard<core::Spinlock> guard(*lock);
    descriptors.clear();
    assigned.clear();
    unusedHandles.clear();
}

template<class Descriptor>
inline std::size_t DescriptorPool<Descriptor>::size() const noexcept
{
    std::lock_guard<core::Spinlock> guard(*lock);
    return descriptors.size();
}

template<class Descriptor>
inline std::size_t DescriptorPool<Descriptor>::numDescriptors() const noexcept
{
    std::lock_guard<core::Spinlock> guard(*lock);
    return (uint32_t)(descriptors.size() - unusedHandles.size());
}
} // namespace magma::descriptor
