#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4702) // unreachable code
#endif

namespace magma
{
namespace core
{
inline int popCount(uint32_t value) noexcept
{
#if defined(_M_IX86) || defined(_M_X64)
    #if defined(MAGMA_SSE)
    return _mm_popcnt_u32(value);
    #elif defined(_MSC_VER)
    return __popcnt(value);
    #endif
#elif defined(_M_ARM)
    #if defined(MAGMA_NEON)
    return (int)vaddlv_u8(vcnt_u8(vcreate_u8((uint64_t)value)));
    #elif __has_builtin(__builtin_popcount)
    return __builtin_popcount(value);
    #endif
#endif // !_M_ARM
    constexpr uint32_t c1 = 0x55555555u;
    constexpr uint32_t c2 = 0x33333333u;
    constexpr uint32_t c4 = 0x0f0f0f0fu;
    uint32_t x = value;
    x -= (x >> 1) & c1;
    x = ((x >> 2) & c2) + (x & c2);
    x = (x + (x >> 4)) & c4;
    x *= 0x01010101u;
    return int(x >> 24);
}
} // namespace core
} // namespace magma

#ifdef _MSC_VER
#pragma warning(pop)
#endif
