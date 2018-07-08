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
#include "debugReportCallback.h"
#include "instance.h"
#include "../allocator/allocator.h"
#include "../misc/instanceExtension.h"

namespace magma
{
DebugReportCallback::DebugReportCallback(std::shared_ptr<const Instance> instance,
    PFN_vkDebugReportCallbackEXT reportCallback,
    VkDebugReportFlagsEXT flags,
    void *userData /* nullptr */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    NonDispatchable(VK_DEBUG_REPORT_OBJECT_TYPE_DEBUG_REPORT_EXT, nullptr, std::move(allocator)),
    instance(std::move(instance))
{
    MAGMA_OPTIONAL_INSTANCE_EXTENSION(vkCreateDebugReportCallbackEXT);
    if (vkCreateDebugReportCallbackEXT)
    {
        VkDebugReportCallbackCreateInfoEXT info;
        info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
        info.pNext = nullptr;
        info.flags = flags;
        info.pfnCallback = reportCallback;
        info.pUserData = userData;
        const VkResult create = vkCreateDebugReportCallbackEXT(MAGMA_HANDLE(instance), &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
        MAGMA_THROW_FAILURE(create, "failed to create debug callback");
    }
}

DebugReportCallback::~DebugReportCallback()
{
    MAGMA_OPTIONAL_INSTANCE_EXTENSION(vkDestroyDebugReportCallbackEXT);
    if (vkDestroyDebugReportCallbackEXT)
        vkDestroyDebugReportCallbackEXT(MAGMA_HANDLE(instance), handle, MAGMA_OPTIONAL_INSTANCE(allocator));
}
} // namespace magma
