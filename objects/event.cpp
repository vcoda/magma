/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
#include "pch.h"
#pragma hdrstop
#include "event.h"
#include "device.h"
#include "../allocator/allocator.h"
#include "../exceptions/errorResult.h"

namespace magma
{
Event::Event(std::shared_ptr<Device> device,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    NonDispatchable(VK_OBJECT_TYPE_EVENT, std::move(device), std::move(allocator))
{
    VkEventCreateInfo eventInfo;
    eventInfo.sType = VK_STRUCTURE_TYPE_EVENT_CREATE_INFO;
    eventInfo.pNext = nullptr;
    eventInfo.flags = 0;
    const VkResult result = vkCreateEvent(MAGMA_HANDLE(device), &eventInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to create event");
}

Event::~Event()
{
    vkDestroyEvent(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
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
