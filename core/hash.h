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
#include "hashCombine.h"

namespace magma
{
namespace core
{
template<typename Type>
constexpr std::size_t hash(const Type& arg) noexcept
{
    std::hash<Type> hasher;
    return hasher(arg);
}

template<typename Type>
constexpr void hashNext(std::size_t& seed, const Type& arg) noexcept
{
    hashCombine(seed, hash(arg));
}

template<typename Type, typename... Args>
constexpr void hashNext(std::size_t& seed, const Type& arg, Args... args) noexcept
{
    hashCombine(seed, hash(arg));
    hashNext(seed, args...);
}

template<typename Type, typename... Args>
constexpr std::size_t hashArgs(const Type& arg, Args... args) noexcept
{
    std::size_t value = hash(arg);
    hashNext(value, args...);
    return value;
}

template<typename Type>
inline std::size_t hashArray(const Type *const arr, std::size_t count) noexcept
{
    std::hash<Type> hasher;
    std::size_t value = 0;
    for (std::size_t i = 0; i < count; ++i)
        hashCombine(value, hasher(arr[i]));
    return value;
}

template<typename Type>
inline std::size_t hashVector(const std::vector<Type>& v) noexcept
{
    std::hash<Type> hasher;
    std::size_t value = 0;
    for (const auto& it : v)
        hashCombine(value, hasher(it));
    return value;
}

template<typename Type>
inline std::size_t hashList(const std::list<Type>& ls) noexcept
{
    std::hash<Type> hasher;
    std::size_t value = 0;
    for (const auto& it : ls)
        hashCombine(value, hasher(it));
    return value;
}

template<typename Type>
inline std::size_t hashString(const std::basic_string<Type>& str) noexcept
{
    std::hash<std::basic_string<Type>> hasher;
    return hasher(str);
}
} // namespace core
} // namespace magma
