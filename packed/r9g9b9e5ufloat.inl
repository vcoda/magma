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
    // 1/(2^(exp - EXP_BIAS - MANTISSA_BITS))
    constexpr double invExpPow2[MAX_VALID_BIASED_EXP + 1] = {
        1.67772e+07,
        8.38861e+06,
        4.1943e+06,
        2.09715e+06,
        1.04858e+06,
        524288.0,
        262144.0,
        131072.0,
        65536.0,
        32768.0,
        16384.0,
        8192.0,
        4096.0,
        2048.0,
        1024.0,
        512.0,
        256.0,
        128.0,
        64.0,
        32.0,
        16.0,
        8.0,
        4.0,
        2.0,
        1.0,
        0.5,
        0.25,
        0.125,
        0.0625,
        0.03125,
        0.015625,
        0.0078125
    };
    r = std::min(std::max(0.f, r), MAX_RGB9E5);
    g = std::min(std::max(0.f, g), MAX_RGB9E5);
    b = std::min(std::max(0.f, b), MAX_RGB9E5);
    float maxRgb = std::max(std::max(r, g), b);
    int exp = std::max(-EXP_BIAS - 1, floorLog2(maxRgb)) + 1 + EXP_BIAS;
    MAGMA_ASSERT(exp >= 0);
    MAGMA_ASSERT(exp <= MAX_VALID_BIASED_EXP);
    int maxm = (int)std::round(maxRgb * invExpPow2[exp]);
    MAGMA_ASSERT(maxm <= MAX_MANTISSA);
    if (MAX_MANTISSA + 1 == maxm)
    {
        exp += 1;
        MAGMA_ASSERT(exp <= MAX_VALID_BIASED_EXP);
    }
    double scale = invExpPow2[exp];
    rm = (uint32_t)std::round(r * scale);
    gm = (uint32_t)std::round(g * scale);
    bm = (uint32_t)std::round(b * scale);
    e = (uint32_t)exp;
}
} // namespace packed
} // namespace magma
