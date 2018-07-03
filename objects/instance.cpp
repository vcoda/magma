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
#include "instance.h"
#include "physicalDevice.h"
#include "../allocator/allocator.h"
#include "../helpers/stackArray.h"
#include "../misc/exception.h"

namespace magma
{
Instance::Instance(const char *applicationName, const char *engineName, uint32_t apiVersion,
    const std::vector<const char *>& layerNames, const std::vector<const char *>& extensionNames,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Dispatchable<VkInstance>(VK_DEBUG_REPORT_OBJECT_TYPE_INSTANCE_EXT, nullptr, std::move(allocator))
{
    VkApplicationInfo appInfo;
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = nullptr;
    appInfo.pApplicationName = applicationName;
    appInfo.applicationVersion = 1;
    appInfo.pEngineName = engineName;
    appInfo.engineVersion = 1;
    appInfo.apiVersion = apiVersion;
    VkInstanceCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledLayerCount = MAGMA_COUNT(layerNames);
    createInfo.ppEnabledLayerNames = layerNames.data();
    createInfo.enabledExtensionCount = MAGMA_COUNT(extensionNames);
    createInfo.ppEnabledExtensionNames = extensionNames.data();
    const VkResult create = vkCreateInstance(&createInfo, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create Vulkan instance");
}

Instance::~Instance()
{
    vkDestroyInstance(handle, MAGMA_OPTIONAL_INSTANCE(allocator));
}

uint32_t Instance::countPhysicalDevices() const
{
    uint32_t physicalDeviceCount = 0;
    const VkResult enumerate = vkEnumeratePhysicalDevices(handle, &physicalDeviceCount, nullptr);
    MAGMA_THROW_FAILURE(enumerate, "failed to count physical devices");
    return physicalDeviceCount;
}

std::shared_ptr<PhysicalDevice> Instance::getPhysicalDevice(uint32_t deviceId)
{
    uint32_t physicalDeviceCount = countPhysicalDevices();
    if (deviceId >= physicalDeviceCount)
        MAGMA_THROW("invalid parameter <deviceId>");
    MAGMA_STACK_ARRAY(VkPhysicalDevice, physicalDevices, physicalDeviceCount);
    const VkResult enumerate = vkEnumeratePhysicalDevices(handle, &physicalDeviceCount, physicalDevices);
    MAGMA_THROW_FAILURE(enumerate, "failed to enumerate physical devices");
    VkPhysicalDevice physicalDevice = physicalDevices[deviceId];
    return std::shared_ptr<PhysicalDevice>(new PhysicalDevice(shared_from_this(), physicalDevice));
}

std::set<std::string> Instance::enumerateExtensions(const char *layerName /* nullptr */) const
{
    uint32_t propertyCount = 0;
    const VkResult count = vkEnumerateInstanceExtensionProperties(layerName, &propertyCount, nullptr);
    MAGMA_THROW_FAILURE(count, "failed to count instance extensions");
    std::vector<VkExtensionProperties> properties(propertyCount);
    const VkResult enumerate = vkEnumerateInstanceExtensionProperties(layerName, &propertyCount, properties.data());
    MAGMA_THROW_FAILURE(enumerate, "failed to enumerate instance extensions");
    std::set<std::string> extensions;
    for (const auto& property : properties)
        extensions.emplace(property.extensionName);
    return extensions;
}
} // namespace magma
