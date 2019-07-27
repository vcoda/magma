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
    /* Fences are a synchronization primitive that can be used
       to insert a dependency from a queue to the host.
       Fences have two states - signaled and unsignaled.
       A fence can be signaled as part of the execution of a queue submission command. */

    class Fence : public NonDispatchable<VkFence>
    {
    public:
        explicit Fence(std::shared_ptr<Device> device,
            bool signaled = false,
            std::shared_ptr<IAllocator> allocator = nullptr);
        ~Fence();
        bool reset() noexcept;
        VkResult getStatus() const noexcept;
        bool wait(uint64_t timeout = UINT64_MAX) const noexcept;
    };
} // namespace magma
