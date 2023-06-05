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

#ifdef MAGMA_DEBUG
    #define MAGMA_VALIDATE_MEMORY_MANAGEMENT 1
#else
    #define MAGMA_VALIDATE_MEMORY_MANAGEMENT 0
#endif

namespace magma
{
AlignedAllocator::AlignedAllocator():
    allocatedSize(0ull),
    internalAllocatedSize(0ull)
{
    numAllocations = {};
    numReallocations = {};
    numFrees = {};
}

AlignedAllocator::~AlignedAllocator()
{
#if MAGMA_VALIDATE_MEMORY_MANAGEMENT
    for (uint32_t scope = VK_SYSTEM_ALLOCATION_SCOPE_COMMAND;
        scope <= VK_SYSTEM_ALLOCATION_SCOPE_INSTANCE; ++scope)
    {   // TODO: write to log?
        MAGMA_ASSERT(numAllocations[scope] == numFrees[scope]);
    }
    MAGMA_ASSERT(0 == allocatedSize);
    MAGMA_ASSERT(allocations.empty());
#endif // MAGMA_VALIDATE_MEMORY_MANAGEMENT
}

void *AlignedAllocator::alloc(std::size_t size,
    std::size_t alignment,
    VkSystemAllocationScope allocationScope)
{
#ifdef _MSC_VER
    void *ptr = _aligned_malloc(size, alignment);
#else
    void *ptr = nullptr;
    const int result = posix_memalign(&ptr, alignment, size);
    if (result != 0)
        throw std::bad_alloc();
#endif
    if (!ptr)
        throw std::bad_alloc();
#ifdef MAGMA_DEBUG
    std::lock_guard<std::mutex> lock(mtx);
    // Add allocation
    allocations[ptr] = {size, allocationScope};
    allocatedSize += size;
    ++numAllocations[allocationScope];
#endif // MAGMA_DEBUG
    return ptr;
}

void *AlignedAllocator::realloc(void *original, std::size_t size, std::size_t alignment,
    VkSystemAllocationScope allocationScope)
{
#ifdef _MSC_VER
    void *ptr = _aligned_realloc(original, size, alignment);
#else
    MAGMA_UNUSED(alignment);
    void *ptr = ::realloc(original, size);
    // TODO: check alignment, use posix_memalign/memcpy if not aligned!
#endif
    if (!ptr)
        throw std::bad_alloc();
#ifdef MAGMA_DEBUG
    std::lock_guard<std::mutex> lock(mtx);
    // Replace old allocation with a new one
    allocatedSize -= allocations.at(original).first;
    allocations.erase(original);
    allocations[ptr] = {size, allocationScope};
    allocatedSize += size;
    ++numReallocations[allocationScope];
#endif // MAGMA_DEBUG
    return ptr;
}

void AlignedAllocator::free(void *ptr) noexcept
{
    if (ptr)
    {
    #ifdef _MSC_VER
        _aligned_free(ptr);
    #else
        ::free(ptr);
    #endif
    #ifdef MAGMA_DEBUG
        std::lock_guard<std::mutex> lock(mtx);
        // Delete allocation
        auto entry = allocations.at(ptr);
        allocations.erase(ptr);
        allocatedSize -= entry.first;
        ++numFrees[entry.second];
    #endif // MAGMA_DEBUG
    }
}

void AlignedAllocator::internalAllocationNotification(std::size_t size,
    VkInternalAllocationType /* allocationType */,
    VkSystemAllocationScope /* allocationScope */) noexcept
{
    MAGMA_UNUSED(size);
#ifdef MAGMA_DEBUG
    std::lock_guard<std::mutex> lock(mtx);
    internalAllocatedSize += size;
#endif // MAGMA_DEBUG
}

void AlignedAllocator::internalFreeNotification(std::size_t size,
    VkInternalAllocationType /* allocationType */,
    VkSystemAllocationScope /* allocationScope */) noexcept
{
    MAGMA_UNUSED(size);
#ifdef MAGMA_DEBUG
    std::lock_guard<std::mutex> lock(mtx);
    internalAllocatedSize -= size;
#endif // MAGMA_DEBUG
}

AllocationStatistic AlignedAllocator::getAllocationStatistic() const noexcept
{
    AllocationStatistic statistic;
#ifdef MAGMA_DEBUG
    std::lock_guard<std::mutex> lock(mtx);
    statistic.aliveCommandAllocations = numAllocations[VK_SYSTEM_ALLOCATION_SCOPE_COMMAND] - numFrees[VK_SYSTEM_ALLOCATION_SCOPE_COMMAND];
    statistic.aliveObjectAllocations = numAllocations[VK_SYSTEM_ALLOCATION_SCOPE_OBJECT] - numFrees[VK_SYSTEM_ALLOCATION_SCOPE_OBJECT];
    statistic.aliveCacheAllocations = numAllocations[VK_SYSTEM_ALLOCATION_SCOPE_CACHE] - numFrees[VK_SYSTEM_ALLOCATION_SCOPE_CACHE];
    statistic.aliveDeviceAllocations = numAllocations[VK_SYSTEM_ALLOCATION_SCOPE_DEVICE] - numFrees[VK_SYSTEM_ALLOCATION_SCOPE_DEVICE];
    statistic.aliveInstanceAllocations = numAllocations[VK_SYSTEM_ALLOCATION_SCOPE_INSTANCE] - numFrees[VK_SYSTEM_ALLOCATION_SCOPE_INSTANCE];
    statistic.allocatedSize = allocatedSize;
    statistic.internalAllocatedSize = internalAllocatedSize;
#endif // MAGMA_DEBUG
    return statistic;
}
} // namespace magma
