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
#include "descriptorSet.h"
#include "pipelineLayout.h"
#include "pipeline.h"
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

bool CommandBuffer::begin() noexcept
{
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    const VkResult begin = vkBeginCommandBuffer(handle, &beginInfo);
    return (VK_SUCCESS == begin);
}

bool CommandBuffer::begin(std::shared_ptr<const RenderPass> renderPass, uint32_t subpass, std::shared_ptr<const Framebuffer> framebuffer,
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
    beginInfo.flags = flags;
    beginInfo.pInheritanceInfo = &inheritanceInfo;
    const VkResult begin = vkBeginCommandBuffer(handle, &beginInfo);
    return (VK_SUCCESS == begin);
}

bool CommandBuffer::end() noexcept
{
    const VkResult end = vkEndCommandBuffer(handle);
    return (VK_SUCCESS == end);
}

void CommandBuffer::beginRenderPass(std::shared_ptr<const RenderPass> renderPass, std::shared_ptr<const Framebuffer> framebuffer,
    VkSubpassContents contents /* VK_SUBPASS_CONTENTS_INLINE */) noexcept
{
    VkRenderPassBeginInfo beginInfo;
    beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    beginInfo.pNext = nullptr;
    beginInfo.renderPass = *renderPass;
    beginInfo.framebuffer = *framebuffer;
    beginInfo.renderArea = renderArea;
    beginInfo.clearValueCount = MAGMA_COUNT(clearValues);
    beginInfo.pClearValues = clearValues.front();
    vkCmdBeginRenderPass(handle, &beginInfo, contents);
}

void CommandBuffer::endRenderPass() noexcept
{
    vkCmdEndRenderPass(handle);
}

void CommandBuffer::beginQuery(std::shared_ptr<QueryPool> queryPool, uint32_t queryIndex, bool precise) noexcept
{
    MAGMA_ASSERT(queryIndex < queryPool->getQueryCount());
    VkQueryControlFlags flags = 0;
    if (precise)
        flags |= VK_QUERY_CONTROL_PRECISE_BIT;
    vkCmdBeginQuery(handle, *queryPool, queryIndex, flags);
}

void CommandBuffer::endQuery(std::shared_ptr<QueryPool> queryPool, uint32_t queryIndex) noexcept
{
    MAGMA_ASSERT(queryIndex < queryPool->getQueryCount());
    vkCmdEndQuery(handle, *queryPool, queryIndex);
}

void CommandBuffer::resetQueryPool(std::shared_ptr<QueryPool> queryPool) noexcept
{
    vkCmdResetQueryPool(handle, *queryPool, 0, queryPool->getQueryCount());
}

void CommandBuffer::copyQueryResults(std::shared_ptr<const QueryPool> queryPool, uint32_t firstQuery, uint32_t queryCount, std::shared_ptr<Buffer> buffer, 
    bool write64Bit, bool wait, VkDeviceSize offset /* 0 */) noexcept
{
    VkQueryResultFlags flags = 0;
    if (write64Bit)
        flags |= VK_QUERY_RESULT_64_BIT;
    if (wait)
        flags |= VK_QUERY_RESULT_WAIT_BIT;
    else
        flags |= VK_QUERY_RESULT_WITH_AVAILABILITY_BIT;
    // TODO: VK_QUERY_RESULT_PARTIAL_BIT
    const VkDeviceSize stride = write64Bit ? sizeof(uint64_t) : sizeof(uint32_t);
    vkCmdCopyQueryPoolResults(handle, *queryPool, firstQuery, queryCount, *buffer, offset, stride, flags);
}

void CommandBuffer::bindDescriptorSet(std::shared_ptr<const DescriptorSet> descriptorSet, std::shared_ptr<const PipelineLayout> pipelineLayout,
    VkPipelineBindPoint pipelineBindPoint /* VK_PIPELINE_BIND_POINT_GRAPHICS */) noexcept
{
    const VkDescriptorSet nativeDescriptorSets[1] = {*descriptorSet};
    vkCmdBindDescriptorSets(handle, pipelineBindPoint, *pipelineLayout, 0, 1, nativeDescriptorSets, 0, nullptr);
}

void CommandBuffer::bindPipeline(std::shared_ptr<const GraphicsPipeline> pipeline) noexcept
{
    vkCmdBindPipeline(handle, VK_PIPELINE_BIND_POINT_GRAPHICS, *pipeline);
}

void CommandBuffer::bindPipeline(std::shared_ptr<const ComputePipeline> pipeline) noexcept
{
    vkCmdBindPipeline(handle, VK_PIPELINE_BIND_POINT_COMPUTE, *pipeline);
}

void CommandBuffer::bindVertexBuffer(uint32_t firstBinding, std::shared_ptr<const VertexBuffer> vertexBuffer, VkDeviceSize offset /* 0 */) noexcept
{
    const VkBuffer dereferencedBuffers[1] = {*vertexBuffer};
    vkCmdBindVertexBuffers(handle, firstBinding, 1, dereferencedBuffers, &offset);
}

void CommandBuffer::bindVertexBuffers(uint32_t firstBinding, const std::vector<std::shared_ptr<const VertexBuffer>>& vertexBuffers, const std::vector<VkDeviceSize>& offsets) noexcept
{
    MAGMA_ASSERT(vertexBuffers.size() > 0);
    MAGMA_ASSERT(vertexBuffers.size() == offsets.size());
    MAGMA_STACK_ARRAY(VkBuffer, dereferencedBuffers, vertexBuffers.size());
    for (const auto& buffer : vertexBuffers)
        dereferencedBuffers.put(*buffer);
    vkCmdBindVertexBuffers(handle, firstBinding, dereferencedBuffers.size(), dereferencedBuffers.data(), offsets.data());
}

void CommandBuffer::bindIndexBuffer(std::shared_ptr<const IndexBuffer> indexBuffer, VkDeviceSize offset /* 0 */) noexcept
{
    vkCmdBindIndexBuffer(handle, *indexBuffer, offset, indexBuffer->getIndexType());
}

void CommandBuffer::copyBuffer(std::shared_ptr<const Buffer> srcBuffer, std::shared_ptr<Buffer> dstBuffer,
    VkDeviceSize srcOffset /* 0 */, VkDeviceSize dstOffset /* 0 */, VkDeviceSize size /* 0 */) noexcept
{
    VkBufferCopy region;
    region.srcOffset = srcOffset;
    region.dstOffset = dstOffset;
    region.size = size ? size : std::min(srcBuffer->getMemory()->getSize(),
                                         dstBuffer->getMemory()->getSize());
    vkCmdCopyBuffer(handle, *srcBuffer, *dstBuffer, 1, &region);
}

void CommandBuffer::copyBuffer(std::shared_ptr<const Buffer> srcBuffer, std::shared_ptr<Buffer> dstBuffer, const VkBufferCopy& region) noexcept
{
    vkCmdCopyBuffer(handle, *srcBuffer, *dstBuffer, 1, &region);
}

void CommandBuffer::copyBuffer(std::shared_ptr<const Buffer> srcBuffer, std::shared_ptr<Buffer> dstBuffer, const std::vector<VkBufferCopy>& regions) noexcept
{
    vkCmdCopyBuffer(handle, *srcBuffer, *dstBuffer, MAGMA_COUNT(regions), regions.data());
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
