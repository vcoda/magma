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
#include <cstddef>
#include <functional>

namespace magma
{
    namespace internal
    {
        // https://www.boost.org/doc/libs/1_46_1/doc/html/hash/reference.html#boost.hash_combine
        constexpr void hashCombine(size_t& seed, size_t hash) noexcept
        {
            seed ^= (hash + 0x9e3779b9 + (seed << 6) + (seed >> 2));
        }

        template<typename Type>
        constexpr void hashNext(size_t& seed, const Type& arg) noexcept
        {
            std::hash<Type> hasher;
            hashCombine(seed, hasher(arg));
        }

        template<typename Type, typename... Args>
        constexpr void hashNext(size_t& seed, const Type& arg, Args... args) noexcept
        {
            std::hash<Type> hasher;
            hashCombine(seed, hasher(arg));
            hashNext(seed, args...);
        }

        template<typename Type, typename... Args>
        constexpr size_t hashArgs(const Type& arg, Args... args) noexcept
        {
            std::hash<Type> hasher;
            size_t value = 0;
            hashCombine(value, hasher(arg));
            hashNext(value, args...);
            return value;
        }

        template<typename Type>
        constexpr size_t hashArray(const Type *const array, size_t count) noexcept
        {
            std::hash<Type> hasher;
            size_t value = 0;
            for (size_t i = 0; i < count; ++i)
                hashCombine(value, hasher(array[i]));
            return value;
        }
    } // namespace internal
} // namespace magma

