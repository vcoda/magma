namespace magma
{
namespace core
{
namespace xmm
{
inline void copyThread(void *dst, const void *src, const size_t blockCount) noexcept
{
    __m128i *vsrc = (__m128i *)src;
    __m128i *vdst = (__m128i *)dst;
    for (size_t i = blockCount; i--; vsrc += MAGMA_XMM_REGISTERS, vdst += MAGMA_XMM_REGISTERS)
    {   // Copy 256 byte block
        __m128i xmm0 = _mm_stream_load_si128(vsrc);
        __m128i xmm1 = _mm_stream_load_si128(vsrc + 1);
        __m128i xmm2 = _mm_stream_load_si128(vsrc + 2);
        __m128i xmm3 = _mm_stream_load_si128(vsrc + 3);
        __m128i xmm4 = _mm_stream_load_si128(vsrc + 4);
        __m128i xmm5 = _mm_stream_load_si128(vsrc + 5);
        __m128i xmm6 = _mm_stream_load_si128(vsrc + 6);
        __m128i xmm7 = _mm_stream_load_si128(vsrc + 7);
        __m128i xmm8 = _mm_stream_load_si128(vsrc + 8);
        __m128i xmm9 = _mm_stream_load_si128(vsrc + 9);
        __m128i xmm10 = _mm_stream_load_si128(vsrc + 10);
        __m128i xmm11 = _mm_stream_load_si128(vsrc + 11);
        __m128i xmm12 = _mm_stream_load_si128(vsrc + 12);
        __m128i xmm13 = _mm_stream_load_si128(vsrc + 13);
        __m128i xmm14 = _mm_stream_load_si128(vsrc + 14);
        __m128i xmm15 = _mm_stream_load_si128(vsrc + 15);
        _mm_stream_si128(vdst,     xmm0);
        _mm_stream_si128(vdst + 1, xmm1);
        _mm_stream_si128(vdst + 2, xmm2);
        _mm_stream_si128(vdst + 3, xmm3);
        _mm_stream_si128(vdst + 4, xmm4);
        _mm_stream_si128(vdst + 5, xmm5);
        _mm_stream_si128(vdst + 6, xmm6);
        _mm_stream_si128(vdst + 7, xmm7);
        _mm_stream_si128(vdst + 8, xmm8);
        _mm_stream_si128(vdst + 9, xmm9);
        _mm_stream_si128(vdst + 10, xmm10);
        _mm_stream_si128(vdst + 11, xmm11);
        _mm_stream_si128(vdst + 12, xmm12);
        _mm_stream_si128(vdst + 13, xmm13);
        _mm_stream_si128(vdst + 14, xmm14);
        _mm_stream_si128(vdst + 15, xmm15);
    }
}
} // namespace xmm
} // namespace core
} // namespace magma
