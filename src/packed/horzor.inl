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
#endif // MAGMA_SSE
} // namespace magma::packed
