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
#include "instance.h"
#include "physicalDevice.h"
#include "physicalDeviceGroup.h"
#include "../misc/application.h"
#include "../allocator/allocator.h"
#include "../exceptions/errorResult.h"
#include "../core/refCountChecker.h"
#include "../misc/extension.h"

#ifdef MAGMA_DEBUG
static magma::core::RefCountChecker _refCountChecker;
#endif

namespace magma
{
Instance::Instance(const NullTerminatedStringArray& enabledLayers, const NullTerminatedStringArray& enabledExtensions,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    const Application *applicationInfo /* nullptr */,
    VkInstanceCreateFlags flags /* 0 */,
#ifdef VK_EXT_debug_report
    PFN_vkDebugReportCallbackEXT debugReportCallback /* nullptr */,
#endif
#ifdef VK_EXT_debug_utils
    PFN_vkDebugUtilsMessengerCallbackEXT debugUtilsCallback /* nullptr */,
#endif
    void *userData /* nullptr */,
    const StructureChain& extendedInfo /* default */):
    Dispatchable<VkInstance>(VK_OBJECT_TYPE_INSTANCE, std::move(allocator)),
    apiVersion(applicationInfo ? applicationInfo->apiVersion : VK_API_VERSION_1_0)
{
    VkInstanceCreateInfo instanceInfo;
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pNext = extendedInfo.headNode();
    instanceInfo.flags = flags;
    instanceInfo.pApplicationInfo = applicationInfo;
    instanceInfo.enabledLayerCount = core::countof(enabledLayers);
    instanceInfo.ppEnabledLayerNames = enabledLayers.data();
    instanceInfo.enabledExtensionCount = core::countof(enabledExtensions);
    instanceInfo.ppEnabledExtensionNames = enabledExtensions.data();
#ifdef VK_EXT_debug_report
    VkDebugReportCallbackCreateInfoEXT debugReportCallbackInfo;
    if (debugReportCallback && !instanceInfo.pNext)
    {
    #if VK_HEADER_VERSION > 1
        debugReportCallbackInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    #else // Compatibility with older SDK
        debugReportCallbackInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
    #endif
        debugReportCallbackInfo.pNext = nullptr;
        debugReportCallbackInfo.flags =
            VK_DEBUG_REPORT_INFORMATION_BIT_EXT |
            VK_DEBUG_REPORT_WARNING_BIT_EXT |
            VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
            VK_DEBUG_REPORT_ERROR_BIT_EXT |
            VK_DEBUG_REPORT_DEBUG_BIT_EXT;
        debugReportCallbackInfo.pfnCallback = debugReportCallback;
        debugReportCallbackInfo.pUserData = userData;
        linkNode(instanceInfo, debugReportCallbackInfo);
    }
#endif // VK_EXT_debug_report
#ifdef VK_EXT_debug_utils
    VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerInfo;
    if (debugUtilsCallback)
    {   // To capture events that occur while creating or destroying an instance
        // an application can link a VkDebugUtilsMessengerCreateInfoEXT structure
        // to the pNext element of the VkInstanceCreateInfo structure.
        debugUtilsMessengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debugUtilsMessengerInfo.pNext = nullptr;
        debugUtilsMessengerInfo.flags = 0;
        debugUtilsMessengerInfo.messageSeverity =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        debugUtilsMessengerInfo.messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debugUtilsMessengerInfo.pfnUserCallback = debugUtilsCallback;
        debugUtilsMessengerInfo.pUserData = userData;
        linkNode(instanceInfo, debugUtilsMessengerInfo);
    }
#endif // VK_EXT_debug_utils
    const VkResult result = vkCreateInstance(&instanceInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
#ifndef MAGMA_NO_EXCEPTIONS
    switch (result)
    {
    case VK_ERROR_INITIALIZATION_FAILED:
        throw exception::InitializationFailed("initialization of Vulkan could not be completed "
            "for implementation-specific reasons");
    case VK_ERROR_INCOMPATIBLE_DRIVER:
        throw exception::IncompatibleDriver("the requested version of Vulkan is not supported "
            "by the driver or is otherwise incompatible for implementation-specific reasons");
    default:
        break;
    }
#endif // !MAGMA_NO_EXCEPTIONS
    MAGMA_HANDLE_RESULT(result, "failed to create instance");
    for (auto const& properties: enumerateExtensions())
        extensions.emplace(properties.extensionName);
    // Store enabled layers and extensions
    for (auto const& layer: enabledLayers)
        this->enabledLayers.emplace(layer);
    for (auto const& extension: enabledExtensions)
        this->enabledExtensions.emplace(extension);
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

uint32_t Instance::enumeratePhysicalDevices() const
{
    uint32_t physicalDeviceCount = 0;
    const VkResult result = vkEnumeratePhysicalDevices(handle, &physicalDeviceCount, nullptr);
    MAGMA_HANDLE_RESULT(result, "failed to enumerate physical devices");
    return physicalDeviceCount;
}

std::shared_ptr<PhysicalDevice> Instance::getPhysicalDevice(uint32_t deviceId) const
{
    uint32_t physicalDeviceCount = 0;
    VkResult result = vkEnumeratePhysicalDevices(handle, &physicalDeviceCount, nullptr);
    if (!MAGMA_SUCCEEDED(result))
        return nullptr;
    MAGMA_ASSERT(deviceId < physicalDeviceCount);
    if (deviceId >= physicalDeviceCount)
        return nullptr;
    MAGMA_VLA(VkPhysicalDevice, physicalDevices, physicalDeviceCount);
    result = vkEnumeratePhysicalDevices(handle, &physicalDeviceCount, physicalDevices);
    MAGMA_HANDLE_RESULT(result, "failed to enumerate physical devices");
    VkPhysicalDevice physicalDevice = physicalDevices[deviceId];
    std::shared_ptr<Instance> instance = std::const_pointer_cast<Instance>(shared_from_this());
    return PhysicalDevice::makeShared(instance, physicalDevice, hostAllocator);
}

#ifdef VK_KHR_device_group
std::vector<VkPhysicalDeviceGroupPropertiesKHR> Instance::enumeratePhysicalDeviceGroups() const
{
    uint32_t physicalDeviceGroupCount = 0;
    MAGMA_REQUIRED_INSTANCE_EXTENSION(vkEnumeratePhysicalDeviceGroupsKHR, VK_KHR_DEVICE_GROUP_CREATION_EXTENSION_NAME);
    VkResult result = vkEnumeratePhysicalDeviceGroupsKHR(handle, &physicalDeviceGroupCount, nullptr);
    std::vector<VkPhysicalDeviceGroupPropertiesKHR> physicalDeviceGroups;
    if (physicalDeviceGroupCount)
    {
        physicalDeviceGroups.resize(physicalDeviceGroupCount);
        result = vkEnumeratePhysicalDeviceGroupsKHR(handle, &physicalDeviceGroupCount, physicalDeviceGroups.data());
    }
    MAGMA_HANDLE_RESULT(result, "failed to enumerate physical device groups");
    return physicalDeviceGroups;
}

std::unique_ptr<PhysicalDeviceGroup> Instance::getPhysicalDeviceGroup(uint32_t groupId) const
{
    const std::vector<VkPhysicalDeviceGroupPropertiesKHR>& deviceGroups = enumeratePhysicalDeviceGroups();
    MAGMA_ASSERT(groupId < deviceGroups.size());
    if (groupId >= core::countof(deviceGroups))
        return nullptr;
    const VkPhysicalDeviceGroupProperties& deviceGroupProperties = deviceGroups[groupId];
    std::shared_ptr<Instance> instance = std::const_pointer_cast<Instance>(shared_from_this());
    std::vector<std::shared_ptr<PhysicalDevice>> physicalDevices;
    for (uint32_t deviceId = 0; deviceId < deviceGroupProperties.physicalDeviceCount; ++deviceId)
    {
        VkPhysicalDevice handle = deviceGroupProperties.physicalDevices[deviceId];
        std::shared_ptr<PhysicalDevice> physicalDevice = PhysicalDevice::makeShared(instance, handle, hostAllocator);
        physicalDevices.emplace_back(std::move(physicalDevice));
    }
    return PhysicalDeviceGroup::makeUnique(std::move(physicalDevices), groupId);
}
#endif // VK_KHR_device_group

uint32_t Instance::enumerateVersion() noexcept
{
#if !defined(VK_VERSION_1_1)
    typedef VkResult (VKAPI_PTR *PFN_vkEnumerateInstanceVersion)(uint32_t* pApiVersion);
#endif
    PFN_vkEnumerateInstanceVersion vkEnumerateInstanceVersion =
        (PFN_vkEnumerateInstanceVersion)vkGetInstanceProcAddr(VK_NULL_HANDLE, "vkEnumerateInstanceVersion");
    if (!vkEnumerateInstanceVersion)
        return VK_API_VERSION_1_0;
    uint32_t apiVersion = 0;
    const VkResult result = vkEnumerateInstanceVersion(&apiVersion);
    if (VK_ERROR_OUT_OF_HOST_MEMORY == result)
        return VK_API_VERSION_1_0;
    return apiVersion;
}

std::vector<VkLayerProperties> Instance::enumerateLayers()
{
    uint32_t propertyCount = 0;
    VkResult result = vkEnumerateInstanceLayerProperties(&propertyCount, nullptr);
    std::vector<VkLayerProperties> properties;
    if (propertyCount)
    {
        properties.resize(propertyCount);
        result = vkEnumerateInstanceLayerProperties(&propertyCount, properties.data());
    }
    MAGMA_HANDLE_RESULT(result, "failed to enumerate instance layers");
    return properties;
}

std::vector<VkExtensionProperties> Instance::enumerateExtensions(const char *layerName /* nullptr */)
{
    uint32_t propertyCount = 0;
    VkResult result = vkEnumerateInstanceExtensionProperties(layerName, &propertyCount, nullptr);
    std::vector<VkExtensionProperties> properties;
    if (propertyCount)
    {
        properties.resize(propertyCount);
        result = vkEnumerateInstanceExtensionProperties(layerName, &propertyCount, properties.data());
    }
    MAGMA_HANDLE_RESULT(result, "failed to enumerate instance extensions");
    return properties;
}

bool Instance::extensionSupported(const char *extensionName) const noexcept
{
    MAGMA_ASSERT(extensionName);
    MAGMA_ASSERT(strlen(extensionName));
    if (!extensionName || !strlen(extensionName))
        return false;
    const auto it = extensions.find(extensionName);
    return it != extensions.end();
}

bool Instance::extensionEnabled(const char *extensionName) const noexcept
{
    if (!extensionSupported(extensionName))
        return false;
    const auto it = enabledExtensions.find(extensionName);
    return it != enabledExtensions.end();
}
} // namespace magma
