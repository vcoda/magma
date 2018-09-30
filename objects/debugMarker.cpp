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
#include "debugMarker.h"
#include "device.h"
#include "../misc/deviceExtension.h"

namespace magma
{
DebugMarker::DebugMarker(VkDebugReportObjectTypeEXT objectType, std::shared_ptr<Device> device, std::shared_ptr<IAllocator> allocator):
    Object(objectType, std::move(device), std::move(allocator))
{}

void DebugMarker::setObjectTag(uint64_t name, size_t tagSize, const void *tag) noexcept
{
#ifdef MAGMA_DEBUG
    if (!device)
        return;
    MAGMA_OPTIONAL_DEVICE_EXTENSION(vkDebugMarkerSetObjectTagEXT);
    if (vkDebugMarkerSetObjectTagEXT)
    {
        VkDebugMarkerObjectTagInfoEXT info;
        info.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_TAG_INFO_EXT;
        info.pNext = nullptr;
        info.objectType = objectType;
        info.object = getHandle();
        info.tagName = name;
        info.tagSize = tagSize;
        info.pTag = tag;
        vkDebugMarkerSetObjectTagEXT(MAGMA_HANDLE(device), &info);
    }
#elif defined(_MSC_VER)
    name;
    tagSize;
    tag;
#endif // MAGMA_DEBUG
}

void DebugMarker::setObjectName(const char *name) noexcept
{
    MAGMA_ASSERT(name);
    MAGMA_ASSERT(strlen(name) > 0);
#ifdef MAGMA_DEBUG
    if (!device)
        return;
    MAGMA_OPTIONAL_DEVICE_EXTENSION(vkDebugMarkerSetObjectNameEXT);
    if (vkDebugMarkerSetObjectNameEXT)
    {
        VkDebugMarkerObjectNameInfoEXT info;
        info.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_NAME_INFO_EXT;
        info.pNext = nullptr;
        info.objectType = objectType;
        info.object = getHandle();
        info.pObjectName = name;
        vkDebugMarkerSetObjectNameEXT(MAGMA_HANDLE(device), &info);
    }
#elif defined(_MSC_VER)
    name;
#endif // MAGMA_DEBUG
}
} // namespace magma