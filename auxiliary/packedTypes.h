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
#include <cstdint>
#include <cmath>
#ifdef MAGMA_SSE
#include <xmmintrin.h>
#include <smmintrin.h>
#else
#include <algorithm>
#endif // !MAGMA_SSE

namespace magma
{
    namespace aux
    {
        /* Unsigned 32-bit normal in the range [0,1] that reflects
           VK_FORMAT_A2B10G10R10_UNORM_PACK32 format. */

        struct UnsignedNormalW2z10y10x10
        {
            union
            {
                struct
                {
                    uint32_t x: 10; // [0,1023]
                    uint32_t y: 10; // [0,1023]
                    uint32_t z: 10; // [0,1023]
                    uint32_t w: 2;  // [0,3]
                };
                uint32_t v;
            };

            UnsignedNormalW2z10y10x10() noexcept = default;
            UnsignedNormalW2z10y10x10(float x, float y, float z, float w = 0.f) noexcept
#if defined(MAGMA_SSE)
                : UnsignedNormalW2z10y10x10(_mm_set_ps(w, z, y, x)) {}
#else
            {
                x = std::min(std::max(0.f, x), 1.f);
                y = std::min(std::max(0.f, y), 1.f);
                z = std::min(std::max(0.f, z), 1.f);
                w = std::min(std::max(0.f, w), 1.f);
                x = std::roundf(x * 1023.f);
                y = std::roundf(y * 1023.f);
                z = std::roundf(z * 1023.f);
                w = std::roundf(w * 3.f);
                this->x = static_cast<uint32_t>(x);
                this->y = static_cast<uint32_t>(y);
                this->z = static_cast<uint32_t>(z);
                this->w = static_cast<uint32_t>(w);
            }
#endif // !MAGMA_SSE
#if defined(MAGMA_SSE)
            UnsignedNormalW2z10y10x10(__m128 v) noexcept
            {
                constexpr float l[4] = {0.f, 0.f, 0.f, 0.f};
                constexpr float h[4] = {1.f, 1.f, 1.f, 1.f};
                constexpr float scale[4] = {1023.f, 1023.f, 1023.f, 3.f};
                __m128 lo = _mm_load_ps(l);
                __m128 hi = _mm_load_ps(h);
                v = _mm_min_ps(_mm_max_ps(lo, v), hi);
                v = _mm_mul_ps(v, _mm_load_ps(scale));
                v = _mm_round_ps(v, _MM_ROUND_NEAREST);
                __m128i vi = _mm_cvtps_epi32(v);
                const uint32_t *p = reinterpret_cast<const uint32_t *>(&vi);
                this->x = p[0];
                this->y = p[1];
                this->z = p[2];
                this->w = p[3];
            }
#elif defined(MAGMA_NEON)
            // TODO
#endif // MAGMA_NEON
        };

        /* Signed 32-bit normal in the range[-1,1] that reflects
           VK_FORMAT_A2B10G10R10_SNORM_PACK32 format. */

        struct SignedNormalW2z10y10x10
        {
            union
            {
                struct
                {
                    int32_t x: 10; // [-511,511]
                    int32_t y: 10; // [-511,511]
                    int32_t z: 10; // [-511,511]
                    int32_t w: 2;  // [0,3]
                };
                int32_t v;
            };

            SignedNormalW2z10y10x10() noexcept = default;
            SignedNormalW2z10y10x10(float x, float y, float z, float w = 0.f) noexcept
#if defined(MAGMA_SSE)
                : SignedNormalW2z10y10x10(_mm_set_ps(w, z, y, x)) {}
#else
            {
                x = std::min(std::max(-1.f, x), 1.f);
                y = std::min(std::max(-1.f, y), 1.f);
                z = std::min(std::max(-1.f, z), 1.f);
                w = std::min(std::max(0.f, w), 1.f);
                x = std::roundf(x * 511.f);
                y = std::roundf(y * 511.f);
                z = std::roundf(z * 511.f);
                w = std::roundf(w * 3.f);
                this->x = static_cast<int32_t>(x);
                this->y = static_cast<int32_t>(y);
                this->z = static_cast<int32_t>(z);
                this->w = static_cast<int32_t>(w);
            }
#endif // !MAGMA_SSE
#if defined(MAGMA_SSE)
            SignedNormalW2z10y10x10(__m128 v) noexcept
            {
                constexpr float l[4] = {-1.f, -1.f, -1.f, 0.f};
                constexpr float h[4] = {1.f, 1.f, 1.f, 1.f};
                constexpr float scale[4] = {511.f, 511.f, 511.f, 3.f};
                __m128 lo = _mm_load_ps(l);
                __m128 hi = _mm_load_ps(h);
                v = _mm_min_ps(_mm_max_ps(lo, v), hi);
                v = _mm_mul_ps(v, _mm_load_ps(scale));
                v = _mm_round_ps(v, _MM_ROUND_NEAREST);
                __m128i vi = _mm_cvtps_epi32(v);
                const int32_t *p = reinterpret_cast<const int32_t *>(&vi);
                this->x = p[0];
                this->y = p[1];
                this->z = p[2];
                this->w = p[3];
            }
#elif defined(MAGMA_NEON)
            // TODO
#endif // MAGMA_NEON
        };
    } // namespace aux
} // namespace magma
