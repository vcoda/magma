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
#include <smmintrin.h>
#include "../shared.h"

namespace magma
{
    MAGMA_INLINE void *zeroMemory(void *dst, size_t size) noexcept
    {
        MAGMA_ASSERT(dst);
        MAGMA_ASSERT(size > 0);
#ifdef _M_AMD64
        MAGMA_ASSERT(MAGMA_ALIGNED(dst));
        const __m128i _0 = _mm_setzero_si128();
        __m128i *vdst = reinterpret_cast<__m128i *>(dst);
        _mm_prefetch(reinterpret_cast<const char *>(vdst), _MM_HINT_NTA);
        const size_t blockCount = size / (sizeof(__m128i) * MAGMA_XMM_REGISTERS);
        size_t i;
        for (i = blockCount; i--; vdst += MAGMA_XMM_REGISTERS)
        {   // Zero 256 byte block
            _mm_stream_si128(vdst + 0, _0);
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
        const size_t tailSize = size - (sizeof(__m128i) * MAGMA_XMM_REGISTERS * blockCount);
        if (tailSize > 0)
        {
            const size_t registerCount = tailSize / sizeof(__m128i);
            MAGMA_ASSERT(registerCount < MAGMA_XMM_REGISTERS);
            for (i = 0; i < registerCount; ++i)
            {   // Zero remained 16-byte blocks
                _mm_stream_si128(vdst++, _0);
            }
            uint8_t *bdst = reinterpret_cast<uint8_t *>(vdst);
            const size_t byteCount = tailSize % sizeof(__m128i);
            MAGMA_ASSERT(byteCount < sizeof(__m128i));
            for (i = 0; i < byteCount; ++i)
            {   // Zero remained bytes
                *bdst++ = 0;
            }
        }
        return dst;
#else
        return memset(dst, 0, size);
#endif // _M_AMD64
    }
} // namespace magma
