namespace magma::packed
{
inline B4g4r4a4Unorm::B4g4r4a4Unorm(float b, float g, float r, float a) noexcept
{
#ifdef MAGMA_SSE
    __m128 v = _mm_set_ps(b, g, r, a);
    v = _mm_max_ps(v, _mm_setzero_ps());
    v = _mm_min_ps(v, _mm_set_ps1(1.f));
    v = _mm_mul_ps(v, _mm_set_ps1(15.f));
    v = _mm_round_ps(v, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
    __m128 bitshift = _mm_set_ps(4096.f, 256.f, 16.f, 1.f); // 12, 8, 4, 0
    v = _mm_mul_ps(v, bitshift);
    __m128i iv = _mm_cvtps_epi32(v);
    iv = _mm_horizontal_or(iv);
    this->v = (uint16_t)_mm_extract_epi16(iv, 0);
#elif defined(MAGMA_NEON)
    float32x4_t v = {a, r, g, b};
    v = vmaxq_f32(v, vdupq_n_f32(0.f));
    v = vminq_f32(v, vdupq_n_f32(1.f));
    v = vmulq_f32(v, vdupq_n_f32(15.f));
    v = vrndnq_f32(v);
    float32x4_t bitshift = {1.f, 16.f, 256.f, 4096.f}; // 0, 4, 8, 12
    v = vmulq_f32(v, bitshift);
    int32x4_t iv = vcvtq_s32_f32(v);
    int32x2_t ored = vhorizontal_or(iv);
    this->v = (uint16_t)vget_lane_s32(ored, 0);
#else // FPU
    b = std::clamp(b, 0.f, 1.f);
    g = std::clamp(g, 0.f, 1.f);
    r = std::clamp(r, 0.f, 1.f);
    a = std::clamp(a, 0.f, 1.f);
    b = std::roundf(b * 15.f);
    g = std::roundf(g * 15.f);
    r = std::roundf(r * 15.f);
    a = std::roundf(a * 15.f);
    this->a = uint16_t(a) & 0xF;
    this->r = uint16_t(r) & 0xF;
    this->g = uint16_t(g) & 0xF;
    this->b = uint16_t(b) & 0xF;
#endif // FPU
}

inline B4g4r4a4Unorm::B4g4r4a4Unorm(uint8_t b, uint8_t g, uint8_t r, uint8_t a) noexcept:
    B4g4r4a4Unorm(b/255.f, g/255.f, r/255.f, a/255.f)
{}

inline B4g4r4a4Unorm::B4g4r4a4Unorm(const float v[4]) noexcept:
    B4g4r4a4Unorm(v[0], v[1], v[2], v[3])
{}

inline B4g4r4a4Unorm::B4g4r4a4Unorm(const uint8_t v[4]) noexcept:
    B4g4r4a4Unorm(v[0]/255.f, v[1]/255.f, v[2]/255.f, v[3]/255.f)
{}
} // namespace magma::packed
