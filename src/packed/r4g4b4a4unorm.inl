namespace magma::packed
{
inline R4g4b4a4Unorm::R4g4b4a4Unorm(float r, float g, float b, float a) noexcept
{
#ifdef MAGMA_SSE
    __m128 v = _mm_set_ps(r, g, b, a); // Most to least significant bit order
    v = _mm_max_ps(v, _mm_setzero_ps());
    v = _mm_min_ps(v, _mm_set_ps1(1.f));
    v = _mm_mul_ps(v, _mm_set_ps1(15.f));
    __m128i iv = _mm_cvtps_epi32(v); // Convert to int with rounding
    this->v =
        (((uint16_t)_mm_extract_epi16(iv, 6) & 0xF) << 12) |
        (((uint16_t)_mm_extract_epi16(iv, 4) & 0xF) << 8) |
        (((uint16_t)_mm_extract_epi16(iv, 2) & 0xF) << 4) |
        ((uint16_t)_mm_extract_epi16(iv, 0) & 0xF);
#elif defined(MAGMA_NEON)
    #error NEON codepath not implemented
#else
    r = std::min(std::max(0.f, r), 1.f);
    g = std::min(std::max(0.f, g), 1.f);
    b = std::min(std::max(0.f, b), 1.f);
    a = std::min(std::max(0.f, a), 1.f);
    r = std::roundf(r * 15.f);
    g = std::roundf(g * 15.f);
    b = std::roundf(b * 15.f);
    a = std::roundf(a * 15.f);
    v = (((uint16_t)r & 0xF) << 12) |
        (((uint16_t)g & 0xF) << 8) |
        (((uint16_t)b & 0xF) << 4) |
        ((uint16_t)a & 0xF);
#endif // MAGMA_NEON
}

inline R4g4b4a4Unorm::R4g4b4a4Unorm(uint8_t r, uint8_t g, uint8_t b, uint8_t a) noexcept:
    R4g4b4a4Unorm(r/255.f, g/255.f, b/255.f, a/255.f)
{}

inline R4g4b4a4Unorm::R4g4b4a4Unorm(const float v[4]) noexcept:
    R4g4b4a4Unorm(v[0], v[1], v[2], v[3])
{}

inline R4g4b4a4Unorm::R4g4b4a4Unorm(const uint8_t v[4]) noexcept:
    R4g4b4a4Unorm(v[0]/255.f, v[1]/255.f, v[2]/255.f, v[3]/255.f)
{}
} // namespace magma::packed
