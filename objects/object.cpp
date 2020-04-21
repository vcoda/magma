/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2019 Victor Coda.

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
#include "../allocator/objectAllocator.h"
#include "../misc/deviceExtension.h"
#include "../misc/exception.h"
#include "../helpers/castToDebugReport.h"

#if !defined(MAGMA_DEBUG)
#ifdef VK_EXT_debug_utils
#undef VK_EXT_debug_utils
#endif
#ifdef VK_EXT_debug_marker
#undef VK_EXT_debug_marker
#endif
#endif // !MAGMA_DEBUG

namespace magma
{
Object::Object(VkObjectType objectType, std::shared_ptr<Device> device, std::shared_ptr<IAllocator> allocator) noexcept:
    objectType(objectType),
    device(std::move(device)),
    allocator(std::move(allocator))
{}

void Object::setObjectName(const char *name) noexcept
{
    MAGMA_ASSERT(name);
    MAGMA_ASSERT(strlen(name) > 0);
#ifdef VK_EXT_debug_utils
    if (device)
    {
        MAGMA_OPTIONAL_DEVICE_EXTENSION(vkSetDebugUtilsObjectNameEXT);
        if (vkSetDebugUtilsObjectNameEXT)
        {
            VkDebugUtilsObjectNameInfoEXT info;
            info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
            info.pNext = nullptr;
            info.objectType = objectType;
            info.objectHandle = getHandle();
            info.pObjectName = name;
            vkSetDebugUtilsObjectNameEXT(MAGMA_HANDLE(device), &info);
        }
    }
#elif defined(VK_EXT_debug_marker)
    if (device)
    {
        MAGMA_OPTIONAL_DEVICE_EXTENSION(vkDebugMarkerSetObjectNameEXT);
        if (vkDebugMarkerSetObjectNameEXT)
        {
            VkDebugMarkerObjectNameInfoEXT info;
            info.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_NAME_INFO_EXT;
            info.pNext = nullptr;
            info.objectType = helpers::castToDebugReportType(objectType);
            info.object = getHandle();
            info.pObjectName = name;
            vkDebugMarkerSetObjectNameEXT(MAGMA_HANDLE(device), &info);
        }
    }
#else
    MAGMA_UNUSED(name);
#endif // VK_EXT_debug_marker
}

void Object::setObjectTag(uint64_t tagName, std::size_t tagSize, const void *tag) noexcept
{
    MAGMA_ASSERT(tagName);
    MAGMA_ASSERT(tagSize);
    MAGMA_ASSERT(tag);
#ifdef VK_EXT_debug_utils
    if (device)
    {
        MAGMA_OPTIONAL_DEVICE_EXTENSION(vkSetDebugUtilsObjectTagEXT);
        if(vkSetDebugUtilsObjectTagEXT)
        {
            VkDebugUtilsObjectTagInfoEXT info;
            info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_TAG_INFO_EXT;
            info.pNext = nullptr;
            info.objectType = objectType;
            info.objectHandle = getHandle();
            info.tagName = tagName;
            info.tagSize = tagSize;
            info.pTag = tag;
            vkSetDebugUtilsObjectTagEXT(MAGMA_HANDLE(device), &info);
        }
    }
#elif defined(VK_EXT_debug_marker)
    if (device)
    {
        MAGMA_OPTIONAL_DEVICE_EXTENSION(vkDebugMarkerSetObjectTagEXT);
        if (vkDebugMarkerSetObjectTagEXT)
        {
            VkDebugMarkerObjectTagInfoEXT info;
            info.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_TAG_INFO_EXT;
            info.pNext = nullptr;
            info.objectType = helpers::castToDebugReportType(objectType);
            info.object = getHandle();
            info.tagName = tagName;
            info.tagSize = tagSize;
            info.pTag = tag;
            vkDebugMarkerSetObjectTagEXT(MAGMA_HANDLE(device), &info);
        }
    }
#else
    MAGMA_UNUSED(tagName);
    MAGMA_UNUSED(tagSize);
    MAGMA_UNUSED(tag);
#endif // VK_EXT_debug_marker
}

std::shared_ptr<IObjectAllocator> Object::objectAllocator;
#ifdef MAGMA_DEBUG
std::atomic<int64_t> Object::allocCount;
#endif

void Object::overrideDefaultAllocator(std::shared_ptr<IObjectAllocator> allocator)
{
#ifdef MAGMA_DEBUG
    if (allocCount)
        MAGMA_THROW("object allocator should be defined prior any allocations");
#endif
    objectAllocator = std::move(allocator);
}

std::shared_ptr<IObjectAllocator> Object::getOverridenAllocator() noexcept
{
    return objectAllocator;
}
} // namespace magma
