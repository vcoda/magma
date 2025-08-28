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
#include "commandPool.h"
#include "commandBuffer.h"
#include "../allocator/allocator.h"
#include "../misc/extension.h"
#include "../exceptions/errorResult.h"

namespace magma
{
CommandPool::CommandPool(std::shared_ptr<Device> device, uint32_t queueFamilyIndex,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkCommandPoolCreateFlags flags /* VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT */,
    const StructureChain& extendedInfo /* default */):
    NonDispatchable(VK_OBJECT_TYPE_COMMAND_POOL, std::move(device), std::move(allocator)),
    queueFamilyIndex(queueFamilyIndex),
    flags(flags)
{
    VkCommandPoolCreateInfo cmdPoolInfo;
    cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmdPoolInfo.pNext = extendedInfo.headNode();
    cmdPoolInfo.flags = flags;
    cmdPoolInfo.queueFamilyIndex = queueFamilyIndex;
    const VkResult result = vkCreateCommandPool(getNativeDevice(), &cmdPoolInfo, MAGMA_OPTIONAL(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create command pool");
}

CommandPool::~CommandPool()
{
    auto dereferencedCmdBuffers = stackalloc(VkCommandBuffer, cmdPool.size());
    uint32_t commandBufferCount = 0;
    for (auto& weakRef: cmdPool)
    {
        if (!weakRef.expired())
        {
            std::shared_ptr<CommandBuffer> cmdBuffer = weakRef.lock();
            MAGMA_ASSERT(cmdBuffer->getState() != CommandBuffer::State::Pending);
            dereferencedCmdBuffers[commandBufferCount++] = cmdBuffer->getLean().getHandle();
            cmdBuffer->releaseObjectsInUse();
            cmdBuffer->getLean().handle = VK_NULL_HANDLE; // Don't call vkFreeCommandBuffers() in the destructor
        }
    }
    if (commandBufferCount)
        vkFreeCommandBuffers(getNativeDevice(), handle, commandBufferCount, dereferencedCmdBuffers);
    vkDestroyCommandPool(getNativeDevice(), handle, MAGMA_OPTIONAL(hostAllocator));
}

bool CommandPool::reset(bool releaseResources /* false */) noexcept
{
    VkCommandPoolResetFlags flags = 0;
    if (releaseResources)
        flags |= VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT;
    const VkResult result = vkResetCommandPool(getNativeDevice(), handle, flags);
    MAGMA_ASSERT(VK_SUCCESS == result);
    if (result == VK_SUCCESS)
    {   /* Resetting a command pool recycles all of the resources from all
           of the command buffers allocated from the command pool back to
           the command pool. All command buffers that have been allocated
           from the command pool are put in the initial state. */
        for (auto it = cmdPool.begin(); it != cmdPool.end(); )
        {
            if (it->expired())
                it = cmdPool.erase(it);
            else
            {
                std::shared_ptr<CommandBuffer> cmdBuffer = it->lock();
                cmdBuffer->putInitialState();
                cmdBuffer->releaseObjectsInUse();
                ++it;
            }
        }
    }
    return (VK_SUCCESS == result);
}

std::shared_ptr<CommandBuffer> CommandPool::allocateCommandBuffer(VkCommandBufferLevel level,
    const StructureChain& extendedInfo /* default */)
{
    VkCommandBufferAllocateInfo cmdBufferAllocateInfo;
    cmdBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmdBufferAllocateInfo.pNext = extendedInfo.headNode();
    cmdBufferAllocateInfo.commandPool = handle;
    cmdBufferAllocateInfo.level = level;
    cmdBufferAllocateInfo.commandBufferCount = 1;
    VkCommandBuffer commandBuffer;
    const VkResult result = vkAllocateCommandBuffers(getNativeDevice(), &cmdBufferAllocateInfo, &commandBuffer);
    MAGMA_HANDLE_RESULT(result, "failed to allocate command buffer");
    std::shared_ptr<CommandBuffer> cmdBuffer = CommandBuffer::makeShared(level, commandBuffer, this);
    cmdPool.insert(cmdBuffer);
    return cmdBuffer;
}

std::vector<std::shared_ptr<CommandBuffer>> CommandPool::allocateCommandBuffers(VkCommandBufferLevel level, uint32_t commandBufferCount,
    const StructureChain& extendedInfo /* default */)
{
    VkCommandBufferAllocateInfo cmdBufferAllocateInfo;
    cmdBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmdBufferAllocateInfo.pNext = extendedInfo.headNode();
    cmdBufferAllocateInfo.commandPool = handle;
    cmdBufferAllocateInfo.level = level;
    cmdBufferAllocateInfo.commandBufferCount = commandBufferCount;
    auto commandBuffers = stackalloc(VkCommandBuffer, commandBufferCount);
    const VkResult result = vkAllocateCommandBuffers(getNativeDevice(), &cmdBufferAllocateInfo, commandBuffers);
    MAGMA_HANDLE_RESULT(result, "failed to allocate command buffers");
    std::vector<std::shared_ptr<CommandBuffer>> cmdBuffers;
    for (uint32_t i = 0; i < commandBufferCount; ++i)
    {
        std::shared_ptr<CommandBuffer> cmdBuffer = CommandBuffer::makeShared(level, commandBuffers[i], this);
        cmdBuffers.emplace_back(std::move(cmdBuffer));
        cmdPool.insert(cmdBuffers.back());
    }
    return cmdBuffers;
}

#ifdef VK_KHR_maintenance1
void CommandPool::trim(VkCommandPoolTrimFlagsKHR flags /* 0 */)
{
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkTrimCommandPoolKHR, VK_KHR_MAINTENANCE1_EXTENSION_NAME);
    vkTrimCommandPoolKHR(getNativeDevice(), handle, flags);
}
#endif // VK_KHR_maintenance1
} // namespace magma
