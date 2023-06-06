/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

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
#include "debugAlignedAllocator.h"

namespace magma
{
DebugAlignedAllocator::DebugAlignedAllocator():
    allocatedMemorySize(0ull),
    internalAllocatedMemorySize(0ull)
{
    numAllocations = {};
    numReallocations = {};
    numFrees = {};
    numInternalAllocations = {};
    numInternalFrees = {};
}

DebugAlignedAllocator::~DebugAlignedAllocator()
{
    MAGMA_ASSERT(!allocatedMemorySize);
    MAGMA_ASSERT(!internalAllocatedMemorySize);
    MAGMA_ASSERT(allocations.empty());
    for (uint32_t scope = VK_SYSTEM_ALLOCATION_SCOPE_COMMAND;
        scope <= VK_SYSTEM_ALLOCATION_SCOPE_INSTANCE; ++scope)
    {   // TODO: write to log?
        MAGMA_ASSERT(numAllocations[scope] == numFrees[scope]);
    }
}

void *DebugAlignedAllocator::alloc(std::size_t size,
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
#endif // _MSC_VER
    if (!ptr)
        throw std::bad_alloc();
    std::lock_guard<std::mutex> lock(mtx);
    // Add allocation
    allocations[ptr] = {size, allocationScope};
    allocatedMemorySize += size;
    ++numAllocations[allocationScope];
    return ptr;
}

void *DebugAlignedAllocator::realloc(void *original, std::size_t size, std::size_t alignment,
    VkSystemAllocationScope allocationScope)
{
#ifdef _MSC_VER
    void *ptr = _aligned_realloc(original, size, alignment);
#else
    MAGMA_UNUSED(alignment);
    void *ptr = ::realloc(original, size);
    // TODO: check alignment, use posix_memalign/memcpy if not aligned!
#endif // _MSC_VER
    if (!ptr)
        throw std::bad_alloc();
    std::lock_guard<std::mutex> lock(mtx);
    // Replace old allocation with a new one
    allocatedMemorySize -= allocations.at(original).first;
    allocations.erase(original);
    allocations[ptr] = {size, allocationScope};
    allocatedMemorySize += size;
    ++numReallocations[allocationScope];
    return ptr;
}

void DebugAlignedAllocator::free(void *ptr) noexcept
{
    if (ptr)
    {
    #ifdef _MSC_VER
        _aligned_free(ptr);
    #else
        ::free(ptr);
    #endif
        std::lock_guard<std::mutex> lock(mtx);
        // Delete allocation
        auto entry = allocations.at(ptr);
        allocations.erase(ptr);
        allocatedMemorySize -= entry.first;
        ++numFrees[entry.second];
    }
}

void DebugAlignedAllocator::internalAllocationNotification(std::size_t size,
    VkInternalAllocationType /* allocationType */,
    VkSystemAllocationScope allocationScope) noexcept
{
    std::lock_guard<std::mutex> lock(mtx);
    internalAllocatedMemorySize += size;
    ++numInternalAllocations[allocationScope];
}

void DebugAlignedAllocator::internalFreeNotification(std::size_t size,
    VkInternalAllocationType /* allocationType */,
    VkSystemAllocationScope allocationScope) noexcept
{
    std::lock_guard<std::mutex> lock(mtx);
    internalAllocatedMemorySize -= size;
    ++numInternalFrees[allocationScope];
}

AllocationStatistics DebugAlignedAllocator::getAllocationStatistics() const noexcept
{
    std::lock_guard<std::mutex> lock(mtx);
    AllocationStatistics statistics;
    statistics.allocatedMemorySize = allocatedMemorySize;
    statistics.internalAllocatedMemorySize = internalAllocatedMemorySize;
    statistics.allocations.aliveCommandAllocations = numAllocations[VK_SYSTEM_ALLOCATION_SCOPE_COMMAND] - numFrees[VK_SYSTEM_ALLOCATION_SCOPE_COMMAND];
    statistics.allocations.aliveObjectAllocations = numAllocations[VK_SYSTEM_ALLOCATION_SCOPE_OBJECT] - numFrees[VK_SYSTEM_ALLOCATION_SCOPE_OBJECT];
    statistics.allocations.aliveCacheAllocations = numAllocations[VK_SYSTEM_ALLOCATION_SCOPE_CACHE] - numFrees[VK_SYSTEM_ALLOCATION_SCOPE_CACHE];
    statistics.allocations.aliveDeviceAllocations = numAllocations[VK_SYSTEM_ALLOCATION_SCOPE_DEVICE] - numFrees[VK_SYSTEM_ALLOCATION_SCOPE_DEVICE];
    statistics.allocations.aliveInstanceAllocations = numAllocations[VK_SYSTEM_ALLOCATION_SCOPE_INSTANCE] - numFrees[VK_SYSTEM_ALLOCATION_SCOPE_INSTANCE];
    statistics.internalAllocations.aliveCommandAllocations = numInternalAllocations[VK_SYSTEM_ALLOCATION_SCOPE_COMMAND] - numInternalFrees[VK_SYSTEM_ALLOCATION_SCOPE_COMMAND];
    statistics.internalAllocations.aliveObjectAllocations = numInternalAllocations[VK_SYSTEM_ALLOCATION_SCOPE_OBJECT] - numInternalFrees[VK_SYSTEM_ALLOCATION_SCOPE_OBJECT];
    statistics.internalAllocations.aliveCacheAllocations = numInternalAllocations[VK_SYSTEM_ALLOCATION_SCOPE_CACHE] - numInternalFrees[VK_SYSTEM_ALLOCATION_SCOPE_CACHE];
    statistics.internalAllocations.aliveDeviceAllocations = numInternalAllocations[VK_SYSTEM_ALLOCATION_SCOPE_DEVICE] - numInternalFrees[VK_SYSTEM_ALLOCATION_SCOPE_DEVICE];
    statistics.internalAllocations.aliveInstanceAllocations = numInternalAllocations[VK_SYSTEM_ALLOCATION_SCOPE_INSTANCE] - numInternalFrees[VK_SYSTEM_ALLOCATION_SCOPE_INSTANCE];
    return statistics;
}
} // namespace magma
