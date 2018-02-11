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
#include <algorithm>
#include "commandBuffer.h"
#include "device.h"
#include "renderPass.h"
#include "framebuffer.h"
#include "queryPool.h"
#include "vertexBuffer.h"
#include "indexBuffer.h"
#include "image.h"
#include "descriptorSet.h"
#include "pipelineLayout.h"
#include "pipeline.h"
#include "../misc/bufferMemoryBarrier.h"
#include "../helpers/stackArray.h"

namespace magma
{
#ifdef MAGMA_DEBUG
PFN_vkCmdDebugMarkerBeginEXT vkCmdDebugMarkerBegin;
PFN_vkCmdDebugMarkerEndEXT vkCmdDebugMarkerEnd;
PFN_vkCmdDebugMarkerInsertEXT vkCmdDebugMarkerInsert;
#endif // MAGMA_DEBUG

CommandBuffer::CommandBuffer(VkCommandBuffer handle, std::shared_ptr<const Device> device):
    Handle(VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_BUFFER_EXT, handle, device)
{
#ifdef MAGMA_DEBUG
    if (DebugObject::debugMarkerEnabled() && !vkCmdDebugMarkerBegin)
    {
        vkCmdDebugMarkerBegin = (PFN_vkCmdDebugMarkerBeginEXT)vkGetDeviceProcAddr(*device, "vkCmdDebugMarkerBeginEXT");
        vkCmdDebugMarkerEnd = (PFN_vkCmdDebugMarkerEndEXT)vkGetDeviceProcAddr(*device, "vkCmdDebugMarkerEndEXT");
        vkCmdDebugMarkerInsert = (PFN_vkCmdDebugMarkerInsertEXT)vkGetDeviceProcAddr(*device, "vkCmdDebugMarkerInsertEXT");
    }
#endif // MAGMA_DEBUG
}

bool CommandBuffer::begin(VkCommandBufferUsageFlags flags /* 0 */) noexcept
{
    VkCommandBufferBeginInfo beginInfo;
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.pNext = nullptr;
    beginInfo.flags = flags;
    beginInfo.pInheritanceInfo = nullptr;
    const VkResult begin = vkBeginCommandBuffer(handle, &beginInfo);
    MAGMA_ASSERT(VK_SUCCESS == begin);
    return (VK_SUCCESS == begin);
}

bool CommandBuffer::beginInherited(const std::shared_ptr<RenderPass>& renderPass, uint32_t subpass, const std::shared_ptr<Framebuffer>& framebuffer,
    VkCommandBufferUsageFlags flags /* 0 */) noexcept
{
    VkCommandBufferInheritanceInfo inheritanceInfo;
    inheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
    inheritanceInfo.pNext = nullptr;
    inheritanceInfo.renderPass = *renderPass;
    inheritanceInfo.subpass = subpass;
    inheritanceInfo.framebuffer = *framebuffer;
    inheritanceInfo.occlusionQueryEnable = MAGMA_BOOLEAN(occlusionQueryEnable);
    inheritanceInfo.queryFlags = queryFlags;
    inheritanceInfo.pipelineStatistics = pipelineStatistics;
    VkCommandBufferBeginInfo beginInfo;
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.pNext = nullptr;
    beginInfo.flags = flags | VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
    beginInfo.pInheritanceInfo = &inheritanceInfo;
    const VkResult begin = vkBeginCommandBuffer(handle, &beginInfo);
    MAGMA_ASSERT(VK_SUCCESS == begin);
    return (VK_SUCCESS == begin);
}

bool CommandBuffer::end() noexcept
{
    const VkResult end = vkEndCommandBuffer(handle);
    MAGMA_ASSERT(VK_SUCCESS == end);
    return (VK_SUCCESS == end);
}

bool CommandBuffer::reset(bool releaseResources) noexcept
{
    VkCommandBufferResetFlags flags = 0;
    if (releaseResources)
        flags |= VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT;
    const VkResult reset = vkResetCommandBuffer(handle, flags);
    MAGMA_ASSERT(VK_SUCCESS == reset);
    return (VK_SUCCESS == reset);
}

void CommandBuffer::bindPipeline(const std::shared_ptr<GraphicsPipeline>& pipeline) noexcept
{
    vkCmdBindPipeline(handle, VK_PIPELINE_BIND_POINT_GRAPHICS, *pipeline);
}

void CommandBuffer::bindPipeline(const std::shared_ptr<ComputePipeline>& pipeline) noexcept
{
    vkCmdBindPipeline(handle, VK_PIPELINE_BIND_POINT_COMPUTE, *pipeline);
}

// inline void CommandBuffer::setViewport
// inline void CommandBuffer::setScissor
// inline void CommandBuffer::setLineWidth
// inline void CommandBuffer::setDepthBias
// inline void CommandBuffer::setBlendConstants
// inline void CommandBuffer::setDepthBounds
// inline void CommandBuffer::setStencilCompareMask
// inline void CommandBuffer::setStencilWriteMask
// inline void CommandBuffer::setStencilReference

// inline void CommandBuffer::bindDescriptorSet
// inline void CommandBuffer::bindDescriptorSets

// inline void CommandBuffer::bindIndexBuffer
// inline void CommandBuffer::bindVertexBuffer
// inline void CommandBuffer::bindVertexBuffers

// inline void CommandBuffer::draw
// inline void CommandBuffer::drawInstanced
// inline void CommandBuffer::drawIndexed
// inline void CommandBuffer::drawIndexedInstanced
// inline void CommandBuffer::drawIndirect
// inline void CommandBuffer::drawIndexedIndirect
// inline void CommandBuffer::dispatch
// inline void CommandBuffer::dispatchIndirect

void CommandBuffer::copyBuffer(const std::shared_ptr<Buffer>& srcBuffer, const std::shared_ptr<Buffer>& dstBuffer,
    VkDeviceSize srcOffset /* 0 */, VkDeviceSize dstOffset /* 0 */, VkDeviceSize size /* 0 */) const noexcept
{
    VkBufferCopy region;
    region.srcOffset = srcOffset;
    region.dstOffset = dstOffset;
    region.size = size ? size : std::min(srcBuffer->getMemory()->getSize(),
        dstBuffer->getMemory()->getSize());
    vkCmdCopyBuffer(handle, *srcBuffer, *dstBuffer, 1, &region);
}

void CommandBuffer::copyBuffer(const std::shared_ptr<Buffer>& srcBuffer, const std::shared_ptr<Buffer>& dstBuffer, const VkBufferCopy& region) const noexcept
{
    vkCmdCopyBuffer(handle, *srcBuffer, *dstBuffer, 1, &region);
}

void CommandBuffer::copyBuffer(const std::shared_ptr<Buffer>& srcBuffer, const std::shared_ptr<Buffer>& dstBuffer, const std::vector<VkBufferCopy>& regions) const noexcept
{
    vkCmdCopyBuffer(handle, *srcBuffer, *dstBuffer, MAGMA_COUNT(regions), regions.data());
}

void CommandBuffer::copyBufferToImage(const std::shared_ptr<Buffer>& srcBuffer, const std::shared_ptr<Image>& dstImage, VkImageLayout dstImageLayout, const std::vector<VkBufferImageCopy>& regions) const noexcept
{
    vkCmdCopyBufferToImage(handle, *srcBuffer, *dstImage, dstImageLayout, MAGMA_COUNT(regions), regions.data());
}

void CommandBuffer::copyImageToBuffer(const std::shared_ptr<Image>& srcImage, VkImageLayout srcImageLayout, const std::shared_ptr<Buffer>& dstBuffer, const std::vector<VkBufferImageCopy>& regions) const noexcept
{
    vkCmdCopyImageToBuffer(handle, *srcImage, srcImageLayout, *dstBuffer, MAGMA_COUNT(regions), regions.data());
}

void CommandBuffer::pipelineImageBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::vector<VkImageMemoryBarrier>& barriers) noexcept
void CommandBuffer::pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::shared_ptr<Buffer>& buffer, const BufferMemoryBarrier& barrier) noexcept
{
    vkCmdPipelineBarrier(handle, 
        srcStageMask, dstStageMask, 0,
        0, nullptr,
        0, nullptr,
        MAGMA_COUNT(barriers),
        barriers.data());
    const BufferMemoryBarrier bufferBarrier(buffer, barrier);
    vkCmdPipelineBarrier(handle, srcStageMask, dstStageMask, 0, 0, nullptr, 1, &bufferBarrier, 0, nullptr);
}

