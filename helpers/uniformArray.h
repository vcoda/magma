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
#include "../nonCopyable.h"
#include "../shared.h"

namespace magma
{
    namespace helpers
    {
        template<typename Type>
        class UniformArray : public NonCopyable
        {
        public:
            class Iterator;

        public:
            explicit UniformArray(Type *const buffer,
                const uint32_t arraySize) noexcept:
                buffer(buffer),
                arraySize(arraySize)
            {
                MAGMA_ASSERT(buffer);
                MAGMA_ASSERT(arraySize > 0);
            }
            uint32_t getArraySize() const noexcept { return arraySize; }
            constexpr size_t getElementSize() const noexcept { return sizeof(Type); }
            Iterator begin() const noexcept { return Iterator(buffer); }
            Iterator end() const noexcept { return Iterator(buffer + arraySize); }
            Type& operator[](uint32_t index) noexcept
            {
                MAGMA_ASSERT(index < arraySize);
                return *buffer[index];
            }

        private:
            Type *const buffer;
            const uint32_t arraySize;
        };

        template<typename Type>
        class UniformArray<Type>::Iterator
        {
        public:
            Iterator(Type *ptr) noexcept:
                ptr(ptr) {}
            Iterator& operator++() noexcept
            {
                ++ptr;
                return *this;
            }
            Iterator operator++(int) noexcept
            {
                Iterator temp = *this;
                ++ptr;
                return temp;
            }
            Iterator& operator--() noexcept
            {
                --ptr;
                return *this;
            }
            Iterator operator--(int) noexcept
            {
                Iterator temp = *this;
                --ptr;
                return temp;
            }
            bool operator!=(const Iterator& it) const noexcept
                { return ptr != it.ptr; }
            Type& operator*() noexcept
                { return *ptr; }

        private:
            Type *ptr;
        };
    } // namespace helpers
} // namespace magma
