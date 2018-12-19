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
#include "../misc/exception.h"
#include "../internal/shared.h"

namespace magma
{
Event::Event(std::shared_ptr<Device> device,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    NonDispatchable(VK_OBJECT_TYPE_EVENT, std::move(device), std::move(allocator))
{
    VkEventCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_EVENT_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    const VkResult create = vkCreateEvent(MAGMA_HANDLE(device), &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create event");
}

Event::~Event()
{
    vkDestroyEvent(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(allocator));
}

VkResult Event::getStatus() const noexcept
{
    return vkGetEventStatus(MAGMA_HANDLE(device), handle);
}

bool Event::set() noexcept
{
    const VkResult set = vkSetEvent(MAGMA_HANDLE(device), handle);
    MAGMA_ASSERT(VK_SUCCESS == set);
    return (VK_SUCCESS == set);
}

bool Event::reset() noexcept
{
    const VkResult reset = vkResetEvent(MAGMA_HANDLE(device), handle);
    MAGMA_ASSERT(VK_SUCCESS == reset);
    return (VK_SUCCESS == reset);
}
} // namespace magma
