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

namespace magma
{
namespace core
{
constexpr void hashCombine(std::size_t& seed, std::size_t hash) noexcept
{   // https://www.boost.org/doc/libs/1_46_1/doc/html/hash/reference.html#boost.hash_combine
    seed ^= (hash + 0x9e3779b9 + (seed << 6) + (seed >> 2));
}

inline std::size_t hashCombineList(const std::initializer_list<std::size_t>& hashes) noexcept
{
    std::size_t value = 0;
    for (const std::size_t hash : hashes)
        hashCombine(value, hash);
    return value;
}
} // namespace core
} // namespace magma
