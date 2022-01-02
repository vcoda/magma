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
#include "instance.h"
#include "physicalDevice.h"
#include "physicalDeviceGroup.h"
#include "../allocator/allocator.h"
#include "../exceptions/errorResult.h"
#include "../helpers/stackArray.h"
#include "../core/refCountChecker.h"

// Redefine macro because here we use handle directly
// instead of using circular reference to itself
#ifdef MAGMA_HANDLE
#undef MAGMA_HANDLE
#define MAGMA_HANDLE(p) handle
#endif
#include "../misc/extProcAddress.h"

#ifdef MAGMA_DEBUG
static magma::core::RefCountChecker _refCountChecker;
#endif

namespace magma
{
Instance::Instance(const char *applicationName, const char *engineName, uint32_t apiVersion,
    const std::vector<const char *>& layerNames, const std::vector<const char *>& extensionNames,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
#if defined(VK_EXT_debug_utils)
    PFN_vkDebugUtilsMessengerCallbackEXT debugCallback /* nullptr */,
#elif defined(VK_EXT_debug_report)
    PFN_vkDebugReportCallbackEXT debugCallback /* nullptr */,
#endif
    void *userData /* nullptr */):
    Dispatchable<VkInstance>(VK_OBJECT_TYPE_INSTANCE, nullptr, std::move(allocator)),
    apiVersion(apiVersion)
{
    VkApplicationInfo appInfo;
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = nullptr;
    appInfo.pApplicationName = applicationName;
    appInfo.applicationVersion = 1;
    appInfo.pEngineName = engineName;
    appInfo.engineVersion = 1;
    appInfo.apiVersion = apiVersion;
    VkInstanceCreateInfo instanceInfo;
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pNext = nullptr;
    instanceInfo.flags = 0;
    instanceInfo.pApplicationInfo = &appInfo;
    instanceInfo.enabledLayerCount = MAGMA_COUNT(layerNames);
    instanceInfo.ppEnabledLayerNames = layerNames.data();
    instanceInfo.enabledExtensionCount = MAGMA_COUNT(extensionNames);
    instanceInfo.ppEnabledExtensionNames = extensionNames.data();
#if defined(VK_EXT_debug_utils)
    VkDebugUtilsMessengerCreateInfoEXT debugCallbackInfo;
    if (debugCallback)
    {
        debugCallbackInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debugCallbackInfo.pNext = nullptr;
        debugCallbackInfo.flags = 0;
        debugCallbackInfo.messageSeverity =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        debugCallbackInfo.messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debugCallbackInfo.pfnUserCallback = debugCallback;
        debugCallbackInfo.pUserData = userData;
        // To capture events that occur while creating or destroying an instance
        // an application can link a VkDebugUtilsMessengerCreateInfoEXT structure
        // to the pNext element of the VkInstanceCreateInfo structure.
        instanceInfo.pNext = &debugCallbackInfo;
    }
#elif defined(VK_EXT_debug_report)
    VkDebugReportCallbackCreateInfoEXT debugCallbackInfo;
    if (debugCallback)
    {
#ifdef VK_HEADER_VERSION
        debugCallbackInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
#else   // Compatibility with older SDK
        debugCallbackInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
#endif
        debugCallbackInfo.pNext = nullptr;
        debugCallbackInfo.flags =
            VK_DEBUG_REPORT_INFORMATION_BIT_EXT |
            VK_DEBUG_REPORT_WARNING_BIT_EXT |
            VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
            VK_DEBUG_REPORT_ERROR_BIT_EXT |
            VK_DEBUG_REPORT_DEBUG_BIT_EXT;
        debugCallbackInfo.pfnCallback = debugCallback;
        debugCallbackInfo.pUserData = userData;
        instanceInfo.pNext = &debugCallbackInfo;
    }
#endif // VK_EXT_debug_report
    const VkResult result = vkCreateInstance(&instanceInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    switch (result)
    {
    case VK_ERROR_INITIALIZATION_FAILED:
        throw exception::InitializationFailed("failed to create instance");
    case VK_ERROR_INCOMPATIBLE_DRIVER:
        throw exception::IncompatibleDriver("failed to create instance");
    }
    MAGMA_THROW_FAILURE(result, "failed to create instance");
#ifdef MAGMA_DEBUG
    _refCountChecker.addRef();
#endif
}

Instance::~Instance()
{
    vkDestroyInstance(handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
#ifdef MAGMA_DEBUG
    _refCountChecker.release();
#endif
}

uint32_t Instance::countPhysicalDevices() const
{
    uint32_t physicalDeviceCount = 0;
    const VkResult result = vkEnumeratePhysicalDevices(handle, &physicalDeviceCount, nullptr);
    MAGMA_THROW_FAILURE(result, "failed to count physical devices");
    return physicalDeviceCount;
}

std::shared_ptr<PhysicalDevice> Instance::getPhysicalDevice(uint32_t deviceId) const
{
    uint32_t physicalDeviceCount = countPhysicalDevices();
    if (deviceId >= physicalDeviceCount)
        MAGMA_THROW("invalid parameter <deviceId>");
    MAGMA_STACK_ARRAY(VkPhysicalDevice, physicalDevices, physicalDeviceCount);
    const VkResult result = vkEnumeratePhysicalDevices(handle, &physicalDeviceCount, physicalDevices);
    MAGMA_THROW_FAILURE(result, "failed to enumerate physical devices");
    VkPhysicalDevice physicalDevice = physicalDevices[deviceId];
    std::shared_ptr<Instance> self = std::const_pointer_cast<Instance>(shared_from_this());
    return std::shared_ptr<PhysicalDevice>(new PhysicalDevice(self, physicalDevice, hostAllocator));
}

#ifdef VK_KHR_device_group
std::vector<VkPhysicalDeviceGroupPropertiesKHR> Instance::enumeratePhysicalDeviceGroups() const
{
    uint32_t physicalDeviceGroupCount = 0;
    MAGMA_INSTANCE_EXTENSION(vkEnumeratePhysicalDeviceGroupsKHR, VK_KHR_DEVICE_GROUP_CREATION_EXTENSION_NAME);
    VkResult result = vkEnumeratePhysicalDeviceGroupsKHR(handle, &physicalDeviceGroupCount, nullptr);
    MAGMA_THROW_FAILURE(result, "failed to count groups of physical devices");
    std::vector<VkPhysicalDeviceGroupPropertiesKHR> physicalDeviceGroups(physicalDeviceGroupCount);
    if (physicalDeviceGroupCount > 0)
    {
        result = vkEnumeratePhysicalDeviceGroupsKHR(handle, &physicalDeviceGroupCount, physicalDeviceGroups.data());
        MAGMA_THROW_FAILURE(result, "failed to enumerate groups of physical devices");
    }
    return physicalDeviceGroups;
}

std::shared_ptr<PhysicalDeviceGroup> Instance::getPhysicalDeviceGroup(uint32_t groupId)
{
    const std::vector<VkPhysicalDeviceGroupPropertiesKHR>& deviceGroups = enumeratePhysicalDeviceGroups();
    if (groupId >= MAGMA_COUNT(deviceGroups))
        MAGMA_THROW("invalid <groupId> parameter");
    std::vector<std::shared_ptr<PhysicalDevice>> physicalDevices;
    const VkPhysicalDeviceGroupProperties& deviceGroupProperties = deviceGroups[groupId];
    for (uint32_t deviceId = 0; deviceId < deviceGroupProperties.physicalDeviceCount; ++deviceId)
    {
        VkPhysicalDevice physicalDevice = deviceGroupProperties.physicalDevices[deviceId];
        physicalDevices.emplace_back(new PhysicalDevice(shared_from_this(), physicalDevice, hostAllocator));
    }
    return std::shared_ptr<PhysicalDeviceGroup>(new PhysicalDeviceGroup(physicalDevices, groupId));
}
#endif // VK_KHR_device_group

std::vector<VkLayerProperties> Instance::enumerateLayers()
{
    uint32_t propertyCount = 0;
    VkResult result = vkEnumerateInstanceLayerProperties(&propertyCount, nullptr);
    MAGMA_THROW_FAILURE(result, "failed to count instance layers");
    std::vector<VkLayerProperties> layers(propertyCount);
    if (propertyCount > 0)
    {
        result = vkEnumerateInstanceLayerProperties(&propertyCount, layers.data());
        MAGMA_THROW_FAILURE(result, "failed to enumerate instance layers");
    }
    return layers;
}

std::vector<VkExtensionProperties> Instance::enumerateExtensions(const char *layerName /* nullptr */)
{
    uint32_t propertyCount = 0;
    VkResult result = vkEnumerateInstanceExtensionProperties(layerName, &propertyCount, nullptr);
    MAGMA_THROW_FAILURE(result, "failed to count instance extensions");
    std::vector<VkExtensionProperties> extensions(propertyCount);
    if (propertyCount > 0)
    {
        result = vkEnumerateInstanceExtensionProperties(layerName, &propertyCount, extensions.data());
        MAGMA_THROW_FAILURE(result, "failed to enumerate instance extensions");
    }
    return extensions;
}

bool Instance::checkExtensionSupport(const char *extensionName) const
{
    MAGMA_ASSERT(extensionName);
    if (!extensionName || !strlen(extensionName))
        return false;
    if (extensions.empty())
    {   // Query once and cache
        const std::vector<VkExtensionProperties> extensionProperties = enumerateExtensions();
        for (const auto property : extensionProperties)
            extensions.emplace(property.extensionName);
    }
    return extensions.find(extensionName) != extensions.end();
}
} // namespace magma
