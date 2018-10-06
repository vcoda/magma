#include <functional>

namespace magma
{
namespace helpers
{
// https://www.boost.org/doc/libs/1_46_1/doc/html/hash/reference.html#boost.hash_combine
inline void hashCombine(size_t& seed, size_t hash) noexcept
{
    seed ^= (hash + 0x9e3779b9 + (seed << 6) + (seed >> 2));
}

template<typename Type>
inline void hashCombineArg(size_t& seed, const Type& arg) noexcept
{
    std::hash<Type> hasher;
    const size_t hash = hasher(arg);
    hashCombine(seed, hash);
}

template<typename Type>
inline void __nextHash(size_t& seed, const Type& arg) noexcept
{
    hashCombineArg(seed, arg);
}

template<typename Type, typename... Args>
inline void __nextHash(size_t& seed, const Type& arg, Args... args) noexcept
{
    hashCombineArg(seed, arg);
    __nextHash(seed, args...);
}

template<typename Type, typename... Args>
inline size_t hashVariadic(const Type& arg, Args... args) noexcept
{
    size_t value = 0;
    hashCombineArg(value, arg);
    __nextHash(value, args...);
    return value;
}

template<typename Type>
inline size_t hashArray(const Type *const array, size_t count) noexcept
{
    size_t value = 0;
    for (size_t i = 0; i < count; ++i)
        hashCombineArg(value, array[i]);
    return value;
}
} // namespace helpers
} // namespace magma
