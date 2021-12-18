/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
#include "framebuffer.h"
#include "imagelessFramebuffer.h"
#include "renderPass.h"
#include "imageView.h"
#include "fence.h"
#include "accelerationStructure.h"
#include "../misc/geometry.h"
#include "../exceptions/errorResult.h"

namespace magma
{
CommandBuffer::CommandBuffer(VkCommandBufferLevel level, std::shared_ptr<CommandPool> pool):
    Dispatchable(VK_OBJECT_TYPE_COMMAND_BUFFER, pool->getDevice(), nullptr),
    level(level),
    pool(std::move(pool)),
    fence(std::make_shared<Fence>(device))
{
    VkCommandBufferAllocateInfo allocInfo;
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.pNext = nullptr;
    allocInfo.commandPool = MAGMA_HANDLE(pool);
    allocInfo.level = level;
    allocInfo.commandBufferCount = 1;
    const VkResult result = vkAllocateCommandBuffers(MAGMA_HANDLE(device), &allocInfo, &handle);
    MAGMA_THROW_FAILURE(result, "failed to allocate primary command buffer");
    maintenance1KHREnable = device->negativeViewportHeightEnabled(true);
    negativeHeightAMDEnable = device->negativeViewportHeightEnabled(false);
}

CommandBuffer::CommandBuffer(VkCommandBufferLevel level, VkCommandBuffer handle, std::shared_ptr<CommandPool> pool):
    Dispatchable(VK_OBJECT_TYPE_COMMAND_BUFFER, handle, pool->getDevice(), nullptr),
    level(level),
    pool(std::move(pool)),
    fence(std::make_shared<Fence>(this->device))
{
    maintenance1KHREnable = device->negativeViewportHeightEnabled(true);
    negativeHeightAMDEnable = device->negativeViewportHeightEnabled(false);
}

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
#ifdef MAGMA_DEBUG_LABEL
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
#ifdef VK_EXT_conditional_rendering
    VkCommandBufferInheritanceConditionalRenderingInfoEXT conditionalRenderingInfo;
    if (conditionalRenderingEnable)
    {
        conditionalRenderingInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_CONDITIONAL_RENDERING_INFO_EXT;
        conditionalRenderingInfo.pNext = nullptr;
        conditionalRenderingInfo.conditionalRenderingEnable = conditionalRenderingEnable;
        inheritanceInfo.pNext = &conditionalRenderingInfo;
    }
#endif // VK_EXT_conditional_rendering
    VkCommandBufferBeginInfo beginInfo;
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.pNext = nullptr;
    beginInfo.flags = flags | VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
    beginInfo.pInheritanceInfo = &inheritanceInfo;
    const VkResult begin = vkBeginCommandBuffer(handle, &beginInfo);
    MAGMA_ASSERT(VK_SUCCESS == begin);
#ifdef MAGMA_DEBUG_LABEL
    beginMarked = VK_FALSE;
#endif
    return (VK_SUCCESS == begin);
}

void CommandBuffer::end()
{
#ifdef MAGMA_DEBUG_LABEL
    if (beginMarked)
    {
        endDebugLabel();
        beginMarked = VK_FALSE;
    }
#endif // MAGMA_DEBUG_LABEL
    /* Performance - critical commands generally do not have return codes.
       If a run time error occurs in such commands, the implementation will defer
       reporting the error until a specified point. For commands that record
       into command buffers (vkCmd*), run time errors are reported by vkEndCommandBuffer. */
    const VkResult result = vkEndCommandBuffer(handle);
    // This is the only place where command buffer may throw an exception.
    MAGMA_THROW_FAILURE(result, "failed to record command buffer");
}

// inline void CommandBuffer::reset
// inline void CommandBuffer::bindPipeline

void CommandBuffer::setViewport(float x, float y, float width, float height,
    float minDepth /* 0 */, float maxDepth /* 1 */) noexcept
{
    VkViewport viewport;
    viewport.x = x;
    viewport.y = y;
    if (height < 0)
    {
        if (maintenance1KHREnable)
            viewport.y = -height - y; // Move origin to bottom left
    }
    viewport.width = width;
    viewport.height = height;
    if (height < 0)
    {
        if (!(maintenance1KHREnable || negativeHeightAMDEnable))
            viewport.height = -height; // Negative viewport height not supported
    }
    viewport.minDepth = minDepth;
    viewport.maxDepth = maxDepth;
    vkCmdSetViewport(handle, 0, 1, &viewport);
}

