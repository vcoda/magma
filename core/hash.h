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
#include "hashing/constexpr.h"
#include "hashing/fnv1.h"
#include "hashing/fnv1string.h"

namespace magma
{
    namespace core
    {
        template<typename Type>
        constexpr std::size_t hash(Type x) noexcept
        {
            ConstexprHash<Type> hasher;
            return hasher(x);
        }

        template<typename Type>
        constexpr std::size_t hash(Type *p) noexcept
        {
            ConstexprHash<uintptr_t> hasher;
            return hasher(reinterpret_cast<uintptr_t>(p));
        }

        namespace hashing
        {
            template<typename Type>
            constexpr std::size_t hashNext(const std::size_t seed, const Type& arg) noexcept
            {
                return hashCombine(seed, hash(arg));
            }

            template<typename Type, typename... Args>
            constexpr std::size_t hashNext(const std::size_t seed, const Type& arg, Args... args) noexcept
            {
                return hashNext(hashCombine(seed, hash(arg)), args...);
            }
        }

        template<typename Type, typename... Args>
        constexpr std::size_t hashArgs(const Type& arg, Args... args) noexcept
        {
            return hashing::hashNext(hash(arg), args...);
        }

        template<typename T, std::size_t N>
        constexpr std::size_t hashArray(const T (&arr)[N]) noexcept
        {
            return hashing::Fnv1a<T, N, N>().hash(arr);
        }

        template<typename T>
        constexpr std::size_t hashString(const T *str) noexcept
        {
            return hashing::string::Fnv1a<T, 0U>().hash(str);
        }

        template<typename Type>
        inline std::size_t hashArray(const Type arr[], std::size_t count) noexcept
        {
            std::hash<Type> hasher;
            std::size_t value = 0U;
            for (std::size_t i = 0U; i < count; ++i)
                value = hashCombine(value, hasher(arr[i]));
            return value;
        }

        template<typename Type>
        inline std::size_t hashString(const std::basic_string<Type>& str) noexcept
        {
            std::hash<std::basic_string<Type>> hasher;
            return hasher(str);
        }

        inline std::size_t combineHashList(const std::initializer_list<std::size_t>& hashes) noexcept
        {
            std::size_t value = 0U;
            for (const std::size_t hash : hashes)
                value = hashCombine(value, hash);
            return value;
        }
    } // namespace core
} // namespace magma
