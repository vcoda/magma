namespace magma
{
namespace packed
{
inline int floorLog2(float x)
{
    Float32 f;
    f.f = x;
    return f.e - 127;
}

inline R9g9b9e5Ufloat::R9g9b9e5Ufloat(float r, float g, float b) noexcept
{
    constexpr int MANTISSA_BITS = 9;
    constexpr int EXP_BIAS = 15;
    constexpr int MAX_VALID_BIASED_EXP = 31;
    constexpr int MAX_EXP = MAX_VALID_BIASED_EXP - EXP_BIAS;
    constexpr int MAX_MANTISSA = (1 << MANTISSA_BITS) - 1;
    constexpr float MAX_RGB9E5 = float(MAX_MANTISSA) / (1 << MANTISSA_BITS) * (1 << MAX_EXP);
    r = std::min(std::max(0.f, r), MAX_RGB9E5);
    g = std::min(std::max(0.f, g), MAX_RGB9E5);
    b = std::min(std::max(0.f, b), MAX_RGB9E5);
    float maxRgb = std::max(std::max(r, g), b);
    int exp = std::max(-EXP_BIAS - 1, floorLog2(maxRgb)) + 1 + EXP_BIAS;
    MAGMA_ASSERT(exp >= 0);
    MAGMA_ASSERT(exp <= MAX_VALID_BIASED_EXP);
    float denom = powf(2.f, (float)exp - EXP_BIAS - MANTISSA_BITS);
    int maxm = (int)std::roundf(maxRgb / denom);
    MAGMA_ASSERT(maxm <= MAX_MANTISSA);
    if (MAX_MANTISSA + 1 == maxm)
    {
        denom *= 2.f;
        exp += 1;
        MAGMA_ASSERT(exp <= MAX_VALID_BIASED_EXP);
    }
    float scale = 1.f / denom;
    rm = (uint32_t)std::roundf(r * scale);
    gm = (uint32_t)std::roundf(g * scale);
    bm = (uint32_t)std::roundf(b * scale);
    e = (uint32_t)exp;
}
} // namespace packed
} // namespace magma
