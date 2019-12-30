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
#include <cstring>

namespace magma
{
    namespace detail
    {
        template<typename T> inline bool compare(const void *const p1, const void *const p2) noexcept
        {
            if (!p1 && !p2)
                return true;
            if (!p1 || !p2 || p1 == p2)
                return false;
            return !memcmp(p1, p2, sizeof(T));
        }

        template<typename T> inline bool compare(const T *const p1, const T *const p2) noexcept
        {
            if (!p1 && !p2)
                return true;
            if (!p1 || !p2 || p1 == p2)
                return false;
            return !memcmp(p1, p2, sizeof(T));
        }

        template<typename T> inline bool compareArrays(const T *const p1, const T *const p2, size_t count) noexcept
        {
            if (!p1 && !p2)
                return true;
            if (!p1 || !p2 || p1 == p2)
                return false;
            return !memcmp(p1, p2, sizeof(T) * count);
        }
    } // namespace detail
} // namespace magma
