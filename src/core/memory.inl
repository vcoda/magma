namespace magma::core
{
template<class T>
inline T *copy(const T *src) noexcept
{
    T *dst = nullptr;
    if (src)
    {
        dst = MAGMA_NEW T;
        if (dst)
            memcpy(dst, src, sizeof(T));
    }
    return dst;
}

template<class T>
inline T *copy(T *dst, const T *src) noexcept
{
    MAGMA_ASSERT(dst != src);
    if (dst && src && dst != src)
        memcpy(dst, src, sizeof(T));
    return dst;
}

template<class T>
inline T *copyArray(const T *src, std::size_t size) noexcept
{
    T *dst = nullptr;
    if (src && size)
    {
        dst = MAGMA_NEW T[size];
        if (dst)
            memcpy(dst, src, sizeof(T) * size);
    }
    return dst;
}

template<class T>
inline T *copyVector(const std::vector<T>& src) noexcept
{
    T *dst = nullptr;
    if (std::size_t size = src.size())
    {
        dst = MAGMA_NEW T[size];
        if (dst)
            memcpy(dst, src.data(), sizeof(T) * size);
    }
    return dst;
}

template<class T1, class T2>
inline T1 *copyVector(const std::vector<T2>& src) noexcept
{
    static_assert(sizeof(T1) == sizeof(T2), "type size mismatch");
    T1 *dst = nullptr;
    if (std::size_t size = src.size())
    {
        dst = MAGMA_NEW T1[size];
        if (dst)
            memcpy(dst, src.data(), sizeof(T2) * size);
    }
    return dst;
}

template<class T>
inline T *copyInitializerList(const std::initializer_list<T>& src) noexcept
{
    T *dst = nullptr;
    if (std::size_t size = src.size())
    {
        dst = MAGMA_NEW T[size];
        if (dst)
        {
            T *y = dst;
            for (const T& x: src)
                *y++ = x;
        }
    }
    return dst;
}

template<class T1, class T2>
inline T1 *copyInitializerList(const std::initializer_list<T2>& src) noexcept
{
    static_assert(sizeof(T1) == sizeof(T2), "type size mismatch");
    T1 *dst = nullptr;
    if (std::size_t size = src.size())
    {
        dst = MAGMA_NEW T1[size];
        if (dst)
        {
            T1 *y = dst;
            for (const T2& x: src)
                memcpy(y++, &x, sizeof(T2));
        }
    }
    return dst;
}

inline char *copyString(const char *src) noexcept
{
    MAGMA_ASSERT(src);
    if (!src)
        return nullptr;
    const std::size_t count = strlen(src) + 1;
    char *dst = MAGMA_NEW char[count];
    if (dst)
    {
    #if defined(_MSC_VER) || defined(__MINGW32__)
        const errno_t err = strcpy_s(dst, count, src);
        MAGMA_ASSERT(0 == err);
    #else
        strcpy(dst, src);
    #endif // _MSC_VER || __MINGW32__
    }
    return dst;
}

inline char *copyString(char *dst, std::size_t size, const char *src) noexcept
{
#if defined(_MSC_VER) || defined(__MINGW32__)
    const errno_t err = strcpy_s(dst, size, src);
    MAGMA_ASSERT(0 == err);
#else
    MAGMA_ASSERT(strlen(src) < size);
    MAGMA_UNUSED(size);
    strcpy(dst, src);
#endif // _MSC_VER || __MINGW32__
    return dst;
}

inline uint8_t *copyBinaryData(const void *src, std::size_t size) noexcept
{
    uint8_t *dst = nullptr;
    if (src && size)
    {
        dst = MAGMA_NEW uint8_t[size];
        if (dst)
            memcpy(dst, src, size);
    }
    return dst;
}

template<class T>
inline uint8_t *copyBinaryData(const T& src) noexcept
{
    return copyBinaryData(&src, sizeof(T));
}

template<class T>
inline bool compare(const void *p1, const void *p2) noexcept
{
    if (p1 == p2)
        return true;
    if (!p1 || !p2)
        return false;
    return !memcmp(p1, p2, sizeof(T));
}

template<class T>
inline bool compare(const T *p1, const T *p2) noexcept
{
    if (p1 == p2)
        return true;
    if (!p1 || !p2)
        return false;
    return !memcmp(p1, p2, sizeof(T));
}

template<class T>
inline bool compareArrays(const T *p1, const T *p2, std::size_t count) noexcept
{
    if (p1 == p2)
        return true;
    if (!p1 || !p2)
        return false;
    return !memcmp(p1, p2, sizeof(T) * count);
}

template<class T>
inline void zeroMemory(T& var) noexcept
{
    memset(&var, 0, sizeof(T));
}

template<class T, std::size_t N>
inline void zeroMemory(T (&array)[N]) noexcept
{
    memset(array, 0, sizeof(T) * N);
}
} // magma::core
