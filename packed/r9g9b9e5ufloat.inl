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
    #include "rcpExpPow2.h"
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
#elif defined(MAGMA_NEON)
    // TODO:
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
    #include "expPow2.h"
    MAGMA_ASSERT(e <= MAX_VALID_BIASED_EXP);
    float scale = expPow2[e];
    v[0] = rm * scale;
    v[1] = gm * scale;
    v[2] = bm * scale;
}
} // namespace packed
} // namespace magma
