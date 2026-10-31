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

namespace magma
{
    typedef uint64_t hash_t;

    namespace core
    {
        namespace fnv
        {
        #ifdef MAGMA_64
            constexpr uint64_t Prime = 1099511628211ull;
            constexpr uint64_t Basis = 14695981039346656037ull;
        #else
            constexpr uint32_t Prime = 16777619u;
            constexpr uint32_t Basis = 2166136261u;
        #endif // MAGMA_64
        } // namespace fnv

        template<class T>
        struct ConstexprHash
        {
            constexpr hash_t operator()(T x) const noexcept;
        };

        template<class T> constexpr hash_t hash(T x) noexcept;
        template<class T> constexpr hash_t hash(T *p) noexcept;
        constexpr hash_t hashCombine(const hash_t seed, const hash_t hash) noexcept;
        template<class T> constexpr hash_t hashArg(hash_t seed, const T& arg) noexcept;
        template<class T, typename... Args> constexpr hash_t hashArgs(const T& arg, Args... args) noexcept;
        template<class T, std::size_t N> constexpr hash_t hashArray(const T (&arr)[N]) noexcept;
        template<class T> hash_t hashArray(const T arr[], std::size_t count) noexcept;
        constexpr uint32_t hashString(const char *str) noexcept;
        uint32_t hashStringSIMD(const char *str) noexcept;
        uint32_t hashExtension(const char *name) noexcept;
    } // namespace core
} // namespace magma

#define MAGMA_COMPILE_TIME_STRING_HASH(str) std::integral_constant<uint32_t, magma::core::hashString(str)>::value
