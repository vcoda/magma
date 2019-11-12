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
#include "nondispatchable.h"

namespace magma
{
    class CommandBuffer;

    /* Command pools are opaque objects that command buffer memory is allocated from,
       and which allow the implementation to amortize the cost of resource creation
       across multiple command buffers. Command pools are externally synchronized,
       meaning that a command pool must not be used concurrently in multiple threads.
       That includes use via recording commands on any command buffers allocated from the pool,
       as well as operations that allocate, free, and reset command buffers or the pool itself. */

    class CommandPool : public NonDispatchable<VkCommandPool>,
        public std::enable_shared_from_this<CommandPool>
    {
    public:
        explicit CommandPool(std::shared_ptr<Device> device,
            uint32_t queueFamilyIndex,
            bool transient = false,
            bool resetCommandBuffer = true,
            std::shared_ptr<IAllocator> allocator = nullptr);
        ~CommandPool();
        bool reset(bool releaseResources) noexcept;
        std::vector<std::shared_ptr<CommandBuffer>> allocateCommandBuffers(uint32_t count,
            bool primaryLevel);
        void freeCommandBuffers(std::vector<std::shared_ptr<CommandBuffer>>& commandBuffers) noexcept;
        void trim(VkCommandPoolTrimFlags flags = 0);
    };
} // namespace magma
