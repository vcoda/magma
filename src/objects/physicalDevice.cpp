/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2025 Victor Coda.

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
#include "../misc/deviceFeatures.h"
#include "../misc/compatibility.h"
#include "../misc/extension.h"
#include "../allocator/allocator.h"
#include "../exceptions/errorResult.h"

namespace magma
{
PhysicalDevice::PhysicalDevice(Instance *instance, VkPhysicalDevice handle_, std::shared_ptr<IAllocator> allocator) noexcept:
    Dispatchable<VkPhysicalDevice>(VK_OBJECT_TYPE_PHYSICAL_DEVICE, std::move(allocator)),
    instance(instance)
{
    handle = handle_;
    for (auto const& properties: enumerateExtensions())
        extensions.emplace(properties.extensionName);
}

PhysicalDevice::~PhysicalDevice() {}

std::shared_ptr<Device> PhysicalDevice::createDevice(const std::set<DeviceQueueDescriptor>& queueDescriptors_,
    const NullTerminatedStringArray& enabledLayers, const NullTerminatedStringArray& enabledExtensions,
    const VkPhysicalDeviceFeatures& enabledFeatures,
    const StructureChain& enabledExtendedFeatures /* default */,
    const StructureChain& extendedInfo /* default */) const
{   // std::set guarantees that queueFamilyIndex member of each element of queueDescriptors is unique
    std::vector<DeviceQueueDescriptor> queueDescriptors;
    for (auto const& desc: queueDescriptors_)
        queueDescriptors.push_back(desc);
    return Device::makeShared(std::const_pointer_cast<PhysicalDevice>(shared_from_this()),
        queueDescriptors, enabledLayers, enabledExtensions,
        enabledFeatures, enabledExtendedFeatures, extendedInfo,
        hostAllocator);
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

VkImageFormatProperties PhysicalDevice::getImageFormatProperties(VkFormat format, VkImageType type,
    VkImageUsageFlags usage, bool optimalTiling, VkImageCreateFlags flags /* 0 */) const
{
    const VkImageTiling tiling = optimalTiling ? VK_IMAGE_TILING_OPTIMAL : VK_IMAGE_TILING_LINEAR;
    VkImageFormatProperties imageFormatProperties;
    const VkResult result = vkGetPhysicalDeviceImageFormatProperties(handle, format, type, tiling, usage, flags, &imageFormatProperties);
    MAGMA_HANDLE_RESULT(result, "failed to get image format properties of physical device");
    return imageFormatProperties;
}

std::vector<VkSparseImageFormatProperties> PhysicalDevice::getSparseImageFormatProperties(VkFormat format,
    VkImageType type, VkImageUsageFlags usage, bool optimalTiling, uint32_t samples /* 1 */) const
{
    VkSampleCountFlagBits sampleCountBit;
    switch (samples)
    {
    case 1: sampleCountBit = VK_SAMPLE_COUNT_1_BIT; break;
    case 2: sampleCountBit = VK_SAMPLE_COUNT_2_BIT; break;
    case 4: sampleCountBit = VK_SAMPLE_COUNT_4_BIT; break;
    case 8: sampleCountBit = VK_SAMPLE_COUNT_8_BIT; break;
    case 16: sampleCountBit = VK_SAMPLE_COUNT_16_BIT; break;
    case 32: sampleCountBit = VK_SAMPLE_COUNT_32_BIT; break;
    case 64: sampleCountBit = VK_SAMPLE_COUNT_64_BIT; break;
    default: sampleCountBit = VK_SAMPLE_COUNT_1_BIT; break;
    }
    const VkImageTiling tiling = optimalTiling ? VK_IMAGE_TILING_OPTIMAL : VK_IMAGE_TILING_LINEAR;
    uint32_t propertyCount = 0;
    vkGetPhysicalDeviceSparseImageFormatProperties(handle, format, type, sampleCountBit, usage, tiling, &propertyCount, nullptr);
    std::vector<VkSparseImageFormatProperties> sparseImageFormatProperties;
    if (propertyCount)
    {
        sparseImageFormatProperties.resize(propertyCount);
        vkGetPhysicalDeviceSparseImageFormatProperties(handle, format, type, sampleCountBit, usage, tiling, &propertyCount,
            sparseImageFormatProperties.data());
    }
    return sparseImageFormatProperties;
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
    if (propertyCount)
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
    if (propertyCount)
    {
        properties.resize(propertyCount);
        result = vkEnumerateDeviceLayerProperties(handle, &propertyCount, properties.data());
    }
    MAGMA_HANDLE_RESULT(result, "failed to enumerate physical device layers");
    return properties;
}

std::vector<VkExtensionProperties> PhysicalDevice::enumerateExtensions(const char *layerName /* nullptr */) const
{
    uint32_t propertyCount = 0;
    VkResult result = vkEnumerateDeviceExtensionProperties(handle, layerName, &propertyCount, nullptr);
    std::vector<VkExtensionProperties> properties;
    if (propertyCount)
    {
        properties.resize(propertyCount);
        result = vkEnumerateDeviceExtensionProperties(handle, layerName, &propertyCount, properties.data());
    }
    MAGMA_HANDLE_RESULT(result, "failed to enumerate physical device extensions");
    return properties;
}

#ifdef VK_KHR_surface
bool PhysicalDevice::getSurfaceSupport(lent_ptr<const Surface> surface, uint32_t queueFamilyIndex) const noexcept
{
    VkBool32 supported = VK_FALSE;
    const VkResult result = vkGetPhysicalDeviceSurfaceSupportKHR(handle, queueFamilyIndex, *surface, &supported);
    return (VK_SUCCESS == result) && (VK_TRUE == supported);
}

VkSurfaceCapabilitiesKHR PhysicalDevice::getSurfaceCapabilities(lent_ptr<const Surface> surface) const
{
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    const VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(handle, *surface, &surfaceCapabilities);
    MAGMA_HANDLE_RESULT(result, "failed to get surface capabilities of physical device");
    return surfaceCapabilities;
}

std::vector<VkSurfaceFormatKHR> PhysicalDevice::getSurfaceFormats(lent_ptr<const Surface> surface) const
{
    uint32_t surfaceFormatCount = 0;
    VkResult result = vkGetPhysicalDeviceSurfaceFormatsKHR(handle, *surface, &surfaceFormatCount, nullptr);
    std::vector<VkSurfaceFormatKHR> surfaceFormats;
    if (surfaceFormatCount)
    {
        surfaceFormats.resize(surfaceFormatCount);
        result = vkGetPhysicalDeviceSurfaceFormatsKHR(handle, *surface, &surfaceFormatCount, surfaceFormats.data());
    }
    MAGMA_HANDLE_RESULT(result, "failed to get surface formats of physical device");
    return surfaceFormats;
}

std::vector<VkPresentModeKHR> PhysicalDevice::getSurfacePresentModes(lent_ptr<const Surface> surface) const
{
    uint32_t presentModeCount = 0;
    VkResult result = vkGetPhysicalDeviceSurfacePresentModesKHR(handle, *surface, &presentModeCount, nullptr);
    std::vector<VkPresentModeKHR> presentModes;
    if (presentModeCount)
    {
        presentModes.resize(presentModeCount);
        result = vkGetPhysicalDeviceSurfacePresentModesKHR(handle, *surface, &presentModeCount, presentModes.data());
    }
    MAGMA_HANDLE_RESULT(result, "failed to get surface present modes of physical device");
    return presentModes;
}

#ifdef VK_EXT_full_screen_exclusive
bool PhysicalDevice::getFullScreenExclusiveSurfaceSupport(lent_ptr<const Surface> surface) const
{
    VkSurfaceCapabilitiesFullScreenExclusiveEXT fullScreenExclusiveCapabilities = {};
    fullScreenExclusiveCapabilities.sType = VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_FULL_SCREEN_EXCLUSIVE_EXT;
#ifdef VK_KHR_get_surface_capabilities2
    getSurfaceCapabilities2(std::move(surface), &fullScreenExclusiveCapabilities);
#endif
    return (VK_TRUE == fullScreenExclusiveCapabilities.fullScreenExclusiveSupported);
}

std::vector<VkPresentModeKHR> PhysicalDevice::getFullScreenExclusiveSurfacePresentModes(lent_ptr<const Surface> surface,
    VkFullScreenExclusiveEXT fullScreenExclusive
#ifdef VK_KHR_win32_surface
   ,HMONITOR hMonitor /* NULL */
#endif
    ) const
{
    VkSurfaceFullScreenExclusiveInfoEXT fullScreenExclusiveInfo;
    fullScreenExclusiveInfo.sType = VK_STRUCTURE_TYPE_SURFACE_FULL_SCREEN_EXCLUSIVE_INFO_EXT;
    fullScreenExclusiveInfo.pNext = nullptr;
    fullScreenExclusiveInfo.fullScreenExclusive = fullScreenExclusive;
#ifdef VK_KHR_win32_surface
    VkSurfaceFullScreenExclusiveWin32InfoEXT fullScreenExclusiveWin32Info;
    if (hMonitor)
    {
        fullScreenExclusiveInfo.pNext = &fullScreenExclusiveWin32Info;
        fullScreenExclusiveWin32Info.sType = VK_STRUCTURE_TYPE_SURFACE_FULL_SCREEN_EXCLUSIVE_WIN32_INFO_EXT;
        fullScreenExclusiveWin32Info.pNext = nullptr;
        fullScreenExclusiveWin32Info.hmonitor = hMonitor;
    }
#endif // VK_KHR_win32_surface
    std::vector<VkPresentModeKHR> presentModes;
#ifdef VK_KHR_get_surface_capabilities2
    VkPhysicalDeviceSurfaceInfo2KHR surfaceInfo;
    surfaceInfo.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR;
    surfaceInfo.pNext = &fullScreenExclusiveInfo;
    surfaceInfo.surface = *surface;
    uint32_t presentModeCount = 0;
    MAGMA_REQUIRED_INSTANCE_EXTENSION(vkGetPhysicalDeviceSurfacePresentModes2EXT, VK_EXT_FULL_SCREEN_EXCLUSIVE_EXTENSION_NAME);
    VkResult result = vkGetPhysicalDeviceSurfacePresentModes2EXT(handle, &surfaceInfo, &presentModeCount, nullptr);
    if (presentModeCount)
    {
        presentModes.resize(presentModeCount);
        result = vkGetPhysicalDeviceSurfacePresentModes2EXT(handle, &surfaceInfo, &presentModeCount, presentModes.data());
    }
    MAGMA_HANDLE_RESULT(result, "failed to get full-screen exclusive surface present modes of physical device");
#endif // VK_KHR_get_surface_capabilities2
    return presentModes;
}
#endif // VK_EXT_full_screen_exclusive

#ifdef VK_KHR_shared_presentable_image
VkImageUsageFlags PhysicalDevice::getSurfaceSharedPresentFlags(lent_ptr<const Surface> surface) const
{
    VkSharedPresentSurfaceCapabilitiesKHR sharedPresentSurfaceCapabilities = {};
    sharedPresentSurfaceCapabilities.sType = VK_STRUCTURE_TYPE_SHARED_PRESENT_SURFACE_CAPABILITIES_KHR;
#ifdef VK_KHR_get_surface_capabilities2
    getSurfaceCapabilities2(std::move(surface), &sharedPresentSurfaceCapabilities);
#endif
    return sharedPresentSurfaceCapabilities.sharedPresentSupportedUsageFlags;
}
#endif // VK_KHR_shared_presentable_image

#ifdef VK_KHR_device_group
std::vector<VkRect2D> PhysicalDevice::getPresentRectangles(lent_ptr<const Surface> surface) const
{
    uint32_t rectCount = 0;
    MAGMA_REQUIRED_INSTANCE_EXTENSION(vkGetPhysicalDevicePresentRectanglesKHR, VK_KHR_DEVICE_GROUP_EXTENSION_NAME);
    VkResult result = vkGetPhysicalDevicePresentRectanglesKHR(handle, *surface, &rectCount, nullptr);
    std::vector<VkRect2D> presentRects;
    if (rectCount)
    {
        presentRects.resize(rectCount);
        result = vkGetPhysicalDevicePresentRectanglesKHR(handle, *surface, &rectCount, presentRects.data());
    }
    MAGMA_HANDLE_RESULT(result, "failed to query present rectangles for a surface");
    return presentRects;
}
#endif // VK_KHR_device_group

#ifdef VK_KHR_performance_query
uint32_t PhysicalDevice::getNumQueueFamilyPerformanceCounters(uint32_t queueFamilyIndex) const noexcept
{
    uint32_t counterCount = 0;
    MAGMA_REQUIRED_INSTANCE_EXTENSION(vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR, VK_KHR_PERFORMANCE_QUERY_EXTENSION_NAME);
    vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(handle, queueFamilyIndex, &counterCount, nullptr, nullptr);
    return counterCount;
}

std::vector<VkPerformanceCounterKHR> PhysicalDevice::enumerateQueueFamilyPerformanceCounters(uint32_t queueFamilyIndex) const
{
    std::vector<VkPerformanceCounterKHR> counters;
    uint32_t counterCount = getNumQueueFamilyPerformanceCounters(queueFamilyIndex);
    if (counterCount)
    {
        counters.resize(counterCount, {VK_STRUCTURE_TYPE_PERFORMANCE_COUNTER_KHR});
        MAGMA_REQUIRED_INSTANCE_EXTENSION(vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR, VK_KHR_PERFORMANCE_QUERY_EXTENSION_NAME);
        const VkResult result = vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(handle, queueFamilyIndex,
            &counterCount, counters.data(), nullptr);
        MAGMA_HANDLE_RESULT(result, "failed to enumerate performance counters");
    }
    return counters;
}

std::vector<VkPerformanceCounterDescriptionKHR> PhysicalDevice::enumerateQueueFamilyPerformanceCounterDescriptions(uint32_t queueFamilyIndex) const
{
    std::vector<VkPerformanceCounterDescriptionKHR> counterDescriptions;
    uint32_t counterCount = getNumQueueFamilyPerformanceCounters(queueFamilyIndex);
    if (counterCount)
    {
        counterDescriptions.resize(counterCount, {VK_STRUCTURE_TYPE_PERFORMANCE_COUNTER_DESCRIPTION_KHR});
        MAGMA_REQUIRED_INSTANCE_EXTENSION(vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR, VK_KHR_PERFORMANCE_QUERY_EXTENSION_NAME);
        const VkResult result = vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(handle, queueFamilyIndex,
            &counterCount, nullptr, counterDescriptions.data());
        MAGMA_HANDLE_RESULT(result, "failed to enumerate descriptions of performance counters");
    }
    return counterDescriptions;
}

uint32_t PhysicalDevice::getNumQueueFamilyPerformanceQueryPasses(uint32_t queueFamilyIndex, const std::vector<uint32_t>& counterIndices) const
{
    VkQueryPoolPerformanceCreateInfoKHR queryPoolPerformanceInfo;
    queryPoolPerformanceInfo.sType = VK_STRUCTURE_TYPE_QUERY_POOL_PERFORMANCE_CREATE_INFO_KHR;
    queryPoolPerformanceInfo.pNext = nullptr;
    queryPoolPerformanceInfo.queueFamilyIndex = queueFamilyIndex;
    queryPoolPerformanceInfo.counterIndexCount = core::countof(counterIndices);
    queryPoolPerformanceInfo.pCounterIndices = counterIndices.data();
    uint32_t numPasses = 0;
    MAGMA_REQUIRED_INSTANCE_EXTENSION(vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR, VK_KHR_PERFORMANCE_QUERY_EXTENSION_NAME);
    vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(handle, &queryPoolPerformanceInfo, &numPasses);
    return numPasses;
}
#endif // VK_KHR_performance_query

#ifdef VK_AMD_display_native_hdr
bool PhysicalDevice::getSurfaceLocalDimmingSupport(lent_ptr<const Surface> surface) const
{
    VkDisplayNativeHdrSurfaceCapabilitiesAMD nativeHdrSurfaceCapabilities = {};
    nativeHdrSurfaceCapabilities.sType = VK_STRUCTURE_TYPE_DISPLAY_NATIVE_HDR_SURFACE_CAPABILITIES_AMD;
#ifdef VK_KHR_get_surface_capabilities2
    getSurfaceCapabilities2(std::move(surface), &nativeHdrSurfaceCapabilities);
#endif
    return (VK_TRUE == nativeHdrSurfaceCapabilities.localDimmingSupport);
}
#endif //  VK_AMD_display_native_hdr
#endif // VK_KHR_surface

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
#else
    result = VK_TRUE;
#endif
    return (VK_TRUE == result);
}

#ifdef VK_EXT_calibrated_timestamps
std::vector<VkTimeDomainEXT> PhysicalDevice::getCalibrateableTimeDomains() const
{
    uint32_t timeDomainCount = 0;
    MAGMA_REQUIRED_INSTANCE_EXTENSION(vkGetPhysicalDeviceCalibrateableTimeDomainsEXT, VK_EXT_CALIBRATED_TIMESTAMPS_EXTENSION_NAME);
    VkResult result = vkGetPhysicalDeviceCalibrateableTimeDomainsEXT(handle, &timeDomainCount, nullptr);
    std::vector<VkTimeDomainEXT> timeDomains;
    if (timeDomainCount)
    {
        timeDomains.resize(timeDomainCount);
        result = vkGetPhysicalDeviceCalibrateableTimeDomainsEXT(handle, &timeDomainCount, timeDomains.data());
    }
    MAGMA_HANDLE_RESULT(result, "failed to get calibrateable time domains");
    return timeDomains;
}
#endif // VK_EXT_calibrated_timestamps

#ifdef VK_EXT_tooling_info
std::vector<VkPhysicalDeviceToolPropertiesEXT> PhysicalDevice::getToolProperties() const
{
    uint32_t toolCount = 0;
    MAGMA_REQUIRED_INSTANCE_EXTENSION(vkGetPhysicalDeviceToolPropertiesEXT, VK_EXT_TOOLING_INFO_EXTENSION_NAME);
    VkResult result = vkGetPhysicalDeviceToolPropertiesEXT(handle, &toolCount, nullptr);
    std::vector<VkPhysicalDeviceToolPropertiesEXT> toolProperties;
    if (toolCount)
    {
        toolProperties.resize(toolCount);
        result = vkGetPhysicalDeviceToolPropertiesEXT(handle, &toolCount, toolProperties.data());
    }
    MAGMA_HANDLE_RESULT(result, "failed to get tool properties of physical device");
    return toolProperties;
}
#endif // VK_EXT_tooling_info

#ifdef VK_KHR_display
std::vector<VkDisplayPropertiesKHR> PhysicalDevice::getDisplayProperties() const
{
    uint32_t propertyCount = 0;
    MAGMA_REQUIRED_INSTANCE_EXTENSION(vkGetPhysicalDeviceDisplayPropertiesKHR, VK_KHR_DISPLAY_EXTENSION_NAME);
    VkResult result = vkGetPhysicalDeviceDisplayPropertiesKHR(handle, &propertyCount, nullptr);
    std::vector<VkDisplayPropertiesKHR> displayProperties;
    if (propertyCount)
    {
        displayProperties.resize(propertyCount);
        result = vkGetPhysicalDeviceDisplayPropertiesKHR(handle, &propertyCount, displayProperties.data());
    }
    MAGMA_HANDLE_RESULT(result, "failed to get display properties of physical device");
    return displayProperties;
}

std::vector<VkDisplayPlanePropertiesKHR> PhysicalDevice::getDisplayPlaneProperties() const
{
    uint32_t propertyCount = 0;
    MAGMA_REQUIRED_INSTANCE_EXTENSION(vkGetPhysicalDeviceDisplayPropertiesKHR, VK_KHR_DISPLAY_EXTENSION_NAME);
    VkResult result = vkGetPhysicalDeviceDisplayPlanePropertiesKHR(handle, &propertyCount, nullptr);
    std::vector<VkDisplayPlanePropertiesKHR> displayPlaneProperties;
    if (propertyCount)
    {
        displayPlaneProperties.resize(propertyCount);
        result = vkGetPhysicalDeviceDisplayPlanePropertiesKHR(handle, &propertyCount, displayPlaneProperties.data());
    }
    MAGMA_HANDLE_RESULT(result, "failed to get display plane properties of physical device");
    return displayPlaneProperties;
}

std::vector<std::unique_ptr<Display>> PhysicalDevice::getSupportedDisplays(uint32_t planeIndex) const
{
    uint32_t displayCount = 0;
    MAGMA_REQUIRED_INSTANCE_EXTENSION(vkGetDisplayPlaneSupportedDisplaysKHR, VK_KHR_DISPLAY_EXTENSION_NAME);
    VkResult result = vkGetDisplayPlaneSupportedDisplaysKHR(handle, planeIndex, &displayCount, nullptr);
    std::vector<std::unique_ptr<Display>> supportedDisplays;
    if (displayCount)
    {
        auto displays = stackalloc(VkDisplayKHR, displayCount);
        result = vkGetDisplayPlaneSupportedDisplaysKHR(handle, planeIndex, &displayCount, displays);
        if (VK_SUCCESS == result)
        {
            std::shared_ptr<PhysicalDevice> physicalDevice = std::const_pointer_cast<PhysicalDevice>(shared_from_this());
            for (uint32_t i = 0; i < displayCount; ++i)
            {
                std::unique_ptr<Display> display = Display::makeUnique(physicalDevice, displays[i], planeIndex);
                supportedDisplays.emplace_back(std::move(display));
            }
        }
    }
    MAGMA_HANDLE_RESULT(result, "failed to get supported displays of physical device");
    return supportedDisplays;
}
#endif // VK_KHR_display

#ifdef VK_KHR_external_memory_capabilities
VkExternalMemoryPropertiesKHR PhysicalDevice::getExternalBufferProperties(VkExternalMemoryHandleTypeFlagBitsKHR handleType,
    VkBufferUsageFlags usage, VkBufferCreateFlags flags /* 0 */) const
{
    VkPhysicalDeviceExternalBufferInfoKHR externalBufferInfo;
    externalBufferInfo.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_BUFFER_INFO_KHR;
    externalBufferInfo.pNext = nullptr;
    externalBufferInfo.flags = flags;
    externalBufferInfo.usage = usage;
    externalBufferInfo.handleType = handleType;
    VkExternalBufferPropertiesKHR externalBufferProperties = {};
    externalBufferProperties.sType = VK_STRUCTURE_TYPE_EXTERNAL_BUFFER_PROPERTIES_KHR;
    externalBufferProperties.pNext = nullptr;
    MAGMA_REQUIRED_INSTANCE_EXTENSION(vkGetPhysicalDeviceExternalBufferPropertiesKHR, VK_KHR_EXTERNAL_MEMORY_CAPABILITIES_EXTENSION_NAME);
    vkGetPhysicalDeviceExternalBufferPropertiesKHR(handle, &externalBufferInfo, &externalBufferProperties);
    return externalBufferProperties.externalMemoryProperties;
}

#ifdef VK_KHR_get_physical_device_properties2
VkExternalMemoryPropertiesKHR PhysicalDevice::getExternalImageFormatProperties(VkExternalMemoryHandleTypeFlagBitsKHR handleType,
    VkFormat format, VkImageType type, bool optimalTiling, VkImageUsageFlags usage, VkImageCreateFlags flags /* 0 */) const
{
    VkPhysicalDeviceImageFormatInfo2KHR imageFormatInfo;
    VkPhysicalDeviceExternalImageFormatInfoKHR imageFormatInfoExternal;
    VkImageFormatProperties2KHR imageFormatProperties;
    VkExternalImageFormatPropertiesKHR imageFormatPropertiesExternal;
    imageFormatInfo.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_FORMAT_INFO_2_KHR;
    imageFormatInfo.pNext = &imageFormatInfoExternal;
    imageFormatInfo.format = format;
    imageFormatInfo.type = type;
    imageFormatInfo.tiling = optimalTiling ? VK_IMAGE_TILING_OPTIMAL : VK_IMAGE_TILING_LINEAR;
    imageFormatInfo.usage = usage;
    imageFormatInfo.flags = flags;
    imageFormatInfoExternal.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_IMAGE_FORMAT_INFO_KHR;
    imageFormatInfoExternal.pNext = nullptr;
    imageFormatInfoExternal.handleType = handleType;
    imageFormatProperties.sType = VK_STRUCTURE_TYPE_IMAGE_FORMAT_PROPERTIES_2_KHR;
    imageFormatProperties.pNext = &imageFormatPropertiesExternal;
    imageFormatPropertiesExternal.sType = VK_STRUCTURE_TYPE_EXTERNAL_IMAGE_FORMAT_PROPERTIES_KHR;
    imageFormatPropertiesExternal.pNext = nullptr;
    MAGMA_REQUIRED_INSTANCE_EXTENSION(vkGetPhysicalDeviceImageFormatProperties2KHR, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
    const VkResult result = vkGetPhysicalDeviceImageFormatProperties2KHR(handle, &imageFormatInfo, &imageFormatProperties);
    MAGMA_HANDLE_RESULT(result, "failed to get external image format properties");
    return imageFormatPropertiesExternal.externalMemoryProperties;
}
#endif // VK_KHR_get_physical_device_properties2
#endif // VK_KHR_external_memory_capabilities

#ifdef VK_KHR_external_fence_capabilities
VkExternalFencePropertiesKHR PhysicalDevice::getExternalFenceProperties(VkExternalFenceHandleTypeFlagBitsKHR handleType) const
{
    VkPhysicalDeviceExternalFenceInfoKHR externalFenceInfo;
    externalFenceInfo.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_FENCE_INFO_KHR;
    externalFenceInfo.pNext = nullptr;
    externalFenceInfo.handleType = handleType;
    VkExternalFencePropertiesKHR externalFenceProperties = {};
    externalFenceProperties.sType = VK_STRUCTURE_TYPE_EXTERNAL_FENCE_PROPERTIES_KHR;
    MAGMA_REQUIRED_INSTANCE_EXTENSION(vkGetPhysicalDeviceExternalFencePropertiesKHR, VK_KHR_EXTERNAL_FENCE_CAPABILITIES_EXTENSION_NAME);
    vkGetPhysicalDeviceExternalFencePropertiesKHR(handle, &externalFenceInfo, &externalFenceProperties);
    return externalFenceProperties;
}
#endif // VK_KHR_external_fence_capabilities

#ifdef VK_KHR_external_semaphore_capabilities
VkExternalSemaphorePropertiesKHR PhysicalDevice::getExternalSemaphoreProperties(VkExternalSemaphoreHandleTypeFlagBitsKHR handleType) const
{
    VkPhysicalDeviceExternalSemaphoreInfoKHR externalSemaphoreInfo;
    externalSemaphoreInfo.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_SEMAPHORE_INFO_KHR;
    externalSemaphoreInfo.pNext = nullptr;
    externalSemaphoreInfo.handleType = handleType;
    VkExternalSemaphorePropertiesKHR externalSemaphoreProperties = {};
    externalSemaphoreProperties.sType = VK_STRUCTURE_TYPE_EXTERNAL_SEMAPHORE_PROPERTIES_KHR;
    MAGMA_REQUIRED_INSTANCE_EXTENSION(vkGetPhysicalDeviceExternalSemaphorePropertiesKHR, VK_KHR_EXTERNAL_SEMAPHORE_CAPABILITIES_EXTENSION_NAME);
    vkGetPhysicalDeviceExternalSemaphorePropertiesKHR(handle, &externalSemaphoreInfo, &externalSemaphoreProperties);
    return externalSemaphoreProperties;
}
#endif // VK_KHR_external_semaphore_capabilities

#if defined(VK_ANDROID_external_memory_android_hardware_buffer) && defined(VK_KHR_get_physical_device_properties2)
uint64_t PhysicalDevice::getAndroidHardwareBufferUsage(VkFormat format, VkImageUsageFlags usage,
    VkImageCreateFlags flags /* 0 */) const
{
    VkPhysicalDeviceImageFormatInfo2KHR imageFormatInfo;
    VkImageFormatProperties2KHR imageFormatProperties;
    VkAndroidHardwareBufferUsageANDROID hardwareBufferUsage;
    imageFormatInfo.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_FORMAT_INFO_2_KHR;
    imageFormatInfo.pNext = nullptr;
    imageFormatInfo.format = format;
    imageFormatInfo.type = VK_IMAGE_TYPE_2D;
    imageFormatInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageFormatInfo.usage = usage;
    imageFormatInfo.flags = flags;
    imageFormatProperties.sType = VK_STRUCTURE_TYPE_IMAGE_FORMAT_PROPERTIES_2_KHR;
    imageFormatProperties.pNext = &hardwareBufferUsage;
    hardwareBufferUsage.sType = VK_STRUCTURE_TYPE_ANDROID_HARDWARE_BUFFER_USAGE_ANDROID;
    hardwareBufferUsage.pNext = nullptr;
    MAGMA_REQUIRED_INSTANCE_EXTENSION(vkGetPhysicalDeviceImageFormatProperties2KHR, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
    const VkResult result = vkGetPhysicalDeviceImageFormatProperties2KHR(handle, &imageFormatInfo, &imageFormatProperties);
    MAGMA_HANDLE_RESULT(result, "failed to get android hardware buffer usage");
    return hardwareBufferUsage.androidHardwareBufferUsage;
}
#endif // VK_ANDROID_external_memory_android_hardware_buffer && VK_KHR_get_physical_device_properties2

std::shared_ptr<Device> PhysicalDevice::createDefaultDevice() const
{
    const std::set<DeviceQueueDescriptor> queueDescriptors = {
        DeviceQueueDescriptor(this, VK_QUEUE_GRAPHICS_BIT, {QueuePriorityHighest})
    };
    const NullTerminatedStringArray noLayers;
    const NullTerminatedStringArray swapchainExtension = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    const VkPhysicalDeviceFeatures noDeviceFeatures = {};
    StructureChain emptyStructureChain;
    return createDevice(queueDescriptors, noLayers, swapchainExtension, noDeviceFeatures, emptyStructureChain, emptyStructureChain);
}

const std::unique_ptr<DeviceFeatures>& PhysicalDevice::features() const
{
    if (!deviceFeatures)
        deviceFeatures = DeviceFeatures::makeUnique(this);
    return deviceFeatures;
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

VkDeviceSize PhysicalDevice::getDeviceLocalHeapSize() const noexcept
{
    VkDeviceSize deviceLocalHeapSize = 0ull;
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(handle, &memoryProperties);
    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i)
    {
        const VkMemoryType& memoryType = memoryProperties.memoryTypes[i];
        if (memoryType.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
        {
            const uint32_t deviceLocalHeapIndex = memoryType.heapIndex;
            MAGMA_ASSERT(deviceLocalHeapIndex < memoryProperties.memoryHeapCount);
            const VkMemoryHeap& deviceLocalMemoryHeap = memoryProperties.memoryHeaps[deviceLocalHeapIndex];
            if (deviceLocalMemoryHeap.flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)
                deviceLocalHeapSize = std::max(deviceLocalMemoryHeap.size, deviceLocalHeapSize);
        }
    }
    return deviceLocalHeapSize;
}

VkDeviceSize PhysicalDevice::getHostVisibleHeapSize() const noexcept
{
    VkDeviceSize hostVisibleHeapSize = 0ull;
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(handle, &memoryProperties);
    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i)
    {
        const VkMemoryType& memoryType = memoryProperties.memoryTypes[i];
        constexpr VkMemoryPropertyFlags hostVisibleMemoryFlags =
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        if (MAGMA_BITWISE_AND(memoryType.propertyFlags, hostVisibleMemoryFlags))
        {
            const uint32_t hostVisibleHeapIndex = memoryType.heapIndex;
            MAGMA_ASSERT(hostVisibleHeapIndex < memoryProperties.memoryHeapCount);
            const VkMemoryHeap& hostVisibleHeap = memoryProperties.memoryHeaps[hostVisibleHeapIndex];
            hostVisibleHeapSize = std::max(hostVisibleHeap.size, hostVisibleHeapSize);
        }
    }
    return hostVisibleHeapSize;
}

VkDeviceSize PhysicalDevice::getDeviceLocalHostVisibleHeapSize() const noexcept
{
    VkDeviceSize deviceLocalHostVisibleHeapSize = 0ull;
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(handle, &memoryProperties);
    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i)
    {
        const VkMemoryType& memoryType = memoryProperties.memoryTypes[i];
        constexpr VkMemoryPropertyFlags deviceLocalHostVisibleMemoryFlags =
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT |
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        if (MAGMA_BITWISE_AND(memoryType.propertyFlags, deviceLocalHostVisibleMemoryFlags))
        {/* Major GPU vendors expose a 256MiB-ish staging buffer
            with the DEVICE_LOCAL | HOST_VISIBLE | HOST_COHERENT
            flags where the GPU and CPU can both write into shared
            memory visible to each other. This limit correlates with
            the 256MiB PCIE-specified BAR limit that defines the size
            of the VRAM window that the host can access. */
            const uint32_t deviceLocalHostVisibleHeapIndex = memoryType.heapIndex;
            MAGMA_ASSERT(deviceLocalHostVisibleHeapIndex < memoryProperties.memoryHeapCount);
            const VkMemoryHeap& deviceLocalHostVisibleHeap = memoryProperties.memoryHeaps[deviceLocalHostVisibleHeapIndex];
            if (deviceLocalHostVisibleHeap.flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)
                deviceLocalHostVisibleHeapSize = std::max(deviceLocalHostVisibleHeap.size, deviceLocalHostVisibleHeapSize);
        }
    }
    return deviceLocalHostVisibleHeapSize;
}

#ifdef VK_KHR_device_group
VkDeviceSize PhysicalDevice::getDeviceGroupSharedHeapSize() const noexcept
{
    VkDeviceSize deviceGroupSharedHeapSize = 0ull;
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(handle, &memoryProperties);
    for (uint32_t i = 0; i < memoryProperties.memoryHeapCount; ++i)
    {
        const VkMemoryHeap& memoryHeap = memoryProperties.memoryHeaps[i];
        if (memoryHeap.flags & VK_MEMORY_HEAP_MULTI_INSTANCE_BIT_KHR)
            deviceGroupSharedHeapSize = std::max(memoryHeap.size, deviceGroupSharedHeapSize);
    }
    return deviceGroupSharedHeapSize;
}
#endif // VK_KHR_device_group

bool PhysicalDevice::getPipelineCacheCompatibility(const VkPipelineCacheHeaderVersionOne *header) const noexcept
{
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(handle, &properties);
    VkPipelineCacheHeaderVersionOne compatibleHeader;
    compatibleHeader.headerSize = sizeof(VkPipelineCacheHeaderVersionOne);
    compatibleHeader.headerVersion = VK_PIPELINE_CACHE_HEADER_VERSION_ONE;
    compatibleHeader.vendorID = properties.vendorID;
    compatibleHeader.deviceID = properties.deviceID;
    memcpy(compatibleHeader.pipelineCacheUUID, properties.pipelineCacheUUID, VK_UUID_SIZE);
    return core::compare(header, &compatibleHeader);
}

inline VkInstance PhysicalDevice::getNativeInstance() const noexcept
{
    return instance->getHandle();
}

VkPhysicalDeviceFeatures PhysicalDevice::getFeatures2(void *features) const
{
#ifdef VK_KHR_get_physical_device_properties2
    VkPhysicalDeviceFeatures2KHR features2;
    features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR;
    features2.pNext = features;
    MAGMA_REQUIRED_INSTANCE_EXTENSION(vkGetPhysicalDeviceFeatures2KHR, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
    vkGetPhysicalDeviceFeatures2KHR(handle, &features2);
    return features2.features;
#else
    MAGMA_UNUSED(features);
    return {};
#endif // VK_KHR_get_physical_device_properties2
}

VkPhysicalDeviceProperties PhysicalDevice::getProperties2(void *properties) const
{
#ifdef VK_KHR_get_physical_device_properties2
    VkPhysicalDeviceProperties2KHR properties2;
    properties2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2_KHR;
    properties2.pNext = properties;
    MAGMA_REQUIRED_INSTANCE_EXTENSION(vkGetPhysicalDeviceProperties2KHR, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
    vkGetPhysicalDeviceProperties2KHR(handle, &properties2);
    return properties2.properties;
#else
    MAGMA_UNUSED(properties);
    return {};
#endif // VK_KHR_get_physical_device_properties2
}

#ifdef VK_KHR_surface
VkSurfaceCapabilitiesKHR PhysicalDevice::getSurfaceCapabilities2(lent_ptr<const Surface> surface, void *surfaceCapabilities) const
{
#ifdef VK_KHR_get_surface_capabilities2
    VkPhysicalDeviceSurfaceInfo2KHR surfaceInfo;
    surfaceInfo.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR;
    surfaceInfo.pNext = nullptr;
    surfaceInfo.surface = *surface;
    VkSurfaceCapabilities2KHR surfaceCapabilities2;
    surfaceCapabilities2.sType = VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_KHR;
    surfaceCapabilities2.pNext = surfaceCapabilities;
    MAGMA_REQUIRED_INSTANCE_EXTENSION(vkGetPhysicalDeviceSurfaceCapabilities2KHR, VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME);
    const VkResult result = vkGetPhysicalDeviceSurfaceCapabilities2KHR(handle, &surfaceInfo, &surfaceCapabilities2);
    MAGMA_HANDLE_RESULT(result, "failed to get surface capabilities");
    return surfaceCapabilities2.surfaceCapabilities;
#else
    MAGMA_UNUSED(surface);
    MAGMA_UNUSED(surfaceCapabilities);
    return {};
#endif // VK_KHR_get_surface_capabilities2
}
#endif // VK_KHR_surface
} // namespace magma