// inline void CommandBuffer::setScissor
// inline void CommandBuffer::setLineWidth
// inline void CommandBuffer::setDepthBias
// inline void CommandBuffer::setBlendConstants
// inline void CommandBuffer::setDepthBounds
// inline void CommandBuffer::setStencilCompareMask
// inline void CommandBuffer::setStencilWriteMask
// inline void CommandBuffer::setStencilReference
// inline void CommandBuffer::bindDescriptorSet

void CommandBuffer::bindDescriptorSets(const std::shared_ptr<Pipeline>& pipeline, uint32_t firstSet, const std::initializer_list<std::shared_ptr<DescriptorSet>>& descriptorSets,
    const std::initializer_list<uint32_t>& dynamicOffsets /* {} */) noexcept
{
    MAGMA_ASSERT_FOR_EACH(descriptorSets, descriptorSet, pipeline->getLayout()->hasSetLayout(descriptorSet->getLayout()));
    MAGMA_STACK_ARRAY(VkDescriptorSet, dereferencedDescriptorSets, descriptorSets.size());
    uint32_t dirtyCount = 0;
    for (const auto& descriptorSet : descriptorSets)
    {
        dereferencedDescriptorSets.put(*descriptorSet);
        dirtyCount += descriptorSet->getDirtyCount();
    }
    if (dirtyCount > 0)
    {
        std::vector<VkWriteDescriptorSet> descriptorWrites;
        descriptorWrites.reserve(dirtyCount);
        for (const auto& descriptorSet : descriptorSets)
        {
            if (descriptorSet->dirty())
                descriptorSet->populateDescriptorWrites(descriptorWrites);
        }
        vkUpdateDescriptorSets(MAGMA_HANDLE(device), MAGMA_COUNT(descriptorWrites), descriptorWrites.data(), 0, nullptr);
    }
    vkCmdBindDescriptorSets(handle, pipeline->getBindPoint(), *pipeline->getLayout(), firstSet, dereferencedDescriptorSets.size(), dereferencedDescriptorSets, MAGMA_COUNT(dynamicOffsets), dynamicOffsets.begin());
}

// inline void CommandBuffer::bindIndexBuffer
// inline void CommandBuffer::bindVertexBuffer
// inline void CommandBuffer::bindVertexBuffers

#ifdef VK_EXT_transform_feedback
void CommandBuffer::bindTransformFeedbackBuffers(uint32_t firstBinding, const std::vector<std::shared_ptr<TransformFeedbackBuffer>>& transformFeedbackBuffers,
    std::vector<VkDeviceSize> offsets /* empty */, std::vector<VkDeviceSize> sizes /* empty */)
{
    MAGMA_ASSERT(transformFeedbackBuffers.size() > 0);
    if (!offsets.empty())
        MAGMA_ASSERT(offsets.size() >= transformFeedbackBuffers.size());
    if (!sizes.empty())
        MAGMA_ASSERT(sizes.size() >= transformFeedbackBuffers.size());
    MAGMA_OPTIONAL_DEVICE_EXTENSION(vkCmdBindTransformFeedbackBuffersEXT);
    if (vkCmdBindTransformFeedbackBuffersEXT)
    {
        MAGMA_STACK_ARRAY(VkBuffer, dereferencedBuffers, transformFeedbackBuffers.size());
        for (const auto& buffer : transformFeedbackBuffers)
            dereferencedBuffers.put(*buffer);
        if (offsets.empty())
            offsets.resize(transformFeedbackBuffers.size(), 0);
        vkCmdBindTransformFeedbackBuffersEXT(handle, firstBinding, dereferencedBuffers.size(), dereferencedBuffers, offsets.data(), sizes.data());
    }
}
#endif // VK_EXT_transform_feedback

// inline void CommandBuffer::draw
// inline void CommandBuffer::drawInstanced
// inline void CommandBuffer::drawIndexed
// inline void CommandBuffer::drawIndexedInstanced
// inline void CommandBuffer::drawIndirect
// inline void CommandBuffer::drawIndexedIndirect
// inline void CommandBuffer::drawMulti
// inline void CommandBuffer::drawMultiInstanced
// inline void CommandBuffer::drawMultiIndexed
// inline void CommandBuffer::drawMultiIndexedInstanced
// inline void CommandBuffer::drawIndirectByteCount
// inline void CommandBuffer::dispatch
// inline void CommandBuffer::dispatchIndirect

