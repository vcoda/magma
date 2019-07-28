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

namespace magma
{
    namespace internal
    {
        template<typename Type>
        inline bool compare(const void *const src, const void *const dst)
        {
            if (!src && !dst)
                return true;
            if (!src || !dst || src == dst)
                return false;
            return !memcmp(src, dst, sizeof(Type));
        }

        template<typename Type>
        inline bool compare(const Type *const src, const Type *const dst)
        {
            if (!src && !dst)
                return true;
            if (!src || !dst || src == dst)
                return false;
            return !memcmp(src, dst, sizeof(Type));
        }

        template<typename Type>
        inline bool compareArrays(const Type *const src, const Type *const dst, size_t count) noexcept
        {
            if (!src && !dst)
                return true;
            if (!src || !dst || src == dst)
                return false;
            return !memcmp(src, dst, sizeof(Type) * count);
        }
    } // namespace internal
} // namespace magma