void CommandBuffer::pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::vector<std::shared_ptr<Buffer>>& buffers, const BufferMemoryBarrier& barrier) noexcept
{
    MAGMA_STACK_ARRAY(VkBufferMemoryBarrier, barriers, buffers.size());
    for (const auto& buffer : buffers)
        barriers.put(BufferMemoryBarrier(buffer, barrier));
    vkCmdPipelineBarrier(handle, srcStageMask, dstStageMask, 0, 0, nullptr, MAGMA_COUNT(barriers), barriers, 0, nullptr);
}

}

void CommandBuffer::beginQuery(const std::shared_ptr<QueryPool>& queryPool, uint32_t queryIndex, bool precise) noexcept
{
    MAGMA_ASSERT(queryIndex < queryPool->getQueryCount());
    VkQueryControlFlags flags = 0;
    if (precise)
        flags |= VK_QUERY_CONTROL_PRECISE_BIT;
    vkCmdBeginQuery(handle, *queryPool, queryIndex, flags);
}

void CommandBuffer::endQuery(const std::shared_ptr<QueryPool>& queryPool, uint32_t queryIndex) noexcept
{
    MAGMA_ASSERT(queryIndex < queryPool->getQueryCount());
    vkCmdEndQuery(handle, *queryPool, queryIndex);
}

