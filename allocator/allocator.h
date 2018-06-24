/*
Magma - C++1x interface over Khronos Vulkan API.
Copyright (C) 2018 Victor Coda.

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
#include "../vulkan.h"
#include "../nonCopyable.h"

namespace magma
{
    struct AllocationCallbacks : VkAllocationCallbacks
    {
        AllocationCallbacks() noexcept;
    };

    class IAllocator : public AllocationCallbacks, public NonCopyable
    {
    public:
        virtual void *alloc(size_t size,
            size_t alignment,
            VkSystemAllocationScope allocationScope) = 0;
        virtual void *realloc(void *original,
            size_t size,
            size_t alignment,
            VkSystemAllocationScope allocationScope) = 0;
        virtual void free(void *memory) = 0;
        virtual void internalAllocationNotification(size_t size,
            VkInternalAllocationType allocationType,
            VkSystemAllocationScope allocationScope) = 0;
        virtual void internalFreeNotification(size_t size,
            VkInternalAllocationType allocationType,
            VkSystemAllocationScope allocationScope) = 0;
    };
} // namespace magma
