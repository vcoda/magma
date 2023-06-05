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
#pragma once
#include "allocator.h"

namespace magma
{
    /* Trivial aligned allocator for system memory. */

    class AlignedAllocator : public IAllocator
    {
    public:
        AlignedAllocator();
        ~AlignedAllocator();
        virtual void *alloc(std::size_t size,
            std::size_t alignment,
            VkSystemAllocationScope allocationScope) override;
        virtual void *realloc(void *original,
            std::size_t size,
            std::size_t alignment,
            VkSystemAllocationScope allocationScope) override;
        virtual void free(void *ptr) noexcept override;
        virtual void internalAllocationNotification(std::size_t,
            VkInternalAllocationType allocationType,
            VkSystemAllocationScope allocationScope) noexcept override;
        virtual void internalFreeNotification(std::size_t size,
            VkInternalAllocationType allocationType,
            VkSystemAllocationScope allocationScope) noexcept override;
        virtual AllocationStatistic getAllocationStatistic() const noexcept override;

    private:
    #ifdef MAGMA_DEBUG
        std::unordered_map<void*, std::pair<std::size_t, VkSystemAllocationScope>> allocations;
        std::array<uint32_t, VK_SYSTEM_ALLOCATION_SCOPE_INSTANCE + 1>
            numAllocations, numReallocations, numFrees;
        std::size_t allocatedSize;
        std::size_t internalAllocatedSize;
        mutable std::mutex mtx;
    #endif // MAGMA_DEBUG
    };
} // namespace magma
