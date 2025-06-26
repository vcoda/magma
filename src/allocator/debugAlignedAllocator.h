/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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
#include "allocator.h"

namespace magma
{
    /* Debug allocator for system memory that tracks allocation statistics. */

    class DebugAlignedAllocator : public IAllocator
    {
    public:
        DebugAlignedAllocator() noexcept;
        ~DebugAlignedAllocator();
        void *alloc(std::size_t size,
            std::size_t alignment,
            VkSystemAllocationScope allocationScope) override;
        void *realloc(void *original,
            std::size_t size,
            std::size_t alignment,
            VkSystemAllocationScope allocationScope) override;
        void free(void *ptr) noexcept override;
        void internalAllocationNotification(std::size_t,
            VkInternalAllocationType allocationType,
            VkSystemAllocationScope allocationScope) noexcept override;
        void internalFreeNotification(std::size_t size,
            VkInternalAllocationType allocationType,
            VkSystemAllocationScope allocationScope) noexcept override;
        AllocationStatistics getAllocationStatistics() const noexcept override;

    private:
        std::unordered_map<void*, std::pair<std::size_t, VkSystemAllocationScope>> allocations;
        std::array<uint32_t, VK_SYSTEM_ALLOCATION_SCOPE_INSTANCE + 1>
            numAllocations, numReallocations, numFrees,
            numInternalAllocations, numInternalFrees;
        std::size_t allocatedMemorySize;
        std::size_t internalAllocatedMemorySize;
        mutable std::mutex mtx;
    };
} // namespace magma
