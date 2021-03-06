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
#include "pch.h"
#pragma hdrstop
#include "commandPool.h"
#include "commandBuffer.h"
#include "device.h"
#include "../allocator/allocator.h"
#include "../allocator/linearPlacementPool.h"
#include "../misc/deviceExtension.h"
#include "../exceptions/errorResult.h"
#include "../helpers/stackArray.h"

namespace magma
{
CommandPool::CommandPool(std::shared_ptr<Device> device,
    uint32_t queueFamilyIndex,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    bool transient /* false */,
    bool resetCommandBuffer /* true */,
    uint32_t poolCommandBufferCount /* 256 */):
    NonDispatchable(VK_OBJECT_TYPE_COMMAND_POOL, std::move(device), std::move(allocator))
{
    VkCommandPoolCreateInfo poolInfo;
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.pNext = nullptr;
    poolInfo.flags = 0;
    if (transient)
        poolInfo.flags |= VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
    if (resetCommandBuffer)
        poolInfo.flags |= VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndex;
    const VkResult result = vkCreateCommandPool(MAGMA_HANDLE(device), &poolInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to create command pool");
    if (!getOverridenAllocator())
        pool = std::make_unique<memory::LinearPlacementPool>(sizeof(CommandBuffer), poolCommandBufferCount);
}

CommandPool::~CommandPool()
{
    vkDestroyCommandPool(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}

bool CommandPool::reset(bool releaseResources) noexcept
{
    VkCommandPoolResetFlags flags = 0;
    if (releaseResources)
        flags |= VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT;
    const VkResult reset = vkResetCommandPool(MAGMA_HANDLE(device), handle, flags);
    return (VK_SUCCESS == reset);
}

std::vector<std::shared_ptr<CommandBuffer>> CommandPool::allocateCommandBuffers(uint32_t commandBufferCount, bool primaryLevel)
{
    VkCommandBufferAllocateInfo info;
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    info.pNext = nullptr;
    info.commandPool = handle;
    info.level = primaryLevel ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY;
    info.commandBufferCount = commandBufferCount;
    MAGMA_STACK_ARRAY(VkCommandBuffer, cmdBufferHandles, commandBufferCount);
    const VkResult alloc = vkAllocateCommandBuffers(MAGMA_HANDLE(device), &info, cmdBufferHandles);
    MAGMA_THROW_FAILURE(alloc, "failed to allocate command buffers");
    std::vector<std::shared_ptr<CommandBuffer>> commandBuffers;
    for (const VkCommandBuffer handle : cmdBufferHandles)
    {
        CommandBuffer *commandBuffer = nullptr;
        if (pool)
        {   // Optimize command buffer object allocation using placement new
            if (primaryLevel)
                commandBuffer = pool->placementNew<PrimaryCommandBuffer>(handle, shared_from_this());
            else
                commandBuffer = pool->placementNew<SecondaryCommandBuffer>(handle, shared_from_this());
            if (commandBuffer)
                commandBuffers.emplace_back(commandBuffer, [](CommandBuffer *commandBuffer) {
                    // Release resource ownership, but don't delete in memory because of placement new
                    commandBuffer->~CommandBuffer();
                });
        }
        if (!commandBuffer)
        {
            if (primaryLevel)
                commandBuffer = new PrimaryCommandBuffer(handle, shared_from_this());
            else
                commandBuffer = new SecondaryCommandBuffer(handle, shared_from_this());
            commandBuffers.emplace_back(commandBuffer);
        }
    }
    return commandBuffers;
}

void CommandPool::freeCommandBuffers(std::vector<std::shared_ptr<CommandBuffer>>& commandBuffers) noexcept
{
    MAGMA_STACK_ARRAY(VkCommandBuffer, dereferencedCommandBuffers, commandBuffers.size());
    for (const auto& buffer : commandBuffers)
        dereferencedCommandBuffers.put(*buffer);
    vkFreeCommandBuffers(MAGMA_HANDLE(device), handle, dereferencedCommandBuffers.size(), dereferencedCommandBuffers);
    for (auto& buffer : commandBuffers)
        buffer->handle = VK_NULL_HANDLE; // Do not call vkFreeCommandBuffers() in destructor
    commandBuffers.clear();
}

#ifdef VK_KHR_maintenance1
void CommandPool::trim(VkCommandPoolTrimFlagsKHR flags /* 0 */)
{
    MAGMA_DEVICE_EXTENSION(vkTrimCommandPoolKHR, VK_KHR_MAINTENANCE1_EXTENSION_NAME);
    vkTrimCommandPoolKHR(MAGMA_HANDLE(device), handle, flags);
}
#endif // VK_KHR_maintenance1
} // namespace magma
