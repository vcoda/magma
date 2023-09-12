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
#include <emmintrin.h>
#include "zeroThread.inl"
#endif

namespace magma
{
namespace core
{
inline void *zeroMemory(void *dst, const std::size_t size)
{
    MAGMA_ASSERT(dst);
    MAGMA_ASSERT(size > 0);
#if defined(__x86_64__) || defined(_M_AMD64)
    MAGMA_ASSERT(MAGMA_ALIGNED(dst));
    if (size >= MAGMA_COPY_PAGE_SIZE)
    {
        const std::size_t blockCount = size / MAGMA_XMM_BLOCK_SIZE;
        const std::size_t threadBlockCount = blockCount / MAGMA_CONCURRENT_COPY_THREADS;
        const std::size_t threadZeroSize = threadBlockCount * MAGMA_XMM_BLOCK_SIZE;
        std::thread threads[MAGMA_CONCURRENT_COPY_THREADS];
        for (int i = 0; i < MAGMA_CONCURRENT_COPY_THREADS; ++i)
        {
            threads[i] = std::thread(xmm::zeroThread,
                ((char *)dst) + i * threadZeroSize,
                threadBlockCount);
        }
        for (int i = 0; i < MAGMA_CONCURRENT_COPY_THREADS; ++i)
            threads[i].join();
        const std::size_t remnant = size - MAGMA_COPY_PAGE_SIZE * threadBlockCount;
        if (remnant)
        {
            const std::intptr_t offset = MAGMA_CONCURRENT_COPY_THREADS * threadZeroSize;
            memset(((char *)dst) + offset, 0, remnant);
        }
    }
    else
    {   // Little zero
        memset(dst, 0, size);
    }
    return dst;
#else
    return memset(dst, 0, size);
#endif // __x86_64__ || _M_AMD64
}
} // namespace core
} // namespace magma
