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
#include "event.h"
#include "device.h"
#include "../allocator/allocator.h"
#include "../shared.h"

namespace magma
{
Event::Event(std::shared_ptr<const Device> device,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    NonDispatchable(VK_DEBUG_REPORT_OBJECT_TYPE_EVENT_EXT, device, allocator)
{
    VkEventCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_EVENT_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    const VkResult create = vkCreateEvent(*device, &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create event");
}

Event::~Event()
{
    vkDestroyEvent(*device, handle, MAGMA_OPTIONAL_INSTANCE(allocator));
}

VkResult Event::getStatus() const noexcept
{
    return vkGetEventStatus(*device, handle);
}

void Event::set() noexcept
{
    vkSetEvent(*device, handle);
}

bool Event::reset() noexcept
{
    const VkResult reset = vkResetEvent(*device, handle);
    return (VK_SUCCESS == reset);
}
} // namespace magma
