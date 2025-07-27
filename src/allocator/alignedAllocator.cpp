/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2025 Victor Coda.

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
#include "alignedAllocator.h"

namespace magma
{
void *AlignedAllocator::alloc(std::size_t size, std::size_t alignment,
    VkSystemAllocationScope /* allocationScope */)
{
    MAGMA_ASSERT(core::powerOfTwo(alignment));
    void *ptr;
#if defined(_MSC_VER)
    ptr = _aligned_malloc(size, alignment);
#elif defined(__MINGW32__)
    ptr = __mingw_aligned_malloc(size, alignment);
#else
    MAGMA_ASSERT(alignment % sizeof(void *) == 0);
    const int result = posix_memalign(&ptr, alignment, size);
    if (result != 0) // posix_memalign() does not modify memptr on failure
        ptr = nullptr;
#endif // _MSC_VER || __MINGW32__
    if (!ptr)
    {
    #ifdef MAGMA_NO_EXCEPTIONS
        return nullptr;
    #else
        throw std::bad_alloc();
    #endif
    }
    return ptr;
}

void *AlignedAllocator::realloc(void *original, std::size_t size, std::size_t alignment,
    VkSystemAllocationScope allocationScope)
{
    if (0 == size)
    {   // realloc(ptr, 0) equals to free()
        this->free(original);
        return nullptr;
    }
    if (!original)
        return alloc(size, alignment, allocationScope);
    MAGMA_ASSERT(core::powerOfTwo(alignment));
    void *ptr;
#if defined(_MSC_VER)
    ptr = _aligned_realloc(original, size, alignment);
#else
    #if defined(__MINGW32__)
    ptr = __mingw_aligned_malloc(size, alignment);
    #else
    MAGMA_ASSERT(alignment % sizeof(void *) == 0);
    const int result = posix_memalign(&ptr, alignment, size);
    if (result != 0) // posix_memalign() does not modify memptr on failure
        ptr = nullptr;
    #endif
    if (ptr)
    {   // On Unix we need to copy memory between aligned blocks
        memcpy(ptr, original, size);
    #if defined(__MINGW32__)
        __mingw_aligned_free(original);
    #else
        ::free(original);
    #endif
    }
#endif // !_MSC_VER
    if (!ptr)
    {
    #ifdef MAGMA_NO_EXCEPTIONS
        return nullptr;
    #else
        throw std::bad_alloc();
    #endif
    }
    return ptr;
}

void AlignedAllocator::free(void *ptr) noexcept
{
    if (!ptr)
        return;
#if defined(_MSC_VER)
    _aligned_free(ptr);
#elif defined(__MINGW32__)
    __mingw_aligned_free(ptr);
#else
    ::free(ptr);
#endif
}

AllocationStatistics AlignedAllocator::getAllocationStatistics() const noexcept
{   // Not tracked by release allocator
    return {};
}
} // namespace magma
