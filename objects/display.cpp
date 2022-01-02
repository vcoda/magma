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
#include "pch.h"
#pragma hdrstop
#include "display.h"
#include "physicalDevice.h"
#include "../misc/extProcAddress.h"
#include "../exceptions/errorResult.h"

namespace magma
{
#ifdef VK_KHR_display
Display::Display(std::shared_ptr<const PhysicalDevice> physicalDevice, VkDisplayKHR handle, uint32_t planeIndex) noexcept:
    NonDispatchable(VK_OBJECT_TYPE_DISPLAY_KHR, nullptr, nullptr),
    instance(std::move(physicalDevice->getInstance())),
    physicalDevice(std::move(physicalDevice)),
    planeIndex(planeIndex)
{
    this->handle = handle;
}

std::vector<VkDisplayModePropertiesKHR> Display::getModeProperties() const
{
    uint32_t propertyCount;
    MAGMA_INSTANCE_EXTENSION(vkGetDisplayModePropertiesKHR, VK_KHR_DISPLAY_EXTENSION_NAME);
    VkResult result = vkGetDisplayModePropertiesKHR(MAGMA_HANDLE(physicalDevice), handle, &propertyCount, nullptr);
    MAGMA_THROW_FAILURE(result, "failed to count display mode properties");
    std::vector<VkDisplayModePropertiesKHR> displayModeProperties(propertyCount);
    if (propertyCount > 0)
    {
        result = vkGetDisplayModePropertiesKHR(MAGMA_HANDLE(physicalDevice), handle, &propertyCount, displayModeProperties.data());
        MAGMA_THROW_FAILURE(result, "failed to get display mode properties");
    }
    return displayModeProperties;
}
#endif // VK_KHR_display
} // namespace magma
