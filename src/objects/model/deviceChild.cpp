/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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
#include "deviceChild.h"
#include "../instance.h"
#include "../device.h"
#include "../physicalDevice.h"
#include "../privateDataSlot.h"
#include "../../exceptions/errorResult.h"
#include "../../helpers/enumerationCast.h"
#include "../../misc/extension.h"

namespace magma
{
core::Spinlock DeviceChild::mtx;

void DeviceChild::setPrivateData(uint64_t data)
{
    if (!device)
        return;
#ifdef VK_EXT_private_data
    const std::unique_ptr<PrivateDataSlot>& dataSlot = device->getPrivateDataSlot();
    if (dataSlot)
        return dataSlot->setPrivateData(this, data);
#endif // VK_EXT_private_data
    std::unordered_map<uint64_t, uint64_t>& dataMap = device->getPrivateDataMap();
    const uint64_t handle = getObjectHandle();
    std::lock_guard<core::Spinlock> lock(mtx);
    dataMap[handle] = data;
}

uint64_t DeviceChild::getPrivateData() const noexcept
{
    if (!device)
        return 0ull;
#ifdef VK_EXT_private_data
    const std::unique_ptr<PrivateDataSlot>& dataSlot = device->getPrivateDataSlot();
    if (dataSlot)
        return dataSlot->getPrivateData(this);
#endif // VK_EXT_private_data
    std::unordered_map<uint64_t, uint64_t>& dataMap = device->getPrivateDataMap();
    const uint64_t handle = getObjectHandle();
    std::lock_guard<core::Spinlock> lock(mtx);
    auto it = dataMap.find(handle);
    return it != dataMap.end() ? it->second : 0ull;
}

#ifdef MAGMA_DEBUG
void DeviceChild::setDebugName(const char *name)
{
    MAGMA_UNUSED(name);
    MAGMA_ASSERT(name);
    MAGMA_ASSERT(strlen(name));
    if (!device)
        return;
#ifdef VK_EXT_debug_utils
    if (device->getPhysicalDevice()->getInstance()->extensionEnabled(VK_EXT_DEBUG_UTILS_EXTENSION_NAME))
    {
        MAGMA_INSTANCE_EXTENSION(vkSetDebugUtilsObjectNameEXT);
        if (vkSetDebugUtilsObjectNameEXT)
        {
            VkDebugUtilsObjectNameInfoEXT objectNameInfo;
            objectNameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
            objectNameInfo.pNext = nullptr;
            objectNameInfo.objectType = getObjectType();
            objectNameInfo.objectHandle = getObjectHandle();
            objectNameInfo.pObjectName = name;
            const VkResult result = vkSetDebugUtilsObjectNameEXT(getNativeDevice(), &objectNameInfo);
            MAGMA_HANDLE_RESULT(result, "failed to set child name");
            return;
        }
    }
#endif // VK_EXT_debug_utils
#ifdef VK_EXT_debug_marker
    if (device->extensionEnabled(VK_EXT_DEBUG_MARKER_EXTENSION_NAME))
    {
        MAGMA_DEVICE_EXTENSION(vkDebugMarkerSetObjectNameEXT);
        if (vkDebugMarkerSetObjectNameEXT)
        {
            VkDebugMarkerObjectNameInfoEXT objectNameInfo;
            objectNameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_NAME_INFO_EXT;
            objectNameInfo.pNext = nullptr;
            // No future object type handle enumeration values will be added to
            // VkDebugReportObjectTypeEXT since the creation of VkObjectType!
            objectNameInfo.objectType = helpers::objectToDebugReportType(getObjectType());
            objectNameInfo.object = getObjectHandle();
            objectNameInfo.pObjectName = name;
            const VkResult result = vkDebugMarkerSetObjectNameEXT(getNativeDevice(), &objectNameInfo);
            MAGMA_HANDLE_RESULT(result, "failed to set child name");
        }
    }
#endif // VK_EXT_debug_marker
}

void DeviceChild::setDebugTag(uint64_t tagName, size_t tagSize, const void *tag)
{
    MAGMA_UNUSED(tagName);
    MAGMA_UNUSED(tagSize);
    MAGMA_UNUSED(tag);
    MAGMA_ASSERT(tagName);
    MAGMA_ASSERT(tagSize);
    MAGMA_ASSERT(tag);
    if (!device)
        return;
#ifdef VK_EXT_debug_utils
    if (device->getPhysicalDevice()->getInstance()->extensionEnabled(VK_EXT_DEBUG_UTILS_EXTENSION_NAME))
    {
        MAGMA_INSTANCE_EXTENSION(vkSetDebugUtilsObjectTagEXT);
        if (vkSetDebugUtilsObjectTagEXT)
        {
            VkDebugUtilsObjectTagInfoEXT objectTagInfo;
            objectTagInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_TAG_INFO_EXT;
            objectTagInfo.pNext = nullptr;
            objectTagInfo.objectType = getObjectType();
            objectTagInfo.objectHandle = getObjectHandle();
            objectTagInfo.tagName = tagName;
            objectTagInfo.tagSize = tagSize;
            objectTagInfo.pTag = tag;
            const VkResult result = vkSetDebugUtilsObjectTagEXT(getNativeDevice(), &objectTagInfo);
            MAGMA_HANDLE_RESULT(result, "failed to set child tag");
            return;
        }
    }
#endif // VK_EXT_debug_utils
#ifdef VK_EXT_debug_marker
    if (device->extensionEnabled(VK_EXT_DEBUG_MARKER_EXTENSION_NAME))
    {
        MAGMA_DEVICE_EXTENSION(vkDebugMarkerSetObjectTagEXT);
        if (vkDebugMarkerSetObjectTagEXT)
        {
            VkDebugMarkerObjectTagInfoEXT objectTagInfo;
            objectTagInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_TAG_INFO_EXT;
            objectTagInfo.pNext = nullptr;
            // No future object type handle enumeration values will be added to
            // VkDebugReportObjectTypeEXT since the creation of VkObjectType!
            objectTagInfo.objectType = helpers::objectToDebugReportType(getObjectType());
            objectTagInfo.object = getObjectHandle();
            objectTagInfo.tagName = tagName;
            objectTagInfo.tagSize = tagSize;
            objectTagInfo.pTag = tag;
            const VkResult result = vkDebugMarkerSetObjectTagEXT(getNativeDevice(), &objectTagInfo);
            MAGMA_HANDLE_RESULT(result, "failed to set child tag");
        }
    }
#endif // VK_EXT_debug_marker
}
#endif // MAGMA_DEBUG

VkDevice DeviceChild::getNativeDevice() const noexcept
{   // The following Vulkan objects are created before
    // logical device has been initialized or without it:
    // VK_OBJECT_TYPE_SURFACE_KHR
    // VK_OBJECT_TYPE_DEVICE
    // VK_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT
    // VK_OBJECT_TYPE_DEBUG_UTILS_MESSENGER_EXT
    // VK_OBJECT_TYPE_DISPLAY_KHR
    // VK_OBJECT_TYPE_DISPLAY_MODE_KHR
    return MAGMA_OPTIONAL_HANDLE(device);
}

VkPhysicalDevice DeviceChild::getNativePhysicalDevice() const noexcept
{
    return device ? device->getPhysicalDevice()->getHandle() : VK_NULL_HANDLE;
}

VkInstance DeviceChild::getNativeInstance() const noexcept
{
    return device ? device->getPhysicalDevice()->getInstance()->getHandle() : VK_NULL_HANDLE;
}

#if (VK_USE_64_BIT_PTR_DEFINES == 1)
const std::unique_ptr<DeviceResourcePool>& DeviceChild::getResourcePool() noexcept
{
    return device->getResourcePool();
}
#endif // (VK_USE_64_BIT_PTR_DEFINES == 1)

bool DeviceChild::extensionEnabled(const char *name) const noexcept
{
    return device->extensionEnabled(name);
}
} // namespace magma

