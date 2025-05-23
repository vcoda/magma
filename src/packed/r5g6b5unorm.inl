namespace magma::packed
{
inline R5g6b5Unorm::R5g6b5Unorm(float r, float g, float b) noexcept
{
#ifdef MAGMA_SSE
    __m128 v = _mm_set_ps(0.f, r, g, b);
    v = _mm_max_ps(v, _mm_setzero_ps());
    v = _mm_min_ps(v, _mm_set_ps(0.f, 1.f, 1.f, 1.f));
    v = _mm_mul_ps(v, _mm_set_ps(0.f, 31.f, 63.f, 31.f));
    v = _mm_round_ps(v, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
    __m128 bitshift = _mm_set_ps(0.f, 2048.f, 32.f, 1.f); // -, 11, 5, 0
    v = _mm_mul_ps(v, bitshift);
    __m128i iv = _mm_cvtps_epi32(v);
    iv = _mm_horizontal_or(iv);
    this->v = (uint16_t)_mm_extract_epi16(iv, 0);
#elif defined(MAGMA_NEON)
    float32x4_t v = {b, g, r, 0.f};
    v = vmaxq_f32(v, vdupq_n_f32(0.f));
    v = vminq_f32(v, vdupq_n_f32(1.f));
    float32x4_t scale = {31.f, 63.f, 31.f, 0.f};
    v = vmulq_f32(v, scale);
    v = vrndnq_f32(v);
    float32x4_t bitshift = {1.f, 32.f, 2048.f, 0.f}; // 0, 5, 11, -
    v = vmulq_f32(v, bitshift);
    uint32x4_t iv = vcvtq_u32_f32(v);
    uint32x2_t ored = vhorizontal_or(iv);
    this->v = (uint16_t)vget_lane_u32(ored, 0);
#else // FPU
    r = std::clamp(r, 0.f, 1.f);
    g = std::clamp(g, 0.f, 1.f);
    b = std::clamp(b, 0.f, 1.f);
    r = std::roundf(r * 31.f);
    g = std::roundf(g * 63.f);
    b = std::roundf(b * 31.f);
    this->b = uint16_t(b) & 0x1F;
    this->g = uint16_t(g) & 0x3F;
    this->r = uint16_t(r) & 0x1F;
#endif // FPU
}

inline R5g6b5Unorm::R5g6b5Unorm(uint8_t r, uint8_t g, uint8_t b) noexcept:
    R5g6b5Unorm(r/255.f, g/255.f, b/255.f)
{}

inline R5g6b5Unorm::R5g6b5Unorm(const float v[3]) noexcept:
    R5g6b5Unorm(v[0], v[1], v[2])
{}

inline R5g6b5Unorm::R5g6b5Unorm(const uint8_t v[3]) noexcept:
    R5g6b5Unorm(v[0]/255.f, v[1]/255.f, v[2]/255.f)
{}
} // namespace magma::packed
