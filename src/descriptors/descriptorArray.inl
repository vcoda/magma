namespace magma::descriptor
{
template<class Descriptor>
inline void DescriptorArray<Descriptor>::reserve(std::size_t capacity)
{
    std::lock_guard<core::Spinlock> quard(*lock);
    descriptors.reserve(capacity);
    assigned.reserve(capacity);
}

template<class Descriptor>
inline BindlessHandle DescriptorArray<Descriptor>::insert(const Descriptor& descriptor)
{
    std::lock_guard<core::Spinlock> quard(*lock);
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
inline bool DescriptorArray<Descriptor>::erase(BindlessHandle handle)
{
    std::lock_guard<core::Spinlock> quard(*lock);
    if (handle >= assigned.size() || !assigned[handle])
        return false;
    descriptors[handle] = Descriptor{};
    assigned[handle] = false;
    unusedHandles.push_back(handle);
    return true;
}

template<class Descriptor>
inline void DescriptorArray<Descriptor>::clear() noexcept
{
    std::lock_guard<core::Spinlock> quard(*lock);
    descriptors.clear();
    assigned.clear();
    unusedHandles.clear();
}

template<class Descriptor>
inline size_t DescriptorArray<Descriptor>::size() const noexcept
{
    std::lock_guard<core::Spinlock> quard(*lock);
    return descriptors.size();
}

template<class Descriptor>
inline uint32_t DescriptorArray<Descriptor>::numDescriptors() const noexcept
{
    std::lock_guard<core::Spinlock> quard(*lock);
    return (uint32_t)(descriptors.size() - unusedHandles.size());
}
} // namespace magma::descriptor
