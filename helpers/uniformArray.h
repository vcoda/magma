/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2022 Victor Coda.

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
        /* Helper class that allows to iterate and access the elements of UniformBuffer object. */

        template<class Type>
        class UniformArray
        {
        public:
            class Iterator;
            explicit UniformArray(Type *const buffer, uint32_t arraySize) noexcept;
            uint32_t getArraySize() const noexcept { return arraySize; }
            constexpr std::size_t getElementSize() const noexcept { return sizeof(Type); }
            Iterator begin() const noexcept { return Iterator(buffer); }
            Iterator end() const noexcept { return Iterator(buffer + arraySize); }
            Type& operator[](uint32_t index) noexcept;

        private:
            Type *const buffer;
            const uint32_t arraySize;
        };
    } // namespace helpers
} // namespace magma

#include "uniformArray.inl"
