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
#include "pch.h"
#pragma hdrstop
#include "cxxAllocator.h"
#include "../exceptions/exception.h"

namespace magma
{
std::shared_ptr<ICxxAllocator> CxxAllocator::allocator;
#ifdef MAGMA_DEBUG
std::atomic<int64_t> CxxAllocator::allocCount;
#endif

void CxxAllocator::overrideDefaultAllocator(std::shared_ptr<ICxxAllocator> allocator)
{
#ifdef MAGMA_DEBUG
    if (allocCount)
        MAGMA_THROW("object allocator should be defined prior any allocations");
#endif
    CxxAllocator::allocator = std::move(allocator);
}

std::shared_ptr<ICxxAllocator> CxxAllocator::getOverridenAllocator() noexcept
{
    return allocator;
}
} // namespace magma
