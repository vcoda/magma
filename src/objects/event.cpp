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
#include "pch.h"
#pragma hdrstop
#include "event.h"
#include "device.h"
#include "../allocator/allocator.h"
#include "../exceptions/errorResult.h"

namespace magma
{
Event::Event(std::shared_ptr<Device> device,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    const StructureChain& extendedInfo /* default */):
    NonDispatchable(VK_OBJECT_TYPE_EVENT, std::move(device), std::move(allocator))
{
    VkEventCreateInfo eventInfo;
    eventInfo.sType = VK_STRUCTURE_TYPE_EVENT_CREATE_INFO;
    eventInfo.pNext = extendedInfo.chainNodes();
    eventInfo.flags = 0;
    const VkResult result = vkCreateEvent(MAGMA_HANDLE(device), &eventInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create event");
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
    const VkResult result = vkSetEvent(MAGMA_HANDLE(device), handle);
    MAGMA_ASSERT(VK_SUCCESS == result);
    return (VK_SUCCESS == result);
}

bool Event::reset() noexcept
{
    const VkResult result = vkResetEvent(MAGMA_HANDLE(device), handle);
    MAGMA_ASSERT(VK_SUCCESS == result);
    return (VK_SUCCESS == result);
}
} // namespace magma
