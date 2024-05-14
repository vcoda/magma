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
#ifdef MAGMA_DEBUG
#ifdef VK_EXT_debug_marker
void CommandBuffer::beginDebugMarker(const char *name, uint32_t color) noexcept
{
    MAGMA_ASSERT(name);
    MAGMA_ASSERT(strlen(name) > 0);
    MAGMA_DEVICE_EXTENSION(vkCmdDebugMarkerBeginEXT);
    if (vkCmdDebugMarkerBeginEXT && debugMarkerEnabled)
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
}

void CommandBuffer::endDebugMarker() noexcept
{
    MAGMA_DEVICE_EXTENSION(vkCmdDebugMarkerEndEXT);
    if (vkCmdDebugMarkerEndEXT && debugMarkerEnabled)
        vkCmdDebugMarkerEndEXT(handle);
}

void CommandBuffer::insertDebugMarker(const char *name, uint32_t color) noexcept
{
    MAGMA_ASSERT(name);
    MAGMA_ASSERT(strlen(name) > 0);
    MAGMA_DEVICE_EXTENSION(vkCmdDebugMarkerInsertEXT);
    if (vkCmdDebugMarkerInsertEXT && debugMarkerEnabled)
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
}
#endif // VK_EXT_debug_marker

#ifdef VK_EXT_debug_utils
void CommandBuffer::beginDebugLabel(const char *name, uint32_t color) noexcept
{
    MAGMA_ASSERT(name);
    MAGMA_ASSERT(strlen(name) > 0);
    MAGMA_DEVICE_EXTENSION(vkCmdBeginDebugUtilsLabelEXT);
    if (vkCmdBeginDebugUtilsLabelEXT && debugUtilsEnabled)
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
}

void CommandBuffer::endDebugLabel() noexcept
{
    MAGMA_DEVICE_EXTENSION(vkCmdEndDebugUtilsLabelEXT);
    if (vkCmdEndDebugUtilsLabelEXT && debugUtilsEnabled)
        vkCmdEndDebugUtilsLabelEXT(handle);
}

void CommandBuffer::insertDebugLabel(const char *name, uint32_t color) noexcept
{
    MAGMA_ASSERT(name);
    MAGMA_ASSERT(strlen(name) > 0);
    MAGMA_DEVICE_EXTENSION(vkCmdInsertDebugUtilsLabelEXT);
    if (vkCmdInsertDebugUtilsLabelEXT && debugUtilsEnabled)
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
}
#endif // VK_EXT_debug_utils

#ifdef VK_NV_device_diagnostic_checkpoints
void CommandBuffer::setCheckpoint(const char *name) noexcept
{
    MAGMA_ASSERT(name);
    MAGMA_ASSERT(strlen(name) > 0);
    MAGMA_DEVICE_EXTENSION(vkCmdSetCheckpointNV);
    if (vkCmdSetCheckpointNV)
        vkCmdSetCheckpointNV(handle, name);
}
#endif // VK_NV_device_diagnostic_checkpoints
#endif // MAGMA_DEBUG

bool CommandBuffer::begin(const char *blockName, uint32_t blockColor,
    VkCommandBufferUsageFlags flags /* 0 */) noexcept
{
    const bool beginResult = begin(flags);
#ifdef VK_EXT_debug_utils
    beginDebugLabel(blockName, blockColor);
    labeledRecording = VK_TRUE;
#else
    MAGMA_UNUSED(blockName);
    MAGMA_UNUSED(blockColor);
#endif // VK_EXT_debug_utils
    return beginResult;
}

bool CommandBuffer::beginInherited(const std::shared_ptr<RenderPass>& renderPass, uint32_t subpass, const std::shared_ptr<Framebuffer>& framebuffer,
    const char *blockName, uint32_t blockColor,
    VkCommandBufferUsageFlags flags /* 0 */) noexcept
{
    const bool beginResult = beginInherited(renderPass, subpass, framebuffer, flags);
#ifdef VK_EXT_debug_utils
    beginDebugLabel(blockName, blockColor);
    labeledRecording = VK_TRUE;
#else
    MAGMA_UNUSED(blockName);
    MAGMA_UNUSED(blockColor);
#endif // VK_EXT_debug_utils
    return beginResult;
}

