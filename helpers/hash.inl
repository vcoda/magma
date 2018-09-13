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
#include <functional>

namespace magma
{
namespace helpers
{
// https://www.boost.org/doc/libs/1_46_1/doc/html/hash/reference.html#boost.hash_combine
inline void hashCombine(size_t& seed, size_t hash) noexcept
{
    seed ^= (hash + 0x9e3779b9 + (seed << 6) + (seed >> 2));
}

template<typename Type>
inline void hashCombineArg(size_t& seed, const Type& arg) noexcept
{
    std::hash<Type> hasher;
    const size_t hash = hasher(arg);
    hashCombine(seed, hash);
}

template<typename Type>
inline void __nextHash(size_t& seed, const Type& arg) noexcept
{
    hashCombineArg(seed, arg);
}

template<typename Type, typename... Args>
inline void __nextHash(size_t& seed, const Type& arg, Args... args) noexcept
{
    hashCombineArg(seed, arg);
    __nextHash(seed, args...);
}

template<typename Type, typename... Args>
inline size_t hashVariadic(const Type& arg, Args... args) noexcept
{
    size_t value = 0;
    hashCombineArg(value, arg);
    __nextHash(value, args...);
    return value;
}

template<typename Type>
inline size_t hashArray(const Type *const array, size_t count) noexcept
{
    size_t value = 0;
    for (size_t i = 0; i < count; ++i)
        hashCombineArg(value, array[i]);
    return value;
}
} // namespace helpers
} // namespace magma
