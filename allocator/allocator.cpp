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
#include "allocator.h"

namespace magma
{
namespace 
{
void *VKAPI_PTR vkAllocationFunction(
    void *pUserData,
    size_t size,
    size_t alignment,
    VkSystemAllocationScope allocationScope)
{
    AllocationCallbacks *base = reinterpret_cast<AllocationCallbacks *>(pUserData);
    IAllocator *allocator = static_cast<IAllocator *>(base);
    return allocator->alloc(size, alignment, allocationScope);
}

void *VKAPI_PTR vkReallocationFunction(
    void *pUserData,
    void *pOriginal,
    size_t size,
    size_t alignment,
    VkSystemAllocationScope allocationScope)
{
    AllocationCallbacks *base = reinterpret_cast<AllocationCallbacks *>(pUserData);
    IAllocator *allocator = static_cast<IAllocator *>(base);
    return allocator->realloc(pOriginal, size, alignment, allocationScope);
}

void VKAPI_PTR vkFreeFunction(
    void *pUserData,
    void *pMemory)
{
    AllocationCallbacks *base = reinterpret_cast<AllocationCallbacks *>(pUserData);
    IAllocator *allocator = static_cast<IAllocator *>(base);
    return allocator->free(pMemory);
}

void VKAPI_PTR vkInternalAllocationNotification(
    void *pUserData,
    size_t size,
    VkInternalAllocationType allocationType,
    VkSystemAllocationScope allocationScope)
{
    AllocationCallbacks *base = reinterpret_cast<AllocationCallbacks *>(pUserData);
    IAllocator *allocator = static_cast<IAllocator *>(base);
    allocator->internalAllocationNotification(size, allocationType, allocationScope);
}

void VKAPI_PTR vkInternalFreeNotification(
    void *pUserData,
    size_t size,
    VkInternalAllocationType allocationType,
    VkSystemAllocationScope allocationScope)
{
    AllocationCallbacks *base = reinterpret_cast<AllocationCallbacks *>(pUserData);
    IAllocator *allocator = static_cast<IAllocator *>(base);
    allocator->internalFreeNotification(size, allocationType, allocationScope);
}
} // namespace

AllocationCallbacks::AllocationCallbacks()
{
    pUserData = this;
    pfnAllocation = vkAllocationFunction;
    pfnReallocation = vkReallocationFunction;
    pfnFree = vkFreeFunction;
    pfnInternalAllocation = vkInternalAllocationNotification;
    pfnInternalFree = vkInternalFreeNotification;
}
} // namespace magma