void CommandBuffer::copyBuffer(const std::shared_ptr<const Buffer>& srcBuffer, const std::shared_ptr<Buffer>& dstBuffer,
    VkDeviceSize srcOffset /* 0 */, VkDeviceSize dstOffset /* 0 */, VkDeviceSize size /* VK_WHOLE_SIZE */) const noexcept
{
    VkBufferCopy region;
    region.srcOffset = srcOffset;
    region.dstOffset = dstOffset;
    if (VK_WHOLE_SIZE == size)
        region.size = dstBuffer->getSize();
    else
        region.size = size;
    vkCmdCopyBuffer(handle, *srcBuffer, *dstBuffer, 1, &region);
}

void CommandBuffer::copyImage(const std::shared_ptr<const Image>& srcImage, const std::shared_ptr<Image>& dstImage,
    uint32_t mipLevel /* 0 */, const VkOffset3D& srcOffset /* 0, 0, 0 */, const VkOffset3D& dstOffset /* 0, 0, 0 */) const noexcept
{
    VkImageCopy region;
    region.srcSubresource = srcImage->getSubresourceLayers(mipLevel);
    region.srcOffset = srcOffset;
    region.dstSubresource = dstImage->getSubresourceLayers(mipLevel);
    region.dstOffset = dstOffset;
    region.extent = dstImage->getMipExtent(mipLevel);
    vkCmdCopyImage(handle, *srcImage, srcImage->getLayout(), *dstImage, dstImage->getLayout(), 1, &region);
}

void CommandBuffer::blitImage(const std::shared_ptr<const Image>& srcImage, const std::shared_ptr<Image>& dstImage, VkFilter filter,
    uint32_t mipLevel /* 0 */, const VkOffset3D& srcOffset /* 0, 0, 0 */, const VkOffset3D& dstOffset /* 0, 0, 0 */) const noexcept
{
    const VkExtent3D srcExtent = srcImage->getMipExtent(mipLevel);
    const VkExtent3D dstExtent = dstImage->getMipExtent(mipLevel);
    VkImageBlit region;
    region.srcSubresource = srcImage->getSubresourceLayers(mipLevel);
    region.srcOffsets[0] = srcOffset;
    region.srcOffsets[1] = VkOffset3D{
        static_cast<int32_t>(srcExtent.width),
        static_cast<int32_t>(srcExtent.height),
        1};
    region.dstSubresource = dstImage->getSubresourceLayers(mipLevel);
    region.dstOffsets[0] = dstOffset;
    region.dstOffsets[1] = VkOffset3D{
        static_cast<int32_t>(dstExtent.width),
        static_cast<int32_t>(dstExtent.height),
        1};
    vkCmdBlitImage(handle, *srcImage, srcImage->getLayout(), *dstImage, dstImage->getLayout(), 1, &region, filter);
}

// inline void CommandBuffer::copyImage
// inline void CommandBuffer::blitImage
// inline void CommandBuffer::copyBufferToImage
// inline void CommandBuffer::copyImageToBuffer
// inline void CommandBuffer::updateBuffer

void CommandBuffer::fillBuffer(const std::shared_ptr<Buffer>& buffer, uint32_t value,
    VkDeviceSize size /* 0 */,
    VkDeviceSize offset /* 0 */) const noexcept
{
    if (0 == size)
        size = buffer->getSize();
    vkCmdFillBuffer(handle, *buffer, offset, size, value);
}

// inline void CommandBuffer::clearColorImage
// inline void CommandBuffer::clearDepthStencilImage
// inline void CommandBuffer::clearAttachments
// inline void CommandBuffer::resolveImage
// inline void CommandBuffer::setEvent
// inline void CommandBuffer::resetEvent

