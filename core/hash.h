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
template<class Type>
constexpr hash_t hash(Type x) noexcept
{
    ConstexprHash<Type> hasher;
    return hasher(x);
}

template<class Type>
constexpr hash_t hash(Type *p) noexcept
{
    ConstexprHash<uintptr_t> hasher;
    return hasher(reinterpret_cast<uintptr_t>(p));
}

template<class Type>
constexpr hash_t hashArg(hash_t seed, const Type& arg) noexcept
{
    return hashCombine(seed, hash(arg));
}

template<class Type, typename... Args>
constexpr hash_t hashArg(hash_t seed, const Type& arg, Args... args) noexcept
{
    return hashArg(hashCombine(seed, hash(arg)), args...);
}

template<class Type, typename... Args>
constexpr hash_t hashArgs(const Type& arg, Args... args) noexcept
{
    return hashArg(hash(arg), args...);
}

template<typename T, std::size_t N>
constexpr hash_t hashArray(const T (&arr)[N]) noexcept
{
    return hashing::Fnv1a<T, N, N>().hash(arr);
}

template<typename T>
constexpr hash_t hashString(const T *str) noexcept
{
    return hashing::string::Fnv1a<T, 0ull>().hash(str);
}

template<class Type>
inline hash_t hashArray(const Type arr[], std::size_t count) noexcept
{
    std::hash<Type> hasher;
    hash_t value = 0ull;
    for (std::size_t i = 0; i < count; ++i)
        value = hashCombine(value, hasher(arr[i]));
    return value;
}

template<class Type>
inline hash_t hashString(const std::basic_string<Type>& str) noexcept
{
    std::hash<std::basic_string<Type>> hasher;
    return hasher(str);
}

inline hash_t combineHashList(const std::initializer_list<hash_t>& hashes) noexcept
{
    hash_t value = 0ull;
    for (const hash_t hash : hashes)
        value = hashCombine(value, hash);
    return value;
}
} // namespace core
} // namespace magma
