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
#include "commandPool.h"
#include "framebuffer.h"
#include "imagelessFramebuffer.h"
#include "renderPass.h"
#include "imageView.h"
#include "fence.h"
#include "accelerationStructure.h"
#include "../misc/deviceFeatures.h"
#include "../misc/geometry.h"
#include "../exceptions/errorResult.h"

namespace magma
{
CommandBuffer::CommandBuffer(VkCommandBufferLevel level, VkCommandBuffer handle, std::shared_ptr<CommandPool> cmdPool_):
    Dispatchable(VK_OBJECT_TYPE_COMMAND_BUFFER, handle, cmdPool_->getDevice(), nullptr),
    cmdPool(std::move(cmdPool_)),
    fence(std::make_shared<Fence>(device)),
    level(level),
    usageFlags(0),
    state(State::Initial),
    occlusionQueryEnable(VK_FALSE),
    conditionalRenderingEnable(VK_FALSE),
    negativeViewportHeightEnabled(device->getDeviceFeatures()->negativeViewportHeightEnabled()),
    withinRenderPass(VK_FALSE),
    withinConditionalRendering(VK_FALSE),
    withinTransformFeedback(VK_FALSE),
    queryFlags(0),
    pipelineStatistics(0)
{}

CommandBuffer::CommandBuffer(VkCommandBufferLevel level, std::shared_ptr<CommandPool> cmdPool_):
    Dispatchable(VK_OBJECT_TYPE_COMMAND_BUFFER, cmdPool_->getDevice(), nullptr),
    cmdPool(std::move(cmdPool_)),
    fence(std::make_shared<Fence>(device)),
    level(level),
    usageFlags(0),
    state(State::Initial),
    occlusionQueryEnable(VK_FALSE),
    conditionalRenderingEnable(VK_FALSE),
    negativeViewportHeightEnabled(device->getDeviceFeatures()->negativeViewportHeightEnabled()),
    withinRenderPass(VK_FALSE),
    withinConditionalRendering(VK_FALSE),
    withinTransformFeedback(VK_FALSE),
    queryFlags(0),
    pipelineStatistics(0)
{
    VkCommandBufferAllocateInfo cmdBufferAllocateInfo;
    cmdBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmdBufferAllocateInfo.pNext = nullptr;
    cmdBufferAllocateInfo.commandPool = MAGMA_HANDLE(cmdPool);
    cmdBufferAllocateInfo.level = level;
    cmdBufferAllocateInfo.commandBufferCount = 1;
    const VkResult result = vkAllocateCommandBuffers(MAGMA_HANDLE(device), &cmdBufferAllocateInfo, &handle);
    MAGMA_HANDLE_RESULT(result, VK_COMMAND_BUFFER_LEVEL_PRIMARY == level ?
        "failed to allocate primary command buffer" : "failed to allocate secondary command buffer");
}

CommandBuffer::~CommandBuffer()
{   // Release if not freed through command pool
    if (handle)
        vkFreeCommandBuffers(MAGMA_HANDLE(device), MAGMA_HANDLE(cmdPool), 1, &handle);
}

bool CommandBuffer::begin(VkCommandBufferUsageFlags flags /* 0 */) noexcept
{
    VkCommandBufferBeginInfo cmdBufferBeginInfo;
    cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmdBufferBeginInfo.pNext = nullptr;
    cmdBufferBeginInfo.flags = flags;
    cmdBufferBeginInfo.pInheritanceInfo = nullptr;
    const VkResult result = vkBeginCommandBuffer(handle, &cmdBufferBeginInfo);
    MAGMA_ASSERT(VK_SUCCESS == result);
    if (VK_SUCCESS == result)
        state = State::Recording;
    usageFlags = flags;
    return (VK_SUCCESS == result);
}

bool CommandBuffer::beginInherited(const std::shared_ptr<RenderPass>& renderPass, uint32_t subpass, const std::shared_ptr<Framebuffer>& framebuffer,
    VkCommandBufferUsageFlags flags /* 0 */) noexcept
{
    VkCommandBufferBeginInfo cmdBufferBeginInfo;
    VkCommandBufferInheritanceInfo cmdBufferInheritanceInfo;
    cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmdBufferBeginInfo.pNext = nullptr;
    cmdBufferBeginInfo.flags = flags | VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
    cmdBufferBeginInfo.pInheritanceInfo = &cmdBufferInheritanceInfo;
    cmdBufferInheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
    cmdBufferInheritanceInfo.pNext = nullptr;
    cmdBufferInheritanceInfo.renderPass = *renderPass;
    cmdBufferInheritanceInfo.subpass = subpass;
    cmdBufferInheritanceInfo.framebuffer = *framebuffer;
    cmdBufferInheritanceInfo.occlusionQueryEnable = occlusionQueryEnable;
    cmdBufferInheritanceInfo.queryFlags = queryFlags;
    cmdBufferInheritanceInfo.pipelineStatistics = pipelineStatistics;
#ifdef VK_EXT_conditional_rendering
    VkCommandBufferInheritanceConditionalRenderingInfoEXT cmdBufferInheritanceConditionalRenderingInfo;
    if (device->extensionEnabled(VK_EXT_CONDITIONAL_RENDERING_EXTENSION_NAME))
    {
        cmdBufferInheritanceInfo.pNext = &cmdBufferInheritanceConditionalRenderingInfo;
        cmdBufferInheritanceConditionalRenderingInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_CONDITIONAL_RENDERING_INFO_EXT;
        cmdBufferInheritanceConditionalRenderingInfo.pNext = nullptr;
        cmdBufferInheritanceConditionalRenderingInfo.conditionalRenderingEnable = conditionalRenderingEnable;
    }
#endif // VK_EXT_conditional_rendering
    const VkResult result = vkBeginCommandBuffer(handle, &cmdBufferBeginInfo);
    MAGMA_ASSERT(VK_SUCCESS == result);
    if (VK_SUCCESS == result)
        state = State::Recording;
    usageFlags = flags;
    return (VK_SUCCESS == result);
}

void CommandBuffer::end()
{
    MAGMA_ASSERT(State::Recording == state);
    if (State::Recording == state)
    {
    #ifdef MAGMA_DEBUG_LABEL
        endDebugLabel();
    #endif // MAGMA_DEBUG_LABEL
        /* Performance - critical commands generally do not have return codes.
           If a run time error occurs in such commands, the implementation will defer
           reporting the error until a specified point. For commands that record
           into command buffers (vkCmd*), run time errors are reported by vkEndCommandBuffer. */
        const VkResult result = vkEndCommandBuffer(handle);
        // This is the only place where command buffer may throw an exception.
        MAGMA_HANDLE_RESULT(result, "failed to record command buffer");
        state = State::Executable;
    }
}

bool CommandBuffer::reset(bool releaseResources /* false */) noexcept
{   // The command buffer can be in any state other than pending
    MAGMA_ASSERT(state != State::Pending);
    VkCommandBufferResetFlags flags = 0;
    if (releaseResources)
        flags |= VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT;
    const VkResult result = vkResetCommandBuffer(handle, flags);
    MAGMA_ASSERT(VK_SUCCESS == result);
    if (VK_SUCCESS == result)
    {
        state = State::Initial;
        withinRenderPass = VK_FALSE;
        withinConditionalRendering = VK_FALSE;
        withinTransformFeedback = VK_FALSE;
        return true;
    }
    return false;
}

// inline void CommandBuffer::bindPipeline

void CommandBuffer::setViewport(float x, float y, float width, float height,
    float minDepth /* 0 */, float maxDepth /* 1 */) noexcept
{
    VkViewport viewport;
    viewport.x = x;
    viewport.y = y;
    if (height < 0)
    {
        if (device->getDeviceFeatures()->maintenanceEnabled(1))
            viewport.y = -height - y; // Move origin to bottom left
    }
    viewport.width = width;
    viewport.height = height;
    if (height < 0)
    {
        if (!negativeViewportHeightEnabled)
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
    MAGMA_ASSERT_FOR_EACH(descriptorSets, descriptorSet, pipeline->getLayout()->hasLayout(descriptorSet->getLayout()));
    MAGMA_ASSERT_FOR_EACH(descriptorSets, descriptorSet, !descriptorSet->dirty());
    MAGMA_STACK_ARRAY(VkDescriptorSet, dereferencedDescriptorSets, descriptorSets.size());
    for (const auto& descriptorSet: descriptorSets)
        dereferencedDescriptorSets.put(*descriptorSet);
    vkCmdBindDescriptorSets(handle, pipeline->getBindPoint(), *pipeline->getLayout(), firstSet, dereferencedDescriptorSets.size(), dereferencedDescriptorSets, MAGMA_COUNT(dynamicOffsets), dynamicOffsets.begin());
}

// inline void CommandBuffer::bindIndexBuffer
// inline void CommandBuffer::bindVertexBuffer
// inline void CommandBuffer::bindVertexBuffers

#ifdef VK_EXT_transform_feedback
void CommandBuffer::bindTransformFeedbackBuffer(uint32_t firstBinding, const std::shared_ptr<TransformFeedbackBuffer>& transformFeedbackBuffer,
    VkDeviceSize offset /* 0 */, VkDeviceSize size /* VK_WHOLE_SIZE */)
{
    MAGMA_DEVICE_EXTENSION(vkCmdBindTransformFeedbackBuffersEXT);
    if (vkCmdBindTransformFeedbackBuffersEXT)
        vkCmdBindTransformFeedbackBuffersEXT(handle, firstBinding, 1, transformFeedbackBuffer->getHandleAddress(), &offset, &size);
}

void CommandBuffer::bindTransformFeedbackBuffers(uint32_t firstBinding, const std::initializer_list<std::shared_ptr<TransformFeedbackBuffer>>& transformFeedbackBuffers,
    std::vector<VkDeviceSize> offsets /* empty */, const std::initializer_list<VkDeviceSize>& sizes /* empty */)
{
    MAGMA_ASSERT(transformFeedbackBuffers.size() > 0);
    if (offsets.size() > 0) {
        MAGMA_ASSERT(offsets.size() >= transformFeedbackBuffers.size());
    }
    if (sizes.size() > 0) {
        MAGMA_ASSERT(sizes.size() >= transformFeedbackBuffers.size());
    }
    MAGMA_DEVICE_EXTENSION(vkCmdBindTransformFeedbackBuffersEXT);
    if (vkCmdBindTransformFeedbackBuffersEXT)
    {
        MAGMA_STACK_ARRAY(VkBuffer, dereferencedBuffers, transformFeedbackBuffers.size());
        for (const auto& buffer : transformFeedbackBuffers)
            dereferencedBuffers.put(*buffer);
        if (offsets.empty())
            offsets.resize(transformFeedbackBuffers.size(), 0);
        vkCmdBindTransformFeedbackBuffersEXT(handle, firstBinding, dereferencedBuffers.size(), dereferencedBuffers, offsets.data(), sizes.begin());
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
    VkBufferCopy bufferCopy;
    bufferCopy.srcOffset = srcOffset;
    bufferCopy.dstOffset = dstOffset;
    bufferCopy.size = (VK_WHOLE_SIZE == size) ? dstBuffer->getSize() : size;
    vkCmdCopyBuffer(handle, *srcBuffer, *dstBuffer, 1, &bufferCopy);
}

void CommandBuffer::copyImage(const std::shared_ptr<const Image>& srcImage, const std::shared_ptr<Image>& dstImage,
    uint32_t mipLevel /* 0 */, const VkOffset3D& srcOffset /* 0, 0, 0 */, const VkOffset3D& dstOffset /* 0, 0, 0 */) const noexcept
{
    VkImageCopy imageCopy;
    imageCopy.srcSubresource = srcImage->getSubresourceLayers(mipLevel);
    imageCopy.srcOffset = srcOffset;
    imageCopy.dstSubresource = dstImage->getSubresourceLayers(mipLevel);
    imageCopy.dstOffset = dstOffset;
    imageCopy.extent = dstImage->calculateMipExtent(mipLevel);
    vkCmdCopyImage(handle, *srcImage, srcImage->getLayout(), *dstImage, dstImage->getLayout(), 1, &imageCopy);
}

void CommandBuffer::blitImage(const std::shared_ptr<const Image>& srcImage, const std::shared_ptr<Image>& dstImage, VkFilter filter,
    uint32_t mipLevel /* 0 */, const VkOffset3D& srcOffset /* 0, 0, 0 */, const VkOffset3D& dstOffset /* 0, 0, 0 */) const noexcept
{
    const VkExtent3D srcExtent = srcImage->calculateMipExtent(mipLevel);
    const VkExtent3D dstExtent = dstImage->calculateMipExtent(mipLevel);
    VkImageBlit imageBlit;
    imageBlit.srcSubresource = srcImage->getSubresourceLayers(mipLevel);
    imageBlit.srcOffsets[0] = srcOffset;
    imageBlit.srcOffsets[1].x = srcExtent.width;
    imageBlit.srcOffsets[1].y = srcExtent.height,
    imageBlit.srcOffsets[1].z = 1;
    imageBlit.dstSubresource = dstImage->getSubresourceLayers(mipLevel);
    imageBlit.dstOffsets[0] = dstOffset;
    imageBlit.dstOffsets[1].x = dstExtent.width;
    imageBlit.dstOffsets[1].y = dstExtent.height,
    imageBlit.dstOffsets[1].z = 1;
    vkCmdBlitImage(handle, *srcImage, srcImage->getLayout(), *dstImage, dstImage->getLayout(), 1, &imageBlit, filter);
}

// inline void CommandBuffer::copyImage
// inline void CommandBuffer::blitImage
// inline void CommandBuffer::copyBufferToImage
// inline void CommandBuffer::copyImageToBuffer
// inline void CommandBuffer::updateBuffer

void CommandBuffer::fillBuffer(const std::shared_ptr<Buffer>& buffer, uint32_t value,
    VkDeviceSize size /* VK_WHOLE_SIZE */,
    VkDeviceSize offset /* 0 */) const noexcept
{
    vkCmdFillBuffer(handle, *buffer, offset, size, value);
}

// inline void CommandBuffer::clearColorImage
// inline void CommandBuffer::clearDepthStencilImage
// inline void CommandBuffer::clearAttachments
// inline void CommandBuffer::resolveImage
// inline void CommandBuffer::setEvent
// inline void CommandBuffer::resetEvent
// inline void CommandBuffer::waitEvent

void CommandBuffer::waitEvents(const std::vector<std::shared_ptr<Event>>& events, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
    const std::vector<MemoryBarrier>& memoryBarriers /* {} */,
    const std::vector<BufferMemoryBarrier>& bufferMemoryBarriers /* {} */,
    const std::vector<ImageMemoryBarrier>& imageMemoryBarriers /* {} */) const noexcept
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
// inline void CommandBuffer::copyQueryResults
// inline void CommandBuffer::copyQueryResultsWithAvailability
// inline void CommandBuffer::pushConstants()
// inline void CommandBuffer::pushConstantBlock()

void CommandBuffer::beginRenderPass(const std::shared_ptr<RenderPass>& renderPass, const std::shared_ptr<Framebuffer>& framebuffer,
    const std::vector<ClearValue>& clearValues /* {} */,
    const VkRect2D& renderArea /* {0, 0, 0, 0} */,
    VkSubpassContents contents /* VK_SUBPASS_CONTENTS_INLINE */) noexcept
{
    if (clearValues.empty()) {
        MAGMA_ASSERT(!renderPass->hasClearOp());
    }
    VkRenderPassBeginInfo renderPassBeginInfo;
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.pNext = nullptr;
    renderPassBeginInfo.renderPass = *renderPass;
    renderPassBeginInfo.framebuffer = *framebuffer;
    renderPassBeginInfo.renderArea.offset = renderArea.offset;
    renderPassBeginInfo.renderArea.extent = (renderArea.extent.width || renderArea.extent.height) ? renderArea.extent : framebuffer->getExtent();
    renderPassBeginInfo.clearValueCount = MAGMA_COUNT(clearValues);
    renderPassBeginInfo.pClearValues = reinterpret_cast<const VkClearValue *>(clearValues.data());
    vkCmdBeginRenderPass(handle, &renderPassBeginInfo, contents);
    withinRenderPass = VK_TRUE;
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
    VkRenderPassBeginInfo renderPassBeginInfo;
    VkRenderPassAttachmentBeginInfoKHR renderPassBeginAttachmentInfo;
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.pNext = &renderPassBeginAttachmentInfo;
    renderPassBeginInfo.renderPass = *renderPass;
    renderPassBeginInfo.framebuffer = *framebuffer;
    renderPassBeginInfo.renderArea.offset = renderArea.offset;
    renderPassBeginInfo.renderArea.extent = (renderArea.extent.width || renderArea.extent.height) ? renderArea.extent : framebuffer->getExtent();
    renderPassBeginInfo.clearValueCount = MAGMA_COUNT(clearValues);
    renderPassBeginInfo.pClearValues = reinterpret_cast<const VkClearValue *>(clearValues.data());
    renderPassBeginAttachmentInfo.sType =  VK_STRUCTURE_TYPE_RENDER_PASS_ATTACHMENT_BEGIN_INFO_KHR;
    renderPassBeginAttachmentInfo.pNext = nullptr;
    renderPassBeginAttachmentInfo.attachmentCount = MAGMA_COUNT(dereferencedAttachments);
    renderPassBeginAttachmentInfo.pAttachments = dereferencedAttachments;
    vkCmdBeginRenderPass(handle, &renderPassBeginInfo, contents);
    withinRenderPass = VK_TRUE;
}
#endif // VK_KHR_imageless_framebuffer

// inline CommandBuffer::nextSubpass
// inline CommandBuffer::endRenderPass

#ifdef VK_KHR_device_group
// inline CommandBuffer::setDeviceMask
// inline CommandBuffer::dispatchBase

bool CommandBuffer::beginDeviceGroup(uint32_t deviceMask,
    VkCommandBufferUsageFlags flags /* 0 */) noexcept
{
    VkCommandBufferBeginInfo cmdBufferBeginInfo;
    VkDeviceGroupCommandBufferBeginInfo cmdBufferBeginDeviceGroupInfo;
    cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmdBufferBeginInfo.pNext = &cmdBufferBeginDeviceGroupInfo;
    cmdBufferBeginInfo.flags = flags;
    cmdBufferBeginInfo.pInheritanceInfo = nullptr;
    cmdBufferBeginDeviceGroupInfo.sType = VK_STRUCTURE_TYPE_DEVICE_GROUP_COMMAND_BUFFER_BEGIN_INFO;
    cmdBufferBeginDeviceGroupInfo.pNext = nullptr;
    cmdBufferBeginDeviceGroupInfo.deviceMask = deviceMask;
    const VkResult result = vkBeginCommandBuffer(handle, &cmdBufferBeginInfo);
    MAGMA_ASSERT(VK_SUCCESS == result);
    if (VK_SUCCESS == result)
        state = State::Recording;
    return (VK_SUCCESS == result);
}

void CommandBuffer::beginDeviceGroupRenderPass(uint32_t deviceMask,
    const std::shared_ptr<RenderPass>& renderPass, const std::shared_ptr<Framebuffer>& framebuffer,
    const std::vector<VkRect2D>& deviceRenderAreas /* {} */, const std::vector<ClearValue>& clearValues /* {} */,
    VkSubpassContents contents /* VK_SUBPASS_CONTENTS_INLINE */) noexcept
{
    if (clearValues.empty()) {
        MAGMA_ASSERT(!renderPass->hasClearOp());
    }
    VkRenderPassBeginInfo renderPassBeginInfo;
    VkDeviceGroupRenderPassBeginInfo renderPassBeginDeviceGroupInfo;
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.pNext = &renderPassBeginDeviceGroupInfo;
    renderPassBeginInfo.renderPass = *renderPass;
    renderPassBeginInfo.framebuffer = *framebuffer;
    renderPassBeginInfo.renderArea.offset = VkOffset2D{0, 0};
    renderPassBeginInfo.renderArea.extent = deviceRenderAreas.empty() ? framebuffer->getExtent() : VkExtent2D{0, 0};
    renderPassBeginInfo.clearValueCount = MAGMA_COUNT(clearValues);
    renderPassBeginInfo.pClearValues = reinterpret_cast<const VkClearValue *>(clearValues.data());
    renderPassBeginDeviceGroupInfo.sType = VK_STRUCTURE_TYPE_DEVICE_GROUP_RENDER_PASS_BEGIN_INFO;
    renderPassBeginDeviceGroupInfo.pNext = nullptr;
    renderPassBeginDeviceGroupInfo.deviceMask = deviceMask;
    renderPassBeginDeviceGroupInfo.deviceRenderAreaCount = MAGMA_COUNT(deviceRenderAreas);
    renderPassBeginDeviceGroupInfo.pDeviceRenderAreas = deviceRenderAreas.data();
    vkCmdBeginRenderPass(handle, &renderPassBeginInfo, contents);
    withinRenderPass = VK_TRUE;
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
    MAGMA_STACK_ARRAY(VkImageView, dereferencedAttachments, attachments.size());
    for (const auto& attachment : attachments)
        dereferencedAttachments.put(*attachment);
    VkRenderPassBeginInfo renderPassBeginInfo;
    VkDeviceGroupRenderPassBeginInfo renderPassBeginDeviceGroupInfo;
    VkRenderPassAttachmentBeginInfoKHR renderPassBeginAttachmentInfo;
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.pNext = &renderPassBeginDeviceGroupInfo;
    renderPassBeginInfo.renderPass = *renderPass;
    renderPassBeginInfo.framebuffer = *framebuffer;
    renderPassBeginInfo.renderArea.offset = VkOffset2D{0, 0};
    renderPassBeginInfo.renderArea.extent = deviceRenderAreas.empty() ? framebuffer->getExtent() : VkExtent2D{0, 0};
    renderPassBeginInfo.clearValueCount = MAGMA_COUNT(clearValues);
    renderPassBeginInfo.pClearValues = reinterpret_cast<const VkClearValue *>(clearValues.data());
    renderPassBeginDeviceGroupInfo.sType = VK_STRUCTURE_TYPE_DEVICE_GROUP_RENDER_PASS_BEGIN_INFO;
    renderPassBeginDeviceGroupInfo.pNext = &renderPassBeginAttachmentInfo;
    renderPassBeginDeviceGroupInfo.deviceMask = deviceMask;
    renderPassBeginDeviceGroupInfo.deviceRenderAreaCount = MAGMA_COUNT(deviceRenderAreas);
    renderPassBeginDeviceGroupInfo.pDeviceRenderAreas = deviceRenderAreas.data();
    renderPassBeginAttachmentInfo.sType =  VK_STRUCTURE_TYPE_RENDER_PASS_ATTACHMENT_BEGIN_INFO_KHR;
    renderPassBeginAttachmentInfo.pNext = nullptr;
    renderPassBeginAttachmentInfo.attachmentCount = MAGMA_COUNT(dereferencedAttachments);
    renderPassBeginAttachmentInfo.pAttachments = dereferencedAttachments;
    vkCmdBeginRenderPass(handle, &renderPassBeginInfo, contents);
    withinRenderPass = VK_TRUE;
}
#endif // VK_KHR_imageless_framebuffer
#endif // VK_KHR_device_group

#ifdef VK_EXT_conditional_rendering
void CommandBuffer::beginConditionalRendering(const std::shared_ptr<Buffer>& buffer,
    VkDeviceSize offset /* 0 */,
    bool inverted /* false */) noexcept
{
    MAGMA_ASSERT(offset <= buffer->getSize() - sizeof(uint32_t));
    MAGMA_ASSERT(offset % sizeof(uint32_t) == 0);
    MAGMA_DEVICE_EXTENSION(vkCmdBeginConditionalRenderingEXT);
    if (vkCmdBeginConditionalRenderingEXT)
    {
        VkConditionalRenderingBeginInfoEXT conditionalRenderingBeginInfo;
        conditionalRenderingBeginInfo.sType = VK_STRUCTURE_TYPE_CONDITIONAL_RENDERING_BEGIN_INFO_EXT;
        conditionalRenderingBeginInfo.pNext = nullptr;
        conditionalRenderingBeginInfo.buffer = *buffer;
        conditionalRenderingBeginInfo.offset = offset;
        conditionalRenderingBeginInfo.flags = inverted ? VK_CONDITIONAL_RENDERING_INVERTED_BIT_EXT : 0;
        vkCmdBeginConditionalRenderingEXT(handle, &conditionalRenderingBeginInfo);
        withinConditionalRendering = VK_TRUE;
    }
}

void CommandBuffer::endConditionalRendering() noexcept
{
    MAGMA_ASSERT(withinConditionalRendering);
    MAGMA_DEVICE_EXTENSION(vkCmdEndConditionalRenderingEXT);
    if (vkCmdEndConditionalRenderingEXT)
    {
        vkCmdEndConditionalRenderingEXT(handle);
        withinConditionalRendering = VK_FALSE;
    }
}
#endif // VK_EXT_conditional_rendering

#ifdef VK_EXT_transform_feedback
// inline void CommandBuffer::beginTransformFeedback()
// inline void CommandBuffer::endTransformFeedback()

void CommandBuffer::beginTransformFeedback(uint32_t firstCounterBuffer, const std::initializer_list<std::shared_ptr<TransformFeedbackCounterBuffer>>& counterBuffers,
    const std::initializer_list<VkDeviceSize>& counterBufferOffsets /* empty */) noexcept
{
    if (counterBufferOffsets.size() > 0) {
        MAGMA_ASSERT(counterBufferOffsets.size() >= counterBuffers.size());
    }
    MAGMA_DEVICE_EXTENSION(vkCmdBeginTransformFeedbackEXT);
    if (vkCmdBeginTransformFeedbackEXT)
    {
        MAGMA_STACK_ARRAY(VkBuffer, dereferencedCounterBuffers, counterBuffers.size());
        for (const auto& buffer : counterBuffers)
            dereferencedCounterBuffers.put(*buffer);
        vkCmdBeginTransformFeedbackEXT(handle, firstCounterBuffer, dereferencedCounterBuffers.size(), dereferencedCounterBuffers, counterBufferOffsets.begin());
        withinTransformFeedback = VK_TRUE;
    }
}

void CommandBuffer::endTransformFeedback(uint32_t firstCounterBuffer, const std::initializer_list<std::shared_ptr<TransformFeedbackCounterBuffer>>& counterBuffers,
    const std::initializer_list<VkDeviceSize>& counterBufferOffsets /* empty */) noexcept
{
    MAGMA_ASSERT(withinTransformFeedback);
    if (counterBufferOffsets.size() > 0) {
        MAGMA_ASSERT(counterBufferOffsets.size() >= counterBuffers.size());
    }
    MAGMA_DEVICE_EXTENSION(vkCmdEndTransformFeedbackEXT);
    if (vkCmdEndTransformFeedbackEXT)
    {
        MAGMA_STACK_ARRAY(VkBuffer, dereferencedCounterBuffers, counterBuffers.size());
        for (const auto& buffer : counterBuffers)
            dereferencedCounterBuffers.put(*buffer);
        vkCmdEndTransformFeedbackEXT(handle, firstCounterBuffer, dereferencedCounterBuffers.size(), dereferencedCounterBuffers, counterBufferOffsets.begin());
        withinTransformFeedback = VK_FALSE;
    }
}
#endif // VK_EXT_transform_feedback

#ifdef VK_NV_ray_tracing
void CommandBuffer::buildAccelerationStructure(const std::shared_ptr<Buffer>& instanceData, VkDeviceSize instanceOffset, bool update,
    const std::shared_ptr<AccelerationStructure>& dst, const std::shared_ptr<AccelerationStructure>& src,
    const std::shared_ptr<Buffer>& scratch, VkDeviceSize scratchOffset /* 0 */) noexcept
{
    MAGMA_DEVICE_EXTENSION(vkCmdBuildAccelerationStructureNV);
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
    MAGMA_DEVICE_EXTENSION(vkCmdCopyAccelerationStructureNV);
    if (vkCmdCopyAccelerationStructureNV)
        vkCmdCopyAccelerationStructureNV(handle, *dst, *src, clone ? VK_COPY_ACCELERATION_STRUCTURE_MODE_CLONE_NV : VK_COPY_ACCELERATION_STRUCTURE_MODE_COMPACT_NV);
}

void CommandBuffer::writeAccelerationStructureProperties(const std::shared_ptr<AccelerationStructure>& accelerationStructure,
    const std::shared_ptr<AccelerationStructureCompactedSizeQuery>& queryPool, uint32_t firstQuery) noexcept
{
    MAGMA_ASSERT(firstQuery < queryPool->getQueryCount());
    MAGMA_DEVICE_EXTENSION(vkCmdWriteAccelerationStructuresPropertiesNV);
    if (vkCmdWriteAccelerationStructuresPropertiesNV)
        vkCmdWriteAccelerationStructuresPropertiesNV(handle, 1, accelerationStructure->getHandleAddress(), queryPool->getType(), *queryPool, firstQuery);
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
    MAGMA_DEVICE_EXTENSION(vkCmdTraceRaysNV);
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
} // namespace magma
