/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2022 Victor Coda.

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
#if defined(__x86_64__) || defined(_M_AMD64)
#include <smmintrin.h>
#include "copyThread.inl"
#endif

namespace magma
{
namespace core
{
inline void *copyMemory(void *dst, const void *src, const std::size_t size)
{
    MAGMA_ASSERT(dst);
    MAGMA_ASSERT(src);
    MAGMA_ASSERT(size > 0);
    /* On x64 platform all allocations should have 16-byte alignment.
       On x86 it's hard to follow this restriction, as standard alignment
       there is 8 bytes: https://msdn.microsoft.com/en-us/library/ycsb6wwf.aspx
       Default std::vector also has 8-byte alignment, using custom allocator
       for it means incompatibility with other codebase. So SIMD copy performed
       only for x64 target. */
#if defined(__x86_64__) || defined(_M_AMD64)
    MAGMA_ASSERT(MAGMA_ALIGNED(dst));
    MAGMA_ASSERT(MAGMA_ALIGNED(src));
    if (size >= MAGMA_COPY_PAGE_SIZE)
    {
        const std::size_t blockCount = size / MAGMA_XMM_BLOCK_SIZE;
        const std::size_t threadBlockCount = blockCount / MAGMA_CONCURRENT_COPY_THREADS;
        const std::size_t threadCopySize = threadBlockCount * MAGMA_XMM_BLOCK_SIZE;
        std::thread threads[MAGMA_CONCURRENT_COPY_THREADS];
        for (int i = 0; i < MAGMA_CONCURRENT_COPY_THREADS; ++i)
        {
            threads[i] = std::thread(xmm::copyThread,
                ((char *)dst) + i * threadCopySize,
                ((const char *)src) + i * threadCopySize,
                threadBlockCount);
        }
        for (int i = 0; i < MAGMA_CONCURRENT_COPY_THREADS; ++i)
            threads[i].join();
        const std::size_t remnant = size - MAGMA_COPY_PAGE_SIZE * threadBlockCount;
        if (remnant)
        {
            const std::intptr_t offset = MAGMA_CONCURRENT_COPY_THREADS * threadCopySize;
            memcpy(((char *)dst) + offset,
                ((const char *)src) + offset,
                remnant);
        }
    }
    else
    {   // Little copy
        memcpy(dst, src, size);
    }
    return dst;
#else
    return memcpy(dst, src, size);
#endif // __x86_64__ || _M_AMD64
}
} // namespace core
} // namespace magma
