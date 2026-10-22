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

constexpr uint32_t hashString(const char *str) noexcept
{
    uint32_t crc = 0xFFFFFFFF;
    while (*str)
    {
        crc ^= (uint8_t)*str++;
        for (int i = 0; i < 8; ++i)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0x82f63b78;
            else
                crc = (crc >> 1);
        }
    }
    return crc ^ 0xFFFFFFFF;
}

#ifdef MAGMA_NEON
inline int neon_pos(uint8x16_t reg, uint8x16_t val)
{
    uint8x16_t eq = vceqq_u8(reg, val);
#ifdef MAGMA_X64
    uint8x8_t narrowed = vshrn_n_u16(vreinterpretq_u16_u8(eq), 4);
    uint64_t mask = vget_lane_u64(vreinterpret_u64_u8(narrowed), 0);
    if (0 == mask) return 16;
    #ifdef _MSC_VER
    return (int)_CountTrailingZeros64(mask) >> 2;
    #else
    return __builtin_ctzll(mask) >> 2;
    #endif
#else
    const uint8x16_t indices = {
        0, 1, 2, 3, 4, 5, 6, 7,
        8, 9, 10, 11, 12, 13, 14, 15
    };
    uint8x16_t positions = vbslq_u8(eq, indices, vdupq_n_u8(16));
    uint8x8_t min = vpmin_u8(
        vget_low_u8(positions),
        vget_high_u8(positions));
    min = vpmin_u8(min, min);
    min = vpmin_u8(min, min);
    min = vpmin_u8(min, min);
    return vget_lane_u8(min, 0);
#endif // MAGMA_X64
}
#endif // MAGMA_NEON

inline uint32_t hashStringSIMD(const char *str) noexcept
{
    uint32_t crc = 0xFFFFFFFF;
#ifdef MAGMA_SSE
    __m128i zero = _mm_setzero_si128();
    while (1)
    {
        __m128i reg = _mm_loadu_si128((const __m128i *)str);
        int n = _mm_cmpistri(zero, reg, _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_EACH);
        if (16 == n)
        {
        #ifdef MAGMA_X64
            crc = (uint32_t)_mm_crc32_u64(crc, _mm_cvtsi128_si64(reg));
            crc = (uint32_t)_mm_crc32_u64(crc, _mm_extract_epi64(reg, 1));
        #else
            crc = _mm_crc32_u32(crc, (uint32_t)_mm_cvtsi128_si32(reg));
            crc = _mm_crc32_u32(crc, (uint32_t)_mm_cvtsi128_si32(_mm_srli_si128(reg, 4)));
            crc = _mm_crc32_u32(crc, (uint32_t)_mm_cvtsi128_si32(_mm_srli_si128(reg, 8)));
            crc = _mm_crc32_u32(crc, (uint32_t)_mm_cvtsi128_si32(_mm_srli_si128(reg, 12)));
        #endif // MAGMA_X64
            str += 16;
        }
        else
        {
            if (n & 8)
            {
            #ifdef MAGMA_X64
                crc = (uint32_t)_mm_crc32_u64(crc, _mm_cvtsi128_si64(reg));
            #else
                crc = _mm_crc32_u32(crc, (uint32_t)_mm_cvtsi128_si32(reg));
                crc = _mm_crc32_u32(crc, (uint32_t)_mm_cvtsi128_si32(_mm_srli_si128(reg, 4)));
            #endif // MAGMA_X64
                str += 8;
            }
            if (n & 4)
            {
                crc = _mm_crc32_u32(crc, *(const uint32_t *)str);
                str += 4;
            }
            if (n & 2)
            {
                crc = _mm_crc32_u16(crc, *(const uint16_t *)str);
                str += 2;
            }
            if (n & 1) crc = _mm_crc32_u8(crc, *(const uint8_t *)str);
            break;
        }
    }
    return crc ^ 0xFFFFFFFF;
#elif defined(MAGMA_NEON) && defined(__ARM_FEATURE_CRC32)
    uint8x16_t zero = vdupq_n_u8(0);
    while (1)
    {
        uint8x16_t reg = vld1q_u8((const uint8_t *)str);
        int n = neon_pos(reg, zero);
        if (16 == n)
        {
        #ifdef MAGMA_X64
            uint64x2_t v = vreinterpretq_u64_u8(reg);
            crc = __crc32cd(crc, vgetq_lane_u64(v, 0));
            crc = __crc32cd(crc, vgetq_lane_u64(v, 1));
        #else
            uint32x4_t v = vreinterpretq_u32_u8(reg);
            crc = __crc32cw(crc, vgetq_lane_u32(v, 0));
            crc = __crc32cw(crc, vgetq_lane_u32(v, 1));
            crc = __crc32cw(crc, vgetq_lane_u32(v, 2));
            crc = __crc32cw(crc, vgetq_lane_u32(v, 3));
        #endif // MAGMA_X64
            str += 16;
        }
        else
        {
            if (n & 8)
            {
            #ifdef MAGMA_X64
                uint64x2_t v = vreinterpretq_u64_u8(reg);
                crc = __crc32cd(crc, vgetq_lane_u64(v, 0));
            #else
                uint32x4_t v = vreinterpretq_u32_u8(reg);
                crc = __crc32cw(crc, vgetq_lane_u32(v, 0));
                crc = __crc32cw(crc, vgetq_lane_u32(v, 1));
            #endif // MAGMA_X64
                str += 8;
            }
            if (n & 4)
            {
                crc = __crc32cw(crc, *(const uint32_t *)str);
                str += 4;
            }
            if (n & 2)
            {
                crc = __crc32ch(crc, *(const uint16_t *)str);
                str += 2;
            }
            if (n & 1) crc = __crc32cb(crc, *(const uint8_t *)str);
            break;
        }
    }
    return crc ^ 0xFFFFFFFF;
#else
    return hashString(str);
#endif // MAGMA_SSE
}
} // namespace magma::core
