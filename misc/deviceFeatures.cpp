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
DeviceFeatures::DeviceFeatures(std::shared_ptr<const Device> device) noexcept:
    parent(std::move(device))
{}

DeviceFeatures::FormatFeaturesSupport DeviceFeatures::checkFormatFeaturesSupport(VkFormat format, const VkFormatFeatureFlags flags) const noexcept
{
    FormatFeaturesSupport result = {};
    if (std::shared_ptr<const Device> device = parent.lock())
    {
        std::shared_ptr<const PhysicalDevice> physicalDevice = device->getPhysicalDevice();
        const VkFormatProperties properties = physicalDevice->getFormatProperties(format);
        const VkFormatFeatureFlags linearTilingFlags = (properties.linearTilingFeatures & flags);
        const VkFormatFeatureFlags optimalTilingFlags = (properties.optimalTilingFeatures & flags);
        const VkFormatFeatureFlags bufferFlags = (properties.bufferFeatures & flags);
        result.linear = (linearTilingFlags == flags);
        result.optimal = (optimalTilingFlags == flags);
        result.buffer = (bufferFlags == flags);
    }
    return result;
}

bool DeviceFeatures::maintenanceEnabled(uint8_t index) const noexcept
{
    MAGMA_ASSERT((index > 0) && (index < 10));
    if ((index < 1) || (index > 9) || parent.expired())
        return false;
    const char extensionName[] = {
        'V','K','_','K','H','R','_','m','a','i','n','t','e','n','a','n','c','e',
        char('0' + index), '\0'
    };
    std::shared_ptr<const Device> device = parent.lock();
    return device->extensionEnabled(extensionName);
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

bool DeviceFeatures::hasLocalHostVisibleMemory() const noexcept
{
    if (std::shared_ptr<const Device> device = parent.lock())
    {
        std::shared_ptr<const PhysicalDevice> physicalDevice = device->getPhysicalDevice();
        const VkPhysicalDeviceMemoryProperties memoryProperties = physicalDevice->getMemoryProperties();
        for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i)
        {
            const VkMemoryType& memoryType = memoryProperties.memoryTypes[i];
            const VkMemoryPropertyFlags deviceLocalHostVisibleFlags =
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT |
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
            if ((memoryType.propertyFlags & deviceLocalHostVisibleFlags) == deviceLocalHostVisibleFlags)
                return true;
        }
    }
    return false;
}
} // namespace magma
