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
#pragma once
#include "../core/noncopyable.h"

namespace magma
{
    struct AllocationCallbacks : VkAllocationCallbacks
    {
        AllocationCallbacks() noexcept;
    };

    /* Vulkan provides applications the opportunity to perform host memory allocations
       on behalf of the Vulkan implementation. If this feature is not used,
       the implementation will perform its own memory allocations. Since most memory
       allocations are off the critical path, this is not meant as a performance feature.
       Rather, this can be useful for certain embedded systems, for debugging purposes
       (e.g. putting a guard page after all host allocations), or for memory allocation logging. */

    class IAllocator : public AllocationCallbacks, public core::NonCopyable
    {
    public:
        virtual void *alloc(std::size_t size,
            std::size_t alignment,
            VkSystemAllocationScope allocationScope) = 0;
        virtual void *realloc(void *original,
            std::size_t size,
            std::size_t alignment,
            VkSystemAllocationScope allocationScope) = 0;
        virtual void free(void *memory) = 0;
        virtual void internalAllocationNotification(std::size_t size,
            VkInternalAllocationType allocationType,
            VkSystemAllocationScope allocationScope) = 0;
        virtual void internalFreeNotification(std::size_t size,
            VkInternalAllocationType allocationType,
            VkSystemAllocationScope allocationScope) = 0;
    };
} // namespace magma
