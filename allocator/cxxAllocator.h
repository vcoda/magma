/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
#include <memory>
#include "../core/destructible.h"

namespace magma
{
    /* Provides allocation functions for new/delete overloads. */

    class ICxxAllocator : public core::IDestructible
    {
    public:
        virtual void *alloc(std::size_t size) = 0;
        virtual void free(void *p) noexcept = 0;
        virtual std::size_t getBytesAllocated() const noexcept = 0;
    };

    /* Provides overloaded new and delete operators for user-defined allocator.
        Developers have to take the following into account:
        std::make_shared uses ::new, so if any special behavior has been set up
        using a class-specific operator new, it will differ from std::shared_ptr<T>(new T(args...)).
        https://en.cppreference.com/w/cpp/memory/shared_ptr/make_shared */

    class CxxAllocator
    {
    public:
        void *operator new(std::size_t size);
        void *operator new(std::size_t size, const std::nothrow_t&) noexcept;
        void* operator new(std::size_t, void* where) noexcept;
        void operator delete(void *ptr);
        void operator delete(void *, void *) {}

        static void overrideDefaultAllocator(std::shared_ptr<ICxxAllocator> allocator);
        static std::shared_ptr<ICxxAllocator> getOverridenAllocator() noexcept;

    private:
        static std::shared_ptr<ICxxAllocator> allocator;
#ifdef MAGMA_DEBUG
        static std::atomic<int64_t> allocCount;
#endif
    };
} // namespace magma

#include "cxxAllocator.inl"
