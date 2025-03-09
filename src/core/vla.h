/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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
        /* Variable-length arrays is a C99 (not C++) feature.
           But as allocation on stack is much faster than in the heap,
           we implement our own VLA with alloca()/_malloca() functions.
           In debug build, VLA always allocated in the heap memory. */

        template<class T>
        class VariableLengthArray final
        {
        public:
            explicit VariableLengthArray(void *ptr, std::size_t len) noexcept;
            ~VariableLengthArray();
            T *begin() noexcept { return array; }
            const T *begin() const noexcept { return array; }
            T *end() noexcept { return array + len; }
            const T *end() const noexcept { return array + len; }
            uint32_t length() const noexcept { return len; }
            uint32_t count() const noexcept { return cnt; }
            void put(const T& element) noexcept;

            operator T *() noexcept { return array; }
            operator const T *() const noexcept { return array; }

            T& operator[](int i) noexcept { return array[i]; }
            const T& operator[](int i) const noexcept { return array[i]; }

        private:
            T *const array;
            const uint32_t len;
            uint32_t cnt;
        };
    } // namespace core
} // namespace magma

#include "vla.inl"
