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
#pragma once
#include "model/nondispatchable.h"

namespace magma
{
    class CommandBuffer;
    class Queue;

    /* Fences are a synchronization primitive that can be
       used to insert a dependency from a queue to the host. */

    class Fence : public NonDispatchable<VkFence>
    {
    public:
        enum class State : uint8_t;
        explicit Fence(std::shared_ptr<Device> device,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkFenceCreateFlags flags = 0,
            const StructureChain& extendedInfo = StructureChain());
        ~Fence();
        bool reset() noexcept;
        State getStatus() const noexcept;
        bool wait(uint64_t timeout = std::numeric_limits<uint64_t>::max());

    protected:
        Fence(std::shared_ptr<IAllocator> allocator,
            std::shared_ptr<Device> device) noexcept;

    private:
        void completeExecutionOnSignaled(lent_ptr<CommandBuffer> cmdBuffer);

        std::unordered_set<CommandBuffer *> submittedCmdBuffers;
        friend Queue;
    };

    /* Fences have two states - signaled and unsignaled.
       A fence can be signaled as part of the execution of
       a queue submission command. */

    enum class Fence::State : uint8_t
    {
        Signaled, Unsignaled
    };
} // namespace magma
