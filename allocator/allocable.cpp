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
#include "allocable.h"
#include "../allocator/objectAllocator.h"
#include "../misc/exception.h"

namespace magma
{
std::shared_ptr<IObjectAllocator> Allocable::allocator;
int32_t Allocable::allocCount = 0;

void *Allocable::operator new(std::size_t size)
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

void *Allocable::operator new(std::size_t size, const std::nothrow_t&) noexcept
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

void Allocable::operator delete(void *ptr)
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

void Allocable::setAllocator(std::shared_ptr<IObjectAllocator> allocator)
{
    if (allocCount)
        throw Exception("allocator should be defined only when allocation count is zero");
    allocator = std::move(allocator);
}

std::shared_ptr<IObjectAllocator> Allocable::getAllocator() noexcept
{
    return allocator;
}
} // namespace magma