void CommandBuffer::beginRenderPass(const std::shared_ptr<RenderPass>& renderPass, const std::shared_ptr<Framebuffer>& framebuffer,
    const std::vector<ClearValue>& clearValues, const char *passName, uint32_t passColor,
    const VkRect2D& renderArea /* {0, 0, 0, 0} */,
    VkSubpassContents contents /* VK_SUBPASS_CONTENTS_INLINE */) noexcept
{
    beginRenderPass(renderPass, framebuffer, clearValues, renderArea, contents);
#ifdef VK_EXT_debug_utils
    beginDebugLabel(passName, passColor);
    labeledRenderPass = VK_TRUE;
#else
    MAGMA_UNUSED(passName);
    MAGMA_UNUSED(passColor);
#endif // VK_EXT_debug_utils
}

#ifdef VK_KHR_imageless_framebuffer
void CommandBuffer::beginRenderPass(const std::shared_ptr<RenderPass>& renderPass, const std::shared_ptr<ImagelessFramebuffer>& framebuffer,
    const std::vector<std::shared_ptr<ImageView>>& attachments, const std::vector<ClearValue>& clearValues,
    const char *passName, uint32_t passColor,
    const VkRect2D& renderArea /* {0, 0, 0, 0} */,
    VkSubpassContents contents /* VK_SUBPASS_CONTENTS_INLINE */) noexcept
{
    beginRenderPass(renderPass, framebuffer, attachments, clearValues, renderArea, contents);
#ifdef VK_EXT_debug_utils
    beginDebugLabel(passName, passColor);
    labeledRenderPass = VK_TRUE;
#else
    MAGMA_UNUSED(passName);
    MAGMA_UNUSED(passColor);
#endif // VK_EXT_debug_utils
}
#endif // VK_KHR_imageless_framebuffer

#ifdef VK_KHR_device_group
bool CommandBuffer::beginDeviceGroup(uint32_t deviceMask, const char *blockName, uint32_t blockColor,
    VkCommandBufferUsageFlags flags /* 0 */) noexcept
{
    const bool result = beginDeviceGroup(deviceMask, flags);
#ifdef VK_EXT_debug_utils
    beginDebugLabel(blockName, blockColor);
    labeledRecording = VK_TRUE;
#else
    MAGMA_UNUSED(blockName);
    MAGMA_UNUSED(blockColor);
#endif // VK_EXT_debug_utils
    return result;
}

void CommandBuffer::beginDeviceGroupRenderPass(uint32_t deviceMask,
    const std::shared_ptr<RenderPass>& renderPass, const std::shared_ptr<Framebuffer>& framebuffer,
    const std::vector<VkRect2D>& deviceRenderAreas, const std::vector<ClearValue>& clearValues,
    const char *passName, uint32_t passColor,
    VkSubpassContents contents /* VK_SUBPASS_CONTENTS_INLINE */) noexcept
{
    beginDeviceGroupRenderPass(deviceMask, renderPass, framebuffer, deviceRenderAreas, clearValues, contents);
#ifdef VK_EXT_debug_utils
    beginDebugLabel(passName, passColor);
    labeledRenderPass = VK_TRUE;
#else
    MAGMA_UNUSED(passName);
    MAGMA_UNUSED(passColor);
#endif // VK_EXT_debug_utils
}

#ifdef VK_KHR_imageless_framebuffer
void CommandBuffer::beginDeviceGroupRenderPass(uint32_t deviceMask,
    const std::shared_ptr<RenderPass>& renderPass, const std::shared_ptr<ImagelessFramebuffer>& framebuffer,
    const std::vector<std::shared_ptr<ImageView>>& attachments, const std::vector<VkRect2D>& deviceRenderAreas,
    const std::vector<ClearValue>& clearValues, const char *passName, uint32_t passColor,
    VkSubpassContents contents /* VK_SUBPASS_CONTENTS_INLINE */) noexcept
{
    beginDeviceGroupRenderPass(deviceMask, renderPass, framebuffer, attachments, deviceRenderAreas, clearValues, contents);
#ifdef VK_EXT_debug_utils
    beginDebugLabel(passName, passColor);
    labeledRenderPass = VK_TRUE;
#else
    MAGMA_UNUSED(passName);
    MAGMA_UNUSED(passColor);
#endif // VK_EXT_debug_utils
}
#endif // VK_KHR_imageless_framebuffer
#endif // VK_KHR_device_group
} // namespace magma
