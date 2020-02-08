namespace magma
{
namespace core
{
namespace xmm
{
inline void zeroThread(void *dst, const size_t blockCount) noexcept
{
    const __m128i _0 = _mm_setzero_si128();
    __m128i *vdst = (__m128i *)dst;
    for (size_t i = blockCount; i--; vdst += MAGMA_XMM_REGISTERS)
    {   // Zero 256 byte block
        _mm_stream_si128(vdst,     _0);
        _mm_stream_si128(vdst + 1, _0);
        _mm_stream_si128(vdst + 2, _0);
        _mm_stream_si128(vdst + 3, _0);
        _mm_stream_si128(vdst + 4, _0);
        _mm_stream_si128(vdst + 5, _0);
        _mm_stream_si128(vdst + 6, _0);
        _mm_stream_si128(vdst + 7, _0);
        _mm_stream_si128(vdst + 8, _0);
        _mm_stream_si128(vdst + 9, _0);
        _mm_stream_si128(vdst + 10, _0);
        _mm_stream_si128(vdst + 11, _0);
        _mm_stream_si128(vdst + 12, _0);
        _mm_stream_si128(vdst + 13, _0);
        _mm_stream_si128(vdst + 14, _0);
        _mm_stream_si128(vdst + 15, _0);
    }
}
} // namespace xmm
} // namespace core
} // namespace magma