void CommandBuffer::resetQueryPool(const std::shared_ptr<QueryPool>& queryPool) noexcept
{
    vkCmdResetQueryPool(handle, *queryPool, 0, queryPool->getQueryCount());
}

void CommandBuffer::copyQueryResults(const std::shared_ptr<QueryPool>& queryPool, const std::shared_ptr<Buffer>& buffer, bool wait,
    uint32_t firstQuery /* 0 */, uint32_t queryCount /* std::numeric_limits<uint32_t>::max() */,
    VkDeviceSize dstOffset /* 0 */, bool write64Bit /* true */) noexcept
{
    if (std::numeric_limits<uint32_t>::max() == queryCount)
        queryCount = queryPool->getQueryCount();
    MAGMA_ASSERT(firstQuery + queryCount <= queryPool->getQueryCount());
    MAGMA_ASSERT(dstOffset < buffer->getMemory()->getSize());
    VkQueryResultFlags flags = 0;
    if (write64Bit)
        flags |= VK_QUERY_RESULT_64_BIT;
    if (wait)
        flags |= VK_QUERY_RESULT_WAIT_BIT;
    else
        flags |= VK_QUERY_RESULT_WITH_AVAILABILITY_BIT;
    // TODO: VK_QUERY_RESULT_PARTIAL_BIT
    const VkDeviceSize stride = write64Bit ? sizeof(uint64_t) : sizeof(uint32_t);
    vkCmdCopyQueryPoolResults(handle, *queryPool, firstQuery, queryCount, *buffer, dstOffset, stride, flags);
}

void CommandBuffer::beginRenderPass(const std::shared_ptr<RenderPass>& renderPass, const std::shared_ptr<Framebuffer>& framebuffer, const ClearValue& clearValue,
    VkSubpassContents contents /* VK_SUBPASS_CONTENTS_INLINE */) noexcept
{
    beginRenderPass(renderPass, framebuffer, {clearValue}, contents);
}

void CommandBuffer::beginRenderPass(const std::shared_ptr<RenderPass>& renderPass, const std::shared_ptr<Framebuffer>& framebuffer, const std::initializer_list<ClearValue>& clearValues,
    VkSubpassContents contents /* VK_SUBPASS_CONTENTS_INLINE */) noexcept
{
    VkRenderPassBeginInfo beginInfo;
    beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    beginInfo.pNext = nullptr;
    beginInfo.renderPass = *renderPass;
    beginInfo.framebuffer = *framebuffer;
    beginInfo.renderArea = renderArea;
    beginInfo.clearValueCount = MAGMA_COUNT(clearValues);
    MAGMA_STACK_ARRAY(VkClearValue, dereferencedClearValues, beginInfo.clearValueCount);
    for (const auto& clearValue : clearValues)
        dereferencedClearValues.put(clearValue);
    beginInfo.pClearValues = dereferencedClearValues;
    vkCmdBeginRenderPass(handle, &beginInfo, contents);
}

void CommandBuffer::nextSubpass() noexcept
{
    // TODO: implement
}

void CommandBuffer::endRenderPass() noexcept
{
    vkCmdEndRenderPass(handle);
}

void CommandBuffer::executeCommands(const std::vector<std::shared_ptr<CommandBuffer>>& commandBuffers) noexcept
{
    MAGMA_STACK_ARRAY(VkCommandBuffer, dereferencedCommandBuffers, commandBuffers.size());
    for (const auto& commandBuffer : commandBuffers)
        dereferencedCommandBuffers.put(*commandBuffer);
    vkCmdExecuteCommands(handle, dereferencedCommandBuffers.size(), dereferencedCommandBuffers);
}

void CommandBuffer::beginDebugMarker(const char *name, const float color[4]) noexcept
{
#ifdef MAGMA_DEBUG
    if (vkCmdDebugMarkerBegin)
    {
        VkDebugMarkerMarkerInfoEXT info;
        info.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT;
        info.pNext = nullptr;
        info.pMarkerName = name;
        memcpy(info.color, color, sizeof(color));
        vkCmdDebugMarkerBegin(handle, &info);
    }
#endif // MAGMA_DEBUG
}

void CommandBuffer::endDebugMarker() noexcept
{
#ifdef MAGMA_DEBUG
    if (vkCmdDebugMarkerEnd)
        vkCmdDebugMarkerEnd(handle);
#endif // MAGMA_DEBUG
}

void CommandBuffer::insertDebugMarker(const char *name) noexcept
{
#ifdef MAGMA_DEBUG
    if (vkCmdDebugMarkerInsert)
    {
        VkDebugMarkerMarkerInfoEXT info;
        info.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT;
        info.pNext = nullptr;
        info.pMarkerName = name;
        info.color[0] = 0.f;
        info.color[1] = 0.f;
        info.color[2] = 0.f;
        info.color[3] = 1.f;
        vkCmdDebugMarkerInsert(handle, &info);
    }
#endif // MAGMA_DEBUG
}
} // namespace magma
