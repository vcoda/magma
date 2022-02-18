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
    // 2^(exp - EXP_BIAS - MANTISSA_BITS)
    constexpr double expPow2[MAX_VALID_BIASED_EXP + 1] = {
        5.96046e-08,
        1.19209e-07,
        2.38419e-07,
        4.76837e-07,
        9.53674e-07,
        1.90735e-06,
        3.8147e-06,
        7.62939e-06,
        1.52588e-05,
        3.05176e-05,
        6.10352e-05,
        0.00012207,
        0.000244141,
        0.000488281,
        0.000976563,
        0.00195313,
        0.00390625,
        0.0078125,
        0.015625,
        0.03125,
        0.0625,
        0.125,
        0.25,
        0.5,
        1.0,
        2.0,
        4.0,
        8.0,
        16.0,
        32.0,
        64.0,
        128.0
    };
    r = std::min(std::max(0.f, r), MAX_RGB9E5);
    g = std::min(std::max(0.f, g), MAX_RGB9E5);
    b = std::min(std::max(0.f, b), MAX_RGB9E5);
    float maxRgb = std::max(std::max(r, g), b);
    int exp = std::max(-EXP_BIAS - 1, floorLog2(maxRgb)) + 1 + EXP_BIAS;
    MAGMA_ASSERT(exp >= 0);
    MAGMA_ASSERT(exp <= MAX_VALID_BIASED_EXP);
    int maxm = (int)std::round(maxRgb / expPow2[exp]); // max(r, g, b) / (2^(exp - EXP_BIAS - MANTISSA_BITS))
    MAGMA_ASSERT(maxm <= MAX_MANTISSA);
    if (MAX_MANTISSA + 1 == maxm)
    {
        ++exp;
        MAGMA_ASSERT(exp <= MAX_VALID_BIASED_EXP);
    }
    double scale = 1. / expPow2[exp];
    rm = (uint32_t)std::round(r * scale);
    gm = (uint32_t)std::round(g * scale);
    bm = (uint32_t)std::round(b * scale);
    e = (uint32_t)exp;
}

inline void R9g9b9e5Ufloat::unpack(float v[3]) noexcept
{
    constexpr int MANTISSA_BITS = 9;
    constexpr int EXP_BIAS = 15;
    int exp = e - EXP_BIAS - MANTISSA_BITS;
    float scale = powf(2.f, (float)exp);
    v[0] = rm * scale;
    v[1] = gm * scale;
    v[2] = bm * scale;
}
} // namespace packed
} // namespace magma

/* Lookup table generation code:

constexpr int MANTISSA_BITS = 9;
constexpr int EXP_BIAS = 15;
constexpr int MAX_VALID_BIASED_EXP = 31;
for (int exp = 0; exp <= MAX_VALID_BIASED_EXP; ++exp)
{
    double val = pow(2.0, (double)exp - EXP_BIAS - MANTISSA_BITS);
    std::cout << val << "," << std::endl;
}
*/
