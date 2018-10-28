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
#include "../misc/instanceExtension.h"
#include "../misc/exception.h"
#include "../helpers/stackArray.h"
#include "../utilities/refCountChecker.h"

// Redefine macro for Instance object as here we use handle directly
#undef MAGMA_INSTANCE_EXTENSION
#define MAGMA_INSTANCE_EXTENSION(func, extension)\
    magma::InstanceExtension<PFN_##func> func(handle, MAGMA_STRINGIZE(func), extension)

#ifdef MAGMA_DEBUG
static magma::utilities::RefCountChecker _refCountChecker;
#endif

namespace magma
{
Instance::Instance(const char *applicationName, const char *engineName, uint32_t apiVersion,
    const std::vector<const char *>& layerNames, const std::vector<const char *>& extensionNames,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Dispatchable<VkInstance>(VK_OBJECT_TYPE_INSTANCE, nullptr, std::move(allocator))
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
#ifdef MAGMA_DEBUG
    _refCountChecker.addRef();
#endif
}

Instance::~Instance()
{
    vkDestroyInstance(handle, MAGMA_OPTIONAL_INSTANCE(allocator));
#ifdef MAGMA_DEBUG
    _refCountChecker.release();
#endif
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
    return std::shared_ptr<PhysicalDevice>(new PhysicalDevice(shared_from_this(), physicalDevice, allocator));
}

std::vector<VkPhysicalDeviceGroupProperties> Instance::enumeratePhysicalDeviceGroups() const
{
    uint32_t physicalDeviceGroupCount = 0;
    MAGMA_INSTANCE_EXTENSION(vkEnumeratePhysicalDeviceGroupsKHR, VK_KHR_DEVICE_GROUP_CREATION_EXTENSION_NAME);
    const VkResult count = vkEnumeratePhysicalDeviceGroupsKHR(handle, &physicalDeviceGroupCount, nullptr);
    MAGMA_THROW_FAILURE(count, "failed to count groups of physical devices");
    std::vector<VkPhysicalDeviceGroupProperties> physicalDeviceGroups(physicalDeviceGroupCount);
    if (physicalDeviceGroupCount > 0)
    {
        const VkResult enumerate = vkEnumeratePhysicalDeviceGroupsKHR(handle, &physicalDeviceGroupCount, physicalDeviceGroups.data());
        MAGMA_THROW_FAILURE(enumerate, "failed to enumerate groups of physical devices");
    }
    return physicalDeviceGroups;
}

std::shared_ptr<PhysicalDeviceGroup> Instance::getPhysicalDeviceGroup(uint32_t groupId)
{
    const std::vector<VkPhysicalDeviceGroupProperties>& deviceGroups = enumeratePhysicalDeviceGroups();
    if (groupId >= MAGMA_COUNT(deviceGroups))
        MAGMA_THROW("invalid <groupId> parameter");
    std::vector<std::shared_ptr<PhysicalDevice>> physicalDevices;
    const VkPhysicalDeviceGroupProperties& deviceGroupProperties = deviceGroups[groupId];
    for (uint32_t deviceId = 0; deviceId < deviceGroupProperties.physicalDeviceCount; ++deviceId)
    {
        VkPhysicalDevice physicalDevice = deviceGroupProperties.physicalDevices[deviceId];
        physicalDevices.emplace_back(new PhysicalDevice(shared_from_this(), physicalDevice, allocator));
    }
    return std::shared_ptr<PhysicalDeviceGroup>(new PhysicalDeviceGroup(physicalDevices, groupId));
}

std::vector<VkLayerProperties> Instance::enumerateLayers()
{
    uint32_t propertyCount = 0;
    const VkResult count = vkEnumerateInstanceLayerProperties(&propertyCount, nullptr);
    MAGMA_THROW_FAILURE(count, "failed to count instance layers");
    std::vector<VkLayerProperties> layers(propertyCount);
    if (propertyCount > 0)
    {
        const VkResult enumerate = vkEnumerateInstanceLayerProperties(&propertyCount, layers.data());
        MAGMA_THROW_FAILURE(enumerate, "failed to enumerate instance layers");
    }
    return layers;
}

std::vector<VkExtensionProperties> Instance::enumerateExtensions(const char *layerName /* nullptr */)
{
    uint32_t propertyCount = 0;
    const VkResult count = vkEnumerateInstanceExtensionProperties(layerName, &propertyCount, nullptr);
    MAGMA_THROW_FAILURE(count, "failed to count instance extensions");
    std::vector<VkExtensionProperties> extensions(propertyCount);
    if (propertyCount > 0)
    {
        const VkResult enumerate = vkEnumerateInstanceExtensionProperties(layerName, &propertyCount, extensions.data());
        MAGMA_THROW_FAILURE(enumerate, "failed to enumerate instance extensions");
    }
    return extensions;
}
} // namespace magma
