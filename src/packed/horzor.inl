namespace magma::packed
{
#ifdef MAGMA_SSE
inline __m128i _mm_horizontal_or(__m128i v)
{
    __m128i v2 = _mm_shuffle_epi32(v, _MM_SHUFFLE(3, 2, 3, 2));
    v = _mm_or_si128(v, v2); // x = x|z, y = y|w
    v2 = _mm_shuffle_epi32(v, _MM_SHUFFLE(1, 1, 1, 1)); // move z to the x
    return _mm_or_si128(v, v2); // x|y|z|w
}
#elif defined(MAGMA_NEON)
inline int32x2_t vhorizontal_or(int32x4_t v)
{
    int32x2_t v1 = vget_low_s32(v);
    int32x2_t v2 = vget_high_s32(v);
    v1 = vorr_s32(v1, v2); // x = x|z, y = y|w
    v2 = vext_s32(v1, v1, 1); // move y to x
    return vorr_s32(v1, v2); // x = x|y|z|w
}
#endif // MAGMA_NEON
} // namespace magma::packed
