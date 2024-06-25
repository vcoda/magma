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

namespace magma
{
    namespace helpers
    {
        /* As variable length arrays are not part of C++1x standard,
           we implement our own. Remember to prevent scenarios with
           multiple allocation in cycles/inlined functions, as
           allocated stack memory freed only when function returns. */

        template<class Type>
        class StackArray final
        {
        public:
            // Only small arrays could be safely allocated on the stack
            static constexpr size_t MaxElements = 1024 / sizeof(Type);

            explicit StackArray(void *const stack, size_t size) noexcept:
                stack(static_cast<Type *>(stack)),
                length(static_cast<uint32_t>(size)),
                pos(0)
            {
                if (!std::is_pod<Type>())
                {
                    for (Type *p = begin(); p != end(); ++p)
                        new(p) Type();
                }
            #ifdef MAGMA_DEBUG
                bytesAllocated = sizeof(Type) * length;
            #endif
            }

            ~StackArray()
            {
                if (!std::is_pod<Type>())
                {
                    for (Type *p = begin(); p != end(); ++p)
                        p->~Type();
                }
                MAGMA_FREEA(stack);
            }

            // Support range-based loops
            Type *begin() noexcept { return stack; }
            const Type *begin() const noexcept { return stack; }
            Type *end() noexcept { return stack + length; }
            const Type *end() const noexcept { return stack + length; }
            uint32_t size() const noexcept { return length; }
            uint32_t count() const noexcept { return pos; }
            bool empty() const noexcept { return !pos; }
            // Do not store or pass stack pointer anywhere, use inside function scope only
            operator Type *() noexcept { return stack; }
            operator const Type *() const noexcept { return stack; }

            uint32_t put(const Type& elem) noexcept
            {
                MAGMA_ASSERT(stack);
                MAGMA_ASSERT(pos < length);
                stack[pos++] = elem;
                return pos;
            }

            Type& operator[](int i) noexcept
            {
                MAGMA_ASSERT(stack);
                MAGMA_ASSERT(i < static_cast<int>(length));
                return stack[i];
            }

            const Type& operator[](int i) const noexcept
            {
                MAGMA_ASSERT(stack);
                MAGMA_ASSERT(i < static_cast<int>(length));
                return stack[i];
            }

        private:
            Type *const stack;
            const uint32_t length;
            uint32_t pos;
        #ifdef MAGMA_DEBUG
            size_t bytesAllocated;
        #endif
        };
    } // namespace helpers
} // namespace magma

// Macro to call alloca() in the stack frame of the variable declaration
#define MAGMA_STACK_ARRAY(Type, var, count)\
    MAGMA_ASSERT(count < magma::helpers::StackArray<Type>::MaxElements);\
    magma::helpers::StackArray<Type> var(MAGMA_ALLOCA(sizeof(Type) * (count)), (count))
