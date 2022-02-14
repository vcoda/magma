namespace magma
{
namespace packed
{
inline R5g5b5a1Unorm::R5g5b5a1Unorm(float r, float g, float b, float a) noexcept
{
#ifdef MAGMA_SSE
    constexpr float hi[4] = {1.f, 1.f, 1.f, 1.f};
    constexpr float scale[4] = {31.f, 31.f, 31.f, 1.f};
    __m128 v = _mm_set_ps(r, g, b, a); // Most to least significant bit order
    v = _mm_max_ps(v, _mm_setzero_ps());
    v = _mm_min_ps(v, _mm_load_ps(hi));
    v = _mm_mul_ps(v, _mm_load_ps(scale));
    __m128i iv = _mm_cvtps_epi32(v); // Convert to int with rounding
    this->v =
        (((uint16_t)_mm_extract_epi16(iv, 6) & 0x1F) << 11) |
        (((uint16_t)_mm_extract_epi16(iv, 4) & 0x1F) << 6) |
        (((uint16_t)_mm_extract_epi16(iv, 2) & 0x1F) << 1) |
        ((uint16_t)_mm_extract_epi16(iv, 0) & 0x1);
#else
    r = std::min(std::max(0.f, r), 1.f);
    g = std::min(std::max(0.f, g), 1.f);
    b = std::min(std::max(0.f, b), 1.f);
    a = std::min(std::max(0.f, a), 1.f);
    r = std::roundf(r * 31.f);
    g = std::roundf(g * 31.f);
    b = std::roundf(b * 31.f);
    v = (((uint16_t)r & 0x1F) << 11) |
        (((uint16_t)g & 0x1F) << 6) |
        (((uint16_t)b & 0x1F) << 1) |
        ((uint16_t)a & 0x1);
#endif
}

inline R5g5b5a1Unorm::R5g5b5a1Unorm(uint8_t r, uint8_t g, uint8_t b, uint8_t a) noexcept:
    R5g5b5a1Unorm(r/255.f, g/255.f, b/255.f, a/255.f)
{}

inline R5g5b5a1Unorm::R5g5b5a1Unorm(const float v[4]) noexcept:
    R5g5b5a1Unorm(v[0], v[1], v[2], v[3])
{}

inline R5g5b5a1Unorm::R5g5b5a1Unorm(const uint8_t v[4]) noexcept:
    R5g5b5a1Unorm(v[0]/255.f, v[1]/255.f, v[2]/255.f, v[3]/255.f)
{}
} // namespace packed
} // namespace magma
