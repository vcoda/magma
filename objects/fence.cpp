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
#include "fence.h"
#include "device.h"
#include "../allocator/allocator.h"
#include "../misc/exception.h"
#include "../shared.h"

namespace magma
{
Fence::Fence(std::shared_ptr<const Device> device, 
    bool signaled /* false */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    NonDispatchable(VK_DEBUG_REPORT_OBJECT_TYPE_FENCE_EXT, std::move(device), std::move(allocator))
{
    VkFenceCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    if (signaled)
        info.flags |= VK_FENCE_CREATE_SIGNALED_BIT;
    const VkResult create = vkCreateFence(MAGMA_HANDLE(device), &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create fence");
}

Fence::~Fence()
{
    vkDestroyFence(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(allocator));
}

bool Fence::reset() noexcept
{
    const VkResult reset = vkResetFences(MAGMA_HANDLE(device), 1, &handle);
    return (VK_SUCCESS == reset);
}

VkResult Fence::getStatus() const noexcept
{
    return vkGetFenceStatus(MAGMA_HANDLE(device), handle);
}

bool Fence::wait(uint64_t timeout /* UINT64_MAX */) const noexcept
{
    const VkResult wait = vkWaitForFences(MAGMA_HANDLE(device), 1, &handle, VK_TRUE, timeout);
    return (VK_SUCCESS == wait) || (VK_TIMEOUT == wait);
}
} // namespace magma
