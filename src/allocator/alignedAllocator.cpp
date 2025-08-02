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
#elif defined(__MINGW32__) || defined(__MINGW64__)
    ptr = __mingw_aligned_malloc(size, alignment);
#else
    MAGMA_ASSERT(alignment % sizeof(void *) == 0);
    const int result = posix_memalign(&ptr, alignment, size);
    if (result != 0) // posix_memalign() does not modify memptr on failure
        ptr = nullptr;
#endif // !_MSC_VER && !__MINGW__
    MAGMA_HANDLE_OUT_OF_MEMORY(ptr);
#if !defined(_MSC_VER)
    std::lock_guard<core::Spinlock> lock(mtx);
    allocations[ptr] = size;
#endif
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
    std::size_t oldSize;
    {
        std::lock_guard<core::Spinlock> lock(mtx);
        oldSize = allocations.at(original);
    }
    #if defined(__MINGW32__) || defined(__MINGW64__)
    ptr = __mingw_aligned_malloc(size, alignment);
    #else
    MAGMA_ASSERT(alignment % sizeof(void *) == 0);
    const int result = posix_memalign(&ptr, alignment, size);
    if (result != 0) // posix_memalign() does not modify memptr on failure
        ptr = nullptr;
    #endif // !__MINGW__
    if (ptr)
    {   // On Unix we need to copy memory between aligned blocks
        memcpy(ptr, original, std::min(size, oldSize));
    #if defined(__MINGW32__) || defined(__MINGW64__)
        __mingw_aligned_free(original);
    #else
        ::free(original);
    #endif
    }
#endif // !_MSC_VER
    MAGMA_HANDLE_OUT_OF_MEMORY(ptr);
#if !defined(_MSC_VER)
    std::lock_guard<core::Spinlock> lock(mtx);
    allocations.erase(original);
    allocations[ptr] = size;
#endif
    return ptr;
}

void AlignedAllocator::free(void *ptr) noexcept
{
    if (!ptr)
        return;
#if defined(_MSC_VER)
    _aligned_free(ptr);
#else
    #if defined(__MINGW32__) || defined(__MINGW64__)
    __mingw_aligned_free(ptr);
    #else
    ::free(ptr);
    #endif
    std::lock_guard<core::Spinlock> lock(mtx);
    allocations.erase(ptr);
#endif // !_MSC_VER
}

AllocationStatistics AlignedAllocator::getAllocationStatistics() const noexcept
{   // Not tracked by release allocator
    return {};
}
} // namespace magma
