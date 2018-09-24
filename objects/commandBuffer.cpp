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
#include "deviceMemory.h"
#include "framebuffer.h"
#include "renderPass.h"
#include "image.h"
#include "event.h"
#include "fence.h"
#include "queryPool.h"
#include "../barriers/globalMemoryBarrier.h"
#include "../barriers/bufferMemoryBarrier.h"
#include "../barriers/imageMemoryBarrier.h"
#include "../misc/deviceExtension.h"

namespace magma
{
CommandBuffer::CommandBuffer(VkCommandBuffer handle, std::shared_ptr<Device> device, std::shared_ptr<CommandPool> pool):
    Dispatchable(VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_BUFFER_EXT, std::move(device), nullptr),
    pool(std::move(pool)),
    fence(std::make_shared<Fence>(this->device))
{
    this->handle = handle;
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

void CommandBuffer::end()
{
    /* Performance - critical commands generally do not have return codes.
       If a run time error occurs in such commands, the implementation will defer
       reporting the error until a specified point. For commands that record
       into command buffers (vkCmd*), run time errors are reported by vkEndCommandBuffer. */
    const VkResult result = vkEndCommandBuffer(handle);
    /* This is the only place where command buffer may throw and exception. */
    MAGMA_THROW_FAILURE(result, "failed to record command buffer");
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

// void CommandBuffer::bindPipeline
// void CommandBuffer::setViewport
// void CommandBuffer::setScissor
// void CommandBuffer::setLineWidth
// void CommandBuffer::setDepthBias
// void CommandBuffer::setBlendConstants
// void CommandBuffer::setDepthBounds
// void CommandBuffer::setStencilCompareMask
// void CommandBuffer::setStencilWriteMask
// void CommandBuffer::setStencilReference
// void CommandBuffer::bindDescriptorSet
// void CommandBuffer::bindDescriptorSets
// void CommandBuffer::bindIndexBuffer
// void CommandBuffer::bindVertexBuffer
// void CommandBuffer::bindVertexBuffers
// void CommandBuffer::draw
// void CommandBuffer::drawInstanced
// void CommandBuffer::drawIndexed
// void CommandBuffer::drawIndexedInstanced
// void CommandBuffer::drawIndirect
// void CommandBuffer::drawIndexedIndirect
// void CommandBuffer::dispatch
// void CommandBuffer::dispatchIndirect

void CommandBuffer::copyBuffer(const std::shared_ptr<Buffer>& srcBuffer, const std::shared_ptr<Buffer>& dstBuffer,
    VkDeviceSize srcOffset /* 0 */, VkDeviceSize dstOffset /* 0 */, VkDeviceSize size /* VK_WHOLE_SIZE */) const noexcept
{
    VkBufferCopy region;
    region.srcOffset = srcOffset;
    region.dstOffset = dstOffset;
    if (VK_WHOLE_SIZE == size)
        region.size = dstBuffer->getMemory()->getSize();
    else
        region.size = size;
    vkCmdCopyBuffer(handle, *srcBuffer, *dstBuffer, 1, &region);
}

void CommandBuffer::copyBuffer(const std::shared_ptr<Buffer>& srcBuffer, const std::shared_ptr<Buffer>& dstBuffer, const VkBufferCopy& region) const noexcept
{
    vkCmdCopyBuffer(handle, *srcBuffer, *dstBuffer, 1, &region);
}

void CommandBuffer::copyImage(const std::shared_ptr<Image>& srcImage, const std::shared_ptr<Image>& dstImage, const VkImageCopy& region) const noexcept
{
    vkCmdCopyImage(handle, *srcImage, srcImage->getLayout(), *dstImage, dstImage->getLayout(), 1, &region);
}

void CommandBuffer::blitImage(const std::shared_ptr<Image>& srcImage, const std::shared_ptr<Image>& dstImage, const VkImageBlit& region, VkFilter filter) const noexcept
{
    vkCmdBlitImage(handle, *srcImage, srcImage->getLayout(), *dstImage, dstImage->getLayout(), 1, &region, filter);
}

void CommandBuffer::copyBufferToImage(const std::shared_ptr<Buffer>& srcBuffer, const std::shared_ptr<Image>& dstImage, VkImageLayout dstImageLayout, const VkBufferImageCopy& region) const noexcept
{
    vkCmdCopyBufferToImage(handle, *srcBuffer, *dstImage, dstImageLayout, 1, &region);
}

void CommandBuffer::copyImageToBuffer(const std::shared_ptr<Image>& srcImage, const std::shared_ptr<Buffer>& dstBuffer, const VkBufferImageCopy& region) const noexcept
{
    vkCmdCopyImageToBuffer(handle, *srcImage, srcImage->getLayout(), *dstBuffer, 1, &region);
}

void CommandBuffer::updateBuffer(const std::shared_ptr<Buffer>& buffer, VkDeviceSize dataSize, const void *data,
    VkDeviceSize offset /* 0 */) const noexcept
{
    /* Buffer updates performed with vkCmdUpdateBuffer first copy the data
       into command buffer memory when the command is recorded
       (which requires additional storage and may incur an additional allocation),
       and then copy the data from the command buffer into dstBuffer
       when the command is executed on a device. */
    vkCmdUpdateBuffer(handle, *buffer, offset, dataSize, data);
}

void CommandBuffer::fillBuffer(const std::shared_ptr<Buffer>& buffer, uint32_t value,
    VkDeviceSize size /* 0 */,
    VkDeviceSize offset /* 0 */) const noexcept
{
    if (0 == size)
        size = buffer->getMemory()->getSize();
    vkCmdFillBuffer(handle, *buffer, offset, size, value);
}

void CommandBuffer::clearColorImage(const std::shared_ptr<Image>& image, const ClearColor& color, const VkImageSubresourceRange& range) const noexcept
{
    const VkClearColorValue& clearColor = static_cast<const VkClearValue&>(color).color;
    vkCmdClearColorImage(handle, *image, VK_IMAGE_LAYOUT_GENERAL, &clearColor, 1, &range);
}

void CommandBuffer::clearDepthStencilImage(const std::shared_ptr<Image>& image, const ClearDepthStencil& depthStencil, const VkImageSubresourceRange& range) const noexcept
{
    const VkClearDepthStencilValue& clearDepthStencil = static_cast<const VkClearValue&>(depthStencil).depthStencil;
    vkCmdClearDepthStencilImage(handle, *image, VK_IMAGE_LAYOUT_GENERAL, &clearDepthStencil, 1, &range);
}

void CommandBuffer::clearAttachments(const std::initializer_list<ClearAttachment>& attachments, const VkClearRect& clearRect) const noexcept
{
    vkCmdClearAttachments(handle, MAGMA_COUNT(attachments), attachments.begin(), 1, &clearRect);
}

void CommandBuffer::resolveImage(const std::shared_ptr<Image>& srcImage, const std::shared_ptr<Image>& dstImage, const VkImageResolve& region) const noexcept
{
    vkCmdResolveImage(handle, *srcImage, srcImage->getLayout(), *dstImage, dstImage->getLayout(), 1, &region);
}

void CommandBuffer::setEvent(const std::shared_ptr<Event>& event, VkPipelineStageFlags stageMask) noexcept
{
    MAGMA_ASSERT(!(stageMask & VK_PIPELINE_STAGE_HOST_BIT));
    vkCmdSetEvent(handle, *event, stageMask);
}

void CommandBuffer::resetEvent(const std::shared_ptr<Event>& event, VkPipelineStageFlags stageMask) noexcept
{
    MAGMA_ASSERT(!(stageMask & VK_PIPELINE_STAGE_HOST_BIT));
    vkCmdResetEvent(handle, *event, stageMask);
}

void CommandBuffer::waitEvent(const std::shared_ptr<Event>& event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
    const std::vector<GlobalMemoryBarrier>& memoryBarriers /* {} */,
    const std::vector<BufferMemoryBarrier>& bufferMemoryBarriers /* {} */,
    const std::vector<ImageMemoryBarrier>& imageMemoryBarriers /* {} */) noexcept
{
    MAGMA_ASSERT(srcStageMask);
    MAGMA_ASSERT(dstStageMask);
    const VkEvent dereferencedEvents[1] = {*event};
    vkCmdWaitEvents(handle, 1, dereferencedEvents, srcStageMask, dstStageMask,
        MAGMA_COUNT(memoryBarriers),
        memoryBarriers.data(),
        MAGMA_COUNT(bufferMemoryBarriers),
        bufferMemoryBarriers.data(),
        MAGMA_COUNT(imageMemoryBarriers),
        imageMemoryBarriers.data());
}

void CommandBuffer::waitEvents(const std::vector<std::shared_ptr<Event>>& events, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
    const std::vector<GlobalMemoryBarrier>& memoryBarriers /* {} */,
    const std::vector<BufferMemoryBarrier>& bufferMemoryBarriers /* {} */,
    const std::vector<ImageMemoryBarrier>& imageMemoryBarriers /* {} */) noexcept
{
    MAGMA_ASSERT(srcStageMask);
    MAGMA_ASSERT(dstStageMask);
    MAGMA_STACK_ARRAY(VkEvent, dereferencedEvents, events.size());
    for (const auto& event : events)
        dereferencedEvents.put(*event);
    vkCmdWaitEvents(handle, dereferencedEvents.size(), dereferencedEvents, srcStageMask, dstStageMask,
        MAGMA_COUNT(memoryBarriers),
        memoryBarriers.data(),
        MAGMA_COUNT(bufferMemoryBarriers),
        bufferMemoryBarriers.data(),
        MAGMA_COUNT(imageMemoryBarriers),
        imageMemoryBarriers.data());
}

void CommandBuffer::pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const GlobalMemoryBarrier& barrier,
    VkDependencyFlags dependencyFlags /* 0 */) noexcept
{
    vkCmdPipelineBarrier(handle, srcStageMask, dstStageMask, dependencyFlags,
        1, &barrier,
        0, nullptr,
        0, nullptr);
}

void CommandBuffer::pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const BufferMemoryBarrier& barrier,
    VkDependencyFlags dependencyFlags /* 0 */) noexcept
{
    vkCmdPipelineBarrier(handle, srcStageMask, dstStageMask, dependencyFlags,
        0, nullptr,
        1, &barrier,
        0, nullptr);
}

void CommandBuffer::pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const ImageMemoryBarrier& barrier,
    VkDependencyFlags dependencyFlags /* 0 */) noexcept
{
    vkCmdPipelineBarrier(handle, srcStageMask, dstStageMask, dependencyFlags,
        0, nullptr,
        0, nullptr,
        1, &barrier);
}

