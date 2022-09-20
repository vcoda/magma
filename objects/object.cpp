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
#include "object.h"
#include "device.h"
#include "../misc/extProcAddress.h"
#include "../helpers/castToDebugReport.h"
#include "../exceptions/errorResult.h"

namespace magma
{
Object::Object(std::shared_ptr<Device> device, std::shared_ptr<IAllocator> hostAllocator) noexcept:
    device(std::move(device)),
    hostAllocator(std::move(hostAllocator))
{}

void Object::setDebugName(const char *name)
{
    MAGMA_UNUSED(name);
    MAGMA_ASSERT(name);
    MAGMA_ASSERT(strlen(name) > 0);
#ifdef MAGMA_DEBUG
    VkResult result = VK_SUCCESS;
    if (device)
    {
#ifdef VK_EXT_debug_marker
        MAGMA_DEVICE_EXTENSION(vkDebugMarkerSetObjectNameEXT);
        if (vkDebugMarkerSetObjectNameEXT)
        {
            VkDebugMarkerObjectNameInfoEXT objectNameInfo;
            objectNameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_NAME_INFO_EXT;
            objectNameInfo.pNext = nullptr;
            objectNameInfo.objectType = helpers::objectToDebugReportType(getObjectType());
            objectNameInfo.object = getHandle();
            objectNameInfo.pObjectName = name;
            result = vkDebugMarkerSetObjectNameEXT(MAGMA_HANDLE(device), &objectNameInfo);
        }
        else
        {
#endif // VK_EXT_debug_marker
#ifdef VK_EXT_debug_utils
            MAGMA_DEVICE_EXTENSION(vkSetDebugUtilsObjectNameEXT);
            if (vkSetDebugUtilsObjectNameEXT)
            {
                VkDebugUtilsObjectNameInfoEXT objectNameInfo;
                objectNameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
                objectNameInfo.pNext = nullptr;
                objectNameInfo.objectType = getObjectType();
                objectNameInfo.objectHandle = getHandle();
                objectNameInfo.pObjectName = name;
                result = vkSetDebugUtilsObjectNameEXT(MAGMA_HANDLE(device), &objectNameInfo);
            }
#endif // VK_EXT_debug_utils
#ifdef VK_EXT_debug_marker
        }
#endif
    }
    MAGMA_THROW_FAILURE(result, "failed to set debug object name");
#endif // MAGMA_DEBUG
}

void Object::setDebugTag(uint64_t tagName, std::size_t tagSize, const void *tag)
{
    MAGMA_UNUSED(tagName);
    MAGMA_UNUSED(tagSize);
    MAGMA_UNUSED(tag);
    MAGMA_ASSERT(tagName);
    MAGMA_ASSERT(tagSize > 0);
    MAGMA_ASSERT(tag);
#ifdef MAGMA_DEBUG
    VkResult result = VK_SUCCESS;
    if (device)
    {
#ifdef VK_EXT_debug_marker
        MAGMA_DEVICE_EXTENSION(vkDebugMarkerSetObjectTagEXT);
        if (vkDebugMarkerSetObjectTagEXT)
        {
            VkDebugMarkerObjectTagInfoEXT objectTagInfo;
            objectTagInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_TAG_INFO_EXT;
            objectTagInfo.pNext = nullptr;
            objectTagInfo.objectType = helpers::objectToDebugReportType(getObjectType());
            objectTagInfo.object = getHandle();
            objectTagInfo.tagName = tagName;
            objectTagInfo.tagSize = tagSize;
            objectTagInfo.pTag = tag;
            result = vkDebugMarkerSetObjectTagEXT(MAGMA_HANDLE(device), &objectTagInfo);
        }
        else
        {
#endif // VK_EXT_debug_marker
#ifdef VK_EXT_debug_utils
            MAGMA_DEVICE_EXTENSION(vkSetDebugUtilsObjectTagEXT);
            if (vkSetDebugUtilsObjectTagEXT)
            {
                VkDebugUtilsObjectTagInfoEXT objectTagInfo;
                objectTagInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_TAG_INFO_EXT;
                objectTagInfo.pNext = nullptr;
                objectTagInfo.objectType = getObjectType();
                objectTagInfo.objectHandle = getHandle();
                objectTagInfo.tagName = tagName;
                objectTagInfo.tagSize = tagSize;
                objectTagInfo.pTag = tag;
                result = vkSetDebugUtilsObjectTagEXT(MAGMA_HANDLE(device), &objectTagInfo);
            }
#endif // VK_EXT_debug_utils
#ifdef VK_EXT_debug_marker
        }
#endif
    }
    MAGMA_THROW_FAILURE(result, "failed to set object tag");
#endif // MAGMA_DEBUG
}
} // namespace magma
