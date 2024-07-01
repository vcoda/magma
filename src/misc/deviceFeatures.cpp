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
    owner(std::move(device))
{}

DeviceFeatures::FormatFeatures DeviceFeatures::supportsFormatFeatures(VkFormat format, VkFormatFeatureFlags flags) const noexcept
{
    FormatFeatures features = {};
    if (auto device = owner.lock())
    {
        const std::shared_ptr<const PhysicalDevice>& physicalDevice = device->getPhysicalDevice();
        const VkFormatProperties formatProperties = physicalDevice->getFormatProperties(format);
        features.linear = MAGMA_BITWISE_AND(formatProperties.linearTilingFeatures, flags);
        features.optimal= MAGMA_BITWISE_AND(formatProperties.optimalTilingFeatures, flags);
        features.buffer = MAGMA_BITWISE_AND(formatProperties.bufferFeatures, flags);
    }
    return features;
}

#ifdef VK_KHR_external_memory_capabilities
DeviceFeatures::ExternalMemoryFeatures DeviceFeatures::supportsExternalBuffer(VkExternalMemoryHandleTypeFlagBits handleType,
    VkBufferUsageFlags usage, VkBufferCreateFlags flags /* 0 */) const
{
    ExternalMemoryFeatures features = {};
    if (auto device = owner.lock())
    {
        const std::shared_ptr<PhysicalDevice>& physicalDevice = device->getPhysicalDevice();
        const std::shared_ptr<Instance>& instance = physicalDevice->getInstance();
        if (instance->extensionEnabled(VK_KHR_EXTERNAL_MEMORY_CAPABILITIES_EXTENSION_NAME) &&
            instance->extensionEnabled(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME))
        {
            const VkExternalMemoryPropertiesKHR externalMemoryProperties =
                physicalDevice->getExternalBufferProperties(handleType, usage, flags);
            const VkExternalMemoryFeatureFlagsKHR& externalMemoryFeatures = externalMemoryProperties.externalMemoryFeatures;
            features.dedicatedOnly = MAGMA_BITWISE_AND(externalMemoryFeatures, VK_EXTERNAL_MEMORY_FEATURE_DEDICATED_ONLY_BIT_KHR);
            features.exportable = MAGMA_BITWISE_AND(externalMemoryFeatures, VK_EXTERNAL_MEMORY_FEATURE_EXPORTABLE_BIT_KHR);
            features.importable = MAGMA_BITWISE_AND(externalMemoryFeatures, VK_EXTERNAL_MEMORY_FEATURE_IMPORTABLE_BIT_KHR);
        }
    }
    return features;
}

DeviceFeatures::ExternalMemoryFeatures DeviceFeatures::supportsExternalImage(VkExternalMemoryHandleTypeFlagBits handleType,
    VkFormat format, VkImageUsageFlags usage,
    VkImageType imageType /* VK_IMAGE_TYPE_2D */,
    bool optimalTiling /* true */,
    VkImageCreateFlags flags /* 0 */) const
{
    ExternalMemoryFeatures features = {};
    if (auto device = owner.lock())
    {
        const std::shared_ptr<PhysicalDevice>& physicalDevice = device->getPhysicalDevice();
        const std::shared_ptr<Instance>& instance = physicalDevice->getInstance();
        if (instance->extensionEnabled(VK_KHR_EXTERNAL_MEMORY_CAPABILITIES_EXTENSION_NAME) &&
            instance->extensionEnabled(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME))
        {
            const VkExternalMemoryPropertiesKHR externalMemoryProperties =
                physicalDevice->getExternalImageFormatProperties(handleType, format, imageType, optimalTiling, usage, flags);
            const VkExternalMemoryFeatureFlagsKHR& externalMemoryFeatures = externalMemoryProperties.externalMemoryFeatures;
            features.dedicatedOnly = MAGMA_BITWISE_AND(externalMemoryFeatures, VK_EXTERNAL_MEMORY_FEATURE_DEDICATED_ONLY_BIT_KHR);
            features.exportable = MAGMA_BITWISE_AND(externalMemoryFeatures, VK_EXTERNAL_MEMORY_FEATURE_EXPORTABLE_BIT_KHR);
            features.importable = MAGMA_BITWISE_AND(externalMemoryFeatures, VK_EXTERNAL_MEMORY_FEATURE_IMPORTABLE_BIT_KHR);
        }
    }
    return features;
}
#endif // VK_KHR_external_memory_capabilities

#ifdef VK_KHR_external_fence_capabilities
DeviceFeatures::ExternalFeatures DeviceFeatures::supportsExternalFence(VkExternalFenceHandleTypeFlagBitsKHR handleType) const
{
    ExternalFeatures features = {};
    if (auto device = owner.lock())
    {
        const std::shared_ptr<PhysicalDevice>& physicalDevice = device->getPhysicalDevice();
        const std::shared_ptr<Instance>& instance = physicalDevice->getInstance();
        if (instance->extensionEnabled(VK_KHR_EXTERNAL_FENCE_CAPABILITIES_EXTENSION_NAME) &&
            instance->extensionEnabled(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME))
        {
            const VkExternalFencePropertiesKHR externalFenceProperties = physicalDevice->getExternalFenceProperties(handleType);
            const VkExternalFenceFeatureFlagsKHR& externalFenceFeatures = externalFenceProperties.externalFenceFeatures;
            features.exportable = MAGMA_BITWISE_AND(externalFenceFeatures, VK_EXTERNAL_FENCE_FEATURE_EXPORTABLE_BIT_KHR);
            features.importable = MAGMA_BITWISE_AND(externalFenceFeatures, VK_EXTERNAL_FENCE_FEATURE_IMPORTABLE_BIT_KHR);
        }
    }
    return features;
}
#endif // VK_KHR_external_fence_capabilities

