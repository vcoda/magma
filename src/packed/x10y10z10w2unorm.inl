namespace magma::packed
{
inline X10y10z10w2Unorm::X10y10z10w2Unorm(float x, float y, float z, uint32_t w /* 0 */) noexcept
{
#ifdef MAGMA_SSE
    __m128 v = _mm_set_ps(0.f, z, y, x);
    v = _mm_max_ps(v, _mm_setzero_ps());
    v = _mm_min_ps(v, _mm_set_ps1(1.f));
    v = _mm_mul_ps(v, _mm_set_ps(3.f * 536870912.f, 1023.f * 1048576.f, 1023.f * 1024.f, 1023.f)); // 2^10, 2^20, 2^30
    __m128i iv = _mm_cvtps_epi32(v);
    iv = _mm_and_si128(iv, _mm_set_epi32(0x0, 0x3FF << 20, 0x3FF << 10, 0x3FF)); // Mask off any fraction
    v = _mm_castsi128_ps(iv);
    // Horizontal bitwise OR
    v = mm_permute_ps(v, _MM_SHUFFLE(0, 3, 2, 1));
    iv = _mm_or_si128(iv, _mm_castps_si128(v));
    v = mm_permute_ps(v, _MM_SHUFFLE(0, 3, 2, 1));
    iv = _mm_or_si128(iv, _mm_castps_si128(v));
    this->v = ((w & 0x3) << 30) | _mm_cvtsi128_si32(iv);
#elif defined(MAGMA_NEON)
    float32x4_t v = {x, y, z, 0.f};
    v = vmaxq_f32(v, vdupq_n_f32(0.f));
    v = vminq_f32(v, vdupq_n_f32(1.f));
    v = vmulq_f32(v, vdupq_n_f32(1023.f));
    v = vrndnq_f32(v);
    uint32x4_t uv = vcvtnq_u32_f32(v);
    uint32x4_t mask = vdupq_n_u32(0x3FF);
    uv = vandq_u32(uv, mask);
    uint32x4_t shifts = {0, 10, 20, 30};
    uv = vshlq_u32(uv, shifts);
    uint32x4_t v1 = vorrq_u32(uv, vextq_u32(uv, uv, 1));
    uint32x4_t v2 = vorrq_u32(v1, vextq_u32(v1, v1, 2));
    this->v = vgetq_lane_u32(v2, 0) | (w & 0x3) << 30;
#else
    x = std::min(std::max(0.f, x), 1.f);
    y = std::min(std::max(0.f, y), 1.f);
    z = std::min(std::max(0.f, z), 1.f);
    x = std::roundf(x * 1023.f);
    y = std::roundf(y * 1023.f);
    z = std::roundf(z * 1023.f);
    v = ((w & 0x3) << 30) |
        (((uint32_t)z & 0x3FF) << 20) |
        (((uint32_t)y & 0x3FF) << 10) |
        ((uint32_t)x & 0x3FF);
#endif // MAGMA_NEON
}
} // namespace magma::packed
