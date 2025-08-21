/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2025 Victor Coda.

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
#include "commandBuffer.h"
#include "../allocator/allocator.h"
#include "../exceptions/errorResult.h"

namespace magma
{
Fence::Fence(std::shared_ptr<IAllocator> allocator, std::shared_ptr<Device> device) noexcept:
    NonDispatchable(VK_OBJECT_TYPE_FENCE, std::move(device), std::move(allocator))
{}

Fence::Fence(std::shared_ptr<Device> device,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkFenceCreateFlags flags /* 0 */,
    const StructureChain& extendedInfo /* default */):
    NonDispatchable(VK_OBJECT_TYPE_FENCE, std::move(device), std::move(allocator))
{
    VkFenceCreateInfo fenceInfo;
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.pNext = extendedInfo.headNode();
    fenceInfo.flags = flags;
    const VkResult result = vkCreateFence(getNativeDevice(), &fenceInfo, MAGMA_OPTIONAL(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create fence");
}

Fence::~Fence()
{
    vkDestroyFence(getNativeDevice(), handle, MAGMA_OPTIONAL(hostAllocator));
}

bool Fence::reset() noexcept
{
    const VkResult reset = vkResetFences(getNativeDevice(), 1, &handle);
    MAGMA_ASSERT(VK_SUCCESS == reset);
    return (VK_SUCCESS == reset);
}

Fence::State Fence::getStatus() const noexcept
{
    const VkResult result = vkGetFenceStatus(getNativeDevice(), handle);
    return (VK_SUCCESS == result) ? State::Signaled : State::Unsignaled;
}

bool Fence::wait(uint64_t timeout /* std::numeric_limits<uint64_t>::max() */)
{
    constexpr VkBool32 waitAll = VK_TRUE;
    const VkResult result = vkWaitForFences(getNativeDevice(), 1, &handle, waitAll, timeout);
    MAGMA_HANDLE_RESULT(result, "failed to wait fence");
    if (VK_SUCCESS == result)
    {   // Fence to be signaled once all submitted command buffers have completed execution
        for (auto cmdBuffer: submittedCmdBuffers)
            cmdBuffer->executionFinished();
        submittedCmdBuffers.clear();
    }
    return (result != VK_TIMEOUT); // VK_SUCCESS or VK_TIMEOUT
}

void Fence::completeExecutionOnSignaled(lent_ptr<CommandBuffer> cmdBuffer)
{
    MAGMA_ASSERT(cmdBuffer);
    submittedCmdBuffers.insert(cmdBuffer.get());
}
} // namespace magma
