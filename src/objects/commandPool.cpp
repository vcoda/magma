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
#include "commandPool.h"
#include "primaryCommandBuffer.h"
#include "secondaryCommandBuffer.h"
#include "../allocator/allocator.h"
#include "../misc/extension.h"
#include "../exceptions/errorResult.h"

namespace magma
{
CommandPool::CommandPool(std::shared_ptr<Device> device, uint32_t queueFamilyIndex,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    bool transient /* false */,
    bool resetCommandBuffer /* true */,
    const StructureChain& extendedInfo /* default */):
    NonDispatchable(VK_OBJECT_TYPE_COMMAND_POOL, std::move(device), std::move(allocator)),
    queueFamilyIndex(queueFamilyIndex)
{
    VkCommandPoolCreateInfo cmdPoolInfo;
    cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmdPoolInfo.pNext = extendedInfo.chainNodes();
    cmdPoolInfo.flags = 0;
    if (transient)
        cmdPoolInfo.flags |= VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
    if (resetCommandBuffer)
        cmdPoolInfo.flags |= VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    cmdPoolInfo.queueFamilyIndex = queueFamilyIndex;
    const VkResult result = vkCreateCommandPool(MAGMA_HANDLE(device), &cmdPoolInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create command pool");
}

CommandPool::~CommandPool()
{
    vkDestroyCommandPool(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}

bool CommandPool::reset(bool releaseResources /* false */) noexcept
{
    VkCommandPoolResetFlags flags = 0;
    if (releaseResources)
        flags |= VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT;
    const VkResult result = vkResetCommandPool(MAGMA_HANDLE(device), handle, flags);
    MAGMA_ASSERT(VK_SUCCESS == result);
    return (VK_SUCCESS == result);
}

std::vector<std::shared_ptr<CommandBuffer>> CommandPool::allocateCommandBuffers(uint32_t commandBufferCount, bool primaryLevel,
    const StructureChain& extendedInfo /* default */)
{
    VkCommandBufferAllocateInfo cmdBufferAllocateInfo;
    cmdBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmdBufferAllocateInfo.pNext = extendedInfo.chainNodes();
    cmdBufferAllocateInfo.commandPool = handle;
    cmdBufferAllocateInfo.level = primaryLevel ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY;
    cmdBufferAllocateInfo.commandBufferCount = commandBufferCount;
    MAGMA_STACK_ARRAY(VkCommandBuffer, commandBuffers, commandBufferCount);
    const VkResult result = vkAllocateCommandBuffers(MAGMA_HANDLE(device), &cmdBufferAllocateInfo, commandBuffers);
    MAGMA_HANDLE_RESULT(result, "failed to allocate command buffers");
    std::vector<std::shared_ptr<CommandBuffer>> cmdBuffers;
    for (VkCommandBuffer handle: commandBuffers)
    {
        CommandBuffer *cmdBuffer;
        if (primaryLevel)
            cmdBuffer = new PrimaryCommandBuffer(handle, shared_from_this());
        else
            cmdBuffer = new SecondaryCommandBuffer(handle, shared_from_this());
        cmdBuffers.emplace_back(cmdBuffer);
    }
    return cmdBuffers;
}

void CommandPool::freeCommandBuffers(std::vector<std::shared_ptr<CommandBuffer>>& cmdBuffers) noexcept
{
    MAGMA_STACK_ARRAY(VkCommandBuffer, commandBuffers, cmdBuffers.size());
    for (auto& cmdBuffer: cmdBuffers)
    {
        commandBuffers.put(*cmdBuffer);
        cmdBuffer->handle = VK_NULL_HANDLE; // Don't call vkFreeCommandBuffers() in destructor
    }
    vkFreeCommandBuffers(MAGMA_HANDLE(device), handle, commandBuffers.size(), commandBuffers);
    cmdBuffers.clear();
}

#ifdef VK_KHR_maintenance1
void CommandPool::trim(VkCommandPoolTrimFlagsKHR flags /* 0 */)
{
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkTrimCommandPoolKHR, VK_KHR_MAINTENANCE1_EXTENSION_NAME);
    vkTrimCommandPoolKHR(MAGMA_HANDLE(device), handle, flags);
}
#endif // VK_KHR_maintenance1
} // namespace magma
