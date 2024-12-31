/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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
#if defined(_MSC_VER) || defined(__MINGW32__)
    #include <intrin.h>
#endif

/* std::popcount() is not available until C++20.
   https://stackoverflow.com/questions/109023/count-the-number-of-set-bits-in-a-32-bit-integer */

namespace magma
{
namespace core
{
inline uint32_t popcount(uint32_t x) noexcept
{
#if (defined(_MSC_VER) || defined(__MINGW32__)) && (defined(_M_IX86) || defined(_M_X64))
    return __popcnt(x);
#elif defined(__GNUC__) || defined(__clang__) || defined(__INTEL_COMPILER)
    return (uint32_t)__builtin_popcount(x);
#else
    constexpr uint32_t c1 = 0x55555555u;
    constexpr uint32_t c2 = 0x33333333u;
    constexpr uint32_t c4 = 0x0f0f0f0fu;
    x -= (x >> 1) & c1;
    x = ((x >> 2) & c2) + (x & c2);
    x = (x + (x >> 4)) & c4;
    x *= 0x01010101u;
    return x >> 24;
#endif
}

inline uint64_t popcount(uint64_t x) noexcept
{
#if (defined(_MSC_VER) || defined(__MINGW32__)) && defined(_M_X64)
    return __popcnt64(x);
#elif defined(__GNUC__) || defined(__clang__) || defined(__INTEL_COMPILER)
    return (uint64_t)__builtin_popcountll(x);
#else
    constexpr uint64_t c1 = 0x5555555555555555ull;
    constexpr uint64_t c2 = 0x3333333333333333ull;
    constexpr uint64_t c4 = 0x0f0f0f0f0f0f0f0full;
    x -= (x >> 1) & c1;
    x = ((x >> 2) & c2) + (x & c2);
    x = (x + (x >> 4)) & c4;
    x *= 0x0101010101010101ull;
    return x >> 56;
#endif
}
} // namespace core
} // namespace magma
