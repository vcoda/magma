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
#include <cstdint>
#ifdef MAGMA_SSE2_INTRINSICS
#include <xmmintrin.h>
#include <emmintrin.h>
#ifdef MAGMA_SSE4_INTRINSICS
#include <smmintrin.h>
#endif
#else
#include <algorithm>
#endif // MAGMA_SSE2_INTRINSICS

namespace magma
{
    namespace aux
    {
        /* Unsigned 32-bit normal that reflects
           VK_FORMAT_A2B10G10R10_UNORM_PACK32 format. */

        struct UnsignedNormalX10y10z10w2
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

            UnsignedNormalX10y10z10w2() noexcept = default;
            UnsignedNormalX10y10z10w2(float x, float y, float z, float w = 0.f) noexcept
            {
                constexpr float l[4] = {-1.f, -1.f, -1.f, 0.f};
                constexpr float h[4] = {1.f, 1.f, 1.f, 1.f};
                constexpr float mul[4] = {.5f, .5f, .5f, 1.f};
                constexpr float add[4] = {.5f, .5f, .5f, 0.f};
                constexpr float scale[4] = {1023.f, 1023.f, 1023.f, 3.f};
#           ifdef MAGMA_SSE2_INTRINSICS
                __m128 lo = _mm_load_ps(l);
                __m128 hi = _mm_load_ps(h);
                __m128 v = _mm_set_ps(w, z, y, x);
                v = _mm_min_ps(_mm_max_ps(v, lo), hi);
                v = _mm_mul_ps(v, _mm_load_ps(mul));
                v = _mm_add_ps(v, _mm_load_ps(add));
                v = _mm_mul_ps(v, _mm_load_ps(scale));
                v = _mm_round_ps(v, _MM_ROUND_NEAREST);
                __m128i vi = _mm_cvtps_epi32(v);
                const uint32_t *p = reinterpret_cast<const uint32_t *>(&vi);
                this->x = p[0];
                this->y = p[1];
                this->z = p[2];
                this->w = p[3];
#           else
                x = std::min(std::max(x, l[0]), h[0]);
                y = std::min(std::max(y, l[1]), h[1]);
                z = std::min(std::max(z, l[2]), h[2]);
                w = std::min(std::max(w, l[3]), h[3]);
                x = x * .5f + .5f;
                y = y * .5f + .5f;
                z = z * .5f + .5f;
                x = std::roundf(x * scale[0]);
                y = std::roundf(y * scale[1]);
                z = std::roundf(z * scale[2]);
                w = std::roundf(w * scale[3]);
                this->x = static_cast<uint32_t>(x);
                this->y = static_cast<uint32_t>(y);
                this->z = static_cast<uint32_t>(z);
                this->w = static_cast<uint32_t>(w);
#           endif // MAGMA_SSE2_INTRINSICS
            }
        };

        /* Signed 32-bit normal that reflects
           VK_FORMAT_A2B10G10R10_SNORM_PACK32 format. */

        struct SignedNormalX10y10z10w2
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

            SignedNormalX10y10z10w2() noexcept = default;
            SignedNormalX10y10z10w2(float x, float y, float z, float w = 0.f) noexcept
            {
                constexpr float l[4] = {-1.f, -1.f, -1.f, 0.f};
                constexpr float h[4] = {1.f, 1.f, 1.f, 1.f};
                constexpr float scale[4] = {511.f, 511.f, 511.f, 3.f};
#           ifdef MAGMA_SSE2_INTRINSICS
                __m128 lo = _mm_load_ps(l);
                __m128 hi = _mm_load_ps(h);
                __m128 v = _mm_set_ps(w, z, y, x);
                v = _mm_min_ps(_mm_max_ps(v, lo), hi);
                v = _mm_mul_ps(v, _mm_load_ps(scale));
                v = _mm_round_ps(v, _MM_ROUND_NEAREST);
                __m128i vi = _mm_cvtps_epi32(v);
                const int32_t *p = reinterpret_cast<const int32_t *>(&vi);
                this->x = p[0];
                this->y = p[1];
                this->z = p[2];
                this->w = p[3];
#           else
                x = std::min(std::max(x, l[0]), h[0]);
                y = std::min(std::max(y, l[1]), h[1]);
                z = std::min(std::max(z, l[2]), h[2]);
                w = std::min(std::max(w, l[3]), h[3]);
                x = std::roundf(x * scale[0]);
                y = std::roundf(y * scale[1]);
                z = std::roundf(z * scale[2]);
                w = std::roundf(w * scale[3]);
                this->x = static_cast<int32_t>(x);
                this->y = static_cast<int32_t>(y);
                this->z = static_cast<int32_t>(z);
                this->w = static_cast<int32_t>(w);
#           endif // MAGMA_SSE2_INTRINSICS
            }
        };
    } // namespace aux
} // namespace magma
