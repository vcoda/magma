/*
Magma - C++1x interface over Khronos Vulkan API.
Copyright (C) 2018 Victor Coda.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://www.gnu.org/licenses/>.
*/
#pragma once
#include <cstring>
#if defined(_M_AMD64) || defined(__x86_64__)
#include <thread>
#include <smmintrin.h>
#endif
#include "shared.h"

namespace magma
{
    namespace internal
    {
#if defined(_M_AMD64) || defined(__x86_64__)
        MAGMA_INLINE void __copyThread(void *dst, const void *src, size_t blockCount) noexcept
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
#endif // _M_AMD64 || __x86_64__

        MAGMA_INLINE void *copyMemory(void *dst, const void *src, size_t size)
        {
            MAGMA_ASSERT(dst);
            MAGMA_ASSERT(src);
            MAGMA_ASSERT(size > 0);
            /* On x64 platform all allocations should have 16-byte alignment.
               On x86 it's hard to follow this restriction, as standard alignment
               there is 8 bytes: https://msdn.microsoft.com/en-us/library/ycsb6wwf.aspx
               Default std::vector also has 8-byte alignment, using custom allocator
               for it means incompatibility with other codebase. So SSE copy performed
               only for x64 target. */
#if defined(_M_AMD64) || defined(__x86_64__)
            MAGMA_ASSERT(MAGMA_ALIGNED(dst));
            MAGMA_ASSERT(MAGMA_ALIGNED(src));
            constexpr size_t BLOCK_SIZE = sizeof(__m128i) * MAGMA_XMM_REGISTERS;
            constexpr int NUM_THREADS = 4;
            if (size <= BLOCK_SIZE * NUM_THREADS)
                memcpy(dst, src, size);
            else
            {
                size_t blockCount = size / BLOCK_SIZE;
                size_t threadBlockCount = blockCount / NUM_THREADS;
                size_t threadCopySize = threadBlockCount * BLOCK_SIZE;
                std::thread threads[NUM_THREADS];
                for (int i = 0; i < NUM_THREADS; ++i)
                {
                    threads[i] = std::thread(__copyThread,
                        ((char *)dst) + i * threadCopySize,
                        ((const char *)src) + i * threadCopySize,
                        threadBlockCount);
                }
                for (int i = 0; i < NUM_THREADS; ++i)
                    threads[i].join();
                size_t residualSize = size - (BLOCK_SIZE * threadBlockCount * NUM_THREADS);
                if (residualSize > 0)
                {
                    memcpy(((char *)dst) + NUM_THREADS * threadCopySize,
                        ((const char *)src) + NUM_THREADS * threadCopySize,
                        residualSize);
                }
            }
            return dst;
#else
            return memcpy(dst, src, size);
#endif // _M_AMD64 || __x86_64__
        }
    } // namespace internal
} // namespace magma
