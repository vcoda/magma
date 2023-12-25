/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2021 Victor Coda.

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
#include "noncopyable.h"

namespace magma
{
    namespace core
    {
        /* Linear memory allocator with alignment. */

        class LinearAllocator : public NonCopyable
        {
        public:
            explicit LinearAllocator(const std::size_t poolSize) noexcept:
                poolSize(poolSize), head(nullptr), space(0) {}

            void *alloc(const std::size_t size, const std::size_t alignment)
            {
                if (!powerOfTwo(alignment))
                    return nullptr;
                if (pool.empty())
                {
                    pool.resize(poolSize);
                    std::fill(pool.begin(), pool.end(), '\0');
                    head = pool.data();
                    space = pool.size();
                }
                // https://en.cppreference.com/w/cpp/memory/align
                void *ptr = std::align(alignment, size, head, space);
                if (ptr)
                {
                    head = (char *)head + size;
                    space -= size;
                }
                return ptr;
            }

            void reset() noexcept
            {
                if (!pool.empty())
                {
                    head = pool.data();
                    space = pool.size();
                }
            }

            bool hasSpace(const std::size_t size) const noexcept
            {
                return pool.empty()
                    ? poolSize >= size
                    : space >= size;
            }

        private:
            bool powerOfTwo(const std::size_t x) const noexcept
            {
                return (x & (x - 1)) == 0;
            }

            const std::size_t poolSize;
            std::vector<char> pool;
            void *head;
            std::size_t space;
        };
    } // namespace core
} // namespace magma
