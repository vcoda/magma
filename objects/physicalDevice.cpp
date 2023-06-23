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
#include "physicalDevice.h"
#include "instance.h"
#include "device.h"
#include "surface.h"
#include "display.h"
#include "pipelineCache.h"
#include "../misc/extProcAddress.h"
#include "../exceptions/errorResult.h"
#include "../helpers/stackArray.h"
#include "../core/compare.h"

namespace magma
{
PhysicalDevice::PhysicalDevice(std::shared_ptr<Instance> instance, VkPhysicalDevice handle,
    std::shared_ptr<IAllocator> allocator) noexcept:
    Dispatchable<VkPhysicalDevice>(VK_OBJECT_TYPE_PHYSICAL_DEVICE, std::move(allocator)),
    instance(std::move(instance))
{
    this->handle = handle;
    for (const auto& properties: enumerateExtensions())
        extensions.emplace(properties.extensionName);
}

std::shared_ptr<Device> PhysicalDevice::createDevice(const std::vector<DeviceQueueDescriptor>& queueDescriptors,
    const std::vector<const char *>& enabledLayers, const std::vector<const char *>& enabledExtensions,
    const VkPhysicalDeviceFeatures& deviceFeatures, const std::vector<void *>& deviceExtendedFeatures /* empty */,
    const StructureChain& extendedInfo /* default */) const
{
    return std::shared_ptr<Device>(new Device(
        std::const_pointer_cast<PhysicalDevice>(shared_from_this()),
        queueDescriptors,
        enabledLayers,
        enabledExtensions,
        deviceFeatures,
        deviceExtendedFeatures,
        extendedInfo,
        hostAllocator));
}

VkPhysicalDeviceFeatures PhysicalDevice::getFeatures() const noexcept
{
    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceFeatures(handle, &features);
    return features;
}

VkFormatProperties PhysicalDevice::getFormatProperties(VkFormat format) const noexcept
{
    VkFormatProperties formatProperties;
    vkGetPhysicalDeviceFormatProperties(handle, format, &formatProperties);
    return formatProperties;
}

VkImageFormatProperties PhysicalDevice::getImageFormatProperties(VkFormat format,
    VkImageType imageType, bool optimalTiling, VkImageUsageFlags usage,
    VkImageCreateFlags flags /* 0 */) const
{
    VkImageFormatProperties imageFormatProperties;
    const VkResult result = vkGetPhysicalDeviceImageFormatProperties(handle, format, imageType,
        optimalTiling ? VK_IMAGE_TILING_OPTIMAL : VK_IMAGE_TILING_LINEAR,
        usage, flags, &imageFormatProperties);
    MAGMA_THROW_FAILURE(result, "failed to get image format properties of physical device");
    return imageFormatProperties;
}

VkPhysicalDeviceProperties PhysicalDevice::getProperties() const noexcept
{
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(handle, &properties);
    return properties;
}

std::vector<VkQueueFamilyProperties> PhysicalDevice::getQueueFamilyProperties() const
{
    uint32_t propertyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(handle, &propertyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilyProperties;
    if (propertyCount > 0)
    {
        queueFamilyProperties.resize(propertyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(handle, &propertyCount, queueFamilyProperties.data());
    }
    return queueFamilyProperties;
}

VkPhysicalDeviceMemoryProperties PhysicalDevice::getMemoryProperties() const noexcept
{
    VkPhysicalDeviceMemoryProperties memoryProperties = {};
    vkGetPhysicalDeviceMemoryProperties(handle, &memoryProperties);
    return memoryProperties;
}

std::vector<VkLayerProperties> PhysicalDevice::enumerateLayers() const
{
    uint32_t propertyCount = 0;
    VkResult result = vkEnumerateDeviceLayerProperties(handle, &propertyCount, nullptr);
    std::vector<VkLayerProperties> properties;
    if (propertyCount > 0)
    {
        properties.resize(propertyCount);
        result = vkEnumerateDeviceLayerProperties(handle, &propertyCount, properties.data());
    }
    MAGMA_THROW_FAILURE(result, "failed to enumerate physical device layers");
    return properties;
}

std::vector<VkExtensionProperties> PhysicalDevice::enumerateExtensions(const char *layerName /* nullptr */) const
{
    uint32_t propertyCount = 0;
    VkResult result = vkEnumerateDeviceExtensionProperties(handle, layerName, &propertyCount, nullptr);
    std::vector<VkExtensionProperties> properties;
    if (propertyCount > 0)
    {
        properties.resize(propertyCount);
        result = vkEnumerateDeviceExtensionProperties(handle, layerName, &propertyCount, properties.data());
    }
    MAGMA_THROW_FAILURE(result, "failed to enumerate physical device extensions");
    return properties;
}

#ifdef VK_KHR_surface
bool PhysicalDevice::getSurfaceSupport(std::shared_ptr<const Surface> surface, uint32_t queueFamilyIndex) const noexcept
{
    VkBool32 supported = VK_FALSE;
    const VkResult result = vkGetPhysicalDeviceSurfaceSupportKHR(handle, queueFamilyIndex, *surface, &supported);
    return (VK_SUCCESS == result) && (VK_TRUE == supported);
}

VkSurfaceCapabilitiesKHR PhysicalDevice::getSurfaceCapabilities(std::shared_ptr<const Surface> surface) const
{
    VkSurfaceCapabilitiesKHR surfaceCaps;
    const VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(handle, *surface, &surfaceCaps);
    MAGMA_THROW_FAILURE(result, "failed to get surface capabilities of physical device");
    return surfaceCaps;
}

std::vector<VkSurfaceFormatKHR> PhysicalDevice::getSurfaceFormats(std::shared_ptr<const Surface> surface) const
{
    uint32_t surfaceFormatCount = 0;
    VkResult result = vkGetPhysicalDeviceSurfaceFormatsKHR(handle, *surface, &surfaceFormatCount, nullptr);
    std::vector<VkSurfaceFormatKHR> surfaceFormats;
    if (surfaceFormatCount > 0)
    {
        surfaceFormats.resize(surfaceFormatCount);
        result = vkGetPhysicalDeviceSurfaceFormatsKHR(handle, *surface, &surfaceFormatCount, surfaceFormats.data());
    }
    MAGMA_THROW_FAILURE(result, "failed to get surface formats of physical device");
    return surfaceFormats;
}

std::vector<VkPresentModeKHR> PhysicalDevice::getSurfacePresentModes(std::shared_ptr<const Surface> surface) const
{
    uint32_t presentModeCount = 0;
    VkResult result = vkGetPhysicalDeviceSurfacePresentModesKHR(handle, *surface, &presentModeCount, nullptr);
    std::vector<VkPresentModeKHR> presentModes;
    if (presentModeCount > 0)
    {
        presentModes.resize(presentModeCount);
        result = vkGetPhysicalDeviceSurfacePresentModesKHR(handle, *surface, &presentModeCount, presentModes.data());
    }
    MAGMA_THROW_FAILURE(result, "failed to get surface present modes of physical device");
    return presentModes;
}
#endif // VK_KHR_surface

#ifdef VK_AMD_display_native_hdr
bool PhysicalDevice::getSurfaceLocalDimmingSupport(std::shared_ptr<const Surface> surface) const
{
    VkDisplayNativeHdrSurfaceCapabilitiesAMD nativeHdrSurfaceCaps = {};
    nativeHdrSurfaceCaps.sType = VK_STRUCTURE_TYPE_DISPLAY_NATIVE_HDR_SURFACE_CAPABILITIES_AMD;
#ifdef VK_KHR_get_surface_capabilities2
    getSurfaceCapabilities2(std::move(surface), &nativeHdrSurfaceCaps);
#endif
    return (VK_TRUE == nativeHdrSurfaceCaps.localDimmingSupport);
}
#endif //  VK_AMD_display_native_hdr

#ifdef VK_EXT_full_screen_exclusive
std::vector<VkPresentModeKHR> PhysicalDevice::getSurfaceFullScreenExclusivePresentModes(std::shared_ptr<const Surface> surface,
    VkFullScreenExclusiveEXT fullScreenExclusive, void *hMonitor /* nullptr */) const
{
    std::vector<VkPresentModeKHR> presentModes;
#ifdef VK_USE_PLATFORM_WIN32_KHR
    VkSurfaceFullScreenExclusiveWin32InfoEXT fullScreenExclusiveWin32SurfaceInfo;
    fullScreenExclusiveWin32SurfaceInfo.sType = VK_STRUCTURE_TYPE_SURFACE_FULL_SCREEN_EXCLUSIVE_WIN32_INFO_EXT;
    fullScreenExclusiveWin32SurfaceInfo.pNext = nullptr;
    fullScreenExclusiveWin32SurfaceInfo.hmonitor = reinterpret_cast<HMONITOR>(hMonitor);
#endif // VK_USE_PLATFORM_WIN32_KHR
    VkSurfaceFullScreenExclusiveInfoEXT fullScreenExclusiveSurfaceInfo;
    fullScreenExclusiveSurfaceInfo.sType = VK_STRUCTURE_TYPE_SURFACE_FULL_SCREEN_EXCLUSIVE_INFO_EXT;
#ifdef VK_USE_PLATFORM_WIN32_KHR
    fullScreenExclusiveSurfaceInfo.pNext = hMonitor ? &fullScreenExclusiveWin32SurfaceInfo : nullptr;
#else
    fullScreenExclusiveSurfaceInfo.pNext = nullptr;
    MAGMA_UNUSED(hMonitor);
#endif
    fullScreenExclusiveSurfaceInfo.fullScreenExclusive = fullScreenExclusive;
#ifdef VK_KHR_get_surface_capabilities2
    VkPhysicalDeviceSurfaceInfo2KHR surfaceInfo;
    surfaceInfo.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR;
    surfaceInfo.pNext = &fullScreenExclusiveSurfaceInfo;
    surfaceInfo.surface = *surface;
    uint32_t presentModeCount = 0;
    MAGMA_REQUIRED_INSTANCE_EXTENSION(vkGetPhysicalDeviceSurfacePresentModes2EXT, VK_EXT_FULL_SCREEN_EXCLUSIVE_EXTENSION_NAME);
    VkResult result = vkGetPhysicalDeviceSurfacePresentModes2EXT(handle, &surfaceInfo, &presentModeCount, nullptr);
    if (presentModeCount > 0)
    {
        presentModes.resize(presentModeCount);
        result = vkGetPhysicalDeviceSurfacePresentModes2EXT(handle, &surfaceInfo, &presentModeCount, presentModes.data());
    }
    MAGMA_THROW_FAILURE(result, "failed to get full-screen exclusive surface present modes of physical device");
#endif // VK_KHR_get_surface_capabilities2
    return presentModes;
}

bool PhysicalDevice::getSurfaceFullScreenExclusiveSupport(std::shared_ptr<const Surface> surface) const
{
    VkSurfaceCapabilitiesFullScreenExclusiveEXT surfaceFullScreenCaps = {};
    surfaceFullScreenCaps.sType = VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_FULL_SCREEN_EXCLUSIVE_EXT;
#ifdef VK_KHR_get_surface_capabilities2
    getSurfaceCapabilities2(std::move(surface), &surfaceFullScreenCaps);
#endif
    return (VK_TRUE == surfaceFullScreenCaps.fullScreenExclusiveSupported);
}
#endif // VK_EXT_full_screen_exclusive

#ifdef VK_KHR_shared_presentable_image
VkImageUsageFlags PhysicalDevice::getSurfaceSharedPresentFlags(std::shared_ptr<const Surface> surface) const
{
    VkSharedPresentSurfaceCapabilitiesKHR sharedPresentSurfaceCaps = {};
    sharedPresentSurfaceCaps.sType = VK_STRUCTURE_TYPE_SHARED_PRESENT_SURFACE_CAPABILITIES_KHR;
#ifdef VK_KHR_get_surface_capabilities2
    getSurfaceCapabilities2(std::move(surface), &sharedPresentSurfaceCaps);
#endif
    return sharedPresentSurfaceCaps.sharedPresentSupportedUsageFlags;
}
#endif // VK_KHR_shared_presentable_image

bool PhysicalDevice::getPresentationSupport(uint32_t queueFamilyIndex,
    void *display /* nullptr */,
    const void *visualID /* nullptr */) const noexcept
{
    VkBool32 result;
#if defined(VK_USE_PLATFORM_WIN32_KHR)
    MAGMA_UNUSED(display);
    MAGMA_UNUSED(visualID);
    result = vkGetPhysicalDeviceWin32PresentationSupportKHR(handle, queueFamilyIndex);
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
    MAGMA_ASSERT(display);
    MAGMA_ASSERT(visualID);
    result = vkGetPhysicalDeviceXlibPresentationSupportKHR(handle, queueFamilyIndex,
        reinterpret_cast<::Display *>(display),
        *reinterpret_cast<const VisualID *>(visualID));
#elif defined(VK_USE_PLATFORM_XCB_KHR)
    MAGMA_ASSERT(display);
    MAGMA_ASSERT(visualID);
    result = vkGetPhysicalDeviceXcbPresentationSupportKHR(handle, queueFamilyIndex,
        reinterpret_cast<xcb_connection_t *>(display),
        *reinterpret_cast<const xcb_visualid_t *>(visualID));
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
    MAGMA_ASSERT(display);
    MAGMA_UNUSED(visualID);
    result = vkGetPhysicalDeviceWaylandPresentationSupportKHR(handle, queueFamilyIndex,
        reinterpret_cast<wl_display *>(display));
#elif defined(VK_USE_PLATFORM_MIR_KHR)
    MAGMA_ASSERT(display);
    MAGMA_UNUSED(visualID);
    result = vkGetPhysicalDeviceMirPresentationSupportKHR(handle, queueFamilyIndex,
        reinterpret_cast<MirConnection *>(display));
#elif defined(VK_USE_PLATFORM_SCREEN_QNX)
    MAGMA_ASSERT(display);
    MAGMA_UNUSED(visualID);
    result = vkGetPhysicalDeviceScreenPresentationSupportQNX(handle, queueFamilyIndex,
        reinterpret_cast<_screen_window *>(display));
#endif
    return (VK_TRUE == result);
}

#ifdef VK_EXT_tooling_info
std::vector<VkPhysicalDeviceToolPropertiesEXT> PhysicalDevice::getToolProperties() const
{
    std::vector<VkPhysicalDeviceToolPropertiesEXT> toolProperties;
    uint32_t toolCount = 0;
    MAGMA_REQUIRED_INSTANCE_EXTENSION(vkGetPhysicalDeviceToolPropertiesEXT, VK_EXT_TOOLING_INFO_EXTENSION_NAME);
    VkResult result = vkGetPhysicalDeviceToolPropertiesEXT(handle, &toolCount, nullptr);
    if (toolCount > 0)
    {
        toolProperties.resize(toolCount);
        result = vkGetPhysicalDeviceToolPropertiesEXT(handle, &toolCount, toolProperties.data());
    }
    MAGMA_THROW_FAILURE(result, "failed to get tool properties of physical device");
    return toolProperties;
}
#endif // VK_EXT_tooling_info

#ifdef VK_KHR_display
std::vector<VkDisplayPropertiesKHR> PhysicalDevice::getDisplayProperties() const
{
    std::vector<VkDisplayPropertiesKHR> displayProperties;
    uint32_t propertyCount = 0;
    MAGMA_REQUIRED_INSTANCE_EXTENSION(vkGetPhysicalDeviceDisplayPropertiesKHR, VK_KHR_DISPLAY_EXTENSION_NAME);
    VkResult result = vkGetPhysicalDeviceDisplayPropertiesKHR(handle, &propertyCount, nullptr);
    if (propertyCount > 0)
    {
        displayProperties.resize(propertyCount);
        result = vkGetPhysicalDeviceDisplayPropertiesKHR(handle, &propertyCount, displayProperties.data());
    }
    MAGMA_THROW_FAILURE(result, "failed to get display properties of physical device");
    return displayProperties;
}

std::vector<VkDisplayPlanePropertiesKHR> PhysicalDevice::getDisplayPlaneProperties() const
{
    std::vector<VkDisplayPlanePropertiesKHR> displayPlaneProperties;
    uint32_t propertyCount = 0;
    MAGMA_REQUIRED_INSTANCE_EXTENSION(vkGetPhysicalDeviceDisplayPropertiesKHR, VK_KHR_DISPLAY_EXTENSION_NAME);
    VkResult result = vkGetPhysicalDeviceDisplayPlanePropertiesKHR(handle, &propertyCount, nullptr);
    if (propertyCount > 0)
    {
        displayPlaneProperties.resize(propertyCount);
        result = vkGetPhysicalDeviceDisplayPlanePropertiesKHR(handle, &propertyCount, displayPlaneProperties.data());
    }
    MAGMA_THROW_FAILURE(result, "failed to get display plane properties of physical device");
    return displayPlaneProperties;
}

std::vector<std::shared_ptr<Display>> PhysicalDevice::getSupportedDisplays(uint32_t planeIndex) const
{
    std::vector<std::shared_ptr<Display>> supportedDisplays;
    uint32_t displayCount = 0;
    MAGMA_REQUIRED_INSTANCE_EXTENSION(vkGetDisplayPlaneSupportedDisplaysKHR, VK_KHR_DISPLAY_EXTENSION_NAME);
    VkResult result = vkGetDisplayPlaneSupportedDisplaysKHR(handle, planeIndex, &displayCount, nullptr);
    if (displayCount > 0)
    {
        MAGMA_STACK_ARRAY(VkDisplayKHR, displays, displayCount);
        result = vkGetDisplayPlaneSupportedDisplaysKHR(handle, planeIndex, &displayCount, displays);
        if (VK_SUCCESS == result)
        {
            for (const VkDisplayKHR handle : displays)
                supportedDisplays.emplace_back(new Display(shared_from_this(), handle, planeIndex));
        }
    }
    MAGMA_THROW_FAILURE(result, "failed to get supported displays of physical device");
    return supportedDisplays;
}
#endif // VK_KHR_display

#ifdef VK_AMD_device_coherent_memory
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceCoherentMemoryFeaturesAMD PhysicalDevice::getCoherentMemoryFeatures() const
{
    VkPhysicalDeviceCoherentMemoryFeaturesAMD coherentMemoryFeatures = {};
    coherentMemoryFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COHERENT_MEMORY_FEATURES_AMD;
    getFeatures2(&coherentMemoryFeatures);
    return coherentMemoryFeatures;
}
#endif // VK_AMD_device_coherent_memory

#ifdef VK_AMD_shader_core_properties
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceShaderCorePropertiesAMD PhysicalDevice::getShaderCoreProperties() const
{
    VkPhysicalDeviceShaderCorePropertiesAMD shaderCoreProperties = {};
    shaderCoreProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_CORE_PROPERTIES_AMD;
    getProperties2(&shaderCoreProperties);
    return shaderCoreProperties;
}
#endif // VK_AMD_shader_core_properties

#ifdef VK_ARM_rasterization_order_attachment_access
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceRasterizationOrderAttachmentAccessFeaturesARM PhysicalDevice::getRasterizationOrderAttachmentAccessFeatures() const
{
    VkPhysicalDeviceRasterizationOrderAttachmentAccessFeaturesARM rasterizationOrderAttachmentAccessFeatures = {};
    rasterizationOrderAttachmentAccessFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RASTERIZATION_ORDER_ATTACHMENT_ACCESS_FEATURES_ARM;
    getFeatures2(&rasterizationOrderAttachmentAccessFeatures);
    return rasterizationOrderAttachmentAccessFeatures;
}
#endif // VK_ARM_rasterization_order_attachment_access

#ifdef VK_EXT_4444_formats
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDevice4444FormatsFeaturesEXT PhysicalDevice::get4444FormatsFeatures() const
{
    VkPhysicalDevice4444FormatsFeaturesEXT _4444FormatsFeatures = {};
    _4444FormatsFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_4444_FORMATS_FEATURES_EXT;
    getFeatures2(&_4444FormatsFeatures);
    return _4444FormatsFeatures;
}
#endif // VK_EXT_4444_formats

#ifdef VK_EXT_astc_decode_mode
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceASTCDecodeFeaturesEXT PhysicalDevice::getASTCDecodeFeatures() const
{
    VkPhysicalDeviceASTCDecodeFeaturesEXT astcDecodeFeatures = {};
    astcDecodeFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ASTC_DECODE_FEATURES_EXT;
    getFeatures2(&astcDecodeFeatures);
    return astcDecodeFeatures;
}
#endif // VK_EXT_astc_decode_mode

#ifdef VK_EXT_blend_operation_advanced
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceBlendOperationAdvancedFeaturesEXT PhysicalDevice::getBlendOperationAdvancedFeatures() const
{
    VkPhysicalDeviceBlendOperationAdvancedFeaturesEXT blendOperationAdvancedFeatures = {};
    blendOperationAdvancedFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BLEND_OPERATION_ADVANCED_FEATURES_EXT;
    getFeatures2(&blendOperationAdvancedFeatures);
    return blendOperationAdvancedFeatures;
}

// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceBlendOperationAdvancedPropertiesEXT PhysicalDevice::getBlendOperationAdvancedProperties() const
{
    VkPhysicalDeviceBlendOperationAdvancedPropertiesEXT blendOperationAdvancedProperties = {};
    blendOperationAdvancedProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BLEND_OPERATION_ADVANCED_PROPERTIES_EXT;
    getProperties2(&blendOperationAdvancedProperties);
    return blendOperationAdvancedProperties;
}
#endif // VK_EXT_blend_operation_advanced

#ifdef VK_EXT_border_color_swizzle
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceBorderColorSwizzleFeaturesEXT PhysicalDevice::getBorderColorSwizzleFeatures() const
{
    VkPhysicalDeviceBorderColorSwizzleFeaturesEXT borderColorSwizzleFeatures = {};
    borderColorSwizzleFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BORDER_COLOR_SWIZZLE_FEATURES_EXT;
    getFeatures2(&borderColorSwizzleFeatures);
    return borderColorSwizzleFeatures;
}
#endif // VK_EXT_border_color_swizzle

#ifdef VK_EXT_buffer_device_address
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceBufferAddressFeaturesEXT PhysicalDevice::getBufferAddressFeatures() const
{
    VkPhysicalDeviceBufferAddressFeaturesEXT bufferAddressFeatures = {};
    bufferAddressFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_ADDRESS_FEATURES_EXT;
    getFeatures2(&bufferAddressFeatures);
    return bufferAddressFeatures;
}
#endif // VK_EXT_buffer_device_address

#ifdef VK_EXT_color_write_enable
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceColorWriteEnableFeaturesEXT PhysicalDevice::getColorWriteEnableFeatures() const
{
    VkPhysicalDeviceColorWriteEnableFeaturesEXT colorWriteEnableFeatures = {};
    colorWriteEnableFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COLOR_WRITE_ENABLE_FEATURES_EXT;
    getFeatures2(&colorWriteEnableFeatures);
    return colorWriteEnableFeatures;
}
#endif // VK_EXT_color_write_enable

#ifdef VK_EXT_conditional_rendering
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceConditionalRenderingFeaturesEXT PhysicalDevice::getConditionalRenderingFeatures() const
{
    VkPhysicalDeviceConditionalRenderingFeaturesEXT conditionalRenderingFeatures = {};
    conditionalRenderingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CONDITIONAL_RENDERING_FEATURES_EXT;
    getFeatures2(&conditionalRenderingFeatures);
    return conditionalRenderingFeatures;
}
#endif // VK_EXT_conditional_rendering

#ifdef VK_EXT_conservative_rasterization
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceConservativeRasterizationPropertiesEXT PhysicalDevice::getConservativeRasterizationProperties() const
{
    VkPhysicalDeviceConservativeRasterizationPropertiesEXT conservativeRasterizationProperties = {};
    conservativeRasterizationProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CONSERVATIVE_RASTERIZATION_PROPERTIES_EXT;
    getProperties2(&conservativeRasterizationProperties);
    return conservativeRasterizationProperties;
}
#endif // VK_EXT_conservative_rasterization

#ifdef VK_EXT_custom_border_color
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceCustomBorderColorFeaturesEXT PhysicalDevice::getCustomBorderColorFeatures() const
{
    VkPhysicalDeviceCustomBorderColorFeaturesEXT customBorderColorFeatures = {};
    customBorderColorFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CUSTOM_BORDER_COLOR_FEATURES_EXT;
    getFeatures2(&customBorderColorFeatures);
    return customBorderColorFeatures;
}

// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceCustomBorderColorPropertiesEXT PhysicalDevice::getCustomBorderColorProperties() const
{
    VkPhysicalDeviceCustomBorderColorPropertiesEXT customBorderColorProperties = {};
    customBorderColorProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CUSTOM_BORDER_COLOR_PROPERTIES_EXT;
    getProperties2(&customBorderColorProperties);
    return customBorderColorProperties;
}
#endif // VK_EXT_custom_border_color

#ifdef VK_EXT_depth_clip_control
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceDepthClipControlFeaturesEXT PhysicalDevice::getDepthClipControlFeatures() const
{
    VkPhysicalDeviceDepthClipControlFeaturesEXT depthClipControlFeatures = {};
    depthClipControlFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_CLIP_CONTROL_FEATURES_EXT;
    getFeatures2(&depthClipControlFeatures);
    return depthClipControlFeatures;
}
#endif // VK_EXT_depth_clip_control

#ifdef VK_EXT_depth_clip_enable
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceDepthClipEnableFeaturesEXT PhysicalDevice::getDepthClipEnableFeatures() const
{
    VkPhysicalDeviceDepthClipEnableFeaturesEXT depthClipEnableFeatures = {};
    depthClipEnableFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_CLIP_ENABLE_FEATURES_EXT;
    getFeatures2(&depthClipEnableFeatures);
    return depthClipEnableFeatures;
}
#endif // VK_EXT_depth_clip_enable

#ifdef VK_EXT_descriptor_indexing
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceDescriptorIndexingFeaturesEXT PhysicalDevice::getDescriptorIndexingFeatures() const
{
    VkPhysicalDeviceDescriptorIndexingFeaturesEXT descriptorIndexingFeatures = {};
    descriptorIndexingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES_EXT;
    getFeatures2(&descriptorIndexingFeatures);
    return descriptorIndexingFeatures;
}

// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceDescriptorIndexingPropertiesEXT PhysicalDevice::getDescriptorIndexingProperties() const
{
    VkPhysicalDeviceDescriptorIndexingPropertiesEXT descriptorIndexingProperties = {};
    descriptorIndexingProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_PROPERTIES_EXT;
    getProperties2(&descriptorIndexingProperties);
    return descriptorIndexingProperties;
}
#endif // VK_EXT_descriptor_indexing

#ifdef VK_EXT_device_memory_report
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceDeviceMemoryReportFeaturesEXT PhysicalDevice::getDeviceMemoryReportFeatures() const
{
    VkPhysicalDeviceDeviceMemoryReportFeaturesEXT deviceMemoryReportFeatures = {};
    deviceMemoryReportFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEVICE_MEMORY_REPORT_FEATURES_EXT;
    getFeatures2(&deviceMemoryReportFeatures);
    return deviceMemoryReportFeatures;
}
#endif // VK_EXT_device_memory_report

#ifdef VK_EXT_discard_rectangles
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceDiscardRectanglePropertiesEXT PhysicalDevice::getDiscardRectangleProperties() const
{
    VkPhysicalDeviceDiscardRectanglePropertiesEXT discardRectangleProperties = {};
    discardRectangleProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DISCARD_RECTANGLE_PROPERTIES_EXT;
    getProperties2(&discardRectangleProperties);
    return discardRectangleProperties;
}
#endif // VK_EXT_discard_rectangles

#ifdef VK_EXT_extended_dynamic_state
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceExtendedDynamicStateFeaturesEXT PhysicalDevice::getExtendedDynamicStateFeatures() const
{
    VkPhysicalDeviceExtendedDynamicStateFeaturesEXT extendedDynamicStateFeatures = {};
    extendedDynamicStateFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT;
    getFeatures2(&extendedDynamicStateFeatures);
    return extendedDynamicStateFeatures;
}
#endif // VK_EXT_extended_dynamic_state

#ifdef VK_EXT_extended_dynamic_state2
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceExtendedDynamicState2FeaturesEXT PhysicalDevice::getExtendedDynamicState2Features() const
{
    VkPhysicalDeviceExtendedDynamicState2FeaturesEXT extendedDynamicState2Features = {};
    extendedDynamicState2Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_2_FEATURES_EXT;
    getFeatures2(&extendedDynamicState2Features);
    return extendedDynamicState2Features;
}
#endif // VK_EXT_extended_dynamic_state2

#ifdef VK_EXT_external_memory_host
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceExternalMemoryHostPropertiesEXT PhysicalDevice::getExternalMemoryHostProperties() const
{
    VkPhysicalDeviceExternalMemoryHostPropertiesEXT externalMemoryHostProperties = {};
    externalMemoryHostProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_MEMORY_HOST_PROPERTIES_EXT;
    getProperties2(&externalMemoryHostProperties);
    return externalMemoryHostProperties;
}
#endif // VK_EXT_external_memory_host

#ifdef VK_EXT_fragment_density_map
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceFragmentDensityMapFeaturesEXT PhysicalDevice::getFragmentDensityMapFeatures() const
{
    VkPhysicalDeviceFragmentDensityMapFeaturesEXT fragmentDensityMapFeatures = {};
    fragmentDensityMapFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_DENSITY_MAP_FEATURES_EXT;
    getFeatures2(&fragmentDensityMapFeatures);
    return fragmentDensityMapFeatures;
}

// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceFragmentDensityMapPropertiesEXT PhysicalDevice::getFragmentDensityMapProperties() const
{
    VkPhysicalDeviceFragmentDensityMapPropertiesEXT fragmentDensityMapProperties = {};
    fragmentDensityMapProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_DENSITY_MAP_PROPERTIES_EXT;
    getProperties2(&fragmentDensityMapProperties);
    return fragmentDensityMapProperties;
}
#endif // VK_EXT_fragment_density_map

#ifdef VK_EXT_fragment_density_map2
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceFragmentDensityMap2FeaturesEXT PhysicalDevice::getFragmentDensityMap2Features() const
{
    VkPhysicalDeviceFragmentDensityMap2FeaturesEXT fragmentDensityMap2Features = {};
    fragmentDensityMap2Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_DENSITY_MAP_2_FEATURES_EXT;
    getFeatures2(&fragmentDensityMap2Features);
    return fragmentDensityMap2Features;
}

// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceFragmentDensityMap2PropertiesEXT PhysicalDevice::getFragmentDensityMap2Properties() const
{
    VkPhysicalDeviceFragmentDensityMap2PropertiesEXT fragmentDensityMap2Properties = {};
    fragmentDensityMap2Properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_DENSITY_MAP_2_PROPERTIES_EXT;
    getProperties2(&fragmentDensityMap2Properties);
    return fragmentDensityMap2Properties;
}
#endif // VK_EXT_fragment_density_map2

#ifdef VK_EXT_fragment_shader_interlock
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceFragmentShaderInterlockFeaturesEXT PhysicalDevice::getFragmentShaderInterlockFeatures() const
{
    VkPhysicalDeviceFragmentShaderInterlockFeaturesEXT fragmentShaderInterlockFeatures = {};
    fragmentShaderInterlockFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADER_INTERLOCK_FEATURES_EXT;
    getFeatures2(&fragmentShaderInterlockFeatures);
    return fragmentShaderInterlockFeatures;
}
#endif // VK_EXT_fragment_shader_interlock

#ifdef VK_EXT_graphics_pipeline_library
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceGraphicsPipelineLibraryFeaturesEXT PhysicalDevice::getGraphicsPipelineLibraryFeatures() const
{
    VkPhysicalDeviceGraphicsPipelineLibraryFeaturesEXT graphicsPipelineLibraryFeatures = {};
    graphicsPipelineLibraryFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_GRAPHICS_PIPELINE_LIBRARY_FEATURES_EXT;
    getFeatures2(&graphicsPipelineLibraryFeatures);
    return graphicsPipelineLibraryFeatures;
}

// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceGraphicsPipelineLibraryPropertiesEXT PhysicalDevice::getGraphicsPipelineLibraryProperties() const
{
    VkPhysicalDeviceGraphicsPipelineLibraryPropertiesEXT graphicsPipelineLibraryProperties = {};
    graphicsPipelineLibraryProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_GRAPHICS_PIPELINE_LIBRARY_PROPERTIES_EXT;
    getProperties2(&graphicsPipelineLibraryProperties);
    return graphicsPipelineLibraryProperties;
}
#endif // VK_EXT_graphics_pipeline_library

#ifdef VK_EXT_host_query_reset
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceHostQueryResetFeaturesEXT PhysicalDevice::getHostQueryResetFeatures() const
{
    VkPhysicalDeviceHostQueryResetFeaturesEXT hostQueryResetFeatures = {};
    hostQueryResetFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_QUERY_RESET_FEATURES_EXT;
    getFeatures2(&hostQueryResetFeatures);
    return hostQueryResetFeatures;
}
#endif // VK_EXT_host_query_reset

#ifdef VK_EXT_image_2d_view_of_3d
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceImage2DViewOf3DFeaturesEXT PhysicalDevice::getImage2DViewOf3DFeatures() const
{
    VkPhysicalDeviceImage2DViewOf3DFeaturesEXT image2DViewOf3DFeatures = {};
    image2DViewOf3DFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_2D_VIEW_OF_3D_FEATURES_EXT;
    getFeatures2(&image2DViewOf3DFeatures);
    return image2DViewOf3DFeatures;
}
#endif // VK_EXT_image_2d_view_of_3d

#ifdef VK_EXT_image_robustness
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceImageRobustnessFeaturesEXT PhysicalDevice::getImageRobustnessFeatures() const
{
    VkPhysicalDeviceImageRobustnessFeaturesEXT imageRobustnessFeatures = {};
    imageRobustnessFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_ROBUSTNESS_FEATURES_EXT;
    getFeatures2(&imageRobustnessFeatures);
    return imageRobustnessFeatures;
}
#endif // VK_EXT_image_robustness

#ifdef VK_EXT_image_view_min_lod
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceImageViewMinLodFeaturesEXT PhysicalDevice::getImageViewMinLodFeatures() const
{
    VkPhysicalDeviceImageViewMinLodFeaturesEXT imageViewMinLodFeatures = {};
    imageViewMinLodFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_VIEW_MIN_LOD_FEATURES_EXT;
    getFeatures2(&imageViewMinLodFeatures);
    return imageViewMinLodFeatures;
}
#endif // VK_EXT_image_view_min_lod

#ifdef VK_EXT_index_type_uint8
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceIndexTypeUint8FeaturesEXT PhysicalDevice::getIndexTypeUint8Features() const
{
    VkPhysicalDeviceIndexTypeUint8FeaturesEXT indexTypeUint8Features = {};
    indexTypeUint8Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INDEX_TYPE_UINT8_FEATURES_EXT;
    getFeatures2(&indexTypeUint8Features);
    return indexTypeUint8Features;
}
#endif // VK_EXT_index_type_uint8

#ifdef VK_EXT_inline_uniform_block
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceInlineUniformBlockFeaturesEXT PhysicalDevice::getInlineUniformBlockFeatures() const
{
    VkPhysicalDeviceInlineUniformBlockFeaturesEXT inlineUniformBlockFeatures = {};
    inlineUniformBlockFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_FEATURES_EXT;
    getFeatures2(&inlineUniformBlockFeatures);
    return inlineUniformBlockFeatures;
}

// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceInlineUniformBlockPropertiesEXT PhysicalDevice::getInlineUniformBlockProperties() const
{
    VkPhysicalDeviceInlineUniformBlockPropertiesEXT inlineUniformBlockProperties = {};
    inlineUniformBlockProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_PROPERTIES_EXT;
    getProperties2(&inlineUniformBlockProperties);
    return inlineUniformBlockProperties;
}
#endif // VK_EXT_inline_uniform_block

#ifdef VK_EXT_line_rasterization
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceLineRasterizationFeaturesEXT PhysicalDevice::getLineRasterizationFeatures() const
{
    VkPhysicalDeviceLineRasterizationFeaturesEXT lineRasterizationFeatures = {};
    lineRasterizationFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_LINE_RASTERIZATION_FEATURES_EXT;
    getFeatures2(&lineRasterizationFeatures);
    return lineRasterizationFeatures;
}

// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceLineRasterizationPropertiesEXT PhysicalDevice::getLineRasterizationProperties() const
{
    VkPhysicalDeviceLineRasterizationPropertiesEXT lineRasterizationProperties = {};
    lineRasterizationProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_LINE_RASTERIZATION_PROPERTIES_EXT;
    getProperties2(&lineRasterizationProperties);
    return lineRasterizationProperties;
}
#endif // VK_EXT_line_rasterization

#ifdef VK_EXT_memory_budget
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceMemoryBudgetPropertiesEXT PhysicalDevice::getMemoryBudgetProperties() const
{
    VkPhysicalDeviceMemoryBudgetPropertiesEXT memoryBudgetProperties = {};
    memoryBudgetProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_BUDGET_PROPERTIES_EXT;
    getProperties2(&memoryBudgetProperties);
    return memoryBudgetProperties;
}
#endif // VK_EXT_memory_budget

#ifdef VK_EXT_memory_priority
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceMemoryPriorityFeaturesEXT PhysicalDevice::getMemoryPriorityFeatures() const
{
    VkPhysicalDeviceMemoryPriorityFeaturesEXT memoryPriorityFeatures = {};
    memoryPriorityFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PRIORITY_FEATURES_EXT;
    getFeatures2(&memoryPriorityFeatures);
    return memoryPriorityFeatures;
}
#endif // VK_EXT_memory_priority

#ifdef VK_EXT_multi_draw
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceMultiDrawFeaturesEXT PhysicalDevice::getMultiDrawFeatures() const
{
    VkPhysicalDeviceMultiDrawFeaturesEXT multiDrawFeatures = {};
    multiDrawFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTI_DRAW_FEATURES_EXT;
    getFeatures2(&multiDrawFeatures);
    return multiDrawFeatures;
}

// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceMultiDrawPropertiesEXT PhysicalDevice::getMultiDrawProperties() const
{
    VkPhysicalDeviceMultiDrawPropertiesEXT multiDrawProperties = {};
    multiDrawProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTI_DRAW_PROPERTIES_EXT;
    getProperties2(&multiDrawProperties);
    return multiDrawProperties;
}
#endif // VK_EXT_multi_draw

#ifdef VK_EXT_pageable_device_local_memory
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDevicePageableDeviceLocalMemoryFeaturesEXT PhysicalDevice::getPageableDeviceLocalMemoryFeatures() const
{
    VkPhysicalDevicePageableDeviceLocalMemoryFeaturesEXT pageableDeviceLocalMemoryFeatures = {};
    pageableDeviceLocalMemoryFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PAGEABLE_DEVICE_LOCAL_MEMORY_FEATURES_EXT;
    getFeatures2(&pageableDeviceLocalMemoryFeatures);
    return pageableDeviceLocalMemoryFeatures;
}
#endif // VK_EXT_pageable_device_local_memory

#ifdef VK_EXT_pci_bus_info
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDevicePCIBusInfoPropertiesEXT PhysicalDevice::getPCIBusInfoProperties() const
{
    VkPhysicalDevicePCIBusInfoPropertiesEXT pciBusInfoProperties = {};
    pciBusInfoProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PCI_BUS_INFO_PROPERTIES_EXT;
    getProperties2(&pciBusInfoProperties);
    return pciBusInfoProperties;
}
#endif // VK_EXT_pci_bus_info

#ifdef VK_EXT_physical_device_drm
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceDrmPropertiesEXT PhysicalDevice::getDrmProperties() const
{
    VkPhysicalDeviceDrmPropertiesEXT drmProperties = {};
    drmProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DRM_PROPERTIES_EXT;
    getProperties2(&drmProperties);
    return drmProperties;
}
#endif // VK_EXT_physical_device_drm

#ifdef VK_EXT_pipeline_creation_cache_control
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDevicePipelineCreationCacheControlFeaturesEXT PhysicalDevice::getPipelineCreationCacheControlFeatures() const
{
    VkPhysicalDevicePipelineCreationCacheControlFeaturesEXT pipelineCreationCacheControlFeatures = {};
    pipelineCreationCacheControlFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PIPELINE_CREATION_CACHE_CONTROL_FEATURES_EXT;
    getFeatures2(&pipelineCreationCacheControlFeatures);
    return pipelineCreationCacheControlFeatures;
}
#endif // VK_EXT_pipeline_creation_cache_control

#ifdef VK_EXT_primitive_topology_list_restart
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDevicePrimitiveTopologyListRestartFeaturesEXT PhysicalDevice::getPrimitiveTopologyListRestartFeatures() const
{
    VkPhysicalDevicePrimitiveTopologyListRestartFeaturesEXT primitiveTopologyListRestartFeatures = {};
    primitiveTopologyListRestartFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRIMITIVE_TOPOLOGY_LIST_RESTART_FEATURES_EXT;
    getFeatures2(&primitiveTopologyListRestartFeatures);
    return primitiveTopologyListRestartFeatures;
}
#endif // VK_EXT_primitive_topology_list_restart

#ifdef VK_EXT_primitives_generated_query
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDevicePrimitivesGeneratedQueryFeaturesEXT PhysicalDevice::getPrimitivesGeneratedQueryFeatures() const
{
    VkPhysicalDevicePrimitivesGeneratedQueryFeaturesEXT primitivesGeneratedQueryFeatures = {};
    primitivesGeneratedQueryFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRIMITIVES_GENERATED_QUERY_FEATURES_EXT;
    getFeatures2(&primitivesGeneratedQueryFeatures);
    return primitivesGeneratedQueryFeatures;
}
#endif // VK_EXT_primitives_generated_query

#ifdef VK_EXT_private_data
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDevicePrivateDataFeaturesEXT PhysicalDevice::getPrivateDataFeatures() const
{
    VkPhysicalDevicePrivateDataFeaturesEXT privateDataFeatures = {};
    privateDataFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRIVATE_DATA_FEATURES_EXT;
    getFeatures2(&privateDataFeatures);
    return privateDataFeatures;
}
#endif // VK_EXT_private_data

#ifdef VK_EXT_provoking_vertex
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceProvokingVertexFeaturesEXT PhysicalDevice::getProvokingVertexFeatures() const
{
    VkPhysicalDeviceProvokingVertexFeaturesEXT provokingVertexFeatures = {};
    provokingVertexFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROVOKING_VERTEX_FEATURES_EXT;
    getFeatures2(&provokingVertexFeatures);
    return provokingVertexFeatures;
}

// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceProvokingVertexPropertiesEXT PhysicalDevice::getProvokingVertexProperties() const
{
    VkPhysicalDeviceProvokingVertexPropertiesEXT provokingVertexProperties = {};
    provokingVertexProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROVOKING_VERTEX_PROPERTIES_EXT;
    getProperties2(&provokingVertexProperties);
    return provokingVertexProperties;
}
#endif // VK_EXT_provoking_vertex

#ifdef VK_EXT_rgba10x6_formats
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceRGBA10X6FormatsFeaturesEXT PhysicalDevice::getRGBA10X6FormatsFeatures() const
{
    VkPhysicalDeviceRGBA10X6FormatsFeaturesEXT rgba10x6FormatsFeatures = {};
    rgba10x6FormatsFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RGBA10X6_FORMATS_FEATURES_EXT;
    getFeatures2(&rgba10x6FormatsFeatures);
    return rgba10x6FormatsFeatures;
}
#endif // VK_EXT_rgba10x6_formats

#ifdef VK_EXT_robustness2
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceRobustness2FeaturesEXT PhysicalDevice::getRobustness2Features() const
{
    VkPhysicalDeviceRobustness2FeaturesEXT robustness2Features = {};
    robustness2Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_FEATURES_EXT;
    getFeatures2(&robustness2Features);
    return robustness2Features;
}

// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceRobustness2PropertiesEXT PhysicalDevice::getRobustness2Properties() const
{
    VkPhysicalDeviceRobustness2PropertiesEXT robustness2Properties = {};
    robustness2Properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_PROPERTIES_EXT;
    getProperties2(&robustness2Properties);
    return robustness2Properties;
}
#endif // VK_EXT_robustness2

#ifdef VK_EXT_sample_locations
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceSampleLocationsPropertiesEXT PhysicalDevice::getSampleLocationsProperties() const
{
    VkPhysicalDeviceSampleLocationsPropertiesEXT sampleLocationsProperties = {};
    sampleLocationsProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SAMPLE_LOCATIONS_PROPERTIES_EXT;
    getProperties2(&sampleLocationsProperties);
    return sampleLocationsProperties;
}
#endif // VK_EXT_sample_locations

#ifdef VK_EXT_sampler_filter_minmax
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceSamplerFilterMinmaxPropertiesEXT PhysicalDevice::getSamplerFilterMinmaxProperties() const
{
    VkPhysicalDeviceSamplerFilterMinmaxPropertiesEXT samplerFilterMinmaxProperties = {};
    samplerFilterMinmaxProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SAMPLER_FILTER_MINMAX_PROPERTIES_EXT;
    getProperties2(&samplerFilterMinmaxProperties);
    return samplerFilterMinmaxProperties;
}
#endif // VK_EXT_sampler_filter_minmax

#ifdef VK_EXT_scalar_block_layout
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceScalarBlockLayoutFeaturesEXT PhysicalDevice::getScalarBlockLayoutFeatures() const
{
    VkPhysicalDeviceScalarBlockLayoutFeaturesEXT scalarBlockLayoutFeatures = {};
    scalarBlockLayoutFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SCALAR_BLOCK_LAYOUT_FEATURES_EXT;
    getFeatures2(&scalarBlockLayoutFeatures);
    return scalarBlockLayoutFeatures;
}
#endif // VK_EXT_scalar_block_layout

#ifdef VK_EXT_shader_atomic_float
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceShaderAtomicFloatFeaturesEXT PhysicalDevice::getShaderAtomicFloatFeatures() const
{
    VkPhysicalDeviceShaderAtomicFloatFeaturesEXT shaderAtomicFloatFeatures = {};
    shaderAtomicFloatFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_ATOMIC_FLOAT_FEATURES_EXT;
    getFeatures2(&shaderAtomicFloatFeatures);
    return shaderAtomicFloatFeatures;
}
#endif // VK_EXT_shader_atomic_float

#ifdef VK_EXT_shader_atomic_float2
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceShaderAtomicFloat2FeaturesEXT PhysicalDevice::getShaderAtomicFloat2Features() const
{
    VkPhysicalDeviceShaderAtomicFloat2FeaturesEXT shaderAtomicFloat2Features = {};
    shaderAtomicFloat2Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_ATOMIC_FLOAT_2_FEATURES_EXT;
    getFeatures2(&shaderAtomicFloat2Features);
    return shaderAtomicFloat2Features;
}
#endif // VK_EXT_shader_atomic_float2

#ifdef VK_EXT_shader_demote_to_helper_invocation
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceShaderDemoteToHelperInvocationFeaturesEXT PhysicalDevice::getShaderDemoteToHelperInvocationFeatures() const
{
    VkPhysicalDeviceShaderDemoteToHelperInvocationFeaturesEXT shaderDemoteToHelperInvocationFeatures = {};
    shaderDemoteToHelperInvocationFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_DEMOTE_TO_HELPER_INVOCATION_FEATURES_EXT;
    getFeatures2(&shaderDemoteToHelperInvocationFeatures);
    return shaderDemoteToHelperInvocationFeatures;
}
#endif // VK_EXT_shader_demote_to_helper_invocation

#ifdef VK_EXT_shader_image_atomic_int64
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceShaderImageAtomicInt64FeaturesEXT PhysicalDevice::getShaderImageAtomicInt64Features() const
{
    VkPhysicalDeviceShaderImageAtomicInt64FeaturesEXT shaderImageAtomicInt64Features = {};
    shaderImageAtomicInt64Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_IMAGE_ATOMIC_INT64_FEATURES_EXT;
    getFeatures2(&shaderImageAtomicInt64Features);
    return shaderImageAtomicInt64Features;
}
#endif // VK_EXT_shader_image_atomic_int64

#ifdef VK_EXT_subgroup_size_control
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceSubgroupSizeControlFeaturesEXT PhysicalDevice::getSubgroupSizeControlFeatures() const
{
    VkPhysicalDeviceSubgroupSizeControlFeaturesEXT subgroupSizeControlFeatures = {};
    subgroupSizeControlFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_SIZE_CONTROL_FEATURES_EXT;
    getFeatures2(&subgroupSizeControlFeatures);
    return subgroupSizeControlFeatures;
}

// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceSubgroupSizeControlPropertiesEXT PhysicalDevice::getSubgroupSizeControlProperties() const
{
    VkPhysicalDeviceSubgroupSizeControlPropertiesEXT subgroupSizeControlProperties = {};
    subgroupSizeControlProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_SIZE_CONTROL_PROPERTIES_EXT;
    getProperties2(&subgroupSizeControlProperties);
    return subgroupSizeControlProperties;
}
#endif // VK_EXT_subgroup_size_control

#ifdef VK_EXT_texel_buffer_alignment
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceTexelBufferAlignmentFeaturesEXT PhysicalDevice::getTexelBufferAlignmentFeatures() const
{
    VkPhysicalDeviceTexelBufferAlignmentFeaturesEXT texelBufferAlignmentFeatures = {};
    texelBufferAlignmentFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXEL_BUFFER_ALIGNMENT_FEATURES_EXT;
    getFeatures2(&texelBufferAlignmentFeatures);
    return texelBufferAlignmentFeatures;
}

// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceTexelBufferAlignmentPropertiesEXT PhysicalDevice::getTexelBufferAlignmentProperties() const
{
    VkPhysicalDeviceTexelBufferAlignmentPropertiesEXT texelBufferAlignmentProperties = {};
    texelBufferAlignmentProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXEL_BUFFER_ALIGNMENT_PROPERTIES_EXT;
    getProperties2(&texelBufferAlignmentProperties);
    return texelBufferAlignmentProperties;
}
#endif // VK_EXT_texel_buffer_alignment

#ifdef VK_EXT_texture_compression_astc_hdr
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceTextureCompressionASTCHDRFeaturesEXT PhysicalDevice::getTextureCompressionASTCHDRFeatures() const
{
    VkPhysicalDeviceTextureCompressionASTCHDRFeaturesEXT textureCompressionASTCHDRFeatures = {};
    textureCompressionASTCHDRFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXTURE_COMPRESSION_ASTC_HDR_FEATURES_EXT;
    getFeatures2(&textureCompressionASTCHDRFeatures);
    return textureCompressionASTCHDRFeatures;
}
#endif // VK_EXT_texture_compression_astc_hdr

#ifdef VK_EXT_transform_feedback
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceTransformFeedbackFeaturesEXT PhysicalDevice::getTransformFeedbackFeatures() const
{
    VkPhysicalDeviceTransformFeedbackFeaturesEXT transformFeedbackFeatures = {};
    transformFeedbackFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TRANSFORM_FEEDBACK_FEATURES_EXT;
    getFeatures2(&transformFeedbackFeatures);
    return transformFeedbackFeatures;
}

// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceTransformFeedbackPropertiesEXT PhysicalDevice::getTransformFeedbackProperties() const
{
    VkPhysicalDeviceTransformFeedbackPropertiesEXT transformFeedbackProperties = {};
    transformFeedbackProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TRANSFORM_FEEDBACK_PROPERTIES_EXT;
    getProperties2(&transformFeedbackProperties);
    return transformFeedbackProperties;
}
#endif // VK_EXT_transform_feedback

#ifdef VK_EXT_vertex_attribute_divisor
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT PhysicalDevice::getVertexAttributeDivisorFeatures() const
{
    VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT vertexAttributeDivisorFeatures = {};
    vertexAttributeDivisorFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_DIVISOR_FEATURES_EXT;
    getFeatures2(&vertexAttributeDivisorFeatures);
    return vertexAttributeDivisorFeatures;
}

// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceVertexAttributeDivisorPropertiesEXT PhysicalDevice::getVertexAttributeDivisorProperties() const
{
    VkPhysicalDeviceVertexAttributeDivisorPropertiesEXT vertexAttributeDivisorProperties = {};
    vertexAttributeDivisorProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_DIVISOR_PROPERTIES_EXT;
    getProperties2(&vertexAttributeDivisorProperties);
    return vertexAttributeDivisorProperties;
}
#endif // VK_EXT_vertex_attribute_divisor

#ifdef VK_EXT_vertex_input_dynamic_state
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceVertexInputDynamicStateFeaturesEXT PhysicalDevice::getVertexInputDynamicStateFeatures() const
{
    VkPhysicalDeviceVertexInputDynamicStateFeaturesEXT vertexInputDynamicStateFeatures = {};
    vertexInputDynamicStateFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_INPUT_DYNAMIC_STATE_FEATURES_EXT;
    getFeatures2(&vertexInputDynamicStateFeatures);
    return vertexInputDynamicStateFeatures;
}
#endif // VK_EXT_vertex_input_dynamic_state

#ifdef VK_EXT_ycbcr_2plane_444_formats
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceYcbcr2Plane444FormatsFeaturesEXT PhysicalDevice::getYcbcr2Plane444FormatsFeatures() const
{
    VkPhysicalDeviceYcbcr2Plane444FormatsFeaturesEXT ycbcr2Plane444FormatsFeatures = {};
    ycbcr2Plane444FormatsFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_YCBCR_2_PLANE_444_FORMATS_FEATURES_EXT;
    getFeatures2(&ycbcr2Plane444FormatsFeatures);
    return ycbcr2Plane444FormatsFeatures;
}
#endif // VK_EXT_ycbcr_2plane_444_formats

#ifdef VK_EXT_ycbcr_image_arrays
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceYcbcrImageArraysFeaturesEXT PhysicalDevice::getYcbcrImageArraysFeatures() const
{
    VkPhysicalDeviceYcbcrImageArraysFeaturesEXT ycbcrImageArraysFeatures = {};
    ycbcrImageArraysFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_YCBCR_IMAGE_ARRAYS_FEATURES_EXT;
    getFeatures2(&ycbcrImageArraysFeatures);
    return ycbcrImageArraysFeatures;
}
#endif // VK_EXT_ycbcr_image_arrays

#ifdef VK_HUAWEI_invocation_mask
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceInvocationMaskFeaturesHUAWEI PhysicalDevice::getInvocationMaskFeatures() const
{
    VkPhysicalDeviceInvocationMaskFeaturesHUAWEI invocationMaskFeatures = {};
    invocationMaskFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INVOCATION_MASK_FEATURES_HUAWEI;
    getFeatures2(&invocationMaskFeatures);
    return invocationMaskFeatures;
}
#endif // VK_HUAWEI_invocation_mask

#ifdef VK_HUAWEI_subpass_shading
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceSubpassShadingFeaturesHUAWEI PhysicalDevice::getSubpassShadingFeatures() const
{
    VkPhysicalDeviceSubpassShadingFeaturesHUAWEI subpassShadingFeatures = {};
    subpassShadingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBPASS_SHADING_FEATURES_HUAWEI;
    getFeatures2(&subpassShadingFeatures);
    return subpassShadingFeatures;
}

// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceSubpassShadingPropertiesHUAWEI PhysicalDevice::getSubpassShadingProperties() const
{
    VkPhysicalDeviceSubpassShadingPropertiesHUAWEI subpassShadingProperties = {};
    subpassShadingProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBPASS_SHADING_PROPERTIES_HUAWEI;
    getProperties2(&subpassShadingProperties);
    return subpassShadingProperties;
}
#endif // VK_HUAWEI_subpass_shading

#ifdef VK_INTEL_shader_integer_functions2
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceShaderIntegerFunctions2FeaturesINTEL PhysicalDevice::getShaderIntegerFunctions2Features() const
{
    VkPhysicalDeviceShaderIntegerFunctions2FeaturesINTEL shaderIntegerFunctions2Features = {};
    shaderIntegerFunctions2Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_INTEGER_FUNCTIONS_2_FEATURES_INTEL;
    getFeatures2(&shaderIntegerFunctions2Features);
    return shaderIntegerFunctions2Features;
}
#endif // VK_INTEL_shader_integer_functions2

#ifdef VK_KHR_16bit_storage
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDevice16BitStorageFeaturesKHR PhysicalDevice::get16BitStorageFeatures() const
{
    VkPhysicalDevice16BitStorageFeaturesKHR _16BitStorageFeatures = {};
    _16BitStorageFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_16BIT_STORAGE_FEATURES_KHR;
    getFeatures2(&_16BitStorageFeatures);
    return _16BitStorageFeatures;
}
#endif // VK_KHR_16bit_storage

#ifdef VK_KHR_8bit_storage
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDevice8BitStorageFeaturesKHR PhysicalDevice::get8BitStorageFeatures() const
{
    VkPhysicalDevice8BitStorageFeaturesKHR _8BitStorageFeatures = {};
    _8BitStorageFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_8BIT_STORAGE_FEATURES_KHR;
    getFeatures2(&_8BitStorageFeatures);
    return _8BitStorageFeatures;
}
#endif // VK_KHR_8bit_storage

#ifdef VK_KHR_acceleration_structure
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceAccelerationStructureFeaturesKHR PhysicalDevice::getAccelerationStructureFeatures() const
{
    VkPhysicalDeviceAccelerationStructureFeaturesKHR accelerationStructureFeatures = {};
    accelerationStructureFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
    getFeatures2(&accelerationStructureFeatures);
    return accelerationStructureFeatures;
}

// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceAccelerationStructurePropertiesKHR PhysicalDevice::getAccelerationStructureProperties() const
{
    VkPhysicalDeviceAccelerationStructurePropertiesKHR accelerationStructureProperties = {};
    accelerationStructureProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_PROPERTIES_KHR;
    getProperties2(&accelerationStructureProperties);
    return accelerationStructureProperties;
}
#endif // VK_KHR_acceleration_structure

#ifdef VK_KHR_buffer_device_address
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceBufferDeviceAddressFeaturesKHR PhysicalDevice::getBufferDeviceAddressFeatures() const
{
    VkPhysicalDeviceBufferDeviceAddressFeaturesKHR bufferDeviceAddressFeatures = {};
    bufferDeviceAddressFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES_KHR;
    getFeatures2(&bufferDeviceAddressFeatures);
    return bufferDeviceAddressFeatures;
}
#endif // VK_KHR_buffer_device_address

#ifdef VK_KHR_depth_stencil_resolve
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceDepthStencilResolvePropertiesKHR PhysicalDevice::getDepthStencilResolveProperties() const
{
    VkPhysicalDeviceDepthStencilResolvePropertiesKHR depthStencilResolveProperties = {};
    depthStencilResolveProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_STENCIL_RESOLVE_PROPERTIES_KHR;
    getProperties2(&depthStencilResolveProperties);
    return depthStencilResolveProperties;
}
#endif // VK_KHR_depth_stencil_resolve

#ifdef VK_KHR_device_group_creation
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceGroupPropertiesKHR PhysicalDevice::getGroupProperties() const
{
    VkPhysicalDeviceGroupPropertiesKHR groupProperties = {};
    groupProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_GROUP_PROPERTIES_KHR;
    getProperties2(&groupProperties);
    return groupProperties;
}
#endif // VK_KHR_device_group_creation

#ifdef VK_KHR_driver_properties
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceDriverPropertiesKHR PhysicalDevice::getDriverProperties() const
{
    VkPhysicalDeviceDriverPropertiesKHR driverProperties = {};
    driverProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DRIVER_PROPERTIES_KHR;
    getProperties2(&driverProperties);
    return driverProperties;
}
#endif // VK_KHR_driver_properties

#ifdef VK_KHR_dynamic_rendering
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceDynamicRenderingFeaturesKHR PhysicalDevice::getDynamicRenderingFeatures() const
{
    VkPhysicalDeviceDynamicRenderingFeaturesKHR dynamicRenderingFeatures = {};
    dynamicRenderingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR;
    getFeatures2(&dynamicRenderingFeatures);
    return dynamicRenderingFeatures;
}
#endif // VK_KHR_dynamic_rendering

#ifdef VK_KHR_external_memory_capabilities
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceIDPropertiesKHR PhysicalDevice::getIDProperties() const
{
    VkPhysicalDeviceIDPropertiesKHR idProperties = {};
    idProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ID_PROPERTIES_KHR;
    getProperties2(&idProperties);
    return idProperties;
}
#endif // VK_KHR_external_memory_capabilities

#ifdef VK_KHR_fragment_shading_rate
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceFragmentShadingRateFeaturesKHR PhysicalDevice::getFragmentShadingRateFeatures() const
{
    VkPhysicalDeviceFragmentShadingRateFeaturesKHR fragmentShadingRateFeatures = {};
    fragmentShadingRateFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_FEATURES_KHR;
    getFeatures2(&fragmentShadingRateFeatures);
    return fragmentShadingRateFeatures;
}

// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceFragmentShadingRatePropertiesKHR PhysicalDevice::getFragmentShadingRateProperties() const
{
    VkPhysicalDeviceFragmentShadingRatePropertiesKHR fragmentShadingRateProperties = {};
    fragmentShadingRateProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_PROPERTIES_KHR;
    getProperties2(&fragmentShadingRateProperties);
    return fragmentShadingRateProperties;
}
#endif // VK_KHR_fragment_shading_rate

#ifdef VK_KHR_global_priority
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceGlobalPriorityQueryFeaturesKHR PhysicalDevice::getGlobalPriorityQueryFeatures() const
{
    VkPhysicalDeviceGlobalPriorityQueryFeaturesKHR globalPriorityQueryFeatures = {};
    globalPriorityQueryFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_GLOBAL_PRIORITY_QUERY_FEATURES_KHR;
    getFeatures2(&globalPriorityQueryFeatures);
    return globalPriorityQueryFeatures;
}
#endif // VK_KHR_global_priority

#ifdef VK_KHR_imageless_framebuffer
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceImagelessFramebufferFeaturesKHR PhysicalDevice::getImagelessFramebufferFeatures() const
{
    VkPhysicalDeviceImagelessFramebufferFeaturesKHR imagelessFramebufferFeatures = {};
    imagelessFramebufferFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGELESS_FRAMEBUFFER_FEATURES_KHR;
    getFeatures2(&imagelessFramebufferFeatures);
    return imagelessFramebufferFeatures;
}
#endif // VK_KHR_imageless_framebuffer

#ifdef VK_KHR_maintenance2
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDevicePointClippingPropertiesKHR PhysicalDevice::getPointClippingProperties() const
{
    VkPhysicalDevicePointClippingPropertiesKHR pointClippingProperties = {};
    pointClippingProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_POINT_CLIPPING_PROPERTIES_KHR;
    getProperties2(&pointClippingProperties);
    return pointClippingProperties;
}
#endif // VK_KHR_maintenance2

#ifdef VK_KHR_maintenance3
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceMaintenance3PropertiesKHR PhysicalDevice::getMaintenance3Properties() const
{
    VkPhysicalDeviceMaintenance3PropertiesKHR maintenance3Properties = {};
    maintenance3Properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_3_PROPERTIES_KHR;
    getProperties2(&maintenance3Properties);
    return maintenance3Properties;
}
#endif // VK_KHR_maintenance3

#ifdef VK_KHR_maintenance4
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceMaintenance4FeaturesKHR PhysicalDevice::getMaintenance4Features() const
{
    VkPhysicalDeviceMaintenance4FeaturesKHR maintenance4Features = {};
    maintenance4Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_4_FEATURES_KHR;
    getFeatures2(&maintenance4Features);
    return maintenance4Features;
}

// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceMaintenance4PropertiesKHR PhysicalDevice::getMaintenance4Properties() const
{
    VkPhysicalDeviceMaintenance4PropertiesKHR maintenance4Properties = {};
    maintenance4Properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_4_PROPERTIES_KHR;
    getProperties2(&maintenance4Properties);
    return maintenance4Properties;
}
#endif // VK_KHR_maintenance4

#ifdef VK_KHR_multiview
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceMultiviewFeaturesKHR PhysicalDevice::getMultiviewFeatures() const
{
    VkPhysicalDeviceMultiviewFeaturesKHR multiviewFeatures = {};
    multiviewFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_FEATURES_KHR;
    getFeatures2(&multiviewFeatures);
    return multiviewFeatures;
}

// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceMultiviewPropertiesKHR PhysicalDevice::getMultiviewProperties() const
{
    VkPhysicalDeviceMultiviewPropertiesKHR multiviewProperties = {};
    multiviewProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_PROPERTIES_KHR;
    getProperties2(&multiviewProperties);
    return multiviewProperties;
}
#endif // VK_KHR_multiview

#ifdef VK_KHR_performance_query
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDevicePerformanceQueryFeaturesKHR PhysicalDevice::getPerformanceQueryFeatures() const
{
    VkPhysicalDevicePerformanceQueryFeaturesKHR performanceQueryFeatures = {};
    performanceQueryFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PERFORMANCE_QUERY_FEATURES_KHR;
    getFeatures2(&performanceQueryFeatures);
    return performanceQueryFeatures;
}

// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDevicePerformanceQueryPropertiesKHR PhysicalDevice::getPerformanceQueryProperties() const
{
    VkPhysicalDevicePerformanceQueryPropertiesKHR performanceQueryProperties = {};
    performanceQueryProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PERFORMANCE_QUERY_PROPERTIES_KHR;
    getProperties2(&performanceQueryProperties);
    return performanceQueryProperties;
}
#endif // VK_KHR_performance_query

#ifdef VK_KHR_pipeline_executable_properties
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDevicePipelineExecutablePropertiesFeaturesKHR PhysicalDevice::getPipelineExecutablePropertiesFeatures() const
{
    VkPhysicalDevicePipelineExecutablePropertiesFeaturesKHR pipelineExecutablePropertiesFeatures = {};
    pipelineExecutablePropertiesFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PIPELINE_EXECUTABLE_PROPERTIES_FEATURES_KHR;
    getFeatures2(&pipelineExecutablePropertiesFeatures);
    return pipelineExecutablePropertiesFeatures;
}
#endif // VK_KHR_pipeline_executable_properties

#ifdef VK_KHR_present_id
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDevicePresentIdFeaturesKHR PhysicalDevice::getPresentIdFeatures() const
{
    VkPhysicalDevicePresentIdFeaturesKHR presentIdFeatures = {};
    presentIdFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRESENT_ID_FEATURES_KHR;
    getFeatures2(&presentIdFeatures);
    return presentIdFeatures;
}
#endif // VK_KHR_present_id

#ifdef VK_KHR_present_wait
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDevicePresentWaitFeaturesKHR PhysicalDevice::getPresentWaitFeatures() const
{
    VkPhysicalDevicePresentWaitFeaturesKHR presentWaitFeatures = {};
    presentWaitFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRESENT_WAIT_FEATURES_KHR;
    getFeatures2(&presentWaitFeatures);
    return presentWaitFeatures;
}
#endif // VK_KHR_present_wait

#ifdef VK_KHR_push_descriptor
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDevicePushDescriptorPropertiesKHR PhysicalDevice::getPushDescriptorProperties() const
{
    VkPhysicalDevicePushDescriptorPropertiesKHR pushDescriptorProperties = {};
    pushDescriptorProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PUSH_DESCRIPTOR_PROPERTIES_KHR;
    getProperties2(&pushDescriptorProperties);
    return pushDescriptorProperties;
}
#endif // VK_KHR_push_descriptor

#ifdef VK_KHR_ray_query
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceRayQueryFeaturesKHR PhysicalDevice::getRayQueryFeatures() const
{
    VkPhysicalDeviceRayQueryFeaturesKHR rayQueryFeatures = {};
    rayQueryFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_QUERY_FEATURES_KHR;
    getFeatures2(&rayQueryFeatures);
    return rayQueryFeatures;
}
#endif // VK_KHR_ray_query

#ifdef VK_KHR_ray_tracing_pipeline
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceRayTracingPipelineFeaturesKHR PhysicalDevice::getRayTracingPipelineFeatures() const
{
    VkPhysicalDeviceRayTracingPipelineFeaturesKHR rayTracingPipelineFeatures = {};
    rayTracingPipelineFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR;
    getFeatures2(&rayTracingPipelineFeatures);
    return rayTracingPipelineFeatures;
}

// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceRayTracingPipelinePropertiesKHR PhysicalDevice::getRayTracingPipelineProperties() const
{
    VkPhysicalDeviceRayTracingPipelinePropertiesKHR rayTracingPipelineProperties = {};
    rayTracingPipelineProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_PROPERTIES_KHR;
    getProperties2(&rayTracingPipelineProperties);
    return rayTracingPipelineProperties;
}
#endif // VK_KHR_ray_tracing_pipeline

#ifdef VK_KHR_sampler_ycbcr_conversion
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceSamplerYcbcrConversionFeaturesKHR PhysicalDevice::getSamplerYcbcrConversionFeatures() const
{
    VkPhysicalDeviceSamplerYcbcrConversionFeaturesKHR samplerYcbcrConversionFeatures = {};
    samplerYcbcrConversionFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SAMPLER_YCBCR_CONVERSION_FEATURES_KHR;
    getFeatures2(&samplerYcbcrConversionFeatures);
    return samplerYcbcrConversionFeatures;
}
#endif // VK_KHR_sampler_ycbcr_conversion

#ifdef VK_KHR_separate_depth_stencil_layouts
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceSeparateDepthStencilLayoutsFeaturesKHR PhysicalDevice::getSeparateDepthStencilLayoutsFeatures() const
{
    VkPhysicalDeviceSeparateDepthStencilLayoutsFeaturesKHR separateDepthStencilLayoutsFeatures = {};
    separateDepthStencilLayoutsFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SEPARATE_DEPTH_STENCIL_LAYOUTS_FEATURES_KHR;
    getFeatures2(&separateDepthStencilLayoutsFeatures);
    return separateDepthStencilLayoutsFeatures;
}
#endif // VK_KHR_separate_depth_stencil_layouts

#ifdef VK_KHR_shader_atomic_int64
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceShaderAtomicInt64FeaturesKHR PhysicalDevice::getShaderAtomicInt64Features() const
{
    VkPhysicalDeviceShaderAtomicInt64FeaturesKHR shaderAtomicInt64Features = {};
    shaderAtomicInt64Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_ATOMIC_INT64_FEATURES_KHR;
    getFeatures2(&shaderAtomicInt64Features);
    return shaderAtomicInt64Features;
}
#endif // VK_KHR_shader_atomic_int64

#ifdef VK_KHR_shader_clock
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceShaderClockFeaturesKHR PhysicalDevice::getShaderClockFeatures() const
{
    VkPhysicalDeviceShaderClockFeaturesKHR shaderClockFeatures = {};
    shaderClockFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_CLOCK_FEATURES_KHR;
    getFeatures2(&shaderClockFeatures);
    return shaderClockFeatures;
}
#endif // VK_KHR_shader_clock

#ifdef VK_KHR_shader_float16_int8
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceFloat16Int8FeaturesKHR PhysicalDevice::getFloat16Int8Features() const
{
    VkPhysicalDeviceFloat16Int8FeaturesKHR float16Int8Features = {};
    float16Int8Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FLOAT16_INT8_FEATURES_KHR;
    getFeatures2(&float16Int8Features);
    return float16Int8Features;
}
#endif // VK_KHR_shader_float16_int8

#ifdef VK_KHR_shader_float_controls
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceFloatControlsPropertiesKHR PhysicalDevice::getFloatControlsProperties() const
{
    VkPhysicalDeviceFloatControlsPropertiesKHR floatControlsProperties = {};
    floatControlsProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FLOAT_CONTROLS_PROPERTIES_KHR;
    getProperties2(&floatControlsProperties);
    return floatControlsProperties;
}
#endif // VK_KHR_shader_float_controls

#ifdef VK_KHR_shader_integer_dot_product
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceShaderIntegerDotProductFeaturesKHR PhysicalDevice::getShaderIntegerDotProductFeatures() const
{
    VkPhysicalDeviceShaderIntegerDotProductFeaturesKHR shaderIntegerDotProductFeatures = {};
    shaderIntegerDotProductFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_INTEGER_DOT_PRODUCT_FEATURES_KHR;
    getFeatures2(&shaderIntegerDotProductFeatures);
    return shaderIntegerDotProductFeatures;
}

// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceShaderIntegerDotProductPropertiesKHR PhysicalDevice::getShaderIntegerDotProductProperties() const
{
    VkPhysicalDeviceShaderIntegerDotProductPropertiesKHR shaderIntegerDotProductProperties = {};
    shaderIntegerDotProductProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_INTEGER_DOT_PRODUCT_PROPERTIES_KHR;
    getProperties2(&shaderIntegerDotProductProperties);
    return shaderIntegerDotProductProperties;
}
#endif // VK_KHR_shader_integer_dot_product

#ifdef VK_KHR_shader_subgroup_extended_types
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceShaderSubgroupExtendedTypesFeaturesKHR PhysicalDevice::getShaderSubgroupExtendedTypesFeatures() const
{
    VkPhysicalDeviceShaderSubgroupExtendedTypesFeaturesKHR shaderSubgroupExtendedTypesFeatures = {};
    shaderSubgroupExtendedTypesFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_SUBGROUP_EXTENDED_TYPES_FEATURES_KHR;
    getFeatures2(&shaderSubgroupExtendedTypesFeatures);
    return shaderSubgroupExtendedTypesFeatures;
}
#endif // VK_KHR_shader_subgroup_extended_types

#ifdef VK_KHR_shader_subgroup_uniform_control_flow
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceShaderSubgroupUniformControlFlowFeaturesKHR PhysicalDevice::getShaderSubgroupUniformControlFlowFeatures() const
{
    VkPhysicalDeviceShaderSubgroupUniformControlFlowFeaturesKHR shaderSubgroupUniformControlFlowFeatures = {};
    shaderSubgroupUniformControlFlowFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_SUBGROUP_UNIFORM_CONTROL_FLOW_FEATURES_KHR;
    getFeatures2(&shaderSubgroupUniformControlFlowFeatures);
    return shaderSubgroupUniformControlFlowFeatures;
}
#endif // VK_KHR_shader_subgroup_uniform_control_flow

#ifdef VK_KHR_shader_terminate_invocation
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceShaderTerminateInvocationFeaturesKHR PhysicalDevice::getShaderTerminateInvocationFeatures() const
{
    VkPhysicalDeviceShaderTerminateInvocationFeaturesKHR shaderTerminateInvocationFeatures = {};
    shaderTerminateInvocationFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_TERMINATE_INVOCATION_FEATURES_KHR;
    getFeatures2(&shaderTerminateInvocationFeatures);
    return shaderTerminateInvocationFeatures;
}
#endif // VK_KHR_shader_terminate_invocation

#ifdef VK_KHR_synchronization2
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceSynchronization2FeaturesKHR PhysicalDevice::getSynchronization2Features() const
{
    VkPhysicalDeviceSynchronization2FeaturesKHR synchronization2Features = {};
    synchronization2Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES_KHR;
    getFeatures2(&synchronization2Features);
    return synchronization2Features;
}
#endif // VK_KHR_synchronization2

#ifdef VK_KHR_timeline_semaphore
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceTimelineSemaphoreFeaturesKHR PhysicalDevice::getTimelineSemaphoreFeatures() const
{
    VkPhysicalDeviceTimelineSemaphoreFeaturesKHR timelineSemaphoreFeatures = {};
    timelineSemaphoreFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_FEATURES_KHR;
    getFeatures2(&timelineSemaphoreFeatures);
    return timelineSemaphoreFeatures;
}

// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceTimelineSemaphorePropertiesKHR PhysicalDevice::getTimelineSemaphoreProperties() const
{
    VkPhysicalDeviceTimelineSemaphorePropertiesKHR timelineSemaphoreProperties = {};
    timelineSemaphoreProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_PROPERTIES_KHR;
    getProperties2(&timelineSemaphoreProperties);
    return timelineSemaphoreProperties;
}
#endif // VK_KHR_timeline_semaphore

#ifdef VK_KHR_uniform_buffer_standard_layout
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceUniformBufferStandardLayoutFeaturesKHR PhysicalDevice::getUniformBufferStandardLayoutFeatures() const
{
    VkPhysicalDeviceUniformBufferStandardLayoutFeaturesKHR uniformBufferStandardLayoutFeatures = {};
    uniformBufferStandardLayoutFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_UNIFORM_BUFFER_STANDARD_LAYOUT_FEATURES_KHR;
    getFeatures2(&uniformBufferStandardLayoutFeatures);
    return uniformBufferStandardLayoutFeatures;
}
#endif // VK_KHR_uniform_buffer_standard_layout

#ifdef VK_KHR_variable_pointers
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceVariablePointersFeaturesKHR PhysicalDevice::getVariablePointersFeatures() const
{
    VkPhysicalDeviceVariablePointersFeaturesKHR variablePointersFeatures = {};
    variablePointersFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VARIABLE_POINTERS_FEATURES_KHR;
    getFeatures2(&variablePointersFeatures);
    return variablePointersFeatures;
}
#endif // VK_KHR_variable_pointers

#ifdef VK_KHR_vulkan_memory_model
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceVulkanMemoryModelFeaturesKHR PhysicalDevice::getVulkanMemoryModelFeatures() const
{
    VkPhysicalDeviceVulkanMemoryModelFeaturesKHR vulkanMemoryModelFeatures = {};
    vulkanMemoryModelFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_MEMORY_MODEL_FEATURES_KHR;
    getFeatures2(&vulkanMemoryModelFeatures);
    return vulkanMemoryModelFeatures;
}
#endif // VK_KHR_vulkan_memory_model

#ifdef VK_KHR_workgroup_memory_explicit_layout
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceWorkgroupMemoryExplicitLayoutFeaturesKHR PhysicalDevice::getWorkgroupMemoryExplicitLayoutFeatures() const
{
    VkPhysicalDeviceWorkgroupMemoryExplicitLayoutFeaturesKHR workgroupMemoryExplicitLayoutFeatures = {};
    workgroupMemoryExplicitLayoutFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_WORKGROUP_MEMORY_EXPLICIT_LAYOUT_FEATURES_KHR;
    getFeatures2(&workgroupMemoryExplicitLayoutFeatures);
    return workgroupMemoryExplicitLayoutFeatures;
}
#endif // VK_KHR_workgroup_memory_explicit_layout

#ifdef VK_KHR_zero_initialize_workgroup_memory
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceZeroInitializeWorkgroupMemoryFeaturesKHR PhysicalDevice::getZeroInitializeWorkgroupMemoryFeatures() const
{
    VkPhysicalDeviceZeroInitializeWorkgroupMemoryFeaturesKHR zeroInitializeWorkgroupMemoryFeatures = {};
    zeroInitializeWorkgroupMemoryFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ZERO_INITIALIZE_WORKGROUP_MEMORY_FEATURES_KHR;
    getFeatures2(&zeroInitializeWorkgroupMemoryFeatures);
    return zeroInitializeWorkgroupMemoryFeatures;
}
#endif // VK_KHR_zero_initialize_workgroup_memory

#ifdef VK_NVX_multiview_per_view_attributes
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceMultiviewPerViewAttributesPropertiesNVX PhysicalDevice::getMultiviewPerViewAttributesProperties() const
{
    VkPhysicalDeviceMultiviewPerViewAttributesPropertiesNVX multiviewPerViewAttributesProperties = {};
    multiviewPerViewAttributesProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_PER_VIEW_ATTRIBUTES_PROPERTIES_NVX;
    getProperties2(&multiviewPerViewAttributesProperties);
    return multiviewPerViewAttributesProperties;
}
#endif // VK_NVX_multiview_per_view_attributes

#ifdef VK_NV_compute_shader_derivatives
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceComputeShaderDerivativesFeaturesNV PhysicalDevice::getComputeShaderDerivativesFeatures() const
{
    VkPhysicalDeviceComputeShaderDerivativesFeaturesNV computeShaderDerivativesFeatures = {};
    computeShaderDerivativesFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COMPUTE_SHADER_DERIVATIVES_FEATURES_NV;
    getFeatures2(&computeShaderDerivativesFeatures);
    return computeShaderDerivativesFeatures;
}
#endif // VK_NV_compute_shader_derivatives

#ifdef VK_NV_cooperative_matrix
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceCooperativeMatrixFeaturesNV PhysicalDevice::getCooperativeMatrixFeatures() const
{
    VkPhysicalDeviceCooperativeMatrixFeaturesNV cooperativeMatrixFeatures = {};
    cooperativeMatrixFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COOPERATIVE_MATRIX_FEATURES_NV;
    getFeatures2(&cooperativeMatrixFeatures);
    return cooperativeMatrixFeatures;
}

// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceCooperativeMatrixPropertiesNV PhysicalDevice::getCooperativeMatrixProperties() const
{
    VkPhysicalDeviceCooperativeMatrixPropertiesNV cooperativeMatrixProperties = {};
    cooperativeMatrixProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COOPERATIVE_MATRIX_PROPERTIES_NV;
    getProperties2(&cooperativeMatrixProperties);
    return cooperativeMatrixProperties;
}
#endif // VK_NV_cooperative_matrix

#ifdef VK_NV_corner_sampled_image
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceCornerSampledImageFeaturesNV PhysicalDevice::getCornerSampledImageFeatures() const
{
    VkPhysicalDeviceCornerSampledImageFeaturesNV cornerSampledImageFeatures = {};
    cornerSampledImageFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CORNER_SAMPLED_IMAGE_FEATURES_NV;
    getFeatures2(&cornerSampledImageFeatures);
    return cornerSampledImageFeatures;
}
#endif // VK_NV_corner_sampled_image

#ifdef VK_NV_coverage_reduction_mode
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceCoverageReductionModeFeaturesNV PhysicalDevice::getCoverageReductionModeFeatures() const
{
    VkPhysicalDeviceCoverageReductionModeFeaturesNV coverageReductionModeFeatures = {};
    coverageReductionModeFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COVERAGE_REDUCTION_MODE_FEATURES_NV;
    getFeatures2(&coverageReductionModeFeatures);
    return coverageReductionModeFeatures;
}
#endif // VK_NV_coverage_reduction_mode

#ifdef VK_NV_dedicated_allocation_image_aliasing
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceDedicatedAllocationImageAliasingFeaturesNV PhysicalDevice::getDedicatedAllocationImageAliasingFeatures() const
{
    VkPhysicalDeviceDedicatedAllocationImageAliasingFeaturesNV dedicatedAllocationImageAliasingFeatures = {};
    dedicatedAllocationImageAliasingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEDICATED_ALLOCATION_IMAGE_ALIASING_FEATURES_NV;
    getFeatures2(&dedicatedAllocationImageAliasingFeatures);
    return dedicatedAllocationImageAliasingFeatures;
}
#endif // VK_NV_dedicated_allocation_image_aliasing

#ifdef VK_NV_device_diagnostics_config
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceDiagnosticsConfigFeaturesNV PhysicalDevice::getDiagnosticsConfigFeatures() const
{
    VkPhysicalDeviceDiagnosticsConfigFeaturesNV diagnosticsConfigFeatures = {};
    diagnosticsConfigFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DIAGNOSTICS_CONFIG_FEATURES_NV;
    getFeatures2(&diagnosticsConfigFeatures);
    return diagnosticsConfigFeatures;
}
#endif // VK_NV_device_diagnostics_config

#ifdef VK_NV_device_generated_commands
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceDeviceGeneratedCommandsFeaturesNV PhysicalDevice::getDeviceGeneratedCommandsFeatures() const
{
    VkPhysicalDeviceDeviceGeneratedCommandsFeaturesNV deviceGeneratedCommandsFeatures = {};
    deviceGeneratedCommandsFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEVICE_GENERATED_COMMANDS_FEATURES_NV;
    getFeatures2(&deviceGeneratedCommandsFeatures);
    return deviceGeneratedCommandsFeatures;
}

// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceDeviceGeneratedCommandsPropertiesNV PhysicalDevice::getDeviceGeneratedCommandsProperties() const
{
    VkPhysicalDeviceDeviceGeneratedCommandsPropertiesNV deviceGeneratedCommandsProperties = {};
    deviceGeneratedCommandsProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEVICE_GENERATED_COMMANDS_PROPERTIES_NV;
    getProperties2(&deviceGeneratedCommandsProperties);
    return deviceGeneratedCommandsProperties;
}
#endif // VK_NV_device_generated_commands

#ifdef VK_NV_external_memory_rdma
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceExternalMemoryRDMAFeaturesNV PhysicalDevice::getExternalMemoryRDMAFeatures() const
{
    VkPhysicalDeviceExternalMemoryRDMAFeaturesNV externalMemoryRDMAFeatures = {};
    externalMemoryRDMAFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_MEMORY_RDMA_FEATURES_NV;
    getFeatures2(&externalMemoryRDMAFeatures);
    return externalMemoryRDMAFeatures;
}
#endif // VK_NV_external_memory_rdma

#ifdef VK_NV_fragment_shader_barycentric
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceFragmentShaderBarycentricFeaturesNV PhysicalDevice::getFragmentShaderBarycentricFeatures() const
{
    VkPhysicalDeviceFragmentShaderBarycentricFeaturesNV fragmentShaderBarycentricFeatures = {};
    fragmentShaderBarycentricFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADER_BARYCENTRIC_FEATURES_NV;
    getFeatures2(&fragmentShaderBarycentricFeatures);
    return fragmentShaderBarycentricFeatures;
}
#endif // VK_NV_fragment_shader_barycentric

#ifdef VK_NV_fragment_shading_rate_enums
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceFragmentShadingRateEnumsFeaturesNV PhysicalDevice::getFragmentShadingRateEnumsFeatures() const
{
    VkPhysicalDeviceFragmentShadingRateEnumsFeaturesNV fragmentShadingRateEnumsFeatures = {};
    fragmentShadingRateEnumsFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_ENUMS_FEATURES_NV;
    getFeatures2(&fragmentShadingRateEnumsFeatures);
    return fragmentShadingRateEnumsFeatures;
}

// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceFragmentShadingRateEnumsPropertiesNV PhysicalDevice::getFragmentShadingRateEnumsProperties() const
{
    VkPhysicalDeviceFragmentShadingRateEnumsPropertiesNV fragmentShadingRateEnumsProperties = {};
    fragmentShadingRateEnumsProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_ENUMS_PROPERTIES_NV;
    getProperties2(&fragmentShadingRateEnumsProperties);
    return fragmentShadingRateEnumsProperties;
}
#endif // VK_NV_fragment_shading_rate_enums

#ifdef VK_NV_inherited_viewport_scissor
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceInheritedViewportScissorFeaturesNV PhysicalDevice::getInheritedViewportScissorFeatures() const
{
    VkPhysicalDeviceInheritedViewportScissorFeaturesNV inheritedViewportScissorFeatures = {};
    inheritedViewportScissorFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INHERITED_VIEWPORT_SCISSOR_FEATURES_NV;
    getFeatures2(&inheritedViewportScissorFeatures);
    return inheritedViewportScissorFeatures;
}
#endif // VK_NV_inherited_viewport_scissor

#ifdef VK_NV_linear_color_attachment
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceLinearColorAttachmentFeaturesNV PhysicalDevice::getLinearColorAttachmentFeatures() const
{
    VkPhysicalDeviceLinearColorAttachmentFeaturesNV linearColorAttachmentFeatures = {};
    linearColorAttachmentFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_LINEAR_COLOR_ATTACHMENT_FEATURES_NV;
    getFeatures2(&linearColorAttachmentFeatures);
    return linearColorAttachmentFeatures;
}
#endif // VK_NV_linear_color_attachment

#ifdef VK_NV_mesh_shader
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceMeshShaderFeaturesNV PhysicalDevice::getMeshShaderFeatures() const
{
    VkPhysicalDeviceMeshShaderFeaturesNV meshShaderFeatures = {};
    meshShaderFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_NV;
    getFeatures2(&meshShaderFeatures);
    return meshShaderFeatures;
}

// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceMeshShaderPropertiesNV PhysicalDevice::getMeshShaderProperties() const
{
    VkPhysicalDeviceMeshShaderPropertiesNV meshShaderProperties = {};
    meshShaderProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_PROPERTIES_NV;
    getProperties2(&meshShaderProperties);
    return meshShaderProperties;
}
#endif // VK_NV_mesh_shader

#ifdef VK_NV_ray_tracing
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceRayTracingPropertiesNV PhysicalDevice::getRayTracingProperties() const
{
    VkPhysicalDeviceRayTracingPropertiesNV rayTracingProperties = {};
    rayTracingProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PROPERTIES_NV;
    getProperties2(&rayTracingProperties);
    return rayTracingProperties;
}
#endif // VK_NV_ray_tracing

#ifdef VK_NV_ray_tracing_motion_blur
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceRayTracingMotionBlurFeaturesNV PhysicalDevice::getRayTracingMotionBlurFeatures() const
{
    VkPhysicalDeviceRayTracingMotionBlurFeaturesNV rayTracingMotionBlurFeatures = {};
    rayTracingMotionBlurFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_MOTION_BLUR_FEATURES_NV;
    getFeatures2(&rayTracingMotionBlurFeatures);
    return rayTracingMotionBlurFeatures;
}
#endif // VK_NV_ray_tracing_motion_blur

#ifdef VK_NV_representative_fragment_test
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceRepresentativeFragmentTestFeaturesNV PhysicalDevice::getRepresentativeFragmentTestFeatures() const
{
    VkPhysicalDeviceRepresentativeFragmentTestFeaturesNV representativeFragmentTestFeatures = {};
    representativeFragmentTestFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_REPRESENTATIVE_FRAGMENT_TEST_FEATURES_NV;
    getFeatures2(&representativeFragmentTestFeatures);
    return representativeFragmentTestFeatures;
}
#endif // VK_NV_representative_fragment_test

#ifdef VK_NV_scissor_exclusive
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceExclusiveScissorFeaturesNV PhysicalDevice::getExclusiveScissorFeatures() const
{
    VkPhysicalDeviceExclusiveScissorFeaturesNV exclusiveScissorFeatures = {};
    exclusiveScissorFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXCLUSIVE_SCISSOR_FEATURES_NV;
    getFeatures2(&exclusiveScissorFeatures);
    return exclusiveScissorFeatures;
}
#endif // VK_NV_scissor_exclusive

#ifdef VK_NV_shader_image_footprint
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceShaderImageFootprintFeaturesNV PhysicalDevice::getShaderImageFootprintFeatures() const
{
    VkPhysicalDeviceShaderImageFootprintFeaturesNV shaderImageFootprintFeatures = {};
    shaderImageFootprintFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_IMAGE_FOOTPRINT_FEATURES_NV;
    getFeatures2(&shaderImageFootprintFeatures);
    return shaderImageFootprintFeatures;
}
#endif // VK_NV_shader_image_footprint

#ifdef VK_NV_shader_sm_builtins
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceShaderSMBuiltinsFeaturesNV PhysicalDevice::getShaderSMBuiltinsFeatures() const
{
    VkPhysicalDeviceShaderSMBuiltinsFeaturesNV shaderSMBuiltinsFeatures = {};
    shaderSMBuiltinsFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_SM_BUILTINS_FEATURES_NV;
    getFeatures2(&shaderSMBuiltinsFeatures);
    return shaderSMBuiltinsFeatures;
}

// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceShaderSMBuiltinsPropertiesNV PhysicalDevice::getShaderSMBuiltinsProperties() const
{
    VkPhysicalDeviceShaderSMBuiltinsPropertiesNV shaderSMBuiltinsProperties = {};
    shaderSMBuiltinsProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_SM_BUILTINS_PROPERTIES_NV;
    getProperties2(&shaderSMBuiltinsProperties);
    return shaderSMBuiltinsProperties;
}
#endif // VK_NV_shader_sm_builtins

#ifdef VK_NV_shading_rate_image
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceShadingRateImageFeaturesNV PhysicalDevice::getShadingRateImageFeatures() const
{
    VkPhysicalDeviceShadingRateImageFeaturesNV shadingRateImageFeatures = {};
    shadingRateImageFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADING_RATE_IMAGE_FEATURES_NV;
    getFeatures2(&shadingRateImageFeatures);
    return shadingRateImageFeatures;
}

// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceShadingRateImagePropertiesNV PhysicalDevice::getShadingRateImageProperties() const
{
    VkPhysicalDeviceShadingRateImagePropertiesNV shadingRateImageProperties = {};
    shadingRateImageProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADING_RATE_IMAGE_PROPERTIES_NV;
    getProperties2(&shadingRateImageProperties);
    return shadingRateImageProperties;
}
#endif // VK_NV_shading_rate_image

#ifdef VK_QCOM_fragment_density_map_offset
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceFragmentDensityMapOffsetFeaturesQCOM PhysicalDevice::getFragmentDensityMapOffsetFeatures() const
{
    VkPhysicalDeviceFragmentDensityMapOffsetFeaturesQCOM fragmentDensityMapOffsetFeatures = {};
    fragmentDensityMapOffsetFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_DENSITY_MAP_OFFSET_FEATURES_QCOM;
    getFeatures2(&fragmentDensityMapOffsetFeatures);
    return fragmentDensityMapOffsetFeatures;
}

// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceFragmentDensityMapOffsetPropertiesQCOM PhysicalDevice::getFragmentDensityMapOffsetProperties() const
{
    VkPhysicalDeviceFragmentDensityMapOffsetPropertiesQCOM fragmentDensityMapOffsetProperties = {};
    fragmentDensityMapOffsetProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_DENSITY_MAP_OFFSET_PROPERTIES_QCOM;
    getProperties2(&fragmentDensityMapOffsetProperties);
    return fragmentDensityMapOffsetProperties;
}
#endif // VK_QCOM_fragment_density_map_offset

#ifdef VK_VALVE_descriptor_set_host_mapping
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceDescriptorSetHostMappingFeaturesVALVE PhysicalDevice::getDescriptorSetHostMappingFeatures() const
{
    VkPhysicalDeviceDescriptorSetHostMappingFeaturesVALVE descriptorSetHostMappingFeatures = {};
    descriptorSetHostMappingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_SET_HOST_MAPPING_FEATURES_VALVE;
    getFeatures2(&descriptorSetHostMappingFeatures);
    return descriptorSetHostMappingFeatures;
}
#endif // VK_VALVE_descriptor_set_host_mapping

#ifdef VK_VALVE_mutable_descriptor_type
// Generated by utility tool. DO NOT MODIFY!
VkPhysicalDeviceMutableDescriptorTypeFeaturesVALVE PhysicalDevice::getMutableDescriptorTypeFeatures() const
{
    VkPhysicalDeviceMutableDescriptorTypeFeaturesVALVE mutableDescriptorTypeFeatures = {};
    mutableDescriptorTypeFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MUTABLE_DESCRIPTOR_TYPE_FEATURES_VALVE;
    getFeatures2(&mutableDescriptorTypeFeatures);
    return mutableDescriptorTypeFeatures;
}
#endif // VK_VALVE_mutable_descriptor_type

std::shared_ptr<Device> PhysicalDevice::createDefaultDevice() const
{
    const std::vector<float> defaultQueuePriorities = {1.0f};
    const std::vector<DeviceQueueDescriptor> queueDescriptors = {
        DeviceQueueDescriptor(shared_from_this(), VK_QUEUE_GRAPHICS_BIT, defaultQueuePriorities)
    };
    const std::vector<const char*> noLayers;
    const std::vector<const char*> swapchainExtension = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    const VkPhysicalDeviceFeatures noDeviceFeatures = {};
    const std::vector<void *> noExtendedDeviceFeatures;
    return createDevice(queueDescriptors, noLayers, swapchainExtension, noDeviceFeatures, noExtendedDeviceFeatures);
}

bool PhysicalDevice::extensionSupported(const char *extensionName) const noexcept
{
    MAGMA_ASSERT(extensionName);
    MAGMA_ASSERT(strlen(extensionName));
    if (!extensionName || !strlen(extensionName))
        return false;
    const auto it = extensions.find(extensionName);
    return it != extensions.end();
}

bool PhysicalDevice::checkPipelineCacheDataCompatibility(const void *cacheData) const noexcept
{
    MAGMA_ASSERT(cacheData);
    if (!cacheData)
        return false;
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(handle, &properties);
    PipelineCache::Header header;
    header.size = sizeof(PipelineCache::Header);
    header.version = VK_PIPELINE_CACHE_HEADER_VERSION_ONE;
    header.vendorID = properties.vendorID;
    header.deviceID = properties.deviceID;
    memcpy(header.cacheUUID, properties.pipelineCacheUUID, VK_UUID_SIZE);
    const PipelineCache::Header *cacheHeader = reinterpret_cast<const PipelineCache::Header *>(cacheData);
    return core::compare(cacheHeader, &header);
}

void PhysicalDevice::getFeatures2(void *physicalDeviceFeatures) const
{
    MAGMA_UNUSED(physicalDeviceFeatures);
#ifdef VK_KHR_get_physical_device_properties2
    VkPhysicalDeviceFeatures2KHR physicalDeviceFeatures2;
    physicalDeviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR;
    physicalDeviceFeatures2.pNext = physicalDeviceFeatures;
    MAGMA_REQUIRED_INSTANCE_EXTENSION(vkGetPhysicalDeviceFeatures2KHR, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
    vkGetPhysicalDeviceFeatures2KHR(handle, &physicalDeviceFeatures2);
#endif
}

void PhysicalDevice::getProperties2(void *physicalDeviceProperties) const
{
    MAGMA_UNUSED(physicalDeviceProperties);
#ifdef VK_KHR_get_physical_device_properties2
    VkPhysicalDeviceProperties2KHR physicalDeviceProperties2;
    physicalDeviceProperties2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2_KHR;
    physicalDeviceProperties2.pNext = physicalDeviceProperties;
    MAGMA_REQUIRED_INSTANCE_EXTENSION(vkGetPhysicalDeviceProperties2KHR, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
    vkGetPhysicalDeviceProperties2KHR(handle, &physicalDeviceProperties2);
#endif // VK_KHR_get_physical_device_properties2
}

void PhysicalDevice::getSurfaceCapabilities2(std::shared_ptr<const Surface> surface, void *surfaceCaps) const
{
    MAGMA_UNUSED(surface);
    MAGMA_UNUSED(surfaceCaps);
#ifdef VK_KHR_get_surface_capabilities2
    VkPhysicalDeviceSurfaceInfo2KHR physicalDeviceSurfaceInfo2;
    physicalDeviceSurfaceInfo2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR;
    physicalDeviceSurfaceInfo2.pNext = nullptr;
    physicalDeviceSurfaceInfo2.surface = *surface;
    VkSurfaceCapabilities2KHR surfaceCaps2;
    surfaceCaps2.sType = VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_KHR;
    surfaceCaps2.pNext = surfaceCaps;
    MAGMA_REQUIRED_INSTANCE_EXTENSION(vkGetPhysicalDeviceSurfaceCapabilities2KHR, VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME);
    const VkResult result = vkGetPhysicalDeviceSurfaceCapabilities2KHR(handle, &physicalDeviceSurfaceInfo2, &surfaceCaps2);
    MAGMA_THROW_FAILURE(result, "failed to get surface capabilities");
#endif // VK_KHR_get_surface_capabilities2
}
} // namespace magma
