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
#include "pch.h"
#pragma hdrstop
#include "commandBuffer.h"
#include "commandPool.h"
#include "device.h"
#include "deviceMemory.h"
#include "framebuffer.h"
#include "renderPass.h"
#include "image.h"
#include "event.h"
#include "fence.h"
#include "queryPool.h"
#include "accelerationStructure.h"
#include "../barriers/globalMemoryBarrier.h"
#include "../barriers/bufferMemoryBarrier.h"
#include "../barriers/imageMemoryBarrier.h"
#include "../misc/geometry.h"
#include "../misc/deviceExtension.h"

namespace magma
{
CommandBuffer::CommandBuffer(VkCommandBufferLevel level, std::shared_ptr<CommandPool> pool):
    Dispatchable(VK_OBJECT_TYPE_COMMAND_BUFFER, pool->getDevice(), nullptr),
    level(level),
    pool(std::move(pool)),
    fence(std::make_shared<Fence>(device))
{
    VkCommandBufferAllocateInfo info;
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    info.pNext = nullptr;
    info.commandPool = MAGMA_HANDLE(pool);
    info.level = level;
    info.commandBufferCount = 1;
    const VkResult alloc = vkAllocateCommandBuffers(MAGMA_HANDLE(device), &info, &handle);
    MAGMA_THROW_FAILURE(alloc, "failed to allocate primary command buffer");
}

CommandBuffer::CommandBuffer(VkCommandBufferLevel level, VkCommandBuffer handle, std::shared_ptr<CommandPool> pool):
    Dispatchable(VK_OBJECT_TYPE_COMMAND_BUFFER, handle, pool->getDevice(), nullptr),
    level(level),
    pool(std::move(pool)),
    fence(std::make_shared<Fence>(this->device))
{}

CommandBuffer::~CommandBuffer()
{
    if (handle) // Release if not freed through command pool
        vkFreeCommandBuffers(MAGMA_HANDLE(device), MAGMA_HANDLE(pool), 1, &handle);
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
#ifdef MAGMA_DEBUG
    beginMarked = VK_FALSE;
#endif
    return (VK_SUCCESS == begin);
}

bool CommandBuffer::beginDeviceGroup(uint32_t deviceMask,
    VkCommandBufferUsageFlags flags /* 0 */) noexcept
{
    VkDeviceGroupCommandBufferBeginInfo deviceGroupBeginInfo;
    deviceGroupBeginInfo.sType = VK_STRUCTURE_TYPE_DEVICE_GROUP_COMMAND_BUFFER_BEGIN_INFO;
    deviceGroupBeginInfo.pNext = nullptr;
    deviceGroupBeginInfo.deviceMask = deviceMask;
    VkCommandBufferBeginInfo beginInfo;
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.pNext = &deviceGroupBeginInfo;
    beginInfo.flags = flags;
    beginInfo.pInheritanceInfo = nullptr;
    const VkResult begin = vkBeginCommandBuffer(handle, &beginInfo);
    MAGMA_ASSERT(VK_SUCCESS == begin);
#ifdef MAGMA_DEBUG
    beginMarked = VK_FALSE;
#endif
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
    inheritanceInfo.occlusionQueryEnable = occlusionQueryEnable;
    inheritanceInfo.queryFlags = queryFlags;
    inheritanceInfo.pipelineStatistics = pipelineStatistics;
    VkCommandBufferInheritanceConditionalRenderingInfoEXT conditionalRenderingInfo;
    if (conditionalRenderingEnable)
    {   // VK_EXT_conditional_rendering
        conditionalRenderingInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_CONDITIONAL_RENDERING_INFO_EXT;
        conditionalRenderingInfo.pNext = nullptr;
        conditionalRenderingInfo.conditionalRenderingEnable = conditionalRenderingEnable;
        inheritanceInfo.pNext = &conditionalRenderingInfo;
    }
    VkCommandBufferBeginInfo beginInfo;
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.pNext = nullptr;
    beginInfo.flags = flags | VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
    beginInfo.pInheritanceInfo = &inheritanceInfo;
    const VkResult begin = vkBeginCommandBuffer(handle, &beginInfo);
    MAGMA_ASSERT(VK_SUCCESS == begin);
#ifdef MAGMA_DEBUG
    beginMarked = VK_FALSE;
#endif
    return (VK_SUCCESS == begin);
}

void CommandBuffer::end()
{
#ifdef MAGMA_DEBUG
    if (beginMarked)
    {
        endDebugLabel();
        beginMarked = VK_FALSE;
    }
#endif
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

void CommandBuffer::copyBufferToImage(const std::shared_ptr<Buffer>& srcBuffer, const std::shared_ptr<Image>& dstImage, VkImageLayout dstImageLayout, const std::vector<VkBufferImageCopy>& regions) const noexcept
{
    vkCmdCopyBufferToImage(handle, *srcBuffer, *dstImage, dstImageLayout, MAGMA_COUNT(regions), regions.data());
}

void CommandBuffer::copyImageToBuffer(const std::shared_ptr<Image>& srcImage, const std::shared_ptr<Buffer>& dstBuffer, const VkBufferImageCopy& region) const noexcept
{
    vkCmdCopyImageToBuffer(handle, *srcImage, srcImage->getLayout(), *dstBuffer, 1, &region);
}

void CommandBuffer::copyImageToBuffer(const std::shared_ptr<Image>& srcImage, const std::shared_ptr<Buffer>& dstBuffer, const std::vector<VkBufferImageCopy>& regions) const noexcept
{
    vkCmdCopyImageToBuffer(handle, *srcImage, srcImage->getLayout(), *dstBuffer, MAGMA_COUNT(regions), regions.data());
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

void CommandBuffer::pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const ImageMemoryBarrier& imageMemoryBarrier,
    VkDependencyFlags dependencyFlags /* 0 */) noexcept
{
    vkCmdPipelineBarrier(handle, srcStageMask, dstStageMask, dependencyFlags,
        0, nullptr,
        0, nullptr,
        1, &imageMemoryBarrier);
    imageMemoryBarrier.resource->setLayout(imageMemoryBarrier.newLayout);
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

void CommandBuffer::beginRenderPass(const std::shared_ptr<RenderPass>& renderPass, const std::shared_ptr<Framebuffer>& framebuffer,
    const std::vector<ClearValue>& clearValues /* {} */,
    VkSubpassContents contents /* VK_SUBPASS_CONTENTS_INLINE */) noexcept
{
    VkRenderPassBeginInfo beginInfo;
    beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    beginInfo.pNext = nullptr;
    beginInfo.renderPass = *renderPass;
    beginInfo.framebuffer = *framebuffer;
    beginInfo.renderArea = renderAreas.empty() ? VkRect2D{0, 0, 0, 0} : renderAreas.front();
    beginInfo.clearValueCount = MAGMA_COUNT(clearValues);
    MAGMA_STACK_ARRAY(VkClearValue, dereferencedClearValues, beginInfo.clearValueCount);
    for (const auto& clearValue : clearValues)
        dereferencedClearValues.put(clearValue);
    beginInfo.pClearValues = dereferencedClearValues;
    vkCmdBeginRenderPass(handle, &beginInfo, contents);
#ifdef MAGMA_DEBUG
    beginRenderPassMarked = VK_FALSE;
#endif
}

void CommandBuffer::beginRenderPassDeviceGroup(const std::shared_ptr<RenderPass>& renderPass, const std::shared_ptr<Framebuffer>& framebuffer, uint32_t deviceMask,
    const std::vector<ClearValue>& clearValues /* {} */,
    VkSubpassContents contents /* VK_SUBPASS_CONTENTS_INLINE */) noexcept
{
    VkDeviceGroupRenderPassBeginInfo deviceGroupBeginInfo;
    deviceGroupBeginInfo.sType = VK_STRUCTURE_TYPE_DEVICE_GROUP_RENDER_PASS_BEGIN_INFO;
    deviceGroupBeginInfo.pNext = nullptr;
    deviceGroupBeginInfo.deviceMask = deviceMask;
    deviceGroupBeginInfo.deviceRenderAreaCount = MAGMA_COUNT(renderAreas);
    deviceGroupBeginInfo.pDeviceRenderAreas = renderAreas.data();
    VkRenderPassBeginInfo beginInfo;
    beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    beginInfo.pNext = &deviceGroupBeginInfo;
    beginInfo.renderPass = *renderPass;
    beginInfo.framebuffer = *framebuffer;
    beginInfo.renderArea = VkRect2D{0, 0, 0, 0}; // Elements of VkDeviceGroupRenderPassBeginInfo::pDeviceRenderAreas override the value of VkRenderPassBeginInfo::renderArea
    beginInfo.clearValueCount = MAGMA_COUNT(clearValues);
    MAGMA_STACK_ARRAY(VkClearValue, dereferencedClearValues, beginInfo.clearValueCount);
    for (const auto& clearValue : clearValues)
        dereferencedClearValues.put(clearValue);
    beginInfo.pClearValues = dereferencedClearValues;
    vkCmdBeginRenderPass(handle, &beginInfo, contents);
#ifdef MAGMA_DEBUG
    beginRenderPassMarked = VK_FALSE;
#endif
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

void CommandBuffer::setDeviceMask(uint32_t deviceMask) noexcept
{
    MAGMA_OPTIONAL_DEVICE_EXTENSION(vkCmdSetDeviceMaskKHR);
    if (vkCmdSetDeviceMaskKHR)
    {
        MAGMA_ASSERT(deviceMask);
        vkCmdSetDeviceMaskKHR(handle, deviceMask);
        this->deviceMask = deviceMask;
    }
}

void CommandBuffer::dispatchBase(uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ,
    uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const noexcept
{
    MAGMA_OPTIONAL_DEVICE_EXTENSION(vkCmdDispatchBaseKHR);
    if (vkCmdDispatchBaseKHR)
        vkCmdDispatchBaseKHR(handle, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
}

void CommandBuffer::beginConditionalRendering(const std::shared_ptr<Buffer>& buffer,
    VkDeviceSize offset /* 0 */,
    bool inverted /* false */) noexcept
{
    MAGMA_ASSERT(offset <= buffer->getMemory()->getSize() - sizeof(uint32_t));
    MAGMA_ASSERT(offset % sizeof(uint32_t) == 0);
    MAGMA_OPTIONAL_DEVICE_EXTENSION(vkCmdBeginConditionalRenderingEXT);
    if (vkCmdBeginConditionalRenderingEXT)
    {
        VkConditionalRenderingBeginInfoEXT beginInfo;
        beginInfo.sType = VK_STRUCTURE_TYPE_CONDITIONAL_RENDERING_BEGIN_INFO_EXT;
        beginInfo.pNext = nullptr;
        beginInfo.buffer = *buffer;
        beginInfo.offset = offset;
        beginInfo.flags = 0;
        if (inverted)
            beginInfo.flags |= VK_CONDITIONAL_RENDERING_INVERTED_BIT_EXT;
        vkCmdBeginConditionalRenderingEXT(handle, &beginInfo);
    }
}

void CommandBuffer::endConditionalRendering() noexcept
{
    MAGMA_OPTIONAL_DEVICE_EXTENSION(vkCmdEndConditionalRenderingEXT);
    if (vkCmdEndConditionalRenderingEXT)
        vkCmdEndConditionalRenderingEXT(handle);
}

void CommandBuffer::buildAccelerationStructure(const std::shared_ptr<Buffer>& instanceData, VkDeviceSize instanceOffset, bool update,
    const std::shared_ptr<AccelerationStructure>& dst, const std::shared_ptr<AccelerationStructure>& src,
    const std::shared_ptr<Buffer>& scratch, VkDeviceSize scratchOffset /* 0 */) noexcept
{
    MAGMA_OPTIONAL_DEVICE_EXTENSION(vkCmdBuildAccelerationStructureNV);
    if (vkCmdBuildAccelerationStructureNV)
    {
        vkCmdBuildAccelerationStructureNV(handle,
            &dst->getInfo(),
            MAGMA_OPTIONAL_HANDLE(instanceData),
            instanceOffset,
            MAGMA_BOOLEAN(update),
            *dst,
            MAGMA_OPTIONAL_HANDLE(src),
            *scratch, scratchOffset);
    }
}

void CommandBuffer::copyAccelerationStructure(const std::shared_ptr<AccelerationStructure>& dst, const std::shared_ptr<AccelerationStructure>& src, bool clone) const noexcept
{
    MAGMA_OPTIONAL_DEVICE_EXTENSION(vkCmdCopyAccelerationStructureNV);
    if (vkCmdCopyAccelerationStructureNV)
    {
        const VkCopyAccelerationStructureModeNV mode = clone ? VK_COPY_ACCELERATION_STRUCTURE_MODE_CLONE_NV : VK_COPY_ACCELERATION_STRUCTURE_MODE_COMPACT_NV;
        vkCmdCopyAccelerationStructureNV(handle, *dst, *src, mode);
    }
}

void CommandBuffer::writeAccelerationStructureProperties(const std::shared_ptr<AccelerationStructure>& accelerationStructure,
    const std::shared_ptr<AccelerationStructureCompactedSizeQuery>& queryPool, uint32_t firstQuery) noexcept
{
    MAGMA_ASSERT(firstQuery < queryPool->getQueryCount());
    MAGMA_OPTIONAL_DEVICE_EXTENSION(vkCmdWriteAccelerationStructuresPropertiesNV);
    if (vkCmdWriteAccelerationStructuresPropertiesNV)
    {
        const VkAccelerationStructureNV dereferencedAccelerationStructure = *accelerationStructure;
        vkCmdWriteAccelerationStructuresPropertiesNV(handle, 1, &dereferencedAccelerationStructure, queryPool->getType(), *queryPool, firstQuery);
    }
}

void CommandBuffer::traceRays(const std::shared_ptr<Buffer>& raygenShaderBindingTableBuffer, VkDeviceSize raygenShaderBindingOffset,
    const std::shared_ptr<Buffer>& missShaderBindingTableBuffer, VkDeviceSize missShaderBindingOffset, VkDeviceSize missShaderBindingStride,
    const std::shared_ptr<Buffer>& hitShaderBindingTableBuffer, VkDeviceSize hitShaderBindingOffset, VkDeviceSize hitShaderBindingStride,
    const std::shared_ptr<Buffer>& callableShaderBindingTableBuffer, VkDeviceSize callableShaderBindingOffset, VkDeviceSize callableShaderBindingStride,
    uint32_t width, uint32_t height, uint32_t depth) noexcept
{
    MAGMA_ASSERT(width);
    MAGMA_ASSERT(height);
    MAGMA_OPTIONAL_DEVICE_EXTENSION(vkCmdTraceRaysNV);
    if (vkCmdTraceRaysNV)
    {
        vkCmdTraceRaysNV(handle,
            *raygenShaderBindingTableBuffer, raygenShaderBindingOffset,
            *missShaderBindingTableBuffer, missShaderBindingOffset, missShaderBindingStride,
            *hitShaderBindingTableBuffer, hitShaderBindingOffset, hitShaderBindingStride,
            *callableShaderBindingTableBuffer, callableShaderBindingOffset, callableShaderBindingStride,
            width, height, depth);
    }
}

std::shared_ptr<Fence> CommandBuffer::getFence() const noexcept
{
    fence->reset();
    return fence;
}
} // namespace magma
