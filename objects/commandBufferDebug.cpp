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
#include "commandBuffer.h"
#include "device.h"
#include "../misc/deviceExtension.h"

namespace magma
{
void CommandBuffer::beginDebugMarker(const char *name, const float color[4]) noexcept
{
    MAGMA_ASSERT(name);
    MAGMA_ASSERT(strlen(name) > 0);
    MAGMA_ASSERT(color);
#ifdef MAGMA_DEBUG
    MAGMA_OPTIONAL_DEVICE_EXTENSION(vkCmdDebugMarkerBeginEXT);
    if (vkCmdDebugMarkerBeginEXT)
    {
        VkDebugMarkerMarkerInfoEXT info;
        info.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT;
        info.pNext = nullptr;
        info.pMarkerName = name;
        for (int i = 0; i < 4; ++i)
            info.color[i] = color[i];
        vkCmdDebugMarkerBeginEXT(handle, &info);
    }
#elif defined(_MSC_VER)
    name;
    color;
#endif // MAGMA_DEBUG
}

void CommandBuffer::endDebugMarker() noexcept
{
#ifdef MAGMA_DEBUG
    MAGMA_OPTIONAL_DEVICE_EXTENSION(vkCmdDebugMarkerEndEXT);
    if (vkCmdDebugMarkerEndEXT)
        vkCmdDebugMarkerEndEXT(handle);
#endif // MAGMA_DEBUG
}

void CommandBuffer::insertDebugMarker(const char *name, const float color[4]) noexcept
{
    MAGMA_ASSERT(name);
    MAGMA_ASSERT(strlen(name) > 0);
    MAGMA_ASSERT(color);
#ifdef MAGMA_DEBUG
    MAGMA_OPTIONAL_DEVICE_EXTENSION(vkCmdDebugMarkerInsertEXT);
    if (vkCmdDebugMarkerInsertEXT)
    {
        VkDebugMarkerMarkerInfoEXT info;
        info.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT;
        info.pNext = nullptr;
        info.pMarkerName = name;
        for (int i = 0; i < 4; ++i)
            info.color[i] = color[i];
        vkCmdDebugMarkerInsertEXT(handle, &info);
    }
#elif defined(_MSC_VER)
    name;
    color;
#endif // MAGMA_DEBUG
}

void CommandBuffer::beginDebugLabel(const char *name, const float color[4]) noexcept
{
    MAGMA_ASSERT(name);
    MAGMA_ASSERT(strlen(name) > 0);
    MAGMA_ASSERT(color);
#ifdef MAGMA_DEBUG
    MAGMA_OPTIONAL_DEVICE_EXTENSION(vkCmdBeginDebugUtilsLabelEXT);
    if (vkCmdBeginDebugUtilsLabelEXT)
    {
        VkDebugUtilsLabelEXT info;
        info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
        info.pNext = nullptr;
        info.pLabelName = name;
        for (int i = 0; i < 4; ++i)
            info.color[i] = color[i];
        vkCmdBeginDebugUtilsLabelEXT(handle, &info);
    }
#elif defined(_MSC_VER)
    name;
    color;
#endif // MAGMA_DEBUG
}

void CommandBuffer::endDebugLabel() noexcept
{
    MAGMA_OPTIONAL_DEVICE_EXTENSION(vkCmdEndDebugUtilsLabelEXT);
    if (vkCmdEndDebugUtilsLabelEXT)
        vkCmdEndDebugUtilsLabelEXT(handle);
}

void CommandBuffer::insertDebugLabel(const char *name, const float color[4]) noexcept
{
    MAGMA_ASSERT(name);
    MAGMA_ASSERT(strlen(name) > 0);
    MAGMA_ASSERT(color);
#ifdef MAGMA_DEBUG
    MAGMA_OPTIONAL_DEVICE_EXTENSION(vkCmdInsertDebugUtilsLabelEXT);
    if (vkCmdInsertDebugUtilsLabelEXT)
    {
        VkDebugUtilsLabelEXT info;
        info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
        info.pNext = nullptr;
        info.pLabelName = name;
        for (int i = 0; i < 4; ++i)
            info.color[i] = color[i];
        vkCmdInsertDebugUtilsLabelEXT(handle, &info);
    }
#elif defined(_MSC_VER)
    name;
    color;
#endif // MAGMA_DEBUG
}
} // namespace magma
