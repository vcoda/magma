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
#include <cstring>
#include "object.h"
#include "device.h"
#include "../allocator/objectAllocator.h"
#include "../misc/deviceExtension.h"
#include "../misc/exception.h"

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
#ifdef MAGMA_DEBUG
    if (!device)
        return;
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
    else
    {   // Old driver? Use EXT_debug_marker extension.
        MAGMA_OPTIONAL_DEVICE_EXTENSION(vkDebugMarkerSetObjectNameEXT);
        if (vkDebugMarkerSetObjectNameEXT)
        {
            VkDebugMarkerObjectNameInfoEXT info;
            info.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_NAME_INFO_EXT;
            info.pNext = nullptr;
            info.objectType = castToDebugReportType(objectType);
            info.object = getHandle();
            info.pObjectName = name;
            vkDebugMarkerSetObjectNameEXT(MAGMA_HANDLE(device), &info);
        }
    }
#elif defined(_MSC_VER)
    name;
#endif // MAGMA_DEBUG
}

void Object::setObjectTag(uint64_t tagName, size_t tagSize, const void *tag) noexcept
{
#ifdef MAGMA_DEBUG
    if (!device)
        return;
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
    else
    {   // Old driver? Use EXT_debug_marker extension.
        MAGMA_OPTIONAL_DEVICE_EXTENSION(vkDebugMarkerSetObjectTagEXT);
        if (vkDebugMarkerSetObjectTagEXT)
        {
            VkDebugMarkerObjectTagInfoEXT info;
            info.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_TAG_INFO_EXT;
            info.pNext = nullptr;
            info.objectType = castToDebugReportType(objectType);
            info.object = getHandle();
            info.tagName = tagName;
            info.tagSize = tagSize;
            info.pTag = tag;
            vkDebugMarkerSetObjectTagEXT(MAGMA_HANDLE(device), &info);
        }
    }
#elif defined(_MSC_VER)
    tagName;
    tagSize;
    tag;
#endif // MAGMA_DEBUG
}

VkDebugReportObjectTypeEXT Object::castToDebugReportType(VkObjectType objectType) const noexcept
{
    switch (objectType)
    {
    case VK_OBJECT_TYPE_UNKNOWN:
        return VK_DEBUG_REPORT_OBJECT_TYPE_UNKNOWN_EXT;
    case VK_OBJECT_TYPE_INSTANCE:
        return VK_DEBUG_REPORT_OBJECT_TYPE_INSTANCE_EXT;
    case VK_OBJECT_TYPE_PHYSICAL_DEVICE:
        return VK_DEBUG_REPORT_OBJECT_TYPE_PHYSICAL_DEVICE_EXT;
    case VK_OBJECT_TYPE_DEVICE:
        return VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_EXT;
    case VK_OBJECT_TYPE_QUEUE:
        return VK_DEBUG_REPORT_OBJECT_TYPE_QUEUE_EXT;
    case VK_OBJECT_TYPE_SEMAPHORE:
        return VK_DEBUG_REPORT_OBJECT_TYPE_SEMAPHORE_EXT;
    case VK_OBJECT_TYPE_COMMAND_BUFFER:
        return VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_BUFFER_EXT;
    case VK_OBJECT_TYPE_FENCE:
        return VK_DEBUG_REPORT_OBJECT_TYPE_FENCE_EXT;
    case VK_OBJECT_TYPE_DEVICE_MEMORY:
        return VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT;
    case VK_OBJECT_TYPE_BUFFER:
        return VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT;
    case VK_OBJECT_TYPE_IMAGE:
        return VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT;
    case VK_OBJECT_TYPE_EVENT:
        return VK_DEBUG_REPORT_OBJECT_TYPE_EVENT_EXT;
    case VK_OBJECT_TYPE_QUERY_POOL:
        return VK_DEBUG_REPORT_OBJECT_TYPE_QUERY_POOL_EXT;
    case VK_OBJECT_TYPE_BUFFER_VIEW:
        return VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_VIEW_EXT;
    case VK_OBJECT_TYPE_IMAGE_VIEW:
        return VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_VIEW_EXT;
    case VK_OBJECT_TYPE_SHADER_MODULE:
        return VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT;
    case VK_OBJECT_TYPE_PIPELINE_CACHE:
        return VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_CACHE_EXT;
    case VK_OBJECT_TYPE_PIPELINE_LAYOUT:
        return VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_LAYOUT_EXT;
    case VK_OBJECT_TYPE_RENDER_PASS:
        return VK_DEBUG_REPORT_OBJECT_TYPE_RENDER_PASS_EXT;
    case VK_OBJECT_TYPE_PIPELINE:
        return VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_EXT;
    case VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT:
        return VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT_EXT;
    case VK_OBJECT_TYPE_SAMPLER:
        return VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_EXT;
    case VK_OBJECT_TYPE_DESCRIPTOR_POOL:
        return VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_POOL_EXT;
    case VK_OBJECT_TYPE_DESCRIPTOR_SET:
        return VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_EXT;
    case VK_OBJECT_TYPE_FRAMEBUFFER:
        return VK_DEBUG_REPORT_OBJECT_TYPE_FRAMEBUFFER_EXT;
    case VK_OBJECT_TYPE_COMMAND_POOL:
        return VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_POOL_EXT;
    case VK_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION:
        return VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION_KHR_EXT;
    case VK_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE:
        return VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE_EXT;
    // Non-core types
    case VK_OBJECT_TYPE_SURFACE_KHR:
        return VK_DEBUG_REPORT_OBJECT_TYPE_SURFACE_KHR_EXT;
    case VK_OBJECT_TYPE_SWAPCHAIN_KHR:
        return VK_DEBUG_REPORT_OBJECT_TYPE_SWAPCHAIN_KHR_EXT;
    case VK_OBJECT_TYPE_DISPLAY_KHR:
        return VK_DEBUG_REPORT_OBJECT_TYPE_DISPLAY_KHR_EXT;
    case VK_OBJECT_TYPE_DISPLAY_MODE_KHR:
        return VK_DEBUG_REPORT_OBJECT_TYPE_DISPLAY_MODE_KHR_EXT;
    case VK_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT:
        return VK_DEBUG_REPORT_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT_EXT;
    case VK_OBJECT_TYPE_OBJECT_TABLE_NVX:
        return VK_DEBUG_REPORT_OBJECT_TYPE_OBJECT_TABLE_NVX_EXT;
    case VK_OBJECT_TYPE_INDIRECT_COMMANDS_LAYOUT_NVX:
        return VK_DEBUG_REPORT_OBJECT_TYPE_INDIRECT_COMMANDS_LAYOUT_NVX_EXT;
    case VK_OBJECT_TYPE_VALIDATION_CACHE_EXT:
        return VK_DEBUG_REPORT_OBJECT_TYPE_VALIDATION_CACHE_EXT_EXT;
    case VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_NV:
        return VK_DEBUG_REPORT_OBJECT_TYPE_ACCELERATION_STRUCTURE_NV_EXT;
    default:
        return VK_DEBUG_REPORT_OBJECT_TYPE_UNKNOWN_EXT;
    }
}
} // namespace magma
