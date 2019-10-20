namespace magma
{
inline void *Object::operator new(std::size_t size)
{
    void *ptr;
    if (objectAllocator)
        ptr = objectAllocator->alloc(size);
    else
        ptr = malloc(size);
    if (!ptr)
        throw std::bad_alloc();
#ifdef MAGMA_DEBUG
    ++allocCount;
#endif
    return ptr;
}

inline void *Object::operator new(std::size_t size, const std::nothrow_t&) noexcept
{
    void *ptr;
    if (objectAllocator)
        ptr = objectAllocator->alloc(size);
    else
        ptr = malloc(size);
#ifdef MAGMA_DEBUG
    if (ptr)
        ++allocCount;
#endif
    return ptr;
}

inline void Object::operator delete(void *ptr)
{
#ifdef MAGMA_DEBUG
    if (ptr)
        --allocCount;
#endif
    if (objectAllocator)
        objectAllocator->free(ptr);
    else
        free(ptr);
}
} // namespace magma
