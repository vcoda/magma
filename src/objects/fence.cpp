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
#include "fence.h"
#include "device.h"
#include "../allocator/allocator.h"
#include "../exceptions/errorResult.h"

namespace magma
{
Fence::Fence(std::shared_ptr<Device> device,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    bool signaled /* false */,
    const StructureChain& extendedInfo /* default */):
    NonDispatchable(VK_OBJECT_TYPE_FENCE, std::move(device), std::move(allocator))
{
    VkFenceCreateInfo fenceInfo;
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.pNext = extendedInfo.chainNodes();
    fenceInfo.flags = 0;
    if (signaled)
        fenceInfo.flags |= VK_FENCE_CREATE_SIGNALED_BIT;
    const VkResult result = vkCreateFence(MAGMA_HANDLE(device), &fenceInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create fence");
}

Fence::~Fence()
{
    vkDestroyFence(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}

bool Fence::reset() noexcept
{
    const VkResult reset = vkResetFences(MAGMA_HANDLE(device), 1, &handle);
    MAGMA_ASSERT(VK_SUCCESS == reset);
    return (VK_SUCCESS == reset);
}

VkResult Fence::getStatus() const noexcept
{
    return vkGetFenceStatus(MAGMA_HANDLE(device), handle);
}

bool Fence::wait(uint64_t timeout /* std::numeric_limits<uint64_t>::max() */) const
{
    constexpr VkBool32 waitAll = VK_TRUE;
    const VkResult result = vkWaitForFences(MAGMA_HANDLE(device), 1, &handle, waitAll, timeout);
    MAGMA_HANDLE_RESULT(result, "failed to wait fence");
    // VK_SUCCESS or VK_TIMEOUT
    return (result != VK_TIMEOUT);
}
} // namespace magma
