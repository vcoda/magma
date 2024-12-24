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
            explicit VariableLengthArray(void *ptr, std::size_t len) noexcept:
                ptr(reinterpret_cast<T *>(ptr)),
                len(static_cast<uint32_t>(len)),
                cnt(0)
            {
                if constexpr (std::is_default_constructible<T>::value)
                {
                    for (T *p = begin(); p != end(); ++p)
                        new(p) T();
                }
            }

            ~VariableLengthArray()
            {
                if constexpr (std::is_destructible<T>::value)
                {
                    for (T *p = begin(); p != end(); ++p)
                        p->~T();
                }
                MAGMA_STACK_FREE(ptr);
            }

            T *begin() noexcept { return ptr; }
            const T *begin() const noexcept { return ptr; }
            T *end() noexcept { return ptr + len; }
            const T *end() const noexcept { return ptr + len; }
            uint32_t length() const noexcept { return len; }
            uint32_t count() const noexcept { return cnt; }

            void put(const T& element) noexcept
            {
                MAGMA_ASSERT(cnt < len);
                ptr[cnt++] = element;
            }

            T& operator[](int i) noexcept { return ptr[i]; }
            const T& operator[](int i) const noexcept { return ptr[i]; }
            operator T*() noexcept { return ptr; }
            operator const T*() const noexcept { return ptr; }

        private:
            T *const ptr;
            const uint32_t len;
            uint32_t cnt;
        };
    } // namespace core
} // namespace magma

#define MAGMA_VLA(Type, var, count)\
    MAGMA_ASSERT(sizeof(Type) * count <= MAGMA_MAX_STACK_SIZE);\
    magma::core::VariableLengthArray<Type> var(MAGMA_STACK_ALLOC(sizeof(Type) * count), count)
