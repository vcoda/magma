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
            uint32_t queueFamilyIndex,
            bool transient = false, bool reset = true);
        ~CommandPool();
        std::shared_ptr<CommandBuffer> allocateCommandBuffer(bool primaryLevel);
        void freeCommandBuffer(std::shared_ptr<CommandBuffer>& commandBuffer);
        std::vector<std::shared_ptr<CommandBuffer>> allocateCommandBuffers(uint32_t count,
            bool primaryLevel);
        void freeCommandBuffers(std::vector<std::shared_ptr<CommandBuffer>>& commandBuffers);
        bool reset(bool releaseResources) noexcept;
    };
} // namespace magma
