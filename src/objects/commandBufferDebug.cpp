/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

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
#include "commandBuffer.h"

namespace magma
{
#ifdef VK_EXT_debug_marker
void CommandBuffer::beginDebugMarker(const char *name, uint32_t color) noexcept
{
    MAGMA_ASSERT(name);
    MAGMA_ASSERT(strlen(name) > 0);
#ifdef MAGMA_DEBUG
    MAGMA_DEVICE_EXTENSION(vkCmdDebugMarkerBeginEXT);
    if (vkCmdDebugMarkerBeginEXT)
    {
        VkDebugMarkerMarkerInfoEXT debugMarkerInfo;
        debugMarkerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT;
        debugMarkerInfo.pNext = nullptr;
        debugMarkerInfo.pMarkerName = name;
        debugMarkerInfo.color[0] = ((color >> 24) & 0xFF) / 255.f; // R
        debugMarkerInfo.color[1] = ((color >> 16) & 0xFF) / 255.f; // G
        debugMarkerInfo.color[2] = ((color >> 8) & 0xFF) / 255.f; // B
        debugMarkerInfo.color[3] = (color & 0xFF) / 255.f; // A
        vkCmdDebugMarkerBeginEXT(handle, &debugMarkerInfo);
    }
#else
    MAGMA_UNUSED(name);
    MAGMA_UNUSED(color);
#endif // MAGMA_DEBUG
}

void CommandBuffer::endDebugMarker() noexcept
{
#ifdef MAGMA_DEBUG
    MAGMA_DEVICE_EXTENSION(vkCmdDebugMarkerEndEXT);
    if (vkCmdDebugMarkerEndEXT)
        vkCmdDebugMarkerEndEXT(handle);
#endif // MAGMA_DEBUG
}

void CommandBuffer::insertDebugMarker(const char *name, uint32_t color) noexcept
{
    MAGMA_ASSERT(name);
    MAGMA_ASSERT(strlen(name) > 0);
#ifdef MAGMA_DEBUG
    MAGMA_DEVICE_EXTENSION(vkCmdDebugMarkerInsertEXT);
    if (vkCmdDebugMarkerInsertEXT)
    {
        VkDebugMarkerMarkerInfoEXT debugMarkerInfo;
        debugMarkerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT;
        debugMarkerInfo.pNext = nullptr;
        debugMarkerInfo.pMarkerName = name;
        debugMarkerInfo.color[0] = ((color >> 24) & 0xFF) / 255.f; // R
        debugMarkerInfo.color[1] = ((color >> 16) & 0xFF) / 255.f; // G
        debugMarkerInfo.color[2] = ((color >> 8) & 0xFF) / 255.f; // B
        debugMarkerInfo.color[3] = (color & 0xFF) / 255.f; // A
        vkCmdDebugMarkerInsertEXT(handle, &debugMarkerInfo);
    }
#else
    MAGMA_UNUSED(name);
    MAGMA_UNUSED(color);
#endif // MAGMA_DEBUG
}
#endif // VK_EXT_debug_marker

#ifdef VK_EXT_debug_utils
void CommandBuffer::beginDebugLabel(const char *name, uint32_t color) noexcept
{
    MAGMA_ASSERT(name);
    MAGMA_ASSERT(strlen(name) > 0);
#ifdef MAGMA_DEBUG
    MAGMA_DEVICE_EXTENSION(vkCmdBeginDebugUtilsLabelEXT);
    if (vkCmdBeginDebugUtilsLabelEXT)
    {
        VkDebugUtilsLabelEXT debugUtilsLabel;
        debugUtilsLabel.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
        debugUtilsLabel.pNext = nullptr;
        debugUtilsLabel.pLabelName = name;
        debugUtilsLabel.color[0] = ((color >> 24) & 0xFF) / 255.f; // R
        debugUtilsLabel.color[1] = ((color >> 16) & 0xFF) / 255.f; // G
        debugUtilsLabel.color[2] = ((color >> 8) & 0xFF) / 255.f; // B
        debugUtilsLabel.color[3] = (color & 0xFF) / 255.f; // A
        vkCmdBeginDebugUtilsLabelEXT(handle, &debugUtilsLabel);
    }
#else
    MAGMA_UNUSED(name);
    MAGMA_UNUSED(color);
#endif // MAGMA_DEBUG
}

void CommandBuffer::endDebugLabel() noexcept
{
    MAGMA_DEVICE_EXTENSION(vkCmdEndDebugUtilsLabelEXT);
    if (vkCmdEndDebugUtilsLabelEXT)
        vkCmdEndDebugUtilsLabelEXT(handle);
}

void CommandBuffer::insertDebugLabel(const char *name, uint32_t color) noexcept
{
    MAGMA_ASSERT(name);
    MAGMA_ASSERT(strlen(name) > 0);
#ifdef MAGMA_DEBUG
    MAGMA_DEVICE_EXTENSION(vkCmdInsertDebugUtilsLabelEXT);
    if (vkCmdInsertDebugUtilsLabelEXT)
    {
        VkDebugUtilsLabelEXT debugUtilsLabel;
        debugUtilsLabel.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
        debugUtilsLabel.pNext = nullptr;
        debugUtilsLabel.pLabelName = name;
        debugUtilsLabel.color[0] = ((color >> 24) & 0xFF) / 255.f; // R
        debugUtilsLabel.color[1] = ((color >> 16) & 0xFF) / 255.f; // G
        debugUtilsLabel.color[2] = ((color >> 8) & 0xFF) / 255.f; // B
        debugUtilsLabel.color[3] = (color & 0xFF) / 255.f; // A
        vkCmdInsertDebugUtilsLabelEXT(handle, &debugUtilsLabel);
    }
#else
    MAGMA_UNUSED(name);
    MAGMA_UNUSED(color);
#endif // MAGMA_DEBUG
}
#endif // VK_EXT_debug_utils

bool CommandBuffer::begin(const char *blockName, uint32_t blockColor,
    VkCommandBufferUsageFlags flags /* 0 */) noexcept
{
    const bool beginResult = begin(flags);
#ifdef MAGMA_DEBUG_LABEL
    beginDebugLabel(blockName, blockColor);
    labeledRecording = VK_TRUE;
#else
    MAGMA_UNUSED(blockName);
    MAGMA_UNUSED(blockColor);
#endif // MAGMA_DEBUG_LABEL
    return beginResult;
}

bool CommandBuffer::beginInherited(const std::shared_ptr<RenderPass>& renderPass, uint32_t subpass, const std::shared_ptr<Framebuffer>& framebuffer,
    const char *blockName, uint32_t blockColor,
    VkCommandBufferUsageFlags flags /* 0 */) noexcept
{
    const bool beginResult = beginInherited(renderPass, subpass, framebuffer, flags);
#ifdef MAGMA_DEBUG_LABEL
    beginDebugLabel(blockName, blockColor);
    labeledRecording = VK_TRUE;
#else
    MAGMA_UNUSED(blockName);
    MAGMA_UNUSED(blockColor);
#endif // MAGMA_DEBUG_LABEL
    return beginResult;
}

void CommandBuffer::beginRenderPass(const std::shared_ptr<RenderPass>& renderPass, const std::shared_ptr<Framebuffer>& framebuffer,
    const std::vector<ClearValue>& clearValues, const char *renderPassName, uint32_t renderPassColor,
    const VkRect2D& renderArea /* {0, 0, 0, 0} */,
    VkSubpassContents contents /* VK_SUBPASS_CONTENTS_INLINE */) noexcept
{
    beginRenderPass(renderPass, framebuffer, clearValues, renderArea, contents);
#ifdef MAGMA_DEBUG_LABEL
    beginDebugLabel(renderPassName, renderPassColor);
    labeledRenderPass = VK_TRUE;
#else
    MAGMA_UNUSED(renderPassName);
    MAGMA_UNUSED(renderPassColor);
#endif // MAGMA_DEBUG_LABEL
}

#ifdef VK_KHR_imageless_framebuffer
void CommandBuffer::beginRenderPass(const std::shared_ptr<RenderPass>& renderPass, const std::shared_ptr<ImagelessFramebuffer>& framebuffer,
    const std::vector<std::shared_ptr<ImageView>>& attachments, const std::vector<ClearValue>& clearValues,
    const char *renderPassName, uint32_t renderPassColor,
    const VkRect2D& renderArea /* {0, 0, 0, 0} */,
    VkSubpassContents contents /* VK_SUBPASS_CONTENTS_INLINE */) noexcept
{
    beginRenderPass(renderPass, framebuffer, attachments, clearValues, renderArea, contents);
#ifdef MAGMA_DEBUG_LABEL
    beginDebugLabel(renderPassName, renderPassColor);
    labeledRenderPass = VK_TRUE;
#else
    MAGMA_UNUSED(renderPassName);
    MAGMA_UNUSED(renderPassColor);
#endif // MAGMA_DEBUG_LABEL
}
#endif // VK_KHR_imageless_framebuffer

#ifdef VK_KHR_device_group
bool CommandBuffer::beginDeviceGroup(uint32_t deviceMask, const char *blockName, uint32_t blockColor,
    VkCommandBufferUsageFlags flags /* 0 */) noexcept
{
    const bool result = beginDeviceGroup(deviceMask, flags);
#ifdef MAGMA_DEBUG_LABEL
    beginDebugLabel(blockName, blockColor);
    labeledRecording = VK_TRUE;
#else
    MAGMA_UNUSED(blockName);
    MAGMA_UNUSED(blockColor);
#endif // MAGMA_DEBUG_LABEL
    return result;
}

void CommandBuffer::beginDeviceGroupRenderPass(uint32_t deviceMask,
    const std::shared_ptr<RenderPass>& renderPass, const std::shared_ptr<Framebuffer>& framebuffer,
    const std::vector<VkRect2D>& deviceRenderAreas, const std::vector<ClearValue>& clearValues,
    const char *renderPassName, uint32_t renderPassColor,
    VkSubpassContents contents /* VK_SUBPASS_CONTENTS_INLINE */) noexcept
{
    beginDeviceGroupRenderPass(deviceMask, renderPass, framebuffer, deviceRenderAreas, clearValues, contents);
#ifdef MAGMA_DEBUG_LABEL
    beginDebugLabel(renderPassName, renderPassColor);
    labeledRenderPass = VK_TRUE;
#else
    MAGMA_UNUSED(renderPassName);
    MAGMA_UNUSED(renderPassColor);
#endif // MAGMA_DEBUG_LABEL
}

#ifdef VK_KHR_imageless_framebuffer
void CommandBuffer::beginDeviceGroupRenderPass(uint32_t deviceMask,
    const std::shared_ptr<RenderPass>& renderPass, const std::shared_ptr<ImagelessFramebuffer>& framebuffer,
    const std::vector<std::shared_ptr<ImageView>>& attachments, const std::vector<VkRect2D>& deviceRenderAreas,
    const std::vector<ClearValue>& clearValues, const char *renderPassName, uint32_t renderPassColor,
    VkSubpassContents contents /* VK_SUBPASS_CONTENTS_INLINE */) noexcept
{
    beginDeviceGroupRenderPass(deviceMask, renderPass, framebuffer, attachments, deviceRenderAreas, clearValues, contents);
#ifdef MAGMA_DEBUG_LABEL
    beginDebugLabel(renderPassName, renderPassColor);
    labeledRenderPass = VK_TRUE;
#else
    MAGMA_UNUSED(renderPassName);
    MAGMA_UNUSED(renderPassColor);
#endif // MAGMA_DEBUG_LABEL
}
#endif // VK_KHR_imageless_framebuffer
#endif // VK_KHR_device_group
} // namespace magma
