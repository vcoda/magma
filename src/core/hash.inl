namespace magma::core
{
template<>
struct ConstexprHash<uint32_t>
{   // Robert Jenkins' reversible 32 bit mix hash function
    constexpr hash_t operator()(uint32_t x) const noexcept
    {
        x += (x << 12); // x *= (1 + (1 << 12))
        x ^= (x >> 22);
        x += (x << 4);  // x *= (1 + (1 << 4))
        x ^= (x >> 9);
        x += (x << 10); // x *= (1 + (1 << 10))
        x ^= (x >> 2);
        // x *= (1 + (1 << 7)) * (1 + (1 << 12))
        x += (x << 7);
        x += (x << 12);
        return x;
    }
};

template<>
struct ConstexprHash<uint64_t>
{   // Thomas Wang 64 bit mix hash function
    constexpr hash_t operator()(uint64_t x) const noexcept
    {
        x = (~x) + (x << 21); // x *= (1 << 21) - 1; x -= 1;
        x = x ^ (x >> 24);
        x = x + (x << 3) + (x << 8); // x *= 1 + (1 << 3) + (1 << 8)
        x = x ^ (x >> 14);
        x = x + (x << 2) + (x << 4); // x *= 1 + (1 << 2) + (1 << 4)
        x = x ^ (x >> 28);
        x = x + (x << 31); // x *= 1 + (1 << 31)
        return x;
    }
};

template<class Int>
constexpr hash_t ConstexprHash<Int>::operator()(const Int x) const noexcept
{
    static_assert(sizeof(Int) <= sizeof(uint64_t), "integral type is too wide");
    if constexpr (sizeof(Int) <= 4)
        return ConstexprHash<uint32_t>()(static_cast<uint32_t>(x));
    else
        return ConstexprHash<uint64_t>()(static_cast<uint64_t>(x));
}

template<>
struct ConstexprHash<float>
{
    constexpr hash_t operator()(const float x) const noexcept
    {
        const float ax = x >= 0.f ? x : -x;
        const float frac = ax - static_cast<uint32_t>(ax);
        const uint32_t fractional = static_cast<uint32_t>(frac * 1'000'000.f); // shift left
        const uint32_t integer = static_cast<uint32_t>(x);
        const ConstexprHash<uint32_t> hasher;
        const hash_t intHash = hasher(integer);
        const hash_t fracHash = hasher(fractional);
        return hashCombine(intHash, fracHash);
    }
};

template<>
struct ConstexprHash<double>
{
    constexpr hash_t operator()(const double x) const noexcept
    {
        const double ax = x >= 0. ? x : -x;
        const double frac = ax - static_cast<uint64_t>(ax);
        const uint64_t fractional = static_cast<uint64_t>(frac * 1'000'000'000.); // shift left
        const uint64_t integer = static_cast<uint64_t>(x);
        const ConstexprHash<uint64_t> hasher;
        const hash_t intHash = hasher(integer);
        const hash_t fracHash = hasher(fractional);
        return hashCombine(intHash, fracHash);
    }
};

template<class T>
constexpr hash_t hash(T x) noexcept
{
    ConstexprHash<T> hasher;
    return hasher(x);
}

template<class T>
constexpr hash_t hash(T *p) noexcept
{
    ConstexprHash<uintptr_t> hasher;
    return hasher(reinterpret_cast<uintptr_t>(p));
}

[[nodiscard]] constexpr hash_t hashCombine(const hash_t seed, const hash_t hash) noexcept
{   // https://www.boost.org/doc/libs/1_46_1/doc/html/hash/reference.html#boost.hash_combine
    return seed ^ (hash + 0x9e3779b9 + (seed << 6) + (seed >> 2));
}

template<class T>
constexpr hash_t hashArg(hash_t seed, const T& arg) noexcept
{
    return hashCombine(seed, hash(arg));
}

template<class T, typename... Args>
constexpr hash_t hashArg(hash_t seed, const T& arg, Args... args) noexcept
{
    hash_t value = hashCombine(seed, hash(arg));
    return hashArg(value, args...);
}

template<class T, typename... Args>
constexpr hash_t hashArgs(const T& arg, Args... args) noexcept
{
    return hashArg(hash(arg), args...);
}

template<class T, std::size_t N>
constexpr hash_t hashArray(const T (&arr)[N]) noexcept
{
    hash_t hash = fnv::Basis;
    for (std::size_t i = 0; i < N; ++i)
        hash = (hash ^ arr[i]) * fnv::Prime;
    return hash;
}

template<class T>
inline hash_t hashArray(const T arr[], std::size_t count) noexcept
{
    std::hash<T> hasher;
    hash_t hash = hasher(arr[0]);
    for (std::size_t i = 1; i < count; ++i)
        hash = hashCombine(hash, hasher(arr[i]));
    return hash;
}

constexpr hash_t hashString(const char *str) noexcept
{
    auto hash = fnv::Basis;
    for (; *str; ++str)
        hash = (hash ^ (uint8_t)*str) * fnv::Prime;
    return hash;
}
} // namespace magma::core
