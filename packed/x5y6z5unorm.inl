namespace magma
{
namespace packed
{
inline X5y6z5unorm::X5y6z5unorm(float x, float y, float z) noexcept
{
#ifdef MAGMA_SSE
    constexpr float hi[4] = {1.f, 1.f, 1.f, 0.f};
    constexpr float scale[4] = {31.f, 63.f, 31.f, 0.f};
    __m128 v = _mm_set_ps(0.f, z, y, x);
    v = _mm_max_ps(v, _mm_setzero_ps());
    v = _mm_min_ps(v, _mm_load_ps(hi));
    v = _mm_mul_ps(v, _mm_load_ps(scale));
    __m128i iv = _mm_cvtps_epi32(v); // Convert to int with rounding
    this->v = (((uint16_t)_mm_extract_epi16(iv, 4) & 0x1F) << 11) |
        (((uint16_t)_mm_extract_epi16(iv, 2) & 0x3FF) << 5) |
        ((uint16_t)_mm_extract_epi16(iv, 0) & 0x1F);
#else
    x = std::min(std::max(0.f, x), 1.f);
    y = std::min(std::max(0.f, y), 1.f);
    z = std::min(std::max(0.f, z), 1.f);
    x = std::roundf(x * 31.f);
    y = std::roundf(y * 63.f);
    z = std::roundf(z * 31.f);
    v = (((uint16_t)z & 0x1F) << 11) |
        (((uint16_t)y & 0x3FF) << 5) |
        ((uint16_t)x & 0x1F);
#endif // MAGMA_SSE
}
} // namespace packed
} // namespace magma
