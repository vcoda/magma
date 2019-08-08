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
#include <functional>
#include <string>

namespace magma
{
    namespace internal
    {
        // https://www.boost.org/doc/libs/1_46_1/doc/html/hash/reference.html#boost.hash_combine
        constexpr void hashCombine(std::size_t& seed, std::size_t hash)
        {
            seed ^= (hash + 0x9e3779b9 + (seed << 6) + (seed >> 2));
        }

        template<typename Type>
        constexpr std::size_t hash(const Type& arg)
        {
            std::hash<Type> hasher;
            return hasher(arg);
        }

        template<typename Type>
        constexpr void hashNext(std::size_t& seed, const Type& arg)
        {
            hashCombine(seed, hash(arg));
        }

        template<typename Type, typename... Args>
        constexpr void hashNext(std::size_t& seed, const Type& arg, Args... args)
        {
            hashCombine(seed, hash(arg));
            hashNext(seed, args...);
        }

        template<typename Type, typename... Args>
        constexpr std::size_t hashArgs(const Type& arg, Args... args)
        {
            std::size_t value = 0;
            hashCombine(value, hash(arg));
            hashNext(value, args...);
            return value;
        }

        template<typename Type>
        constexpr std::size_t hashArray(const Type *const array, std::size_t count)
        {
            std::hash<Type> hasher;
            std::size_t value = 0;
            for (std::size_t i = 0; i < count; ++i)
                hashCombine(value, hasher(array[i]));
            return value;
        }

        template<typename Elem>
        inline std::size_t hashString(const std::basic_string<Elem>& str) noexcept
        {
            std::hash<std::basic_string<Elem>> hasher;
            return hasher(str);
        }

        inline std::size_t combineHashList(const std::initializer_list<std::size_t>& hashes) noexcept
        {
            std::size_t value = 0;
            for (const std::size_t hash : hashes)
                hashCombine(value, hash);
            return value;
        }
    } // namespace internal
} // namespace magma
