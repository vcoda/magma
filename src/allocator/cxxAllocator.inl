namespace magma
{
inline void *CxxAllocator::operator new(std::size_t size)
{
    void *ptr = allocator ?
        allocator->alloc(size) :
        malloc(size);
    if (!ptr)
        throw std::bad_alloc();
#ifdef MAGMA_DEBUG
    ++allocCount;
#endif
    return ptr;
}

inline void *CxxAllocator::operator new(std::size_t size, const std::nothrow_t&) noexcept
{
    void *ptr = allocator ?
        allocator->alloc(size) :
        malloc(size);
#ifdef MAGMA_DEBUG
    if (ptr) ++allocCount;
#endif
    return ptr;
}

inline void* CxxAllocator::operator new(std::size_t, void* where) noexcept
{
    return where;
}

inline void CxxAllocator::operator delete(void *ptr)
{
#ifdef MAGMA_DEBUG
    if (ptr) --allocCount;
#endif
    if (allocator)
        allocator->free(ptr);
    else
        free(ptr);
}

inline void CxxAllocator::operator delete(void *ptr, const std::nothrow_t&)
{
#ifdef MAGMA_DEBUG
    if (ptr) --allocCount;
#endif
    if (allocator)
        allocator->free(ptr);
    else
        free(ptr);
}
} // namespace magma
