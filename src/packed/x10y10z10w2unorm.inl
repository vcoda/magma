namespace magma::packed
{
inline X10y10z10w2Unorm::X10y10z10w2Unorm(float x, float y, float z, uint32_t w /* 0 */) noexcept
{
#ifdef MAGMA_SSE
    __m128 v = _mm_set_ps(float(w), z, y, x);
    v = _mm_max_ps(v, _mm_setzero_ps());
    v = _mm_min_ps(v, _mm_set_ps(3.f, 1.f, 1.f, 1.f));
    __m128 scale = _mm_set_ps(1.f, 1023.f, 1023.f, 1023.f);
    v = _mm_mul_ps(v, scale);
    v = _mm_round_ps(v, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
    __m128 bitshift = _mm_set_ps(1024.f * 1024.f * 1024.f, 1024.f * 1024.f, 1024.f, 1.f);
    v = _mm_mul_ps(v, bitshift);
    __m128i iv = _mm_cvtps_epi32(v);
    iv = _mm_horizontal_or(iv);
    this->v = _mm_cvtsi128_si32(iv);
#elif defined(MAGMA_NEON)
    float32x4_t v = {x, y, z, float(w)};
    v = vmaxq_f32(v, vdupq_n_f32(0.f));
    float32x4_t max = {1.f, 1.f, 1.f, 3.f};
    v = vminq_f32(v, max);
    float32x4_t scale = {1023.f, 1023.f, 1023.f, 1.f};
    v = vmulq_f32(v, scale);
    v = vrndnq_f32(v);
    uint32x4_t iv = vcvtq_u32_f32(v);
    // mask off any fraction
    uint32x4_t mask = {0x3FF, 0x3FF, 0x3FF, 0x3};
    iv = vandq_u32(iv, mask);
    int32x4_t bitshift = {0, 10, 20, 30};
    iv = vshlq_u32(iv, bitshift);
    uint32x2_t ored = vhorizontal_or(iv);
    this->v = vget_lane_u32(ored, 0);
#else // FPU
    x = std::clamp(x, 0.f, 1.f);
    y = std::clamp(y, 0.f, 1.f);
    z = std::clamp(z, 0.f, 1.f);
    x = std::roundf(x * 1023.f);
    y = std::roundf(y * 1023.f);
    z = std::roundf(z * 1023.f);
    this->x = uint32_t(x) & 0x3FF;
    this->y = uint32_t(y) & 0x3FF;
    this->z = uint32_t(z) & 0x3FF;
    this->w = w & 0x3;
#endif // FPU
}
} // namespace magma::packed
