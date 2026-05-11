/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2026 Victor Coda.

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
#ifdef _MSC_VER
#pragma warning(disable: 4201) // nonstandard extension used: nameless struct/union
#endif

namespace magma::ieee754
{
    /* IEEE 754 single-precision binary floating-point format. */

    union float32
    {
        static constexpr uint32_t eps  = 0b00110100000000000000000000000000; // 2^-23                   1.1920928955078125e-7
        static constexpr uint32_t fmin = 0b00000000100000000000000000000000; // 2^-126                  1.1754943508222875e-38
        static constexpr uint32_t fmax = 0b01111111011111111111111111111111; // (2 - 2^-23) * 2^127     3.4028234663852886e+38
        static constexpr uint32_t smin = 0b00000000000000000000000000000001; // 2^-149                  1.401298464324817e-45
        static constexpr uint32_t smax = 0b00000000011111111111111111111111; // (1 - 2^-23) * 2^-126    1.1754942106924262e-38
        static constexpr uint32_t low  = 0b11111111011111111111111111111111; // -fmax
        static constexpr uint32_t one  = 0b00111111100000000000000000000000; // 1.0f
        static constexpr uint32_t inf  = 0b01111111100000000000000000000000; // +inf
        static constexpr uint32_t nan  = 0b01111111110000000000000000000000; // not a number
        static constexpr uint32_t bias = 127;                                // exponent bias

        float f;
        uint32_t i;
        struct
        {
        #ifdef MAGMA_BIG_ENDIAN
            uint32_t sign: 1;
            uint32_t exponent: 8;
            uint32_t mantissa: 23;
        #else
            uint32_t mantissa: 23;
            uint32_t exponent: 8;
            uint32_t sign: 1;
        #endif
        };
    };
} // namespace magma::ieee754
