namespace magma
{
inline void *ObjectAllocator::operator new(std::size_t size)
{
    void *ptr;
    if (allocator)
        ptr = allocator->alloc(size);
    else
        ptr = malloc(size);
    if (!ptr)
        throw std::bad_alloc();
#ifdef MAGMA_DEBUG
    ++allocCount;
#endif
    return ptr;
}

inline void *ObjectAllocator::operator new(std::size_t size, const std::nothrow_t&) noexcept
{
    void *ptr;
    if (allocator)
        ptr = allocator->alloc(size);
    else
        ptr = malloc(size);
#ifdef MAGMA_DEBUG
    if (ptr)
        ++allocCount;
#endif
    return ptr;
}

inline void* ObjectAllocator::operator new(std::size_t, void* where) noexcept
{
    return where;
}

inline void ObjectAllocator::operator delete(void *ptr)
{
#ifdef MAGMA_DEBUG
    if (ptr)
        --allocCount;
#endif
    if (allocator)
        allocator->free(ptr);
    else
        free(ptr);
}
} // namespace magma
