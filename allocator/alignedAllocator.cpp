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
#include "pch.h"
#pragma hdrstop
#include <exception>
#include "alignedAllocator.h"

namespace magma
{
void *AlignedAllocator::alloc(std::size_t size,
    std::size_t alignment,
    VkSystemAllocationScope allocationScope)
{
    MAGMA_UNUSED(allocationScope);
#ifdef _MSC_VER
    void *ptr = _aligned_malloc(size, alignment);
#else
    void *ptr = nullptr;
    posix_memalign(&ptr, alignment, size);
#endif
    if (!ptr)
        throw std::bad_alloc();
    return ptr;
}

void *AlignedAllocator::realloc(void *original, std::size_t size, std::size_t alignment,
    VkSystemAllocationScope allocationScope)
{
    MAGMA_UNUSED(allocationScope);
#ifdef _MSC_VER
    void *ptr = _aligned_realloc(original, size, alignment);
#else
    MAGMA_UNUSED(alignment);
    void *ptr = realloc(original, size);
    // TODO: check alignment, use posix_memalign/memcpy if not aligned!
#endif
    if (!ptr)
        throw std::bad_alloc();
    return ptr;
}

void AlignedAllocator::free(void *memory) noexcept
{
    ::free(memory);
}
} // namespace magma
