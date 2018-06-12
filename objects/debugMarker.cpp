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
#include "debugMarker.h"
#include "../objects/device.h"
#include "../shared.h"

namespace magma
{
#ifdef MAGMA_DEBUG
namespace 
{
PFN_vkDebugMarkerSetObjectTagEXT vkDebugMarkerSetObjectTag;
PFN_vkDebugMarkerSetObjectNameEXT vkDebugMarkerSetObjectName;
}
#endif // MAGMA_DEBUG

DebugMarker::DebugMarker(VkDebugReportObjectTypeEXT objectType, std::shared_ptr<const Device> device):
    objectType(objectType),
    device(std::move(device))
{}

void DebugMarker::setObjectTag(uint64_t name, size_t tagSize, const void *tag) noexcept
{
#ifdef MAGMA_DEBUG
    if (!device)
        return;
    if (!vkDebugMarkerSetObjectTag)
        vkDebugMarkerSetObjectTag = (PFN_vkDebugMarkerSetObjectTagEXT)vkGetDeviceProcAddr(MAGMA_HANDLE(device), "vkDebugMarkerSetObjectTagEXT");
    if (vkDebugMarkerSetObjectTag)
    {
        VkDebugMarkerObjectTagInfoEXT info;
        info.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_TAG_INFO_EXT;
        info.pNext = nullptr;
        info.objectType = objectType;
        info.object = this->getObject();
        info.tagName = name;
        info.tagSize = tagSize;
        info.pTag = tag;
        vkDebugMarkerSetObjectTag(MAGMA_HANDLE(device), &info);
    }
#else
    name; 
    tagSize; 
    tag;
#endif // MAGMA_DEBUG
}

void DebugMarker::setObjectName(const char *name) noexcept
{
#ifdef MAGMA_DEBUG
    if (!device)
        return;
    if (!vkDebugMarkerSetObjectName)
        vkDebugMarkerSetObjectName = (PFN_vkDebugMarkerSetObjectNameEXT)vkGetDeviceProcAddr(MAGMA_HANDLE(device), "vkDebugMarkerSetObjectNameEXT");
    if (vkDebugMarkerSetObjectName)
    {
        VkDebugMarkerObjectNameInfoEXT info;
        info.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_NAME_INFO_EXT;
        info.pNext = nullptr;
        info.objectType = objectType;
        info.object = this->getObject();
        info.pObjectName = name;
        vkDebugMarkerSetObjectName(MAGMA_HANDLE(device), &info);
    }
#else
    name;
#endif // MAGMA_DEBUG
}
} // namespace magma