#ifdef VK_KHR_external_semaphore_capabilities
DeviceFeatures::ExternalFeatures DeviceFeatures::supportsExternalSemaphore(VkExternalSemaphoreHandleTypeFlagBitsKHR handleType) const
{
    ExternalFeatures features = {};
    if (auto device = owner.lock())
    {
        const std::shared_ptr<PhysicalDevice>& physicalDevice = device->getPhysicalDevice();
        const std::shared_ptr<Instance>& instance = physicalDevice->getInstance();
        if (instance->extensionEnabled(VK_KHR_EXTERNAL_SEMAPHORE_CAPABILITIES_EXTENSION_NAME) &&
            instance->extensionEnabled(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME))
        {
            const VkExternalSemaphorePropertiesKHR externalSemaphoreProperties = physicalDevice->getExternalSemaphoreProperties(handleType);
            const VkExternalSemaphoreFeatureFlagsKHR& externalSemaphoreFeatures = externalSemaphoreProperties.externalSemaphoreFeatures;
            features.exportable = MAGMA_BITWISE_AND(externalSemaphoreFeatures, VK_EXTERNAL_SEMAPHORE_FEATURE_EXPORTABLE_BIT_KHR);
            features.importable = MAGMA_BITWISE_AND(externalSemaphoreFeatures, VK_EXTERNAL_SEMAPHORE_FEATURE_IMPORTABLE_BIT_KHR);
        }
    }
    return features;
}
#endif // VK_KHR_external_semaphore_capabilities

bool DeviceFeatures::supportsImageUsage(std::shared_ptr<const Surface> surface, VkImageUsageFlags flags) const
{
    if (auto device = owner.lock())
    {
        const std::shared_ptr<PhysicalDevice>& physicalDevice = device->getPhysicalDevice();
        const VkSurfaceCapabilitiesKHR surfaceCapabilities = physicalDevice->getSurfaceCapabilities(std::move(surface));
        return MAGMA_BITWISE_AND(surfaceCapabilities.supportedUsageFlags, flags);
    }
    return false;
}

/* Usually a CPU can only access 256MB of VRAM. If Resizable BAR
   or Smart Access Memory features are enabled in BIOS, a CPU may
   access the GPU's entire framebuffer. */
bool DeviceFeatures::supportsDeviceLocalHostVisibleMemory() const noexcept
{
    if (auto device = owner.lock())
    {
        const std::shared_ptr<PhysicalDevice>& physicalDevice = device->getPhysicalDevice();
        const VkPhysicalDeviceMemoryProperties memoryProperties = physicalDevice->getMemoryProperties();
        for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i)
        {
            const VkMemoryType& memoryType = memoryProperties.memoryTypes[i];
            if (MAGMA_BITWISE_AND(memoryType.propertyFlags,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT |
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                VK_MEMORY_PROPERTY_HOST_COHERENT_BIT))
                return true;
        }
    }
    return false;
}

bool DeviceFeatures::maintenanceEnabled(uint8_t index) const noexcept
{
    MAGMA_ASSERT((index > 0) && (index < 10));
    if ((index < 1) || (index > 9))
        return false;
    if (auto device = owner.lock())
    {
        const char extensionName[] = {
            'V','K','_','K','H','R','_','m','a','i','n','t','e','n','a','n','c','e',
            char('0' + index), '\0'
        };
        return device->extensionEnabled(extensionName);
    }
    return false;
}

bool DeviceFeatures::negativeViewportHeightEnabled() const noexcept
{
    if (auto device = owner.lock())
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
    if (auto device = owner.lock())
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
    if (auto device = owner.lock())
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
    if (auto device = owner.lock())
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

DeviceFeatures::Vendor DeviceFeatures::getVendor() const noexcept
{
    if (auto device = owner.lock())
    {
        const std::shared_ptr<PhysicalDevice>& physicalDevice = device->getPhysicalDevice();
        const VkPhysicalDeviceProperties properties = physicalDevice->getProperties();
        // https://pcisig.com/membership/member-companies
        switch (properties.vendorID)
        {
        case 0x1002: return Vendor::AMD;
        case 0x1010: return Vendor::ImaginationTechnologies;
        case 0x102B: return Vendor::Matrox;
        case 0x106B: return Vendor::Apple;
        case 0x10DE: return Vendor::Nvidia;
        case 0x1106: return Vendor::VIA;
        case 0x13B5: return Vendor::ARM;
        case 0x14E4: return Vendor::Broadcom;
        case 0x1EB1: return Vendor::VeriSilicon;
        case 0x5143: return Vendor::Qualcomm;
        case 0x5333: return Vendor::S3Graphics;
        case 0x8086: return Vendor::Intel;
        }
    }
    return Vendor::Unknown;
}
} // namespace magma
