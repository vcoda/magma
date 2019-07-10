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
#include <cstring>
#include "physicalDevice.h"
#include "instance.h"
#include "device.h"
#include "surface.h"
#include "display.h"
#include "pipelineCache.h"
#include "../misc/instanceExtension.h"
#include "../helpers/stackArray.h"
#include "../internal/compare.h"

namespace magma
{
PhysicalDevice::PhysicalDevice(std::shared_ptr<Instance> instance, VkPhysicalDevice handle,
    std::shared_ptr<IAllocator> allocator) noexcept:
    Dispatchable<VkPhysicalDevice>(VK_OBJECT_TYPE_PHYSICAL_DEVICE, nullptr, std::move(allocator)),
    instance(std::move(instance))
{
    this->handle = handle;
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
    const VkResult get = vkGetPhysicalDeviceImageFormatProperties(handle, format, imageType,
        optimalTiling ? VK_IMAGE_TILING_OPTIMAL : VK_IMAGE_TILING_LINEAR,
        usage, flags, &imageFormatProperties);
    MAGMA_THROW_FAILURE(get, "failed to get image format properties");
    return imageFormatProperties;
}

VkPhysicalDeviceProperties PhysicalDevice::getProperties() const noexcept
{
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(handle, &properties);
    return properties;
}

std::vector<VkQueueFamilyProperties> PhysicalDevice::getQueueFamilyProperties() const noexcept
{
    uint32_t propertyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(handle, &propertyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilyProperties(propertyCount);
    if (propertyCount > 0)
        vkGetPhysicalDeviceQueueFamilyProperties(handle, &propertyCount, queueFamilyProperties.data());
    return queueFamilyProperties;
}

VkPhysicalDeviceMemoryProperties PhysicalDevice::getMemoryProperties() const noexcept
{
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(handle, &memoryProperties);
    return memoryProperties;
}

std::shared_ptr<Device> PhysicalDevice::createDevice(
    const std::vector<DeviceQueueDescriptor>& queueDescriptors,
    const std::vector<const char *>& layers,
    const std::vector<const char *>& extensions,
    const VkPhysicalDeviceFeatures& deviceFeatures,
    const std::vector<void *>& extendedDeviceFeatures /* {} */) const
{
    return std::shared_ptr<Device>(new Device(
        std::const_pointer_cast<PhysicalDevice>(shared_from_this()),
        queueDescriptors, layers, extensions,
        deviceFeatures, extendedDeviceFeatures,
        this->allocator));
}

std::vector<VkLayerProperties> PhysicalDevice::enumerateLayers() const
{
    uint32_t propertyCount = 0;
    const VkResult count = vkEnumerateDeviceLayerProperties(handle, &propertyCount, nullptr);
    MAGMA_THROW_FAILURE(count, "failed to count device layers");
    std::vector<VkLayerProperties> layers(propertyCount);
    if (propertyCount > 0)
    {
        const VkResult enumerate = vkEnumerateDeviceLayerProperties(handle, &propertyCount, layers.data());
        MAGMA_THROW_FAILURE(enumerate, "failed to enumerate device layers");
    }
    return layers;
}

std::vector<VkExtensionProperties> PhysicalDevice::enumerateExtensions(const char *layerName /* nullptr */) const
{
    uint32_t propertyCount = 0;
    const VkResult count = vkEnumerateDeviceExtensionProperties(handle, layerName, &propertyCount, nullptr);
    MAGMA_THROW_FAILURE(count, "failed to count device extensions");
    std::vector<VkExtensionProperties> extensions(propertyCount);
    if (propertyCount > 0)
    {
        const VkResult enumerate = vkEnumerateDeviceExtensionProperties(handle, layerName, &propertyCount, extensions.data());
        MAGMA_THROW_FAILURE(enumerate, "failed to enumerate device extensions");
    }
    return extensions;
}

bool PhysicalDevice::checkExtensionSupport(const char *extensionName) const
{
    MAGMA_ASSERT(extensionName);
    if (!extensionName || !strlen(extensionName))
        return false;
    std::vector<VkExtensionProperties> extensions = enumerateExtensions();
    for (const auto& property : extensions)
    {
        if (0 == strcmp(extensionName, property.extensionName))
            return true;
    }
    return false;
}

bool PhysicalDevice::getSurfaceSupport(std::shared_ptr<Surface> surface) const noexcept
{
    VkBool32 supported;
    const VkResult get = vkGetPhysicalDeviceSurfaceSupportKHR(handle, 0, *surface, &supported);
    return (VK_SUCCESS == get) && (VK_TRUE == supported);
}

VkSurfaceCapabilitiesKHR PhysicalDevice::getSurfaceCapabilities(std::shared_ptr<const Surface> surface) const
{
    VkSurfaceCapabilitiesKHR caps;
    const VkResult get = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(handle, *surface, &caps);
    MAGMA_THROW_FAILURE(get, "failed to get surface capabilities");
    return caps;
}

std::vector<VkSurfaceFormatKHR> PhysicalDevice::getSurfaceFormats(std::shared_ptr<const Surface> surface) const
{
    uint32_t formatCount;
    const VkResult count = vkGetPhysicalDeviceSurfaceFormatsKHR(handle, *surface, &formatCount, nullptr);
    MAGMA_THROW_FAILURE(count, "failed to count surface formats");
    std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
    if (formatCount > 0)
    {
        const VkResult get = vkGetPhysicalDeviceSurfaceFormatsKHR(handle, *surface, &formatCount, surfaceFormats.data());
        MAGMA_THROW_FAILURE(get, "failed to get surface formats");
    }
    return surfaceFormats;
}

std::vector<VkPresentModeKHR> PhysicalDevice::getSurfacePresentModes(std::shared_ptr<const Surface> surface) const
{
    uint32_t presentModeCount;
    const VkResult count = vkGetPhysicalDeviceSurfacePresentModesKHR(handle, *surface, &presentModeCount, nullptr);
    MAGMA_THROW_FAILURE(count, "failed to count surface present modes");
    std::vector<VkPresentModeKHR> surfacePresentModes(presentModeCount);
    if (presentModeCount > 0)
    {
        const VkResult get = vkGetPhysicalDeviceSurfacePresentModesKHR(handle, *surface, &presentModeCount, surfacePresentModes.data());
        MAGMA_THROW_FAILURE(get, "failed to get surface present modes");
    }
    return surfacePresentModes;
}

bool PhysicalDevice::getPresentationSupport(uint32_t queueFamilyIndex,
    void *display /* nullptr */,
    const void *visualID /* nullptr */) const noexcept
{
#if defined(VK_USE_PLATFORM_WIN32_KHR)
#if defined(_MSC_VER)
    display;
    visualID;
#endif
    const VkBool32 get = vkGetPhysicalDeviceWin32PresentationSupportKHR(handle, queueFamilyIndex);
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
    MAGMA_ASSERT(display);
    MAGMA_ASSERT(visualID);
    const VkBool32 get = vkGetPhysicalDeviceXlibPresentationSupportKHR(handle, queueFamilyIndex,
        reinterpret_cast<::Display *>(display),
        *reinterpret_cast<const VisualID *>(visualID));
#elif defined(VK_USE_PLATFORM_XCB_KHR)
    MAGMA_ASSERT(display);
    MAGMA_ASSERT(visualID);
    const VkBool32 get = vkGetPhysicalDeviceXcbPresentationSupportKHR(handle, queueFamilyIndex,
        reinterpret_cast<xcb_connection_t *>(display),
        *reinterpret_cast<const xcb_visualid_t *>(visualID));
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
    MAGMA_ASSERT(display);
    const VkBool32 get = vkGetPhysicalDeviceWaylandPresentationSupportKHR(handle, queueFamilyIndex,
        reinterpret_cast<wl_display *>(display));
#elif defined(VK_USE_PLATFORM_MIR_KHR)
    MAGMA_ASSERT(display);
    const VkBool32 get = vkGetPhysicalDeviceMirPresentationSupportKHR(handle, queueFamilyIndex,
        reinterpret_cast<MirConnection *>(display));
#endif
    return (get ? true : false);
}

void PhysicalDevice::getExtendedProperties(void *properties) const
{
    MAGMA_ASSERT(properties);
    if (properties)
    {
        VkPhysicalDeviceProperties2 properties2;
        properties2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
        properties2.pNext = properties;
        MAGMA_INSTANCE_EXTENSION(vkGetPhysicalDeviceProperties2KHR, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
        vkGetPhysicalDeviceProperties2KHR(handle, &properties2);
    }
}

std::vector<VkDisplayPropertiesKHR> PhysicalDevice::getDisplayProperties() const
{
    uint32_t propertyCount;
    MAGMA_INSTANCE_EXTENSION(vkGetPhysicalDeviceDisplayPropertiesKHR, VK_KHR_DISPLAY_EXTENSION_NAME);
    const VkResult count = vkGetPhysicalDeviceDisplayPropertiesKHR(handle, &propertyCount, nullptr);
    MAGMA_THROW_FAILURE(count, "failed to count display properties");
    std::vector<VkDisplayPropertiesKHR> displayProperties(propertyCount);
    if (propertyCount > 0)
    {
        const VkResult get = vkGetPhysicalDeviceDisplayPropertiesKHR(handle, &propertyCount, displayProperties.data());
        MAGMA_THROW_FAILURE(get, "failed to get display properties");
    }
    return displayProperties;
}

std::vector<VkDisplayPlanePropertiesKHR> PhysicalDevice::getDisplayPlaneProperties() const
{
    uint32_t propertyCount;
    MAGMA_INSTANCE_EXTENSION(vkGetPhysicalDeviceDisplayPropertiesKHR, VK_KHR_DISPLAY_EXTENSION_NAME);
    const VkResult count = vkGetPhysicalDeviceDisplayPlanePropertiesKHR(handle, &propertyCount, nullptr);
    MAGMA_THROW_FAILURE(count, "failed to count display plane properties");
    std::vector<VkDisplayPlanePropertiesKHR> displayPlaneProperties(propertyCount);
    if (propertyCount > 0)
    {
        const VkResult get = vkGetPhysicalDeviceDisplayPlanePropertiesKHR(handle, &propertyCount, displayPlaneProperties.data());
        MAGMA_THROW_FAILURE(get, "failed to get display plane properties");
    }
    return displayPlaneProperties;
}

std::vector<std::shared_ptr<Display>> PhysicalDevice::getSupportedDisplays(uint32_t planeIndex) const
{
    uint32_t displayCount;
    MAGMA_INSTANCE_EXTENSION(vkGetDisplayPlaneSupportedDisplaysKHR, VK_KHR_DISPLAY_EXTENSION_NAME);
    const VkResult count = vkGetDisplayPlaneSupportedDisplaysKHR(handle, planeIndex, &displayCount, nullptr);
    MAGMA_THROW_FAILURE(count, "failed to count supported displays");
    std::vector<std::shared_ptr<Display>> supportedDisplays;
    if (displayCount > 0)
    {
        MAGMA_STACK_ARRAY(VkDisplayKHR, displays, displayCount);
        const VkResult get = vkGetDisplayPlaneSupportedDisplaysKHR(handle, planeIndex, &displayCount, displays);
        MAGMA_THROW_FAILURE(get, "failed to get supported displays");
        for (const VkDisplayKHR handle : displays)
            supportedDisplays.emplace_back(new Display(shared_from_this(), handle, planeIndex));
    }
    return supportedDisplays;
}

VkPhysicalDeviceShaderCorePropertiesAMD PhysicalDevice::getShaderCoreProperties() const
{
    VkPhysicalDeviceShaderCorePropertiesAMD shaderCoreProperties = {};
    shaderCoreProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_CORE_PROPERTIES_AMD;
    getExtendedProperties(&shaderCoreProperties);
    return shaderCoreProperties;
}

VkPhysicalDeviceRayTracingPropertiesNV PhysicalDevice::getRaytracingProperties() const
{
    VkPhysicalDeviceRayTracingPropertiesNV raytracingProperties = {};
    raytracingProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PROPERTIES_NV;
    getExtendedProperties(&raytracingProperties);
    return raytracingProperties;
}

std::shared_ptr<Device> PhysicalDevice::createDefaultDevice() const
{
    const std::vector<float> defaultQueuePriorities = {1.0f};
    const std::vector<DeviceQueueDescriptor> queueDescriptors = {
        DeviceQueueDescriptor(VK_QUEUE_GRAPHICS_BIT, shared_from_this(), defaultQueuePriorities)
    };
    const std::vector<const char*> noLayers;
    const std::vector<const char*> swapchainExtension = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    const VkPhysicalDeviceFeatures noDeviceFeatures = {};
    const std::vector<void *> noExtendedDeviceFeatures;
    return createDevice(queueDescriptors, noLayers, swapchainExtension, noDeviceFeatures, noExtendedDeviceFeatures);
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
    if (!internal::compare(cacheHeader, &header))
        return false;
    return true;
}

PhysicalDeviceGroup::PhysicalDeviceGroup(const std::vector<std::shared_ptr<PhysicalDevice>>& physicalDevices, uint32_t groupId) noexcept:
    physicalDevices(physicalDevices),
    groupId(groupId)
{}

std::shared_ptr<Device> PhysicalDeviceGroup::createDevice(const std::vector<DeviceQueueDescriptor>& queueDescriptors,
    const std::vector<const char *>& layers,
    const std::vector<const char *>& extensions,
    const VkPhysicalDeviceFeatures& deviceFeatures,
    const std::vector<void *>& extendedDeviceFeatures /* {} */) const
{
    VkDeviceGroupDeviceCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_DEVICE_GROUP_DEVICE_CREATE_INFO;
    info.pNext = nullptr;
    info.physicalDeviceCount = physicalDeviceCount();
    MAGMA_STACK_ARRAY(VkPhysicalDevice, dereferencedPhysicalDevices, info.physicalDeviceCount);
    for (const auto& physicalDevice : physicalDevices)
        dereferencedPhysicalDevices.put(*physicalDevice);
    info.pPhysicalDevices = dereferencedPhysicalDevices;
    std::vector<void *> extendedDeviceGroupFeatures = extendedDeviceFeatures;
    extendedDeviceGroupFeatures.push_back(&info);
    return physicalDevices.front()->createDevice(queueDescriptors,
        layers, extensions,
        deviceFeatures, extendedDeviceGroupFeatures);
}
} // namespace magma
