namespace magma
{
namespace core
{
inline int popCount(uint32_t value) noexcept
{
#if (defined(_M_IX86) || defined(_M_X64)) && defined(_MSC_VER)
    return __popcnt(value);
#elif __has_builtin(__builtin_popcount)
    return __builtin_popcount(value);
#else
    constexpr uint32_t c1 = 0x55555555u;
    constexpr uint32_t c2 = 0x33333333u;
    constexpr uint32_t c4 = 0x0f0f0f0fu;
    uint32_t x = value;
    x -= (x >> 1) & c1;
    x = ((x >> 2) & c2) + (x & c2);
    x = (x + (x >> 4)) & c4;
    x *= 0x01010101u;
    return int(x >> 24);
#endif
}
} // namespace core
} // namespace magma
