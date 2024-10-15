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
#pragma once
#include "model/nondispatchable.h"

namespace magma
{
    class CommandBuffer;

    /* Command pools are opaque objects that command buffer memory
       is allocated from, and which allow the implementation
       to amortize the cost of resource creation across multiple
       command buffers. Command pools are externally synchronized,
       meaning that a command pool must not be used concurrently
       in multiple threads. That includes use via recording commands
       on any command buffers allocated from the pool, as well as
       operations that allocate, free, and reset command buffers
       or the pool itself. */

    class CommandPool : public NonDispatchable<VkCommandPool>
    {
    public:
        explicit CommandPool(std::shared_ptr<Device> device,
            uint32_t queueFamilyIndex,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkCommandPoolCreateFlags flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
            const StructureChain& extendedInfo = StructureChain());
        ~CommandPool();
        uint32_t getQueueFamilyIndex() const noexcept { return queueFamilyIndex; }
        VkCommandPoolCreateFlags getFlags() const noexcept { return flags; }
        bool reset(bool releaseResources = false) noexcept;
        std::vector<std::unique_ptr<CommandBuffer>> allocateCommandBuffers(uint32_t commandBufferCount,
            bool primaryLevel,
            const StructureChain& extendedInfo = StructureChain());
        void freeCommandBuffers(std::vector<std::unique_ptr<CommandBuffer>>& cmdBuffers) noexcept;
    #ifdef VK_KHR_maintenance1
        void trim(VkCommandPoolTrimFlagsKHR flags = 0);
    #endif

    private:
        const uint32_t queueFamilyIndex;
        const VkCommandPoolCreateFlags flags;
    };
} // namespace magma
