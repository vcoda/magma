namespace magma
{
namespace packed
{
inline X10y10z10w2Snorm::X10y10z10w2Snorm(float x, float y, float z, int w /* 0 */) noexcept
{
#ifdef MAGMA_SSE
    __m128 v = _mm_set_ps(0.f, z, y, x);
    v = _mm_max_ps(v, _mm_set_ps(0.f, -1.f, -1.f, -1.f));
    v = _mm_min_ps(v, _mm_set_ps1(1.f));
    v = _mm_mul_ps(v, _mm_set_ps(3.f * 536870912.f, 511.f * 1048576.f, 511.f * 1024.f, 511.f)); // 2^10, 2^20, 2^30
    __m128i iv = _mm_cvtps_epi32(v);
    iv = _mm_and_si128(iv, _mm_set_epi32(0x0, 0x3FF << 20, 0x3FF << 10, 0x3FF)); // Mask off any fraction
    v = _mm_castsi128_ps(iv);
    // Horizontal bitwise OR
    v = mm_permute_ps(v, _MM_SHUFFLE(0, 3, 2, 1));
    iv = _mm_or_si128(iv, _mm_castps_si128(v));
    v = mm_permute_ps(v, _MM_SHUFFLE(0, 3, 2, 1));
    iv = _mm_or_si128(iv, _mm_castps_si128(v));
    this->v = ((w & 0x3) << 30) | _mm_cvtsi128_si32(iv);
#else
    x = std::min(std::max(-1.f, x), 1.f);
    y = std::min(std::max(-1.f, y), 1.f);
    z = std::min(std::max(-1.f, z), 1.f);
    x = std::roundf(x * 511.f);
    y = std::roundf(y * 511.f);
    z = std::roundf(z * 511.f);
    v = ((w & 0x3) << 30) |
        (((int32_t)z & 0x3FF) << 20) |
        (((int32_t)y & 0x3FF) << 10) |
        ((int32_t)x & 0x3FF);
#endif // MAGMA_SSE
}
} // namespace packed
} // namespace magma
