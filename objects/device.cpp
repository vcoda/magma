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
#include "device.h"
#include "fence.h"
#include "../helpers/stackArray.h"

namespace magma
{
Device::Device(VkDevice device):
    Handle(VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_EXT, device)
{}

bool Device::waitIdle() const noexcept
{
    const VkResult wait = vkDeviceWaitIdle(handle);
    return (VK_SUCCESS == wait);
}

bool Device::resetFences(std::vector<std::shared_ptr<const Fence>>& fences) const noexcept
{
    MAGMA_STACK_ARRAY(VkFence, dereferencedFences, fences.size());
    for (const auto& fence : fences)
        dereferencedFences.put(*fence);
    const VkResult reset = vkResetFences(handle, dereferencedFences.size(), dereferencedFences.data());
    return (VK_SUCCESS == reset);
}

bool Device::waitForFences(std::vector<std::shared_ptr<const Fence>>& fences, bool waitAll, 
    uint64_t timeout /* UINT64_MAX */) const noexcept
{
    MAGMA_STACK_ARRAY(VkFence, dereferencedFences, fences.size());
    for (const auto& fence : fences)
        dereferencedFences.put(*fence);
    const VkResult wait = vkWaitForFences(handle, dereferencedFences.size(), dereferencedFences.data(), 
        MAGMA_BOOLEAN(waitAll), timeout);
    return (VK_SUCCESS == wait);
}
} // namespace magma