void CommandBuffer::waitEvent(const std::shared_ptr<Event>& event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
    const std::vector<MemoryBarrier>& memoryBarriers /* {} */,
    const std::vector<BufferMemoryBarrier>& bufferMemoryBarriers /* {} */,
    const std::vector<ImageMemoryBarrier>& imageMemoryBarriers /* {} */) noexcept
{
    MAGMA_ASSERT(srcStageMask);
    MAGMA_ASSERT(dstStageMask);
    const VkEvent dereferencedEvents[1] = {*event};
    MAGMA_STACK_ARRAY(VkImageMemoryBarrier, dereferencedImageMemoryBarriers, imageMemoryBarriers.size());
    for (const auto& barrier : imageMemoryBarriers)
        dereferencedImageMemoryBarriers.put(barrier);
    vkCmdWaitEvents(handle, 1, dereferencedEvents, srcStageMask, dstStageMask,
        MAGMA_COUNT(memoryBarriers),
        memoryBarriers.data(),
        MAGMA_COUNT(bufferMemoryBarriers),
        bufferMemoryBarriers.data(),
        MAGMA_COUNT(imageMemoryBarriers),
        dereferencedImageMemoryBarriers);
}

void CommandBuffer::waitEvents(const std::vector<std::shared_ptr<Event>>& events, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
    const std::vector<MemoryBarrier>& memoryBarriers /* {} */,
    const std::vector<BufferMemoryBarrier>& bufferMemoryBarriers /* {} */,
    const std::vector<ImageMemoryBarrier>& imageMemoryBarriers /* {} */) noexcept
{
    MAGMA_ASSERT(srcStageMask);
    MAGMA_ASSERT(dstStageMask);
    MAGMA_STACK_ARRAY(VkEvent, dereferencedEvents, events.size());
    for (const auto& event : events)
        dereferencedEvents.put(*event);
    MAGMA_STACK_ARRAY(VkImageMemoryBarrier, dereferencedImageMemoryBarriers, imageMemoryBarriers.size());
    for (const auto& barrier : imageMemoryBarriers)
        dereferencedImageMemoryBarriers.put(barrier);
    vkCmdWaitEvents(handle, dereferencedEvents.size(), dereferencedEvents, srcStageMask, dstStageMask,
        MAGMA_COUNT(memoryBarriers),
        memoryBarriers.data(),
        MAGMA_COUNT(bufferMemoryBarriers),
        bufferMemoryBarriers.data(),
        MAGMA_COUNT(imageMemoryBarriers),
        dereferencedImageMemoryBarriers);
}

// inline void CommandBuffer::pipelineBarrier
// inline void CommandBuffer::pipelineBarrier
// inline void CommandBuffer::pipelineBarrier

void CommandBuffer::pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
    const std::vector<MemoryBarrier>& memoryBarriers /* {} */,
    const std::vector<BufferMemoryBarrier>& bufferMemoryBarriers /* {} */,
    const std::vector<ImageMemoryBarrier>& imageMemoryBarriers /* {} */,
    VkDependencyFlags dependencyFlags /* 0 */) noexcept
{
    MAGMA_STACK_ARRAY(VkImageMemoryBarrier, dereferencedImageMemoryBarriers, imageMemoryBarriers.size());
    for (const auto& barrier : imageMemoryBarriers)
        dereferencedImageMemoryBarriers.put(barrier);
    vkCmdPipelineBarrier(handle, srcStageMask, dstStageMask, dependencyFlags,
        MAGMA_COUNT(memoryBarriers), memoryBarriers.data(),
        MAGMA_COUNT(bufferMemoryBarriers), bufferMemoryBarriers.data(),
        MAGMA_COUNT(imageMemoryBarriers), dereferencedImageMemoryBarriers);
    for (const auto& barrier : imageMemoryBarriers)
        barrier.resource->setLayout(barrier.newLayout);
}

// inline void CommandBuffer::beginQuery
// inline void CommandBuffer::endQuery
// inline void CommandBuffer::beginQueryIndexed
// inline void CommandBuffer::endQueryIndexed
// inline void CommandBuffer::resetQueryPool
// inline void CommandBuffer::writeTimestamp

