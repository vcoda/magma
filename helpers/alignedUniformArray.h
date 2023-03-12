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
        /* Helper class that allows to iterate and access the elements of DynamicUniformBuffer object.
           Takes into account an alignment of elements which is determined by hardware requirements. */

        template<typename Type>
        class AlignedUniformArray
        {
        public:
            class Iterator;
            explicit AlignedUniformArray(void *const buffer, const uint32_t arraySize,
                const VkDeviceSize alignment) noexcept;
            uint32_t getArraySize() const noexcept { return arraySize; }
            constexpr std::size_t getElementSize() const noexcept { return sizeof(Type); }
            VkDeviceSize getElementAlignment() const noexcept { return alignment; }
            Iterator begin() const noexcept { return Iterator(buffer, alignment); }
            Iterator end() const noexcept { return Iterator(buffer + arraySize * alignment, alignment); }
            Type& operator[](uint32_t index) noexcept;

        private:
            char *const buffer;
            const uint32_t arraySize;
            const VkDeviceSize alignment;
        };
    } // namespace helpers
} // namespace magma

#include "alignedUniformArray.inl"
