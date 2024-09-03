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
#include "hashing/constexpr.h"
#include "hashing/fnv1.h"
#include "hashing/fnv1string.h"

namespace magma
{
    namespace core
    {
        template<class T> constexpr hash_t hash(T x) noexcept;
        template<class T> constexpr hash_t hash(T *p) noexcept;
        template<class T> constexpr hash_t hashArg(hash_t seed, const T& arg) noexcept;
        template<class T, typename... Args> constexpr hash_t hashArgs(const T& arg, Args... args) noexcept;
        template<class T, std::size_t N> constexpr hash_t hashArray(const T (&arr)[N]) noexcept;
        template<class T> hash_t hashArray(const T arr[], std::size_t count) noexcept;
        template<class T> constexpr hash_t hashString(const T *str) noexcept;
        template<class T> hash_t hashString(const std::basic_string<T>& str) noexcept;
        hash_t combineHashList(const std::initializer_list<hash_t>& hashes) noexcept;
    } // namespace core
} // namespace magma

#include "hash.inl"
