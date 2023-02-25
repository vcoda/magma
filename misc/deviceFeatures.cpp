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
#include "deviceFeatures.h"
#include "../objects/device.h"
#include "../objects/physicalDevice.h"

namespace magma
{
DeviceFeatures::DeviceFeatures(std::shared_ptr<const Device> device):
    parent(device)
{
    char extensionName[] = "VK_KHR_maintenance0";
    for (char index = 1; index <= 4 /* VK_KHR_maintenance4 */; ++index)
    {
        extensionName[18] = '0' + index;
        maintenance.push_back(device->extensionEnabled(extensionName));
    }
}

bool DeviceFeatures::maintenanceEnabled(uint32_t index) const noexcept
{
    MAGMA_ASSERT(index < maintenance.size());
    if (index < maintenance.size())
        return maintenance[index];
    return false;
}

bool DeviceFeatures::negativeViewportHeightEnabled() const noexcept
{
    if (std::shared_ptr<const Device> device = parent.lock())
    {
    #ifdef VK_KHR_maintenance1
        if (device->extensionEnabled(VK_KHR_MAINTENANCE1_EXTENSION_NAME))
            return true;
    #endif
    #ifdef VK_AMD_negative_viewport_height
        if (device->extensionEnabled(VK_AMD_NEGATIVE_VIEWPORT_HEIGHT_EXTENSION_NAME))
            return true;
    #endif
        MAGMA_UNUSED(device);
    }
    return false;
}

bool DeviceFeatures::separateDepthStencilLayoutsEnabled() const noexcept
{
#ifdef VK_KHR_separate_depth_stencil_layouts
    if (std::shared_ptr<const Device> device = parent.lock())
    {
        if (device->extensionEnabled(VK_KHR_SEPARATE_DEPTH_STENCIL_LAYOUTS_EXTENSION_NAME))
        {
            const VkPhysicalDeviceSeparateDepthStencilLayoutsFeaturesKHR *separateDepthStencilFeatures =
                device->getEnabledExtendedFeatures<VkPhysicalDeviceSeparateDepthStencilLayoutsFeaturesKHR>();
            if (separateDepthStencilFeatures)
                return separateDepthStencilFeatures->separateDepthStencilLayouts;
        }
    }
#endif // VK_KHR_separate_depth_stencil_layouts
    return false;
}

bool DeviceFeatures::extendedLinesEnabled() const noexcept
{
#ifdef VK_EXT_line_rasterization
    if (std::shared_ptr<const Device> device = parent.lock())
    {
        if (device->extensionEnabled(VK_EXT_LINE_RASTERIZATION_EXTENSION_NAME))
        {
            const VkPhysicalDeviceLineRasterizationFeaturesEXT *lineRasterizationFeatures =
                device->getEnabledExtendedFeatures<VkPhysicalDeviceLineRasterizationFeaturesEXT>();
            if (lineRasterizationFeatures)
            {
                return lineRasterizationFeatures->rectangularLines ||
                    lineRasterizationFeatures->bresenhamLines ||
                    lineRasterizationFeatures->smoothLines;
            }
        }
    }
#endif // VK_EXT_line_rasterization
    return false;
}

bool DeviceFeatures::stippledLinesEnabled() const noexcept
{
#ifdef VK_EXT_line_rasterization
    if (std::shared_ptr<const Device> device = parent.lock())
    {
        if (device->extensionEnabled(VK_EXT_LINE_RASTERIZATION_EXTENSION_NAME))
        {
            const VkPhysicalDeviceLineRasterizationFeaturesEXT *lineRasterizationFeatures =
                device->getEnabledExtendedFeatures<VkPhysicalDeviceLineRasterizationFeaturesEXT>();
            if (lineRasterizationFeatures)
            {
                return lineRasterizationFeatures->stippledRectangularLines ||
                    lineRasterizationFeatures->stippledBresenhamLines ||
                    lineRasterizationFeatures->stippledSmoothLines;
            }
        }
    }
#endif // VK_EXT_line_rasterization
    return false;
}
} // namespace magma