void CommandBuffer::copyQueryResults(const std::shared_ptr<QueryPool>& queryPool, const std::shared_ptr<Buffer>& buffer, bool wait,
    uint32_t firstQuery /* 0 */, uint32_t queryCount /* std::numeric_limits<uint32_t>::max() */,
    VkDeviceSize dstOffset /* 0 */, bool write64Bit /* true */) noexcept
{
    if (std::numeric_limits<uint32_t>::max() == queryCount)
        queryCount = queryPool->getQueryCount();
    MAGMA_ASSERT(firstQuery + queryCount <= queryPool->getQueryCount());
    MAGMA_ASSERT(dstOffset < buffer->getSize());
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
    const VkRect2D& renderArea /* {0, 0, 0, 0} */,
    VkSubpassContents contents /* VK_SUBPASS_CONTENTS_INLINE */) noexcept
{
    if (clearValues.empty()) {
        MAGMA_ASSERT(!renderPass->hasClearOp());
    }
    VkRenderPassBeginInfo beginInfo;
    beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    beginInfo.pNext = nullptr;
    beginInfo.renderPass = *renderPass;
    beginInfo.framebuffer = *framebuffer;
    beginInfo.renderArea.offset = renderArea.offset;
    if ((renderArea.extent.width != 0) || (renderArea.extent.height != 0))
        beginInfo.renderArea.extent = renderArea.extent;
    else
        beginInfo.renderArea.extent = framebuffer->getExtent();
    beginInfo.clearValueCount = MAGMA_COUNT(clearValues);
    beginInfo.pClearValues = reinterpret_cast<const VkClearValue *>(clearValues.data());
    vkCmdBeginRenderPass(handle, &beginInfo, contents);
#ifdef MAGMA_DEBUG_LABEL
    beginRenderPassMarked = VK_FALSE;
#endif
}

#ifdef VK_KHR_imageless_framebuffer
void CommandBuffer::beginRenderPass(const std::shared_ptr<RenderPass>& renderPass,
    const std::shared_ptr<ImagelessFramebuffer>& framebuffer,
    const std::vector<std::shared_ptr<ImageView>>& attachments,
    const std::vector<ClearValue>& clearValues /* {} */,
    const VkRect2D& renderArea /* {0, 0, 0, 0} */,
    VkSubpassContents contents /* VK_SUBPASS_CONTENTS_INLINE */) noexcept
{
    if (clearValues.empty()) {
        MAGMA_ASSERT(!renderPass->hasClearOp());
    }
    MAGMA_STACK_ARRAY(VkImageView, dereferencedAttachments, attachments.size());
    for (const auto& attachment : attachments)
        dereferencedAttachments.put(*attachment);
    VkRenderPassAttachmentBeginInfoKHR attachmentBeginInfo;
    attachmentBeginInfo.sType =  VK_STRUCTURE_TYPE_RENDER_PASS_ATTACHMENT_BEGIN_INFO_KHR;
    attachmentBeginInfo.pNext = nullptr;
    attachmentBeginInfo.attachmentCount = MAGMA_COUNT(dereferencedAttachments);
    attachmentBeginInfo.pAttachments = dereferencedAttachments;
    VkRenderPassBeginInfo beginInfo;
    beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    beginInfo.pNext = &attachmentBeginInfo;
    beginInfo.renderPass = *renderPass;
    beginInfo.framebuffer = *framebuffer;
    beginInfo.renderArea.offset = renderArea.offset;
    if ((renderArea.extent.width != 0) || (renderArea.extent.height != 0))
        beginInfo.renderArea.extent = renderArea.extent;
    else
        beginInfo.renderArea.extent = framebuffer->getExtent();
    beginInfo.clearValueCount = MAGMA_COUNT(clearValues);
    beginInfo.pClearValues = reinterpret_cast<const VkClearValue *>(clearValues.data());
    vkCmdBeginRenderPass(handle, &beginInfo, contents);
#ifdef MAGMA_DEBUG_LABEL
    beginRenderPassMarked = VK_FALSE;
#endif
}
#endif // VK_KHR_imageless_framebuffer

// CommandBuffer::nextSubpass
// CommandBuffer::endRenderPass

#ifdef VK_KHR_device_group
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

