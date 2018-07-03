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
#include "commandPool.h"
#include "commandBuffer.h"
#include "device.h"
#include "../allocator/allocator.h"
#include "../helpers/stackArray.h"
#include "../misc/exception.h"

namespace magma
{
CommandPool::CommandPool(std::shared_ptr<const Device> device,
    uint32_t queueFamilyIndex,
    bool transient /* false */, 
    bool reset /* true */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    NonDispatchable(VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_POOL_EXT, std::move(device), std::move(allocator))
{
    VkCommandPoolCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    if (transient)
        info.flags |= VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
    if (reset)
        info.flags |= VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    info.queueFamilyIndex = queueFamilyIndex;
    const VkResult create = vkCreateCommandPool(MAGMA_HANDLE(device), &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create command pool");
}

CommandPool::~CommandPool()
{
    vkDestroyCommandPool(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(allocator));
}

bool CommandPool::reset(bool releaseResources) noexcept
{
    VkCommandPoolResetFlags flags = 0;
    if (releaseResources)
        flags |= VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT;
    const VkResult reset = vkResetCommandPool(MAGMA_HANDLE(device), handle, flags);
    return (VK_SUCCESS == reset);
}

std::shared_ptr<CommandBuffer> CommandPool::allocateCommandBuffer(bool primaryLevel)
{
    VkCommandBufferAllocateInfo info;
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    info.pNext = nullptr;
    info.commandPool = handle;
    info.level = primaryLevel ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY;
    info.commandBufferCount = 1;
    VkCommandBuffer commandBuffer;
    const VkResult alloc = vkAllocateCommandBuffers(MAGMA_HANDLE(device), &info, &commandBuffer);
    MAGMA_THROW_FAILURE(alloc, "failed to allocate command buffer");
    return std::shared_ptr<CommandBuffer>(new CommandBuffer(commandBuffer, device, shared_from_this()));
}

void CommandPool::freeCommandBuffer(std::shared_ptr<CommandBuffer>& commandBuffer) noexcept
{
    VkCommandBuffer dereferencedCommandBuffers[1] = {*commandBuffer};
    vkFreeCommandBuffers(MAGMA_HANDLE(device), handle, 1, dereferencedCommandBuffers);
    commandBuffer.reset();
}

std::vector<std::shared_ptr<CommandBuffer>> CommandPool::allocateCommandBuffers(uint32_t count, bool primaryLevel)
{
    VkCommandBufferAllocateInfo info;
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    info.pNext = nullptr;
    info.commandPool = handle;
    info.level = primaryLevel ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY;
    info.commandBufferCount = count;
    MAGMA_STACK_ARRAY(VkCommandBuffer, nativeCommandBuffers, count);
    const VkResult alloc = vkAllocateCommandBuffers(MAGMA_HANDLE(device), &info, nativeCommandBuffers);
    MAGMA_THROW_FAILURE(alloc, "failed to allocate command buffers");
    std::vector<std::shared_ptr<CommandBuffer>> commandBuffers;
    for (const VkCommandBuffer cmdBuffer : nativeCommandBuffers)
        commandBuffers.emplace_back(new CommandBuffer(cmdBuffer, device, shared_from_this()));
    return commandBuffers;
}

void CommandPool::freeCommandBuffers(std::vector<std::shared_ptr<CommandBuffer>>& commandBuffers) noexcept
{
    MAGMA_STACK_ARRAY(VkCommandBuffer, dereferencedCommandBuffers, commandBuffers.size());
    for (const auto& buffer : commandBuffers)
        dereferencedCommandBuffers.put(*buffer);
    vkFreeCommandBuffers(MAGMA_HANDLE(device), handle, dereferencedCommandBuffers.size(), dereferencedCommandBuffers);
    commandBuffers.clear();
}
} // namespace magma
