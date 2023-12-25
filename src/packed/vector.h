/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

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

namespace magma
{
    namespace packed
    {
        /* Specifies a four-component, 32-bit packed unsigned normalized
           format that has a 2-bit W component in bits 30..31, a 10-bit
           Z component in bits 20..29, a 10-bit Y component in bits 10..19,
           and a 10-bit X component in bits 0..9. */

        struct X10y10z10w2Unorm
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

            X10y10z10w2Unorm() noexcept = default;
            explicit X10y10z10w2Unorm(uint32_t v) noexcept: v(v) {}
            explicit X10y10z10w2Unorm(float x, float y, float z, uint32_t w = 0) noexcept;
            explicit X10y10z10w2Unorm(const float v[3], uint32_t w = 0) noexcept:
                X10y10z10w2Unorm(v[0], v[1], v[2], w) {}

            static constexpr auto format = VK_FORMAT_A2B10G10R10_UNORM_PACK32;
        };

        /* Specifies a four-component, 32-bit packed signed normalized
           format that has a 2-bit W component in bits 30..31, a 10-bit
           Z component in bits 20..29, a 10-bit Y component in bits 10..19,
           and a 10-bit X component in bits 0..9. */

        struct X10y10z10w2Snorm
        {
            union
            {
                struct
                {
                    int32_t x: 10; // [-511,511]
                    int32_t y: 10; // [-511,511]
                    int32_t z: 10; // [-511,511]
                    uint32_t w: 2; // [0,3]
                };
                uint32_t v;
            };

            X10y10z10w2Snorm() noexcept = default;
            explicit X10y10z10w2Snorm(uint32_t v) noexcept: v(v) {}
            explicit X10y10z10w2Snorm(float x, float y, float z, uint32_t w = 0) noexcept;
            explicit X10y10z10w2Snorm(const float v[3], uint32_t w = 0) noexcept:
                X10y10z10w2Snorm(v[0], v[1], v[2], w) {}

            static constexpr auto format = VK_FORMAT_A2B10G10R10_SNORM_PACK32;
        };

        /* Specifies a three-component, 32-bit packed unsigned floating-
           point format that has a 10-bit Z component in bits 22..31,
           an 11-bit Y component in bits 11..21, an 11-bit X component
           in bits 0..10. An unsigned 11-bit floating-point number has
           no sign bit, a 5-bit exponent, and a 6-bit mantissa. An unsigned
           10-bit floating-point number has no sign bit, a 5-bit exponent,
           and a 5-bit mantissa. Unsigned 10- and 11-bit floating-point
           numbers are defined here:
           https://www.khronos.org/registry/DataFormat/specs/1.3/dataformat.1.3.html#11bitfp
           https://www.khronos.org/registry/DataFormat/specs/1.3/dataformat.1.3.html#10bitfp */

        struct X11y11z10Ufloat
        {
            union
            {
                struct
                {
                    uint32_t xm: 6; // x-mantissa
                    uint32_t xe: 5; // x-exponent
                    uint32_t ym: 6; // y-mantissa
                    uint32_t ye: 5; // y-exponent
                    uint32_t zm: 5; // z-mantissa
                    uint32_t ze: 5; // z-exponent
                };
                uint32_t v;
            };

            X11y11z10Ufloat() noexcept = default;
            explicit X11y11z10Ufloat(uint32_t v) noexcept: v(v) {}
            explicit X11y11z10Ufloat(float x, float y, float z) noexcept;
            explicit X11y11z10Ufloat(const float v[3]) noexcept:
                X11y11z10Ufloat(v[0], v[1], v[2]) {}

            static constexpr auto format = VK_FORMAT_B10G11R11_UFLOAT_PACK32;
        };
    } // namespace packed
} // namespace magma

#include "x10y10z10w2unorm.inl"
#include "x10y10z10w2snorm.inl"
#include "x11y11z10ufloat.inl"
