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
#include "displayMode.h"
#include "display.h"
#include "physicalDevice.h"
#include "device.h"
#include "../allocator/allocator.h"
#include "../misc/instanceExtension.h"

namespace magma
{
DisplayMode::DisplayMode(std::shared_ptr<const Display> display,
    const VkExtent2D& visibleRegion, uint32_t refreshRate,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    NonDispatchable(VK_OBJECT_TYPE_DISPLAY_MODE_KHR, std::move(display->getDevice()), std::move(allocator)),
    instance(std::move(display->getPhysicalDevice()->getInstance())),
    physicalDevice(std::move(display->getPhysicalDevice())),
    visibleRegion(visibleRegion),
    refreshRate(refreshRate)
{
    VkDisplayModeCreateInfoKHR info;
    info.sType = VK_STRUCTURE_TYPE_DISPLAY_MODE_CREATE_INFO_KHR;
    info.pNext = nullptr;
    info.flags = 0;
    info.parameters.visibleRegion = visibleRegion;
    info.parameters.refreshRate = refreshRate;
    MAGMA_INSTANCE_EXTENSION(vkCreateDisplayModeKHR, VK_KHR_DISPLAY_EXTENSION_NAME);
    const VkResult create = vkCreateDisplayModeKHR(MAGMA_HANDLE(physicalDevice), *display, &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create display mode");
}

const VkDisplayPlaneCapabilitiesKHR& DisplayMode::getPlaneCapabilities(uint32_t planeIndex) const
{
    const auto it = capabilities.find(planeIndex);
    if (it == capabilities.end())
    {
        MAGMA_INSTANCE_EXTENSION(vkGetDisplayPlaneCapabilitiesKHR, VK_KHR_DISPLAY_EXTENSION_NAME);
        VkDisplayPlaneCapabilitiesKHR planeCaps;
        const VkResult get = vkGetDisplayPlaneCapabilitiesKHR(MAGMA_HANDLE(physicalDevice), handle, planeIndex, &planeCaps);
        MAGMA_THROW_FAILURE(get, "failed to get display plane capabilities");
        capabilities[planeIndex] = planeCaps;
    }
    return capabilities[planeIndex];
}
} // namespace magma
