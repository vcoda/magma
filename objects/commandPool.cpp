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
#include "../helpers/stackArray.h"

namespace magma
{
CommandPool::CommandPool(std::shared_ptr<const Device> device,
    VkCommandPoolCreateFlags flags /* VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT */):
    NonDispatchable(VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_POOL_EXT, device)
{
    VkCommandPoolCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = flags;
    info.queueFamilyIndex = 0;
    const VkResult create = vkCreateCommandPool(*device, &info, nullptr, &handle);
    MAGMA_THROW_FAILURE(create, "failed to create command pool");
}

CommandPool::~CommandPool()
{
    vkDestroyCommandPool(*device, handle, nullptr);
}

std::shared_ptr<CommandBuffer> CommandPool::allocateCommandBuffer(
    VkCommandBufferLevel level /* VK_COMMAND_BUFFER_LEVEL_PRIMARY */)
{
    VkCommandBufferAllocateInfo info;
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    info.pNext = nullptr;
    info.commandPool = handle;
    info.level = level;
    info.commandBufferCount = 1;
    VkCommandBuffer commandBuffer;
    const VkResult alloc = vkAllocateCommandBuffers(*device, &info, &commandBuffer);
    MAGMA_THROW_FAILURE(alloc, "failed to allocate command buffer");
    return std::shared_ptr<CommandBuffer>(new CommandBuffer(commandBuffer, device));
}

void CommandPool::freeCommandBuffer(std::shared_ptr<CommandBuffer>& commandBuffer)
{
    VkCommandBuffer dereferencedCommandBuffers[1] = {*commandBuffer};
    vkFreeCommandBuffers(*device, handle, 1, dereferencedCommandBuffers);
    commandBuffer.reset();
}

std::vector<std::shared_ptr<CommandBuffer>> CommandPool::allocateCommandBuffers(uint32_t count,
    VkCommandBufferLevel level /* VK_COMMAND_BUFFER_LEVEL_PRIMARY */)
{
    VkCommandBufferAllocateInfo info;
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    info.pNext = nullptr;
    info.commandPool = handle;
    info.level = level;
    info.commandBufferCount = count;
    MAGMA_STACK_ARRAY(VkCommandBuffer, nativeCommandBuffers, count);
    const VkResult alloc = vkAllocateCommandBuffers(*device, &info, nativeCommandBuffers.data());
    MAGMA_THROW_FAILURE(alloc, "failed to allocate command buffers");
    std::vector<std::shared_ptr<CommandBuffer>> commandBuffers;
    for (const auto& handle : nativeCommandBuffers)
        commandBuffers.push_back(std::shared_ptr<CommandBuffer>(new CommandBuffer(handle, device)));
    return commandBuffers;
}

void CommandPool::freeCommandBuffers(std::vector<std::shared_ptr<CommandBuffer>>& commandBuffers)
{
    MAGMA_STACK_ARRAY(VkCommandBuffer, dereferencedCommandBuffers, commandBuffers.size());
    for (const auto& buffer : commandBuffers)
        dereferencedCommandBuffers.put(*buffer);
    vkFreeCommandBuffers(*device, handle, dereferencedCommandBuffers.size(), dereferencedCommandBuffers.data());
    commandBuffers.clear();
}

bool CommandPool::reset(bool releaseResources) noexcept
{
    VkCommandPoolResetFlags flags = 0;
    if (releaseResources)
        flags |= VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT;
    const VkResult reset = vkResetCommandPool(*device, handle, flags);
    return (VK_SUCCESS == reset);
}
} // namespace magma