void CommandBuffer::beginDeviceGroupRenderPass(uint32_t deviceMask,
    const std::shared_ptr<RenderPass>& renderPass, const std::shared_ptr<Framebuffer>& framebuffer,
    const std::vector<VkRect2D>& deviceRenderAreas /* {} */, const std::vector<ClearValue>& clearValues /* {} */,
    VkSubpassContents contents /* VK_SUBPASS_CONTENTS_INLINE */) noexcept
{
    if (clearValues.empty()) {
        MAGMA_ASSERT(!renderPass->hasClearOp());
    }
    VkDeviceGroupRenderPassBeginInfo deviceGroupBeginInfo;
    deviceGroupBeginInfo.sType = VK_STRUCTURE_TYPE_DEVICE_GROUP_RENDER_PASS_BEGIN_INFO;
    deviceGroupBeginInfo.pNext = nullptr;
    deviceGroupBeginInfo.deviceMask = deviceMask;
    // Elements of VkDeviceGroupRenderPassBeginInfo::pDeviceRenderAreas override the value of VkRenderPassBeginInfo::renderArea
    deviceGroupBeginInfo.deviceRenderAreaCount = MAGMA_COUNT(deviceRenderAreas);
    deviceGroupBeginInfo.pDeviceRenderAreas = deviceRenderAreas.data();
    VkRenderPassBeginInfo beginInfo;
    beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    beginInfo.pNext = &deviceGroupBeginInfo;
    beginInfo.renderPass = *renderPass;
    beginInfo.framebuffer = *framebuffer;
    beginInfo.renderArea.offset = VkOffset2D{0, 0};
    beginInfo.renderArea.extent = deviceRenderAreas.empty() ? framebuffer->getExtent() : VkExtent2D{0, 0};
    beginInfo.clearValueCount = MAGMA_COUNT(clearValues);
    beginInfo.pClearValues = reinterpret_cast<const VkClearValue *>(clearValues.data());
    vkCmdBeginRenderPass(handle, &beginInfo, contents);
#ifdef MAGMA_DEBUG
    beginRenderPassMarked = VK_FALSE;
#endif
}

#ifdef VK_KHR_imageless_framebuffer
void CommandBuffer::beginDeviceGroupRenderPass(uint32_t deviceMask,
    const std::shared_ptr<RenderPass>& renderPass,
    const std::shared_ptr<ImagelessFramebuffer>& framebuffer,
    const std::vector<std::shared_ptr<ImageView>>& attachments,
    const std::vector<VkRect2D>& deviceRenderAreas /* {} */,
    const std::vector<ClearValue>& clearValues /* {} */,
    VkSubpassContents contents /* VK_SUBPASS_CONTENTS_INLINE */) noexcept
{
    if (clearValues.empty()) {
        MAGMA_ASSERT(!renderPass->hasClearOp());
    }
    VkDeviceGroupRenderPassBeginInfo deviceGroupBeginInfo;
    deviceGroupBeginInfo.sType = VK_STRUCTURE_TYPE_DEVICE_GROUP_RENDER_PASS_BEGIN_INFO;
    deviceGroupBeginInfo.pNext = nullptr;
    deviceGroupBeginInfo.deviceMask = deviceMask;
    // Elements of VkDeviceGroupRenderPassBeginInfo::pDeviceRenderAreas override the value of VkRenderPassBeginInfo::renderArea
    deviceGroupBeginInfo.deviceRenderAreaCount = MAGMA_COUNT(deviceRenderAreas);
    deviceGroupBeginInfo.pDeviceRenderAreas = deviceRenderAreas.data();
    MAGMA_STACK_ARRAY(VkImageView, dereferencedAttachments, attachments.size());
    for (const auto& attachment : attachments)
        dereferencedAttachments.put(*attachment);
    VkRenderPassAttachmentBeginInfoKHR attachmentBeginInfo;
    attachmentBeginInfo.sType =  VK_STRUCTURE_TYPE_RENDER_PASS_ATTACHMENT_BEGIN_INFO_KHR;
    attachmentBeginInfo.pNext = &deviceGroupBeginInfo;
    attachmentBeginInfo.attachmentCount = MAGMA_COUNT(dereferencedAttachments);
    attachmentBeginInfo.pAttachments = dereferencedAttachments;
    VkRenderPassBeginInfo beginInfo;
    beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    beginInfo.pNext = &attachmentBeginInfo;
    beginInfo.renderPass = *renderPass;
    beginInfo.framebuffer = *framebuffer;
    beginInfo.renderArea.offset = VkOffset2D{0, 0};
    beginInfo.renderArea.extent = deviceRenderAreas.empty() ? framebuffer->getExtent() : VkExtent2D{0, 0};
    beginInfo.clearValueCount = MAGMA_COUNT(clearValues);
    beginInfo.pClearValues = reinterpret_cast<const VkClearValue *>(clearValues.data());
    vkCmdBeginRenderPass(handle, &beginInfo, contents);
#ifdef MAGMA_DEBUG
    beginRenderPassMarked = VK_FALSE;
#endif
}
#endif // VK_KHR_imageless_framebuffer

