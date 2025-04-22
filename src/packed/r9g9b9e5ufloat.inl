namespace magma::packed
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
    // max(r, g, b)
    __m128 v1 = _mm_movehl_ps(v, v);
    __m128 v2 = _mm_max_ps(v, v1);
    v1 = mm_permute_ps(v, _MM_SHUFFLE(3, 1, 1, 1));
    __m128 max = _mm_max_ps(v1, v2);
    max = _mm_shuffle_ps(max, max, _MM_SHUFFLE(0, 0, 0, 0)); // splat to all
    // floorLog2(max(r, g, b))
    __m128i bits = _mm_castps_si128(max);
    __m128i bias = _mm_set1_epi32(Float32::bias);
    __m128i fl2 = _mm_sub_epi32(_mm_srli_epi32(bits, 23), bias);
    // e = max(-EXP_BIAS - 1, floorLog2(max(r, g, b))) + 1 + EXP_BIAS;
    __m128i e = _mm_max_epi32(fl2, _mm_set1_epi32(-EXP_BIAS - 1));
    e = _mm_add_epi32(e, _mm_set1_epi32(+EXP_BIAS + 1));
    // scale = exp2f(EXP_BIAS + MANTISSA_BITS - e)
    __m128i shift = _mm_sub_epi32(_mm_set1_epi32(EXP_BIAS + MANTISSA_BITS), e);
    __m128i scale = _mm_slli_epi32(_mm_add_epi32(shift, bias), 23);
    // round([r, g, b] * scale)
    __m128i mantissas = _mm_cvtps_epi32(_mm_mul_ps(v, _mm_castsi128_ps(scale)));
    // split mantissas
    __m128i rm = _mm_shuffle_epi32(mantissas, _MM_SHUFFLE(3, 3, 3, 0));
    __m128i gm = _mm_shuffle_epi32(mantissas, _MM_SHUFFLE(3, 3, 3, 1));
    __m128i bm = _mm_shuffle_epi32(mantissas, _MM_SHUFFLE(3, 3, 3, 2));
    // mask & l-shift
    __m128i mask9 = _mm_set1_epi32(0x1FF);
    __m128i mask5 = _mm_set1_epi32(0x1F);
    rm = _mm_and_si128(rm, mask9);
    gm = _mm_slli_epi32(_mm_and_si128(gm, mask9), 9);
    bm = _mm_slli_epi32(_mm_and_si128(bm, mask9), 18);
    e = _mm_slli_epi32(_mm_and_si128(e, mask5), 27);
    // rm | gm | bm | e
    __m128i iv = _mm_or_si128(_mm_or_si128(rm, gm), _mm_or_si128(bm, e));
    this->v = _mm_cvtsi128_si32(iv);
#elif defined(MAGMA_NEON)
    float32x4_t v = {r, g, b, 0.f};
    v = vmaxq_f32(v, vdupq_n_f32(0.f));
    v = vminq_f32(v, vdupq_n_f32(MAX_RGB9E5));
    // Horizontal max3
    float32x2_t v1 = vget_low_f32(v);
    float32x2_t v2 = vget_high_f32(v);
    float32x2_t max3 = vmax_f32(v1, v2);
    max3 = vpmax_f32(max3, max3);
    // TODO: begin vectorize
    float maxRgb = vget_lane_f32(max3, 0);
    int fl = floorLog2(maxRgb);
    int exp = (-EXP_BIAS - 1 > fl ? -EXP_BIAS - 1 : fl) + 1 + EXP_BIAS;
    // TODO: end vectorize
    float32x2_t scale = vdup_n_f32(rcpExpPow2[exp]);
    v1 = vmul_f32(max3, scale); // max(r, g, b) / (2^(exp - EXP_BIAS - MANTISSA_BITS))
    int maxm = vget_lane_s32(vcvt_s32_f32(v1), 0);
    if (MAX_MANTISSA + 1 == maxm) ++exp;
    float32x4_t scaleq = vdupq_n_f32(rcpExpPow2[exp]);
    v = vmulq_f32(v, scaleq);
    v = vrndnq_f32(v);
    uint32x4_t iv = vcvtnq_u32_f32(v);
    this->v =
        (((uint32_t)exp & 0x1F) << 27) |
        (((uint32_t)vgetq_lane_u32(iv, 2) & 0x1FF) << 18) |
        (((uint32_t)vgetq_lane_u32(iv, 1) & 0x1FF) << 9) |
        ((uint32_t)vgetq_lane_u32(iv, 0) & 0x1FF);
#else // FPU
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
#endif // FPU
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
} // namespace magma::packed
