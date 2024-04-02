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
#include "../objects/instance.h"
#include "../objects/device.h"
#include "../objects/physicalDevice.h"
#include "../objects/surface.h"

namespace magma
{
DeviceFeatures::DeviceFeatures(std::shared_ptr<const Device> device) noexcept:
    parent(std::move(device))
{}

DeviceFeatures::FormatFeatures DeviceFeatures::supportsFormatFeatures(VkFormat format, VkFormatFeatureFlags flags) const noexcept
{
    FormatFeatures support = {};
    if (auto device = parent.lock())
    {
        std::shared_ptr<const PhysicalDevice> physicalDevice = device->getPhysicalDevice();
        const VkFormatProperties properties = physicalDevice->getFormatProperties(format);
        const VkFormatFeatureFlags linearTilingFlags = (properties.linearTilingFeatures & flags);
        const VkFormatFeatureFlags optimalTilingFlags = (properties.optimalTilingFeatures & flags);
        const VkFormatFeatureFlags bufferFlags = (properties.bufferFeatures & flags);
        support.linear = (linearTilingFlags == flags);
        support.optimal = (optimalTilingFlags == flags);
        support.buffer = (bufferFlags == flags);
    }
    return support;
}

#ifdef VK_KHR_external_semaphore_capabilities
DeviceFeatures::ExternalSemaphoreFeatures DeviceFeatures::supportsExternalSemaphore(VkExternalSemaphoreHandleTypeFlagBitsKHR handleType) const
{
    ExternalSemaphoreFeatures features = {};
    if (auto device = parent.lock())
    {
        std::shared_ptr<const PhysicalDevice> physicalDevice = device->getPhysicalDevice();
        if (physicalDevice->getInstance()->extensionEnabled(VK_KHR_EXTERNAL_SEMAPHORE_CAPABILITIES_EXTENSION_NAME))
        {
            const VkExternalSemaphorePropertiesKHR properties = physicalDevice->getExternalSemaphoreProperties(handleType);
            if (properties.externalSemaphoreFeatures & VK_EXTERNAL_SEMAPHORE_FEATURE_EXPORTABLE_BIT_KHR)
                features.exportable = VK_TRUE;
            if (properties.externalSemaphoreFeatures & VK_EXTERNAL_SEMAPHORE_FEATURE_IMPORTABLE_BIT_KHR)
                features.importable = VK_TRUE;
        }
    }
    return features;
}
#endif // VK_KHR_external_semaphore_capabilities

bool DeviceFeatures::supportsImageUsage(std::shared_ptr<const Surface> surface, VkImageUsageFlags flags) const
{
    if (auto device = parent.lock())
    {
        std::shared_ptr<const PhysicalDevice> physicalDevice = device->getPhysicalDevice();
        const VkSurfaceCapabilitiesKHR surfaceCapabilities = physicalDevice->getSurfaceCapabilities(std::move(surface));
        for (VkImageUsageFlagBits bit: {
            VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
            VK_IMAGE_USAGE_TRANSFER_DST_BIT,
            VK_IMAGE_USAGE_SAMPLED_BIT,
            VK_IMAGE_USAGE_STORAGE_BIT,
            VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
            VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT,
            VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,
        #ifdef VK_EXT_fragment_density_map
            VK_IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT,
        #endif
        #ifdef VK_KHR_fragment_shading_rate
            VK_IMAGE_USAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR,
        #endif
        #ifdef VK_EXT_attachment_feedback_loop_layout
            VK_IMAGE_USAGE_ATTACHMENT_FEEDBACK_LOOP_BIT_EXT,
        #endif
        #ifdef VK_HUAWEI_invocation_mask
            VK_IMAGE_USAGE_INVOCATION_MASK_BIT_HUAWEI,
        #endif
        #ifdef VK_QCOM_image_processing
            VK_IMAGE_USAGE_SAMPLE_WEIGHT_BIT_QCOM,
            VK_IMAGE_USAGE_SAMPLE_BLOCK_MATCH_BIT_QCOM,
        #endif
            })
        {
            if (flags & bit)
            {
                if (!(surfaceCapabilities.supportedUsageFlags & bit))
                    return false;
            }
        }
    }
    return true;
}

bool DeviceFeatures::supportsDeviceLocalHostVisibleMemory() const noexcept
{
    if (auto device = parent.lock())
    {
        std::shared_ptr<const PhysicalDevice> physicalDevice = device->getPhysicalDevice();
        const VkPhysicalDeviceMemoryProperties memoryProperties = physicalDevice->getMemoryProperties();
        for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i)
        {
            constexpr VkMemoryPropertyFlags deviceLocalHostVisibleFlags =
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT |
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
            const VkMemoryType& memoryType = memoryProperties.memoryTypes[i];
            if ((memoryType.propertyFlags & deviceLocalHostVisibleFlags) == deviceLocalHostVisibleFlags)
                return true;
        }
    }
    return false;
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
    if (auto device = parent.lock())
        return device->extensionEnabled(extensionName);
    return false;
}

bool DeviceFeatures::negativeViewportHeightEnabled() const noexcept
{
    if (auto device = parent.lock())
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
    if (auto device = parent.lock())
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
    if (auto device = parent.lock())
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
    if (auto device = parent.lock())
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
