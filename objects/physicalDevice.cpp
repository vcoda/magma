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
#include "../shared.h"

namespace magma
{
DeviceQueueDescriptor::DeviceQueueDescriptor(VkQueueFlagBits queueType,
    std::shared_ptr<const PhysicalDevice> device,
    const std::vector<float>& queuePriorities)
{
    sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    queueFamilyIndex = getFamilyIndex(queueType, device->getQueueFamilyProperties());
    queueCount = MAGMA_COUNT(queuePriorities);
    for (float priority : queuePriorities)
        MAGMA_ASSERT(priority >= 0.f && priority <= 1.f);
    pQueuePriorities = helpers::copy(new float[queueCount], queuePriorities);
}

DeviceQueueDescriptor::DeviceQueueDescriptor(const DeviceQueueDescriptor& other)
{
    helpers::copy(this, &other);
    pQueuePriorities = helpers::copy(new float[queueCount], other.pQueuePriorities, queueCount);
}

DeviceQueueDescriptor& DeviceQueueDescriptor::operator=(const DeviceQueueDescriptor& other)
{
    if (this != &other)
    {
        helpers::copy(this, &other);
        pQueuePriorities = helpers::copy(new float[queueCount], other.pQueuePriorities, queueCount);
    }
    return *this;
}

DeviceQueueDescriptor::~DeviceQueueDescriptor()
{
    delete[] pQueuePriorities;
}

uint32_t DeviceQueueDescriptor::getFamilyIndex(VkQueueFlagBits queueType,
    const std::vector<VkQueueFamilyProperties>& queueFamilyProperties) const
{
    if (VK_QUEUE_COMPUTE_BIT == queueType)
    {   // Try to find dedicated compute queue
        uint32_t queueFamilyIndex = 0;
        for (const auto& property : queueFamilyProperties)
        {
            if (property.queueFlags & VK_QUEUE_COMPUTE_BIT)
            {
                const VkFlags hasGraphics = property.queueFlags & VK_QUEUE_GRAPHICS_BIT;
                if (!hasGraphics)
                    return queueFamilyIndex;
            }
            ++queueFamilyIndex;
        }
    } else if (VK_QUEUE_TRANSFER_BIT == queueType)
    {   // Try to find dedicated transfer queue
        uint32_t queueFamilyIndex = 0;
        for (const auto& property : queueFamilyProperties)
        {
            if (property.queueFlags & VK_QUEUE_TRANSFER_BIT)
            {
                const VkFlags hasGraphicsOrCompute = property.queueFlags & (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);
                if (!hasGraphicsOrCompute)
                    return queueFamilyIndex;
            }
            ++queueFamilyIndex;
        }
    }
    uint32_t queueFamilyIndex = 0;
    for (const auto& property : queueFamilyProperties)
    {   // Try to find any suitable family
        if (property.queueFlags & queueType)
            return queueFamilyIndex;
        ++queueFamilyIndex;
    }
    MAGMA_THROW("could not find suitable queue family");
    return 0;
}

PhysicalDevice::PhysicalDevice(VkPhysicalDevice physicalDevice):
    Handle(VK_DEBUG_REPORT_OBJECT_TYPE_PHYSICAL_DEVICE_EXT, physicalDevice)
{}

const VkPhysicalDeviceFeatures& PhysicalDevice::getFeatures() const
{
    vkGetPhysicalDeviceFeatures(handle, &features);
    return features;
}

const VkPhysicalDeviceProperties& PhysicalDevice::getProperties() const
{
    vkGetPhysicalDeviceProperties(handle, &properties);
    return properties;
}

VkFormatProperties PhysicalDevice::getFormatProperties(VkFormat format) const
{
    VkFormatProperties formatProperties;
    vkGetPhysicalDeviceFormatProperties(handle, format, &formatProperties);
    return formatProperties;
}

VkImageFormatProperties PhysicalDevice::getImageFormatProperties(VkFormat format, 
    VkImageType imageType, bool optimalTiling, VkImageUsageFlags usage,
    VkImageCreateFlags flags /* 0* */) const
{
    VkImageFormatProperties imageFormatProperties;
    VkResult get = vkGetPhysicalDeviceImageFormatProperties(handle, format, imageType,
        optimalTiling ? VK_IMAGE_TILING_OPTIMAL : VK_IMAGE_TILING_LINEAR,
        usage, flags, &imageFormatProperties);
    MAGMA_THROW_FAILURE(get, "failed to get image format properties");
    return imageFormatProperties;
}

std::vector<VkQueueFamilyProperties> PhysicalDevice::getQueueFamilyProperties() const
{
    std::vector<VkQueueFamilyProperties> queueFamilyProperties;
    uint32_t propertyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(handle, &propertyCount, nullptr);
    if (propertyCount > 0)
    {
        queueFamilyProperties.resize(propertyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(handle, &propertyCount, queueFamilyProperties.data());
    }
    return queueFamilyProperties;
}

const VkPhysicalDeviceMemoryProperties& PhysicalDevice::getMemoryProperties() const
{
    vkGetPhysicalDeviceMemoryProperties(handle, &memoryProperties);
    return memoryProperties;
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

std::vector<VkLayerProperties> PhysicalDevice::enumerateLayerProperties() const
{
    uint32_t propertyCount = 0;
    VkResult count = vkEnumerateDeviceLayerProperties(handle, &propertyCount, nullptr);
    MAGMA_THROW_FAILURE(count, "failed to count device layers");
    std::vector<VkLayerProperties> properties(propertyCount);
    const VkResult enumerate = vkEnumerateDeviceLayerProperties(handle, &propertyCount, properties.data());
    MAGMA_THROW_FAILURE(count, "failed to enumerate device layers");
    return properties;
}

std::shared_ptr<Device> PhysicalDevice::createDevice(
    const std::vector<DeviceQueueDescriptor>& queueDescriptors,
    const std::vector<const char *>& layers, 
    const std::vector<const char *>& extensions, 
    const VkPhysicalDeviceFeatures& deviceFeatures) const
{                                
    VkDeviceCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.queueCreateInfoCount = MAGMA_COUNT(queueDescriptors);
    info.pQueueCreateInfos = queueDescriptors.data();
    info.enabledLayerCount = MAGMA_COUNT(layers);
    info.ppEnabledLayerNames = layers.data();
    info.enabledExtensionCount = MAGMA_COUNT(extensions);
    info.ppEnabledExtensionNames = extensions.data();
    info.pEnabledFeatures = &deviceFeatures;
    VkDevice logicalDevice;
    const VkResult create = vkCreateDevice(handle, &info, nullptr, &logicalDevice);
    MAGMA_THROW_FAILURE(create, "failed to create device");
    std::vector<VkDeviceQueueCreateInfo> deviceQueues;
    for (const auto& desc : queueDescriptors)
        deviceQueues.push_back(desc);
    std::shared_ptr<Device> device(new Device(logicalDevice, shared_from_this(), deviceQueues));
    device->device = device;
    return device;
}

std::shared_ptr<Device> PhysicalDevice::createDefaultDevice() const
{
    std::vector<float> defaultQueuePriorities = {1.0f};
    const std::vector<DeviceQueueDescriptor> queueDescriptors = {
        DeviceQueueDescriptor(VK_QUEUE_GRAPHICS_BIT, shared_from_this(), defaultQueuePriorities)
    };
    const std::vector<const char*> noLayers;
    const std::vector<const char*> extensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    const VkPhysicalDeviceFeatures noFeatures = {0};
    return createDevice(queueDescriptors, noLayers, extensions, noFeatures);
}

bool PhysicalDevice::surfaceSupported(std::shared_ptr<Surface> surface) const
{
    VkBool32 supported = VK_FALSE;
    const VkResult get = vkGetPhysicalDeviceSurfaceSupportKHR(handle, 0, *surface, &supported);
    return (VK_SUCCESS == get) && (VK_TRUE == supported);
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
