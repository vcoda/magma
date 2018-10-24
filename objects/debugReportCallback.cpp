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
#include <cstdarg>
#include "debugReportCallback.h"
#include "instance.h"
#include "../allocator/allocator.h"
#include "../misc/instanceExtension.h"

namespace magma
{
DebugReportCallback::DebugReportCallback(std::shared_ptr<const Instance> instance,
    PFN_vkDebugReportCallbackEXT userCallback,
    VkDebugReportFlagsEXT flags,
    void *userData /* nullptr */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    NonDispatchable(VK_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT, nullptr, std::move(allocator)),
    instance(std::move(instance))
{
    MAGMA_ASSERT(userCallback);
    MAGMA_OPTIONAL_INSTANCE_EXTENSION(vkCreateDebugReportCallbackEXT);
    if (vkCreateDebugReportCallbackEXT)
    {
        VkDebugReportCallbackCreateInfoEXT info;
        info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
        info.pNext = nullptr;
        info.flags = flags;
        info.pfnCallback = userCallback;
        info.pUserData = userData;
        const VkResult create = vkCreateDebugReportCallbackEXT(MAGMA_HANDLE(instance), &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
        MAGMA_THROW_FAILURE(create, "failed to create debug report callback");
    }
}

DebugReportCallback::~DebugReportCallback()
{
    MAGMA_OPTIONAL_INSTANCE_EXTENSION(vkDestroyDebugReportCallbackEXT);
    if (vkDestroyDebugReportCallbackEXT)
        vkDestroyDebugReportCallbackEXT(MAGMA_HANDLE(instance), handle, MAGMA_OPTIONAL_INSTANCE(allocator));
}

void DebugReportCallback::message(VkDebugReportFlagsEXT flags, VkObjectType objectType,
    uint64_t object, size_t location, int32_t messageCode, const char *layerPrefix, const char *format, ...) const noexcept
{
    MAGMA_ASSERT(layerPrefix);
    MAGMA_ASSERT(format);
    MAGMA_OPTIONAL_INSTANCE_EXTENSION(vkDebugReportMessageEXT);
    if (vkDebugReportMessageEXT)
    {
        char message[4096];
        va_list args;
        va_start(args, format);
#ifdef _MSC_VER
        vsprintf_s(message, format, args);
#else
        vsprintf(message, format, args);
#endif
        va_end(args);
        const VkDebugReportObjectTypeEXT debugObjectType = coreTypeToExt(objectType);
        vkDebugReportMessageEXT(MAGMA_HANDLE(instance), flags, debugObjectType, object, location, messageCode, layerPrefix, message);
    }
}

VkDebugReportObjectTypeEXT DebugReportCallback::coreTypeToExt(VkObjectType objectType) const noexcept
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
    default:
        return VK_DEBUG_REPORT_OBJECT_TYPE_UNKNOWN_EXT;
    }
}
} // namespace magma
