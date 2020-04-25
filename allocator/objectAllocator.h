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
#include "../core/noncopyable.h"

namespace magma
{
    namespace mem
    {
        /* User-defined allocator of Magma objects. */

        class IObjectAllocator : public core::NonCopyable
        {
        public:
            virtual void *alloc(std::size_t size) = 0;
            virtual void free(void *p) noexcept = 0;
            virtual std::size_t getBytesAllocated() const noexcept = 0;
        };

        /* Provides user-defined new and delete operators. */

        class Allocator
        {
        public:
            void *operator new(std::size_t size);
            void *operator new(std::size_t size, const std::nothrow_t&) noexcept;
            void* operator new(std::size_t, void* where) noexcept;
            void operator delete(void *ptr);
            void operator delete(void *, void *) {}

            static void overrideDefaultAllocator(std::shared_ptr<IObjectAllocator> allocator);
            static std::shared_ptr<IObjectAllocator> getOverridenAllocator() noexcept;

        private:
            static std::shared_ptr<IObjectAllocator> allocator;
#ifdef MAGMA_DEBUG
            static std::atomic<int64_t> allocCount;
#endif
        };
    } // namespace mem
} // namespace magma

#include "objectAllocator.inl"