void CommandBuffer::dispatchBase(uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ,
    uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const noexcept
{
    MAGMA_OPTIONAL_DEVICE_EXTENSION(vkCmdDispatchBaseKHR);
    if (vkCmdDispatchBaseKHR)
        vkCmdDispatchBaseKHR(handle, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
}
#endif // VK_KHR_device_group

#ifdef VK_EXT_conditional_rendering
void CommandBuffer::beginConditionalRendering(const std::shared_ptr<Buffer>& buffer,
    VkDeviceSize offset /* 0 */,
    bool inverted /* false */) noexcept
{
    MAGMA_ASSERT(offset <= buffer->getSize() - sizeof(uint32_t));
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
#endif // VK_EXT_conditional_rendering

#ifdef VK_EXT_transform_feedback
void CommandBuffer::beginTransformFeedback(uint32_t firstCounterBuffer, const std::vector<std::shared_ptr<TransformFeedbackCounterBuffer>>& counterBuffers, const std::vector<VkDeviceSize>& counterBufferOffsets) noexcept
{
    MAGMA_OPTIONAL_DEVICE_EXTENSION(vkCmdBeginTransformFeedbackEXT);
    if (vkCmdBeginTransformFeedbackEXT)
    {
        MAGMA_STACK_ARRAY(VkBuffer, dereferencedCounterBuffers, counterBuffers.size());
        for (const auto& buffer : counterBuffers)
            dereferencedCounterBuffers.put(*buffer);
        vkCmdBeginTransformFeedbackEXT(handle, firstCounterBuffer, MAGMA_COUNT(counterBuffers), dereferencedCounterBuffers, counterBufferOffsets.data());
    }
}

void CommandBuffer::endTransformFeedback(uint32_t firstCounterBuffer, const std::vector<std::shared_ptr<TransformFeedbackCounterBuffer>>& counterBuffers, const std::vector<VkDeviceSize>& counterBufferOffsets) noexcept
{
    MAGMA_OPTIONAL_DEVICE_EXTENSION(vkCmdEndTransformFeedbackEXT);
    if (vkCmdEndTransformFeedbackEXT)
    {
        MAGMA_STACK_ARRAY(VkBuffer, dereferencedCounterBuffers, counterBuffers.size());
        for (const auto& buffer : counterBuffers)
            dereferencedCounterBuffers.put(*buffer);
        vkCmdEndTransformFeedbackEXT(handle, firstCounterBuffer, MAGMA_COUNT(counterBuffers), dereferencedCounterBuffers, counterBufferOffsets.data());
    }
}
#endif // VK_EXT_transform_feedback

#ifdef VK_NV_ray_tracing
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
    MAGMA_ASSERT(raygenShaderBindingTableBuffer);
    MAGMA_ASSERT(width);
    MAGMA_ASSERT(height);
    MAGMA_ASSERT(depth);
    MAGMA_OPTIONAL_DEVICE_EXTENSION(vkCmdTraceRaysNV);
    if (vkCmdTraceRaysNV)
    {
        vkCmdTraceRaysNV(handle,
            *raygenShaderBindingTableBuffer, raygenShaderBindingOffset,
            MAGMA_OPTIONAL_HANDLE(missShaderBindingTableBuffer), missShaderBindingOffset, missShaderBindingStride,
            MAGMA_OPTIONAL_HANDLE(hitShaderBindingTableBuffer), hitShaderBindingOffset, hitShaderBindingStride,
            MAGMA_OPTIONAL_HANDLE(callableShaderBindingTableBuffer), callableShaderBindingOffset, callableShaderBindingStride,
            width, height, depth);
    }
}
#endif // VK_NV_ray_tracing

std::shared_ptr<Fence> CommandBuffer::getFence() const noexcept
{
    fence->reset();
    return fence;
}

void PrimaryCommandBuffer::executeCommands(const std::vector<std::shared_ptr<CommandBuffer>>& cmdBuffers) noexcept
{
    MAGMA_STACK_ARRAY(VkCommandBuffer, dereferencedCmdBuffers, cmdBuffers.size());
    for (const auto& cmdBuffer : cmdBuffers)
    {
        MAGMA_ASSERT(cmdBuffer->secondary());
        dereferencedCmdBuffers.put(*cmdBuffer);
    }
    vkCmdExecuteCommands(handle, dereferencedCmdBuffers.size(), dereferencedCmdBuffers);
}
} // namespace magma
