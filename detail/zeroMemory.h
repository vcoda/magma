/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2019 Victor Coda.

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
#if defined(_M_AMD64) || defined(__x86_64__)
#include <smmintrin.h>
#endif

namespace magma
{
    namespace detail
    {
#if defined(_M_AMD64) || defined(__x86_64__)
        inline void __zeroThread(void *dst, std::size_t blockCount) noexcept
        {
            const __m128i _0 = _mm_setzero_si128();
            __m128i *vdst = (__m128i *)dst;
            for (std::size_t i = blockCount; i--; vdst += MAGMA_XMM_REGISTERS)
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
#endif // _M_AMD64 || __x86_64__

        inline void *zeroMemory(void *dst, std::size_t size)
        {
            MAGMA_ASSERT(dst);
            MAGMA_ASSERT(size > 0);
#if defined(_M_AMD64) || defined(__x86_64__)
            MAGMA_ASSERT(MAGMA_ALIGNED(dst));
            constexpr std::size_t BLOCK_SIZE = sizeof(__m128i) * MAGMA_XMM_REGISTERS;
            constexpr int NUM_THREADS = 4;
            if (size <= BLOCK_SIZE * NUM_THREADS)
                memset(dst, 0, size);
            else
            {
                std::size_t blockCount = size / BLOCK_SIZE;
                std::size_t threadBlockCount = blockCount / NUM_THREADS;
                std::size_t threadZeroSize = threadBlockCount * BLOCK_SIZE;
                std::thread threads[NUM_THREADS];
                for (int i = 0; i < NUM_THREADS; ++i)
                {
                    threads[i] = std::thread(__zeroThread,
                        ((char *)dst) + i * threadZeroSize,
                        threadBlockCount);
                }
                for (int i = 0; i < NUM_THREADS; ++i)
                    threads[i].join();
                std::size_t residualSize = size - (BLOCK_SIZE * threadBlockCount * NUM_THREADS);
                if (residualSize > 0)
                {
                    memset(((char *)dst) + NUM_THREADS * threadZeroSize,
                        0,
                        residualSize);
                }
            }
            return dst;
#else
            return memset(dst, 0, size);
#endif // _M_AMD64 || __x86_64__
        }
    } // namespace detail
} // namespace magma