void CommandBuffer::pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::vector<std::shared_ptr<Buffer>>& buffers, const BufferMemoryBarrier& barrier,
    VkDependencyFlags dependencyFlags /* 0 */) noexcept
{
    MAGMA_STACK_ARRAY(VkBufferMemoryBarrier, barriers, buffers.size());
    for (const auto& buffer : buffers)
        barriers.put(BufferMemoryBarrier(buffer, barrier));
    vkCmdPipelineBarrier(handle, srcStageMask, dstStageMask, dependencyFlags,
        0, nullptr,
        MAGMA_COUNT(barriers), barriers,
        0, nullptr);
}

void CommandBuffer::pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::vector<std::shared_ptr<Image>>& images, const ImageMemoryBarrier& barrier,
    VkDependencyFlags dependencyFlags /* 0 */) noexcept
{
    MAGMA_STACK_ARRAY(VkImageMemoryBarrier, barriers, images.size());
    for (const auto& image : images)
        barriers.put(ImageMemoryBarrier(image, barrier));
    vkCmdPipelineBarrier(handle, srcStageMask, dstStageMask, dependencyFlags,
        0, nullptr,
        0, nullptr,
        MAGMA_COUNT(barriers), barriers);
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

void CommandBuffer::writeTimestamp(VkPipelineStageFlagBits pipelineStage, const std::shared_ptr<QueryPool>& queryPool, uint32_t queryIndex) noexcept
{
    vkCmdWriteTimestamp(handle, pipelineStage, *queryPool, queryIndex);
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

// void CommandBuffer::pushConstants()
// void CommandBuffer::pushConstantBlock()

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

// CommandBuffer::nextSubpass
// CommandBuffer::endRenderPass

void CommandBuffer::executeCommands(const std::vector<std::shared_ptr<CommandBuffer>>& commandBuffers) noexcept
{
    MAGMA_STACK_ARRAY(VkCommandBuffer, dereferencedCommandBuffers, commandBuffers.size());
    for (const auto& commandBuffer : commandBuffers)
        dereferencedCommandBuffers.put(*commandBuffer);
    vkCmdExecuteCommands(handle, dereferencedCommandBuffers.size(), dereferencedCommandBuffers);
}

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
        memcpy(info.color, color, sizeof(float) * 4);
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
        info.color[0] = color[0];
        info.color[1] = color[1];
        info.color[2] = color[2];
        info.color[3] = color[3];
        vkCmdDebugMarkerInsertEXT(handle, &info);
    }
#elif defined(_MSC_VER)
    name;
    color;
#endif // MAGMA_DEBUG
}

std::shared_ptr<Fence> CommandBuffer::getFence() const noexcept
{
    fence->reset();
    return fence;
}
} // namespace magma
