/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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
    /* Events are a synchronization primitive that can be used
       to insert a fine-grained dependency between commands
       submitted to the same queue, or between the host and a
       queue. Events must not be used to insert a dependency
       between commands submitted to different queues.

       Events have two states - signaled and unsignaled.
       An application can signal an event, or unsignal it,
       on either the host or the device. A device can wait for
       an event to become signaled before executing further
       operations. No command exists to wait for an event to
       become signaled on the host, but the current state of
       an event can be queried. */

    class Event : public NonDispatchable<VkEvent>
    {
    public:
        explicit Event(std::shared_ptr<Device> device,
            std::shared_ptr<IAllocator> allocator = nullptr,
            const StructureChain& extendedInfo = StructureChain());
        ~Event();
        VkResult getStatus() const noexcept;
        bool set() noexcept;
        bool reset() noexcept;
    };
} // namespace magma
