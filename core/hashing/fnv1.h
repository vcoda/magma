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
#pragma warning(disable: 4307) // integral constant overflow
#endif

namespace magma
{
    namespace core
    {
        namespace hashing
        {
            // Fowler-Noll-Vo hash function is a non-cryptographic hash function
            // created by Glenn Fowler, Landon Curt Noll, and Kiem-Phong Vo.
            // https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function

            namespace fnv
            {
#ifdef MAGMA_X64
                constexpr uint64_t prime = 1099511628211ULL;
                constexpr uint64_t basis = 14695981039346656037ULL;
#else
                constexpr uint32_t prime = 16777619U;
                constexpr uint32_t basis = 2166136261U;
#endif // MAGMA_X64
            }

            /* FNV-1 hash algorithm. */

            template<typename T, std::size_t N, std::size_t I>
            struct Fnv1
            {
                constexpr hash_t hash(const T (&a)[N]) noexcept
                {
                    return (Fnv1<T, N, I-1>().hash(a) * a[I-1]) ^ fnv::prime;
                }
            };

            template<typename T, std::size_t N>
            struct Fnv1<T, N, 1>
            {
                constexpr hash_t hash(const T (&a)[N]) noexcept
                {
                    return (fnv::basis * a[0]) ^ fnv::prime;
                }
            };

            /* The FNV-1a hash differs from the FNV-1 hash by only the order in which the multiply and xor is performed. */

            template<typename T, std::size_t N, std::size_t I>
            struct Fnv1a
            {
                constexpr hash_t hash(const T (&a)[N]) noexcept
                {
                    return (Fnv1a<T, N, I-1>().hash(a) ^ a[I-1]) * fnv::prime;
                }
            };

            template<typename T, std::size_t N>
            struct Fnv1a<T, N, 1>
            {
                constexpr hash_t hash(const T (&a)[N]) noexcept
                {
                    return (fnv::basis ^ a[0]) * fnv::prime;
                }
            };
        } // namespace hashing
    } // namespace core
} // namespace magma
