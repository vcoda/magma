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
#include <atomic>
#include <memory>
#include "../allocator/objectAllocator.h"
#include "nonCopyable.h"

namespace magma
{
    namespace internal
    {
        /* Provides user-defined new and delete operators for custom allocations. */

        class Allocable : public internal::NonCopyable
        {
        public:
            void *operator new(std::size_t size)
            {
                void *ptr;
                if (allocator)
                    ptr = allocator->alloc(size);
                else
                    ptr = malloc(size);
                if (!ptr)
                    throw std::bad_alloc();
                ++allocCount;
                return ptr;
            }

            void *operator new(std::size_t size, const std::nothrow_t&) noexcept
            {
                void *ptr;
                if (allocator)
                    ptr = allocator->alloc(size);
                else
                    ptr = malloc(size);
                if (ptr)
                    ++allocCount;
                return ptr;
            }

            void operator delete(void *ptr)
            {
                if (ptr)
                {
                    --allocCount;
                    if (allocator)
                        allocator->free(ptr);
                    else
                        free(ptr);
                }
            }

        public:
            static void setAllocator(std::shared_ptr<IObjectAllocator> allocator);
            static std::shared_ptr<IObjectAllocator> getAllocator() noexcept { return allocator; }

        private:
            static std::shared_ptr<IObjectAllocator> allocator;
            static std::atomic<int32_t> allocCount;
        };
    } // namespace internal
} // namespace magma
