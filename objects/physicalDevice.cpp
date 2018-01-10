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
#include "physicalDevice.h"
#include "instance.h"
#include "device.h"
#include "surface.h"
#include "../helpers/stackArray.h"

namespace magma
{
PhysicalDevice::PhysicalDevice(VkPhysicalDevice physicalDevice):
    Handle(VK_DEBUG_REPORT_OBJECT_TYPE_PHYSICAL_DEVICE_EXT, physicalDevice)
{}

std::shared_ptr<Device> PhysicalDevice::createDevice(const std::vector<const char *>& layers, const std::vector<const char *>& extensions, 
    const VkPhysicalDeviceFeatures& deviceFeatures)
{
    float priority = 0.0f; // TODO: add queue priorities vector
    VkDeviceQueueCreateInfo queueInfo;
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.pNext = nullptr;
    queueInfo.flags = 0;
    queueInfo.queueFamilyIndex = 0;
    queueInfo.queueCount = 1;
    queueInfo.pQueuePriorities = &priority;
    VkDeviceCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.queueCreateInfoCount = 1;
    info.pQueueCreateInfos = &queueInfo;
    info.enabledLayerCount = MAGMA_COUNT(layers);
    info.ppEnabledLayerNames = layers.data();
    info.enabledExtensionCount = MAGMA_COUNT(extensions);
    info.ppEnabledExtensionNames = extensions.data();
    info.pEnabledFeatures = &deviceFeatures;
    VkDevice nativeDevice;
    const VkResult create = vkCreateDevice(handle, &info, nullptr, &nativeDevice);
    MAGMA_THROW_FAILURE(create, "failed to create device");
    std::shared_ptr<Device> device(new Device(nativeDevice));
    device->device = device;
    return device;
}

std::shared_ptr<Device> PhysicalDevice::createDefaultDevice()
{
    const std::vector<const char*> noLayers;
    const std::vector<const char*> KHR_swapchain = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    const VkPhysicalDeviceFeatures noFeatures = {0};
    return createDevice(noLayers, KHR_swapchain, noFeatures);
}

std::set<std::string> PhysicalDevice::enumerateExtensions(const char *layerName /* nullptr */) const
{
    uint32_t propertyCount = 0;
    const VkResult count = vkEnumerateDeviceExtensionProperties(handle, layerName, &propertyCount, nullptr);
    MAGMA_THROW_FAILURE(count, "failed to count device extensions");
    std::vector<VkExtensionProperties> properties(propertyCount);
    const VkResult enumerate = vkEnumerateDeviceExtensionProperties(handle, layerName, &propertyCount, properties.data());
    MAGMA_THROW_FAILURE(count, "failed to enumerate device extensions");
    std::set<std::string> extensions;
    for (const auto& property : properties)
        extensions.insert(property.extensionName);
    return extensions;
}

bool PhysicalDevice::surfaceSupported(std::shared_ptr<Surface> surface) const
{
    VkBool32 supported = VK_FALSE;
    const VkResult get = vkGetPhysicalDeviceSurfaceSupportKHR(handle, 0, *surface, &supported);
    return (VK_SUCCESS == get) && (VK_TRUE == supported);
}

bool PhysicalDevice::depthStencilSupported(VkFormat format) const
{
    VkFormatProperties properties;
    vkGetPhysicalDeviceFormatProperties(handle, format, &properties);
    return (properties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

std::vector<VkSurfaceFormatKHR> PhysicalDevice::getSurfaceFormats(std::shared_ptr<Surface> surface) const
{
    uint32_t formatCount = 0;
    const VkResult count = vkGetPhysicalDeviceSurfaceFormatsKHR(handle, *surface, &formatCount, nullptr);
    MAGMA_THROW_FAILURE(count, "failed to count surface formats");
    std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
    const VkResult get = vkGetPhysicalDeviceSurfaceFormatsKHR(handle, *surface, &formatCount, surfaceFormats.data());
    MAGMA_THROW_FAILURE(get, "failed to get surface formats");
    return surfaceFormats;
}

void PhysicalDevice::getSurfaceCapabilities(std::shared_ptr<Surface> surface, VkSurfaceCapabilitiesKHR& caps) const
{
    const VkResult get = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(handle, *surface, &caps);
    MAGMA_THROW_FAILURE(get, "failed to get surface capabilities");
}
} // namespace magma
