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
        class AlignedUniformArray : public NonCopyable
        {
        public:
            class Iterator;

        public:
            AlignedUniformArray(void *buffer, const uint32_t arraySize, const VkDeviceSize alignment):
                buffer(reinterpret_cast<char *>(buffer)),
                arraySize(arraySize),
                alignment(alignment)
            {
                MAGMA_ASSERT(buffer);
                MAGMA_ASSERT(arraySize > 0);
                MAGMA_ASSERT(alignment > 0);
            }
            uint32_t getArraySize() const { return arraySize; }
            constexpr size_t getElementSize() const { return sizeof(Type); }
            VkDeviceSize getElementAlignment() const { return alignment; }
            Iterator begin() const { return Iterator(buffer, alignment); }
            Iterator end() const { return Iterator(buffer + arraySize * alignment, alignment); }
            Type& operator[](uint32_t index)
            {
                MAGMA_ASSERT(index < arraySize);
                char *const elem = buffer + index * alignment;
                return *reinterpret_cast<Type *>(elem);
            }

        private:
            char *const buffer;
            const uint32_t arraySize;
            const VkDeviceSize alignment;
        };

        template<typename Type>
        class AlignedUniformArray<Type>::Iterator
        {
        public:
            Iterator(char *ptr, const VkDeviceSize alignment):
                ptr(ptr), alignment(alignment) {}
            Iterator& operator++()
            {
                ptr += alignment;
                return *this;
            }
            Iterator operator++(int)
            {
                Iterator temp = *this;
                ptr += alignment;
                return temp;
            }
            Iterator& operator--()
            {
                ptr -= alignment;
                return *this;
            }
            Iterator operator--(int)
            {
                Iterator temp = *this;
                ptr -= alignment;
                return temp;
            }
            bool operator!=(const Iterator& it) const
                { return ptr != it.ptr; }
            Type& operator*()
                { return *reinterpret_cast<Type *>(ptr); }

        private:
            char *ptr;
            const VkDeviceSize alignment;
        };
    } // namespace helpers
} // namespace magma
