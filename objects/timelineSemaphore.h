/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2022 Victor Coda.

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
    /* Introduces a new type of semaphore that has an integer payload identifying a point in a timeline.
       Such timeline semaphores support the following operations:
        * Host query - A host operation that allows querying the payload of the timeline semaphore.
        * Host wait - A host operation that allows a blocking wait for a timeline semaphore to reach a specified value.
        * Host signal - A host operation that allows advancing the timeline semaphore to a specified value.
        * Device wait - A device operation that allows waiting for a timeline semaphore to reach a specified value.
        * Device signal - A device operation that allows advancing the timeline semaphore to a specified value. */

#ifdef VK_KHR_timeline_semaphore
    class BinarySemaphore : public NonDispatchable<VkSemaphore>
    {
    public:
        explicit BinarySemaphore(std::shared_ptr<Device> device,
            std::shared_ptr<IAllocator> allocator = nullptr);
        ~BinarySemaphore();
        void signal(bool value);
    };

    class TimelineSemaphore : public NonDispatchable<VkSemaphore>
    {
    public:
        explicit TimelineSemaphore(std::shared_ptr<Device> device,
            uint64_t initialValue,
            std::shared_ptr<IAllocator> allocator = nullptr);
        ~TimelineSemaphore();
        uint64_t getCounterValue() const;
        void signal(uint64_t counter);
        bool wait(uint64_t counter,
            uint64_t timeout = std::numeric_limits<uint64_t>::max()) const;
    };
#endif // VK_KHR_timeline_semaphore
} // namespace magma
