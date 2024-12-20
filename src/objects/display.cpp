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
#include "display.h"
#include "physicalDevice.h"
#include "instance.h"
#include "../misc/extension.h"
#include "../exceptions/errorResult.h"

namespace magma
{
#ifdef VK_KHR_display
Display::Display(std::shared_ptr<PhysicalDevice> physicalDevice, VkDisplayKHR handle, uint32_t planeIndex) noexcept:
    NonDispatchable(VK_OBJECT_TYPE_DISPLAY_KHR, handle),
    physicalDevice(std::move(physicalDevice)),
    planeIndex(planeIndex)
{}

std::vector<VkDisplayModePropertiesKHR> Display::getModeProperties() const
{
    uint32_t propertyCount = 0;
    MAGMA_REQUIRED_INSTANCE_EXTENSION(vkGetDisplayModePropertiesKHR, VK_KHR_DISPLAY_EXTENSION_NAME);
    VkResult result = vkGetDisplayModePropertiesKHR(getNativePhysicalDevice(), handle, &propertyCount, nullptr);
    std::vector<VkDisplayModePropertiesKHR> displayModeProperties;
    if (propertyCount)
    {
        displayModeProperties.resize(propertyCount);
        result = vkGetDisplayModePropertiesKHR(getNativePhysicalDevice(), handle, &propertyCount, displayModeProperties.data());
    }
    MAGMA_HANDLE_RESULT(result, "failed to get display mode properties");
    return displayModeProperties;
}

VkInstance Display::getNativeInstance() const noexcept
{
    return physicalDevice->getInstance()->getHandle();
}

VkPhysicalDevice Display::getNativePhysicalDevice() const noexcept
{
    return physicalDevice->getHandle();
}
#endif // VK_KHR_display
} // namespace magma
