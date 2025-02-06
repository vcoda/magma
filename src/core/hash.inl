namespace magma::core
{
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

template<class T>
constexpr hash_t hashArg(hash_t seed, const T& arg) noexcept
{
    return hashCombine(seed, hash(arg));
}

template<class T, typename... Args>
constexpr hash_t hashArg(hash_t seed, const T& arg, Args... args) noexcept
{
    return hashArg(hashCombine(seed, hash(arg)), args...);
}

template<class T, typename... Args>
constexpr hash_t hashArgs(const T& arg, Args... args) noexcept
{
    return hashArg(hash(arg), args...);
}

template<class T, std::size_t N>
constexpr hash_t hashArray(const T (&arr)[N]) noexcept
{
    return hashing::Fnv1a<T, N, N>().hash(arr);
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

template<class T>
constexpr hash_t hashString(const T *str) noexcept
{
    return hashing::string::Fnv1a<T, 0ull>().hash(str);
}

template<class T>
inline hash_t hashString(const std::basic_string<T>& str) noexcept
{
    std::hash<std::basic_string<T>> hasher;
    return hasher(str);
}

inline hash_t combineHashList(const std::initializer_list<hash_t>& hashes) noexcept
{
    hash_t value = 0ull;
    for (auto hash: hashes)
        value = hashCombine(value, hash);
    return value;
}
} // namespace magma::core
