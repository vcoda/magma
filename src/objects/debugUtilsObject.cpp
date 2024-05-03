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
#include "debugUtilsObject.h"
#include "device.h"
#include "physicalDevice.h"
#include "instance.h"
#include "privateDataSlot.h"
#include "../exceptions/errorResult.h"
#include "../helpers/enumerationCast.h"

#undef MAGMA_HANDLE
#define MAGMA_HANDLE(obj) *(obj)
#include "../misc/extension.h"

namespace magma
{
VkDevice DebugUtilsObject::getNativeDevice() const noexcept
{
    return device ? device->getHandle() : VK_NULL_HANDLE;
}

void DebugUtilsObject::setDebugName(const IObject *parent, const char *name)
{
    MAGMA_UNUSED(name);
    MAGMA_ASSERT(name);
    MAGMA_ASSERT(strlen(name));
    if (!device)
        return;
#ifdef VK_EXT_debug_utils
    std::shared_ptr<const Instance> instance = device->getPhysicalDevice()->getInstance();
    if (instance->extensionEnabled(VK_EXT_DEBUG_UTILS_EXTENSION_NAME))
    {
        MAGMA_INSTANCE_EXTENSION(vkSetDebugUtilsObjectNameEXT);
        if (vkSetDebugUtilsObjectNameEXT)
        {
            VkDebugUtilsObjectNameInfoEXT objectNameInfo;
            objectNameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
            objectNameInfo.pNext = nullptr;
            objectNameInfo.objectType = parent->getObjectType();
            objectNameInfo.objectHandle = parent->getObjectHandle();
            objectNameInfo.pObjectName = name;
            const VkResult result = vkSetDebugUtilsObjectNameEXT(getNativeDevice(), &objectNameInfo);
            MAGMA_HANDLE_RESULT(result, "failed to set object name");
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
            objectNameInfo.objectType = helpers::objectToDebugReportType(parent->getObjectType());
            objectNameInfo.object = parent->getObjectHandle();
            objectNameInfo.pObjectName = name;
            const VkResult result = vkDebugMarkerSetObjectNameEXT(getNativeDevice(), &objectNameInfo);
            MAGMA_HANDLE_RESULT(result, "failed to set object name");
        }
    }
#endif // VK_EXT_debug_marker
}

void DebugUtilsObject::setDebugTag(const IObject *parent, uint64_t tagName, size_t tagSize, const void *tag)
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
    std::shared_ptr<const Instance> instance = device->getPhysicalDevice()->getInstance();
    if (instance->extensionEnabled(VK_EXT_DEBUG_UTILS_EXTENSION_NAME))
    {
        MAGMA_INSTANCE_EXTENSION(vkSetDebugUtilsObjectTagEXT);
        if (vkSetDebugUtilsObjectTagEXT)
        {
            VkDebugUtilsObjectTagInfoEXT objectTagInfo;
            objectTagInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_TAG_INFO_EXT;
            objectTagInfo.pNext = nullptr;
            objectTagInfo.objectType = parent->getObjectType();
            objectTagInfo.objectHandle = parent->getObjectHandle();
            objectTagInfo.tagName = tagName;
            objectTagInfo.tagSize = tagSize;
            objectTagInfo.pTag = tag;
            const VkResult result = vkSetDebugUtilsObjectTagEXT(getNativeDevice(), &objectTagInfo);
            MAGMA_HANDLE_RESULT(result, "failed to set object tag");
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
            objectTagInfo.objectType = helpers::objectToDebugReportType(parent->getObjectType());
            objectTagInfo.object = parent->getObjectHandle();
            objectTagInfo.tagName = tagName;
            objectTagInfo.tagSize = tagSize;
            objectTagInfo.pTag = tag;
            const VkResult result = vkDebugMarkerSetObjectTagEXT(getNativeDevice(), &objectTagInfo);
            MAGMA_HANDLE_RESULT(result, "failed to set object tag");
        }
    }
#endif // VK_EXT_debug_marker
}
} // namespace magma
