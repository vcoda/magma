/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2019 Victor Coda.

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
        template<typename Type>
        class UniformArray : public detail::NonCopyable
        {
        public:
            class Iterator;
            explicit UniformArray(Type *const buffer,
                const uint32_t arraySize) noexcept:
                buffer(buffer),
                arraySize(arraySize)
            {
                MAGMA_ASSERT(buffer);
                MAGMA_ASSERT(arraySize > 0);
            }
            uint32_t getArraySize() const noexcept { return arraySize; }
            constexpr std::size_t getElementSize() const noexcept { return sizeof(Type); }
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
    } // namespace helpers
} // namespace magma

#include "uniformArrayIterator.h"
