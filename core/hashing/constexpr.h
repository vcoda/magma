/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2022 Victor Coda.

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
#if !_HAS_CXX17
#pragma warning(disable : 4127) // conditional expression is constant
#endif
#endif
#include "combine.h"

namespace magma
{
    namespace core
    {
        template<class Int>
        struct ConstexprHash
        {
            constexpr hash_t operator()(const Int x) const noexcept;
        };

        template<>
        struct ConstexprHash<uint32_t>
        {   // Robert Jenkins' reversible 32 bit mix hash function
            constexpr hash_t operator()(uint32_t x) const noexcept
            {
                x += (x << 12); // x *= (1 + (1 << 12))
                x ^= (x >> 22);
                x += (x << 4);  // x *= (1 + (1 << 4))
                x ^= (x >> 9);
                x += (x << 10); // x *= (1 + (1 << 10))
                x ^= (x >> 2);
                // x *= (1 + (1 << 7)) * (1 + (1 << 12))
                x += (x << 7);
                x += (x << 12);
                return x;
            }
        };

        template<>
        struct ConstexprHash<uint64_t>
        {   // Thomas Wang 64 bit mix hash function
            constexpr hash_t operator()(uint64_t x) const noexcept
            {
                x = (~x) + (x << 21); // x *= (1 << 21) - 1; x -= 1;
                x = x ^ (x >> 24);
                x = x + (x << 3) + (x << 8); // x *= 1 + (1 << 3) + (1 << 8)
                x = x ^ (x >> 14);
                x = x + (x << 2) + (x << 4); // x *= 1 + (1 << 2) + (1 << 4)
                x = x ^ (x >> 28);
                x = x + (x << 31); // x *= 1 + (1 << 31)
                return x;
            }
        };

        template<>
        struct ConstexprHash<float>
        {
            constexpr hash_t operator()(const float x) const noexcept
            {
                const float ax = x >= 0.f ? x : -x;
                const float frac = ax - static_cast<uint32_t>(ax);
                const uint32_t fractional = static_cast<uint32_t>(frac * 1000000.f); // shift left
                const uint32_t integer = static_cast<uint32_t>(x);
                const ConstexprHash<uint32_t> hasher;
                const hash_t intHash = hasher(integer);
                const hash_t fracHash = hasher(fractional);
                return hashCombine(intHash, fracHash);
            }
        };

        template<>
        struct ConstexprHash<double>
        {
            constexpr hash_t operator()(const double x) const noexcept
            {
                const double ax = x >= 0. ? x : -x;
                const double frac = ax - static_cast<uint64_t>(ax);
                const uint64_t fractional = static_cast<uint64_t>(frac * 1000000000.); // shift left
                const uint64_t integer = static_cast<uint64_t>(x);
                const ConstexprHash<uint64_t> hasher;
                const hash_t intHash = hasher(integer);
                const hash_t fracHash = hasher(fractional);
                return hashCombine(intHash, fracHash);
            }
        };

        template<class Int>
        constexpr hash_t ConstexprHash<Int>::operator()(const Int x) const noexcept
        {
            static_assert(sizeof(Int) <= sizeof(uint64_t), "integral type is too wide");
#       ifdef MAGMA_CXX17
            if constexpr
#       else
            if
#       endif
               (sizeof(Int) <= 4)
                return ConstexprHash<uint32_t>()(static_cast<uint32_t>(x));
            else
                return ConstexprHash<uint64_t>()(static_cast<uint64_t>(x));
        }
    } // namespace core
} // namespace magma
