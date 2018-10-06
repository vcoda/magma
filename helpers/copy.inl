#include <cstring>
#include "../shared.h"

namespace magma
{
namespace helpers
{
template<typename Type>
inline Type *copy(const Type *const src)
{
    MAGMA_ASSERT(src);
    Type *dst = new Type();
    memcpy(dst, src, sizeof(Type));
    return dst;
}

template<typename Type>
inline Type *copy(Type *const dst, const Type *const src)
{
    MAGMA_ASSERT(dst);
    MAGMA_ASSERT(src);
    memcpy(dst, src, sizeof(Type));
    return dst;
}

template<typename Type>
inline Type *copyArray(const Type *const src, size_t count)
{
    MAGMA_ASSERT(src);
    MAGMA_ASSERT(count > 0);
    Type *dst = new Type[count];
    memcpy(dst, src, sizeof(Type) * count);
    return dst;
}

template<typename Type>
inline Type *copyVector(const std::vector<Type>& src)
{
    MAGMA_ASSERT(src.size() > 0);
    Type *dst = new Type[src.size()];
    memcpy(dst, src.data(), sizeof(Type) * src.size());
    return dst;
}

template<typename Type>
inline Type *copyInitializer(const std::initializer_list<Type>& src)
{
    MAGMA_ASSERT(src.size() > 0);
    Type *dst = new Type[src.size()];
    memcpy(dst, src.begin(), sizeof(Type) * src.size());
    return dst;
}

inline char *copyString(const char *const src)
{
    MAGMA_ASSERT(src);
    const size_t size = strlen(src) + 1;
    char *const dst = new char[size];
#ifdef _MSC_VER
    const errno_t err = strcpy_s(dst, size, src);
    MAGMA_ASSERT(0 == err);
#else
    strcpy(dst, src);
#endif // _MSC_VER
    return dst;
}

template<typename Type>
inline bool compareArrays(const Type *const src, const Type *const dst, size_t count) noexcept
{
    if (!src && !dst)
        return true;
    if (!src || !dst)
        return false;
    return (memcmp(src, dst, sizeof(Type) * count) == 0);
}
} // namespace helpers
} // namespace magma
