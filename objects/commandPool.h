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
#pragma once
#include <vector>
#include "handle.h"

namespace magma
{
    class Device;
    class CommandBuffer;

    class CommandPool : public NonDispatchable<VkCommandPool>
    {
    public:
        CommandPool(std::shared_ptr<const Device> device,
            VkCommandPoolCreateFlags flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
        ~CommandPool();
        std::shared_ptr<CommandBuffer> allocateCommandBuffer(VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
        void freeCommandBuffer(std::shared_ptr<CommandBuffer> commandBuffer);
        std::vector<std::shared_ptr<CommandBuffer>> allocateCommandBuffers(uint32_t count,
            VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
        void freeCommandBuffers(std::vector<std::shared_ptr<CommandBuffer>>& commandBuffers);
        bool reset(bool releaseResources) noexcept;
    };
} // namespace magma
