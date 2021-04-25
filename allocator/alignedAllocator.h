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
        virtual void *alloc(std::size_t size,
            std::size_t alignment,
            VkSystemAllocationScope allocationScope) override;
        virtual void *realloc(void *original,
            std::size_t size,
            std::size_t alignment,
            VkSystemAllocationScope allocationScope) override;
        virtual void free(void *memory) noexcept override;
        virtual void internalAllocationNotification(std::size_t,
            VkInternalAllocationType, 
            VkSystemAllocationScope) noexcept override {}
        virtual void internalFreeNotification(std::size_t,
            VkInternalAllocationType, 
            VkSystemAllocationScope) noexcept override {}
    };
} // namespace magma
