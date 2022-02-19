namespace magma
{
namespace packed
{
inline int floorLog2(float x)
{
    Float32 f;
    f.f = x;
    return f.e - Float32::bias;
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
    constexpr float rcpExpPow2[MAX_VALID_BIASED_EXP + 1] = {
        16777216.f,
        8388608.f,
        4194304.f,
        2097152.f,
        1048576.f,
        524288.f,
        262144.f,
        131072.f,
        65536.f,
        32768.f,
        16384.f,
        8192.f,
        4096.f,
        2048.f,
        1024.f,
        512.f,
        256.f,
        128.f,
        64.f,
        32.f,
        16.f,
        8.f,
        4.f,
        2.f,
        1.f,
        0.5f,
        0.25f,
        0.125f,
        0.0625f,
        0.03125f,
        0.015625f,
        0.0078125f
    };
#ifdef MAGMA_SSE
    __m128 v = _mm_set_ps(0.f, b, g, r);
    v = _mm_max_ps(v, _mm_setzero_ps());
    v = _mm_min_ps(v, _mm_set_ps1(MAX_RGB9E5));
    // Horizontal max3
    __m128 v1 = _mm_movehl_ps(v, v);
    __m128 v2 = _mm_max_ps(v, v1);
    v1 = mm_permute_ps(v, _MM_SHUFFLE(3, 1, 1, 1));
    __m128 max3 = _mm_max_ps(v1, v2);
    // TODO: begin vectorize
    float maxRgb;
    _mm_store_ss(&maxRgb, max3);
    int fl = floorLog2(maxRgb);
    int exp = (-EXP_BIAS - 1 > fl ? -EXP_BIAS - 1 : fl) + 1 + EXP_BIAS;
    // TODO: end vectorize
    __m128 scale = _mm_set_ss(rcpExpPow2[exp]);
    v2 = _mm_mul_ss(max3, scale); // max(r, g, b) / (2^(exp - EXP_BIAS - MANTISSA_BITS))
    int maxm = _mm_extract_epi32(_mm_cvtps_epi32(v2), 0); // Convert to int with rounding
    if (MAX_MANTISSA + 1 == maxm) ++exp;
    scale = _mm_set_ps1(rcpExpPow2[exp]);
    v = _mm_mul_ps(v, scale);
    __m128i iv = _mm_cvtps_epi32(v); // Convert to int with rounding
    this->v =
        (((uint32_t)exp & 0x1F) << 27) |
        (((uint32_t)_mm_extract_epi32(iv, 2) & 0x1FF) << 18) |
        (((uint32_t)_mm_extract_epi32(iv, 1) & 0x1FF) << 9) |
        ((uint32_t)_mm_extract_epi32(iv, 0) & 0x1FF);
#else
    r = std::min(std::max(0.f, r), MAX_RGB9E5);
    g = std::min(std::max(0.f, g), MAX_RGB9E5);
    b = std::min(std::max(0.f, b), MAX_RGB9E5);
    float maxRgb = std::max(std::max(r, g), b);
    int exp = std::max(-EXP_BIAS - 1, floorLog2(maxRgb)) + 1 + EXP_BIAS;
    MAGMA_ASSERT(exp >= 0);
    MAGMA_ASSERT(exp <= MAX_VALID_BIASED_EXP);
    float scale = rcpExpPow2[exp];
    int maxm = (int)std::roundf(maxRgb * scale); // max(r, g, b) / (2^(exp - EXP_BIAS - MANTISSA_BITS))
    MAGMA_ASSERT(maxm <= MAX_MANTISSA);
    if (MAX_MANTISSA + 1 == maxm)
    {
        ++exp;
        MAGMA_ASSERT(exp <= MAX_VALID_BIASED_EXP);
    }
    scale = rcpExpPow2[exp];
    rm = (uint32_t)std::roundf(r * scale);
    gm = (uint32_t)std::roundf(g * scale);
    bm = (uint32_t)std::roundf(b * scale);
    e = (uint32_t)exp;
#endif // MAGMA_SSE
}

inline void R9g9b9e5Ufloat::unpack(float v[3]) const noexcept
{
    constexpr int MAX_VALID_BIASED_EXP = 31;
    constexpr float expPow2[MAX_VALID_BIASED_EXP + 1] = {
        5.96046448e-08f,
        1.1920929e-07f,
        2.38418579e-07f,
        4.76837158e-07f,
        9.53674316e-07f,
        1.90734863e-06f,
        3.81469727e-06f,
        7.62939453e-06f,
        1.52587891e-05f,
        3.05175781e-05f,
        6.10351563e-05f,
        0.000122070313f,
        0.000244140625f,
        0.00048828125f,
        0.0009765625f,
        0.001953125f,
        0.00390625f,
        0.0078125f,
        0.015625f,
        0.03125f,
        0.0625f,
        0.125f,
        0.25f,
        0.5f,
        1.f,
        2.f,
        4.f,
        8.f,
        16.f,
        32.f,
        64.f,
        128.f
    };
    MAGMA_ASSERT(e <= MAX_VALID_BIASED_EXP);
    float scale = expPow2[e];
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
std::cout.precision(std::numeric_limits<float>::max_digits10);
for (int exp = 0; exp <= MAX_VALID_BIASED_EXP; ++exp)
{
    double val = 1.0 / pow(2.0, (double)exp - EXP_BIAS - MANTISSA_BITS);
    std::cout << (float)val << (val < 1.0 ? "f" : ".f") << "," << std::endl;
}
*/
