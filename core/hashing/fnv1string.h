/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
#include "fnv1.h"

namespace magma
{
    namespace core
    {
        namespace hashing
        {
            namespace string
            {
                /* FNV-1 hash algorithm for C string. */

                template<typename T, std::size_t I>
                struct Fnv1
                {
                    constexpr std::size_t hash(const T *const str, const std::size_t hash) noexcept
                    {
                        return str[I] ? string::Fnv1<T, I+1>().hash(str, (hash * str[I]) ^ fnv::prime) : hash;
                    }
                };

                template<typename T>
                struct Fnv1<T, 0>
                {
                    constexpr std::size_t hash(const T *const str, const std::size_t basis = fnv::basis) noexcept
                    {
                        return string::Fnv1<T, 1>().hash(str, (basis * str[0]) ^ fnv::prime);
                    }
                };

                template<typename T>
                struct Fnv1<T, 1024>
                {
                    constexpr std::size_t hash(const T *const, const std::size_t hash) noexcept
                    {
                        return hash; // End recursion
                    }
                };

                /* FNV-1a hash algorithm for C string. */

                template<typename T, std::size_t I>
                struct Fnv1a
                {
                    constexpr std::size_t hash(const T *const str, const std::size_t hash) noexcept
                    {
                        return str[I] ? string::Fnv1a<T, I+1>().hash(str, (hash ^ str[I]) * fnv::prime) : hash;
                    }
                };

                template<typename T>
                struct Fnv1a<T, 0>
                {
                    constexpr std::size_t hash(const T *const str, const std::size_t basis = fnv::basis) noexcept
                    {
                        return string::Fnv1a<T, 1>().hash(str, (basis ^ str[0]) * fnv::prime);
                    }
                };

                template<typename T>
                struct Fnv1a<T, 1024>
                {
                    constexpr std::size_t hash(const T *const, const std::size_t hash) noexcept
                    {
                        return hash; // End recursion
                    }
                };
            } // namespace string
        } // namespace hashing
    } // namespace core
} // namespace magma
