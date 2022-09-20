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
#include <cstdarg>
#include "debugReportCallback.h"
#include "instance.h"
#include "../allocator/allocator.h"
#include "../misc/extProcAddress.h"
#include "../helpers/castToDebugReport.h"
#include "../exceptions/errorResult.h"

namespace magma
{
#ifdef VK_EXT_debug_report
DebugReportCallback::DebugReportCallback(std::shared_ptr<const Instance> instance,
    PFN_vkDebugReportCallbackEXT userCallback,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkDebugReportFlagsEXT flags /* INFORMATION_BIT | WARNING_BIT_EXT | PERFORMANCE_WARNING_BIT_EXT | ERROR_BIT DEBUG_BIT */,
    void *userData /* nullptr */):
    NonDispatchable(VK_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT, nullptr, std::move(allocator)),
    instance(std::move(instance))
{
    MAGMA_ASSERT(userCallback);
    MAGMA_INSTANCE_EXTENSION(vkCreateDebugReportCallbackEXT);
    if (vkCreateDebugReportCallbackEXT)
    {
        VkDebugReportCallbackCreateInfoEXT debugReportCallbackInfo;
    #ifdef VK_HEADER_VERSION
        debugReportCallbackInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    #else   // Compatibility with old SDK
        debugReportCallbackInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
    #endif
        debugReportCallbackInfo.pNext = nullptr;
        debugReportCallbackInfo.flags = flags;
        debugReportCallbackInfo.pfnCallback = userCallback;
        debugReportCallbackInfo.pUserData = userData;
        const VkResult result = vkCreateDebugReportCallbackEXT(MAGMA_HANDLE(instance), &debugReportCallbackInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
        MAGMA_THROW_FAILURE(result, "failed to create debug report callback");
    }
}

DebugReportCallback::~DebugReportCallback()
{
    MAGMA_INSTANCE_EXTENSION(vkDestroyDebugReportCallbackEXT);
    if (vkDestroyDebugReportCallbackEXT)
        vkDestroyDebugReportCallbackEXT(MAGMA_HANDLE(instance), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}

void DebugReportCallback::message(VkDebugReportFlagsEXT flags, VkObjectType objectType,
    uint64_t object, std::size_t location, int32_t messageCode, const char *layerPrefix, const char *format, ...) const noexcept
{
    MAGMA_ASSERT(layerPrefix);
    MAGMA_ASSERT(format);
    MAGMA_INSTANCE_EXTENSION(vkDebugReportMessageEXT);
    if (vkDebugReportMessageEXT)
    {
        char message[MAGMA_MAX_STRING];
        va_list args;
        va_start(args, format);
#ifdef _MSC_VER
        vsprintf_s(message, format, args);
#else
        vsprintf(message, format, args);
#endif
        va_end(args);
        const VkDebugReportObjectTypeEXT debugObjectType = helpers::objectToDebugReportType(objectType);
        vkDebugReportMessageEXT(MAGMA_HANDLE(instance), flags, debugObjectType, object, location, messageCode, layerPrefix, message);
    }
}
#endif // VK_EXT_debug_report
} // namespace magma
