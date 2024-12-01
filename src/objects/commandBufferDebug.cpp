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
#ifdef MAGMA_DEBUG
#include "commandBuffer.h"

namespace magma
{
#ifdef VK_EXT_debug_marker
void CommandBuffer::beginDebugMarker(const char *name, float r, float g, float b, float a /* 1 */) noexcept
{
    MAGMA_ASSERT(strlen(name));
    if (extensions.EXT_debug_marker)
    {
        VkDebugMarkerMarkerInfoEXT debugMarker;
        debugMarker.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT;
        debugMarker.pNext = nullptr;
        debugMarker.pMarkerName = name;
        debugMarker.color[0] = r;
        debugMarker.color[1] = g;
        debugMarker.color[2] = b;
        debugMarker.color[3] = a;
        MAGMA_DEVICE_EXTENSION(vkCmdDebugMarkerBeginEXT);
        vkCmdDebugMarkerBeginEXT(leanCmd, &debugMarker);
    }
}

void CommandBuffer::endDebugMarker() noexcept
{
    if (extensions.EXT_debug_marker)
    {
        MAGMA_DEVICE_EXTENSION(vkCmdDebugMarkerEndEXT);
        vkCmdDebugMarkerEndEXT(leanCmd);
    }
}

void CommandBuffer::insertDebugMarker(const char *name, float r, float g, float b, float a /* 1 */) noexcept
{
    MAGMA_ASSERT(strlen(name));
    if (extensions.EXT_debug_marker)
    {
        VkDebugMarkerMarkerInfoEXT debugMarker;
        debugMarker.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT;
        debugMarker.pNext = nullptr;
        debugMarker.pMarkerName = name;
        debugMarker.color[0] = r;
        debugMarker.color[1] = g;
        debugMarker.color[2] = b;
        debugMarker.color[3] = a;
        MAGMA_DEVICE_EXTENSION(vkCmdDebugMarkerInsertEXT);
        vkCmdDebugMarkerInsertEXT(leanCmd, &debugMarker);
    }
}
#endif // VK_EXT_debug_marker

#ifdef VK_EXT_debug_utils
void CommandBuffer::beginDebugLabel(const char *name, float r, float g, float b, float a /* 1 */) noexcept
{
    MAGMA_ASSERT(strlen(name));
    if (extensions.EXT_debug_utils)
    {
        VkDebugUtilsLabelEXT debugLabel;
        debugLabel.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
        debugLabel.pNext = nullptr;
        debugLabel.pLabelName = name;
        debugLabel.color[0] = r;
        debugLabel.color[1] = g;
        debugLabel.color[2] = b;
        debugLabel.color[3] = a;
        MAGMA_DEVICE_EXTENSION(vkCmdBeginDebugUtilsLabelEXT);
        vkCmdBeginDebugUtilsLabelEXT(leanCmd, &debugLabel);
    }
}

void CommandBuffer::endDebugLabel() noexcept
{
    if (extensions.EXT_debug_utils)
    {
        MAGMA_DEVICE_EXTENSION(vkCmdEndDebugUtilsLabelEXT);
        vkCmdEndDebugUtilsLabelEXT(leanCmd);
    }
}

void CommandBuffer::insertDebugLabel(const char *name, float r, float g, float b, float a /* 1 */) noexcept
{
    MAGMA_ASSERT(strlen(name));
    if (extensions.EXT_debug_utils)
    {
        VkDebugUtilsLabelEXT debugLabel;
        debugLabel.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
        debugLabel.pNext = nullptr;
        debugLabel.pLabelName = name;
        debugLabel.color[0] = r;
        debugLabel.color[1] = g;
        debugLabel.color[2] = b;
        debugLabel.color[3] = a;
        MAGMA_DEVICE_EXTENSION(vkCmdInsertDebugUtilsLabelEXT);
        vkCmdInsertDebugUtilsLabelEXT(leanCmd, &debugLabel);
    }
}
#endif // VK_EXT_debug_utils

#ifdef VK_AMD_buffer_marker
void CommandBuffer::writeBufferMarker(VkPipelineStageFlagBits pipelineStage, const std::unique_ptr<Buffer>& dstBuffer, VkDeviceSize dstOffset, uint32_t marker) const noexcept
{
    if (extensions.AMD_buffer_marker)
    {
        MAGMA_DEVICE_EXTENSION(vkCmdWriteBufferMarkerAMD);
        vkCmdWriteBufferMarkerAMD(leanCmd, pipelineStage, dstBuffer->getHandle(), dstOffset, marker);
    }
}
#endif // VK_AMD_buffer_marker

#ifdef VK_NV_device_diagnostic_checkpoints
void CommandBuffer::setCheckpoint(const char *name) const noexcept
{
    MAGMA_ASSERT(name);
    MAGMA_ASSERT(strlen(name) > 0);
    if (extensions.NV_device_diagnostic_checkpoints)
    {
        MAGMA_DEVICE_EXTENSION(vkCmdSetCheckpointNV);
        vkCmdSetCheckpointNV(leanCmd, name);
    }
}
#endif // VK_NV_device_diagnostic_checkpoints

void CommandBuffer::pushDebugMarker(const char* name, uint32_t color) noexcept
{
    float r, g, b, a;
    MAGMA_DWORD_TO_FLOAT_RGBA(color, r, g, b, a);
#ifdef VK_EXT_debug_utils
    if (extensions.EXT_debug_utils)
        return beginDebugLabel(name, r, g, b, a);
#endif // VK_EXT_debug_utils
#ifdef VK_EXT_debug_marker
    if (extensions.EXT_debug_marker)
        beginDebugMarker(name, r, g, b, a);
#endif // VK_EXT_debug_utils
}

void CommandBuffer::popDebugMarker() noexcept
{
#ifdef VK_EXT_debug_utils
    if (extensions.EXT_debug_utils)
        return endDebugLabel();
#endif // VK_EXT_debug_utils
#ifdef VK_EXT_debug_marker
    if (extensions.EXT_debug_marker)
        endDebugMarker();
#endif // VK_EXT_debug_utils
}

// https://asawicki.info/news_1677_debugging_vulkan_driver_crash_-_equivalent_of_nvidia_aftermath.html
void CommandBuffer::insertDebugCheckpoint(const char *command, VkPipelineStageFlagBits pipelineStage) const noexcept
{
    MAGMA_UNUSED(pipelineStage);
    MAGMA_ASSERT(State::Recording == state);
#ifdef VK_NV_device_diagnostic_checkpoints
    if (extensions.NV_device_diagnostic_checkpoints)
        return setCheckpoint(command);
#endif // VK_NV_device_diagnostic_checkpoints
    const std::unique_ptr<Buffer>& markerBuffer = getMarkerBuffer();
    if (!markerBuffer)
        return;
    uint64_t offset = markerBuffer->getPrivateData();
    MAGMA_ASSERT(offset < markerBuffer->getSize());
    const uint32_t marker = core::countof(checkpoints);
#ifdef VK_AMD_buffer_marker
    if (extensions.AMD_buffer_marker)
        writeBufferMarker(pipelineStage, markerBuffer, offset, marker);
    else
#endif // VK_AMD_buffer_marker
    if (!renderingPass)
        leanCmd.fillBuffer(markerBuffer.get(), marker, sizeof(uint32_t), offset);
    if (extensions.AMD_buffer_marker || !renderingPass)
    {
        offset += sizeof(uint32_t);
        markerBuffer->setPrivateData(offset);
        checkpoints.push_back(command);
    }
}
} // namespace magma
#endif // MAGMA_DEBUG
