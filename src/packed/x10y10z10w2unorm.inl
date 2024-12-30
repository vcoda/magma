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
    // horizontal OR
    __m128i iv2 = _mm_shuffle_epi32(iv, _MM_SHUFFLE(3, 2, 3, 2));
    iv = _mm_or_si128(iv, iv2); // x = x|z, y = y|w
    iv2 = _mm_shuffle_epi32(iv, _MM_SHUFFLE(1, 1, 1, 1)); // move z to the x
    iv = _mm_or_si128(iv, iv2); // x|y|z|w
    this->v = _mm_cvtsi128_si32(iv);
#elif defined(MAGMA_NEON)
    #error NEON codepath not implemented
#else // FPU
    x = std::min(std::max(0.f, x), 1.f);
    y = std::min(std::max(0.f, y), 1.f);
    z = std::min(std::max(0.f, z), 1.f);
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
