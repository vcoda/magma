/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
#include "../misc/deviceExtension.h"
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
    if (device)
    {
#ifdef VK_EXT_debug_marker
        MAGMA_OPTIONAL_DEVICE_EXTENSION(vkDebugMarkerSetObjectNameEXT);
        if (vkDebugMarkerSetObjectNameEXT)
        {
            VkDebugMarkerObjectNameInfoEXT info;
            info.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_NAME_INFO_EXT;
            info.pNext = nullptr;
            info.objectType = helpers::castToDebugReportType(getObjectType());
            info.object = getHandle();
            info.pObjectName = name;
            const VkResult result = vkDebugMarkerSetObjectNameEXT(MAGMA_HANDLE(device), &info);
            MAGMA_THROW_FAILURE(result, "failed to give a user-friendly name to an object");
        }
        else
        {
#endif // VK_EXT_debug_marker
#ifdef VK_EXT_debug_utils
            MAGMA_OPTIONAL_DEVICE_EXTENSION(vkSetDebugUtilsObjectNameEXT);
            if (vkSetDebugUtilsObjectNameEXT)
            {
                VkDebugUtilsObjectNameInfoEXT info;
                info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
                info.pNext = nullptr;
                info.objectType = getObjectType();
                info.objectHandle = getHandle();
                info.pObjectName = name;
                const VkResult result = vkSetDebugUtilsObjectNameEXT(MAGMA_HANDLE(device), &info);
                MAGMA_THROW_FAILURE(result, "failed to give a user-friendly name to an object");
            }
#endif // VK_EXT_debug_utils
#ifdef VK_EXT_debug_marker
        }
#endif
    }
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
    if (device)
    {
#ifdef VK_EXT_debug_marker
        MAGMA_OPTIONAL_DEVICE_EXTENSION(vkDebugMarkerSetObjectTagEXT);
        if (vkDebugMarkerSetObjectTagEXT)
        {
            VkDebugMarkerObjectTagInfoEXT info;
            info.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_TAG_INFO_EXT;
            info.pNext = nullptr;
            info.objectType = helpers::castToDebugReportType(getObjectType());
            info.object = getHandle();
            info.tagName = tagName;
            info.tagSize = tagSize;
            info.pTag = tag;
            const VkResult result = vkDebugMarkerSetObjectTagEXT(MAGMA_HANDLE(device), &info);
            MAGMA_THROW_FAILURE(result, "failed to attach arbitrary data to an object");
        }
        else
        {
#endif // VK_EXT_debug_marker
#ifdef VK_EXT_debug_utils
            MAGMA_OPTIONAL_DEVICE_EXTENSION(vkSetDebugUtilsObjectTagEXT);
            if (vkSetDebugUtilsObjectTagEXT)
            {
                VkDebugUtilsObjectTagInfoEXT info;
                info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_TAG_INFO_EXT;
                info.pNext = nullptr;
                info.objectType = getObjectType();
                info.objectHandle = getHandle();
                info.tagName = tagName;
                info.tagSize = tagSize;
                info.pTag = tag;
                const VkResult result = vkSetDebugUtilsObjectTagEXT(MAGMA_HANDLE(device), &info);
                MAGMA_THROW_FAILURE(result, "failed to attach arbitrary data to an object");
            }
#endif // VK_EXT_debug_utils
#ifdef VK_EXT_debug_marker
        }
#endif
    }
#endif // MAGMA_DEBUG
}
} // namespace magma
