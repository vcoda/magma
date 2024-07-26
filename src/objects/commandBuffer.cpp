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
#include "device.h"
#include "physicalDevice.h"
#include "instance.h"
#include "commandBuffer.h"
#include "commandPool.h"
#include "framebuffer.h"
#include "imagelessFramebuffer.h"
#include "renderPass.h"
#include "imageView.h"
#include "fence.h"
#include "bottomLevelStructure.h"
#include "topLevelStructure.h"
#include "shaderBindingTable.h"
#include "../raytracing/accelerationStructureGeometry.h"
#include "../misc/featureQuery.h"
#include "../exceptions/errorResult.h"
#include "../core/foreach.h"

namespace magma
{
CommandBuffer::CommandBuffer(VkCommandBufferLevel level, VkCommandBuffer handle, std::shared_ptr<CommandPool> cmdPool_):
    Dispatchable(VK_OBJECT_TYPE_COMMAND_BUFFER, handle),
    cmdPool(cmdPool_),
    device(cmdPool_->getDevice()),
    fence(std::make_unique<Fence>(device)),
    level(level),
    usage(0),
    state(State::Initial),
    resetCommandBuffer((cmdPool_->getFlags() & VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT) != 0),
    debugMarkerEnabled(VK_FALSE),
    debugUtilsEnabled(VK_FALSE),
    occlusionQueryEnable(VK_FALSE),
    conditionalRenderingEnable(VK_FALSE),
    negativeViewportHeightEnabled(device->checkFeatures()->negativeViewportHeightEnabled()),
    inRenderPass(VK_FALSE),
    inConditionalRendering(VK_FALSE),
    inTransformFeedback(VK_FALSE),
    labeledRecording(VK_FALSE),
    labeledRenderPass(VK_FALSE),
    queryFlags(0),
    pipelineStatistics(0),
    markerOffset(0ull)
{
#ifdef VK_EXT_debug_marker
    debugMarkerEnabled = device->extensionEnabled(VK_EXT_DEBUG_MARKER_EXTENSION_NAME);
#endif
#ifdef VK_EXT_debug_utils
    debugUtilsEnabled = device->getPhysicalDevice()->getInstance()->extensionEnabled(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
}

CommandBuffer::CommandBuffer(VkCommandBufferLevel level, std::shared_ptr<CommandPool> cmdPool_):
    Dispatchable(VK_OBJECT_TYPE_COMMAND_BUFFER),
    cmdPool(cmdPool_),
    device(cmdPool_->getDevice()),
    fence(std::make_unique<Fence>(device)),
    level(level),
    usage(0),
    state(State::Initial),
    resetCommandBuffer((cmdPool_->getFlags() & VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT) != 0),
    debugMarkerEnabled(VK_FALSE),
    debugUtilsEnabled(VK_FALSE),
    occlusionQueryEnable(VK_FALSE),
    conditionalRenderingEnable(VK_FALSE),
    negativeViewportHeightEnabled(device->checkFeatures()->negativeViewportHeightEnabled()),
    inRenderPass(VK_FALSE),
    inConditionalRendering(VK_FALSE),
    inTransformFeedback(VK_FALSE),
    labeledRecording(VK_FALSE),
    labeledRenderPass(VK_FALSE),
    queryFlags(0),
    pipelineStatistics(0),
    markerOffset(0ull)
{
#ifdef VK_EXT_debug_marker
    debugMarkerEnabled = device->extensionEnabled(VK_EXT_DEBUG_MARKER_EXTENSION_NAME);
#endif
#ifdef VK_EXT_debug_utils
    debugUtilsEnabled = device->getPhysicalDevice()->getInstance()->extensionEnabled(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
    VkCommandBufferAllocateInfo cmdBufferAllocateInfo;
    cmdBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmdBufferAllocateInfo.pNext = nullptr;
    cmdBufferAllocateInfo.commandPool = *cmdPool_;
    cmdBufferAllocateInfo.level = level;
    cmdBufferAllocateInfo.commandBufferCount = 1;
    const VkResult result = vkAllocateCommandBuffers(getNativeDevice(), &cmdBufferAllocateInfo, &handle);
    MAGMA_HANDLE_RESULT(result, VK_COMMAND_BUFFER_LEVEL_PRIMARY == level ?
        "failed to allocate primary command buffer" : "failed to allocate secondary command buffer");
}

CommandBuffer::~CommandBuffer()
{
    if (handle)
    {   // Release if not freed through command pool
        MAGMA_ASSERT(!cmdPool.expired());
        vkFreeCommandBuffers(getNativeDevice(), *cmdPool.lock(), 1, &handle);
    }
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
    usage = flags;
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
    usage = flags;
    return (VK_SUCCESS == result);
}

void CommandBuffer::end()
{
    MAGMA_ASSERT(State::Recording == state);
    MAGMA_ASSERT(!inRenderPass);
    MAGMA_ASSERT(!inConditionalRendering);
    MAGMA_ASSERT(!inTransformFeedback);
    if (State::Recording == state)
    {
        if (!pipelineBarriers.empty())
        {   // Flush deferred memory barriers
            for (auto const& it: pipelineBarriers)
            {
                const PipelineBarrierBatch& batch = it.second;
                pipelineBarrier(batch.srcStageMask,
                    batch.dstStageMask,
                    batch.memoryBarriers,
                    batch.bufferMemoryBarriers,
                    batch.imageMemoryBarriers,
                    batch.dependencyFlags);
            }
            pipelineBarriers.clear();
        }
        if (labeledRecording)
        {
        #ifdef VK_EXT_debug_utils
            endDebugLabel();
            labeledRecording = VK_FALSE;
        #endif
        }
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
    MAGMA_ASSERT(resetCommandBuffer);
    VkCommandBufferResetFlags flags = 0;
    if (releaseResources)
    {   /* Specifies that most or all memory resources currently owned by
           the command buffer should be returned to the parent command pool.
           If this flag is not set, then the command buffer may hold onto
           memory resources and reuse them when recording commands. */
        flags |= VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT;
    }
    const VkResult result = vkResetCommandBuffer(handle, flags);
    MAGMA_ASSERT(VK_SUCCESS == result);
    if (VK_SUCCESS == result)
    {
        releaseResourcesInUse();
        state = State::Initial;
        inRenderPass = VK_FALSE;
        inConditionalRendering = VK_FALSE;
        inTransformFeedback = VK_FALSE;
        renderPassState.renderPass.reset();
        renderPassState.framebuffer.reset();
        renderPassState.attachments.clear();
        markerOffset = 0ull;
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
        if (device->checkFeatures()->maintenanceEnabled(1))
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
    const std::initializer_list<uint32_t>& dynamicOffsets /* empty */) noexcept
{
    MAGMA_ASSERT_FOR_EACH(descriptorSets, descriptorSet, pipeline->getLayout()->hasLayout(descriptorSet->getLayout()));
    MAGMA_ASSERT_FOR_EACH(descriptorSets, descriptorSet, !descriptorSet->dirty());
    MAGMA_STACK_ARRAY(VkDescriptorSet, dereferencedDescriptorSets, descriptorSets.size());
    for (auto const& descriptorSet: descriptorSets)
    {
        dereferencedDescriptorSets.put(*descriptorSet);
        MAGMA_INUSE(descriptorSet);
    }
    vkCmdBindDescriptorSets(handle, pipeline->getBindPoint(), *pipeline->getLayout(),
        firstSet, dereferencedDescriptorSets.size(), dereferencedDescriptorSets,
        MAGMA_COUNT(dynamicOffsets), dynamicOffsets.begin());
    MAGMA_INUSE(pipeline->getLayout());
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
    {
        vkCmdBindTransformFeedbackBuffersEXT(handle, firstBinding, 1, transformFeedbackBuffer->getHandleAddress(), &offset, &size);
        MAGMA_INUSE(transformFeedbackBuffer);
    }
}

void CommandBuffer::bindTransformFeedbackBuffers(uint32_t firstBinding, const std::initializer_list<std::shared_ptr<TransformFeedbackBuffer>>& transformFeedbackBuffers,
    std::vector<VkDeviceSize> offsets /* empty */,
    const std::initializer_list<VkDeviceSize>& sizes /* empty */)
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
        for (auto const& buffer: transformFeedbackBuffers)
        {
            dereferencedBuffers.put(*buffer);
            MAGMA_INUSE(buffer);
        }
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
    if (size < VK_WHOLE_SIZE)
        bufferCopy.size = size;
    else
    {
        MAGMA_ASSERT(srcBuffer->getSize() >= dstBuffer->getSize());
        bufferCopy.size = dstBuffer->getSize();
    }
    vkCmdCopyBuffer(handle, *srcBuffer, *dstBuffer, 1, &bufferCopy);
    MAGMA_INUSE(srcBuffer);
    MAGMA_INUSE(dstBuffer);
}

void CommandBuffer::copyImage(const std::shared_ptr<const Image>& srcImage, const std::shared_ptr<Image>& dstImage,
    uint32_t mipLevel /* 0 */, const VkOffset3D& srcOffset /* 0, 0, 0 */, const VkOffset3D& dstOffset /* 0, 0, 0 */) const noexcept
{
    const VkImageLayout srcLayout = srcImage->getLayout(mipLevel);
    const VkImageLayout dstLayout = dstImage->getLayout(mipLevel);
    VkImageCopy imageCopy;
    imageCopy.srcSubresource = srcImage->getSubresourceLayers(mipLevel);
    imageCopy.srcOffset = srcOffset;
    imageCopy.dstSubresource = dstImage->getSubresourceLayers(mipLevel);
    imageCopy.dstOffset = dstOffset;
    imageCopy.extent = dstImage->calculateMipExtent(mipLevel);
    vkCmdCopyImage(handle, *srcImage, srcLayout, *dstImage, dstLayout, 1, &imageCopy);
    MAGMA_INUSE(srcImage);
    MAGMA_INUSE(dstImage);
}

void CommandBuffer::blitImage(const std::shared_ptr<const Image>& srcImage, const std::shared_ptr<Image>& dstImage, VkFilter filter,
    uint32_t mipLevel /* 0 */, const VkOffset3D& srcOffset /* 0, 0, 0 */, const VkOffset3D& dstOffset /* 0, 0, 0 */) const noexcept
{
    const VkExtent3D srcExtent = srcImage->calculateMipExtent(mipLevel);
    const VkImageLayout srcLayout = srcImage->getLayout(mipLevel);
    const VkExtent3D dstExtent = dstImage->calculateMipExtent(mipLevel);
    const VkImageLayout dstLayout = dstImage->getLayout(mipLevel);
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
    vkCmdBlitImage(handle, *srcImage, srcLayout, *dstImage, dstLayout, 1, &imageBlit, filter);
    MAGMA_INUSE(srcImage);
    MAGMA_INUSE(dstImage);
}

// inline void CommandBuffer::copyImage
// inline void CommandBuffer::blitImage
// inline void CommandBuffer::copyBufferToImage
// inline void CommandBuffer::copyImageToBuffer
// inline void CommandBuffer::updateBuffer
// inline void CommandBuffer::fillBuffer
// inline void CommandBuffer::clearColorImage
// inline void CommandBuffer::clearDepthStencilImage
// inline void CommandBuffer::clearAttachments
// inline void CommandBuffer::resolveImage
// inline void CommandBuffer::setEvent
// inline void CommandBuffer::resetEvent
// inline void CommandBuffer::waitEvent

void CommandBuffer::waitEvents(const std::vector<std::shared_ptr<Event>>& events, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
    const std::vector<MemoryBarrier>& memoryBarriers /* empty */,
    const std::vector<BufferMemoryBarrier>& bufferMemoryBarriers /* empty */,
    const std::vector<ImageMemoryBarrier>& imageMemoryBarriers /* empty */) const noexcept
{
    MAGMA_ASSERT(srcStageMask);
    MAGMA_ASSERT(dstStageMask);
    MAGMA_STACK_ARRAY(VkEvent, dereferencedEvents, events.size());
    MAGMA_STACK_ARRAY(VkBufferMemoryBarrier, dereferencedBufferMemoryBarriers, bufferMemoryBarriers.size());
    MAGMA_STACK_ARRAY(VkImageMemoryBarrier, dereferencedImageMemoryBarriers, imageMemoryBarriers.size());
    for (auto const& event: events)
    {
        dereferencedEvents.put(*event);
        MAGMA_INUSE(event);
    }
    for (auto const& barrier: bufferMemoryBarriers)
        dereferencedBufferMemoryBarriers.put(barrier);
    for (auto const& barrier: imageMemoryBarriers)
    {
        dereferencedImageMemoryBarriers.put(barrier);
        MAGMA_INUSE(barrier.image);
    }
    vkCmdWaitEvents(handle, dereferencedEvents.size(), dereferencedEvents, srcStageMask, dstStageMask,
        MAGMA_COUNT(memoryBarriers),
        memoryBarriers.data(),
        dereferencedBufferMemoryBarriers.size(),
        dereferencedBufferMemoryBarriers,
        dereferencedImageMemoryBarriers.size(),
        dereferencedImageMemoryBarriers);
}

// inline void CommandBuffer::pipelineBarrier
// inline void CommandBuffer::pipelineBarrier
// inline void CommandBuffer::pipelineBarrier

void CommandBuffer::pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const ImageMemoryBarrier& barrier,
    VkDependencyFlags dependencyFlags /* 0 */) noexcept
{
    vkCmdPipelineBarrier(handle, srcStageMask, dstStageMask, dependencyFlags, 0, nullptr, 0, nullptr, 1, &barrier);
    MAGMA_INUSE(barrier.image);
    uint32_t levelCount = barrier.subresourceRange.levelCount;
    if (VK_REMAINING_MIP_LEVELS == levelCount)
        levelCount = barrier.image->getMipLevels() - barrier.subresourceRange.baseMipLevel;
    MAGMA_ASSERT(barrier.subresourceRange.baseMipLevel + levelCount <= barrier.image->getMipLevels());
    for (uint32_t i = 0; i < levelCount; ++i)
        barrier.image->setLayout(barrier.subresourceRange.baseMipLevel + i, barrier.newLayout);
}

void CommandBuffer::pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
    const std::vector<MemoryBarrier>& memoryBarriers /* empty */,
    const std::vector<BufferMemoryBarrier>& bufferMemoryBarriers /* empty */,
    const std::vector<ImageMemoryBarrier>& imageMemoryBarriers /* empty */,
    VkDependencyFlags dependencyFlags /* 0 */) noexcept
{
    MAGMA_STACK_ARRAY(VkImageMemoryBarrier, dereferencedImageMemoryBarriers, imageMemoryBarriers.size());
    for (auto const& barrier: imageMemoryBarriers)
    {
        dereferencedImageMemoryBarriers.put(barrier);
        MAGMA_INUSE(barrier.image);
    }
    vkCmdPipelineBarrier(handle, srcStageMask, dstStageMask, dependencyFlags,
        MAGMA_COUNT(memoryBarriers),
        memoryBarriers.data(),
        MAGMA_COUNT(bufferMemoryBarriers),
        bufferMemoryBarriers.data(),
        dereferencedImageMemoryBarriers.size(),
        dereferencedImageMemoryBarriers);
    for (auto const& barrier: imageMemoryBarriers)
    {
        uint32_t levelCount = barrier.subresourceRange.levelCount;
        if (VK_REMAINING_MIP_LEVELS == levelCount)
            levelCount = barrier.image->getMipLevels() - barrier.subresourceRange.baseMipLevel;
        MAGMA_ASSERT(barrier.subresourceRange.baseMipLevel + levelCount <= barrier.image->getMipLevels());
        for (uint32_t i = 0; i < levelCount; ++i)
            barrier.image->setLayout(barrier.subresourceRange.baseMipLevel + i, barrier.newLayout);
    }
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
    const std::vector<ClearValue>& clearValues /* empty */,
    const VkRect2D& renderArea /* {0, 0, 0, 0} */,
    VkSubpassContents contents /* VK_SUBPASS_CONTENTS_INLINE */) noexcept
{
    if (clearValues.empty()) {
        MAGMA_ASSERT(!renderPass->usesClear());
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
    MAGMA_INUSE(renderPass);
    MAGMA_INUSE(framebuffer);
    renderPass->begin(framebuffer->getAttachments());
    renderPassState.renderPass = renderPass;
    renderPassState.framebuffer = framebuffer;
    inRenderPass = VK_TRUE;
}

#ifdef VK_KHR_imageless_framebuffer
void CommandBuffer::beginRenderPass(const std::shared_ptr<RenderPass>& renderPass,
    const std::shared_ptr<ImagelessFramebuffer>& framebuffer,
    const std::vector<std::shared_ptr<ImageView>>& attachments,
    const std::vector<ClearValue>& clearValues /* empty */,
    const VkRect2D& renderArea /* {0, 0, 0, 0} */,
    VkSubpassContents contents /* VK_SUBPASS_CONTENTS_INLINE */) noexcept
{
    if (clearValues.empty()) {
        MAGMA_ASSERT(!renderPass->usesClear());
    }
    MAGMA_STACK_ARRAY(VkImageView, dereferencedAttachments, attachments.size());
    for (auto const& attachment: attachments)
    {
        dereferencedAttachments.put(*attachment);
        MAGMA_INUSE(attachment);
    }
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
    renderPassBeginAttachmentInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_ATTACHMENT_BEGIN_INFO_KHR;
    renderPassBeginAttachmentInfo.pNext = nullptr;
    renderPassBeginAttachmentInfo.attachmentCount = MAGMA_COUNT(dereferencedAttachments);
    renderPassBeginAttachmentInfo.pAttachments = dereferencedAttachments;
    vkCmdBeginRenderPass(handle, &renderPassBeginInfo, contents);
    MAGMA_INUSE(renderPass);
    MAGMA_INUSE(framebuffer);
    renderPass->begin(attachments);
    renderPassState = {renderPass, framebuffer, attachments};
    inRenderPass = VK_TRUE;
}
#endif // VK_KHR_imageless_framebuffer

// inline CommandBuffer::nextSubpass

void CommandBuffer::endRenderPass() noexcept
{
    MAGMA_ASSERT(inRenderPass);
    if (inRenderPass)
    {
    #ifdef VK_EXT_debug_utils
        if (labeledRenderPass)
        {
            endDebugLabel();
            labeledRenderPass = VK_FALSE;
        }
    #endif // VK_EXT_debug_utils
        vkCmdEndRenderPass(handle);
        if (!renderPassState.attachments.empty())
            renderPassState.renderPass->end(renderPassState.attachments);
        else if (renderPassState.framebuffer)
            renderPassState.renderPass->end(renderPassState.framebuffer->getAttachments());
        renderPassState.renderPass.reset();
        renderPassState.framebuffer.reset();
        renderPassState.attachments.clear();
        inRenderPass = VK_FALSE;
    }
}

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
    const std::vector<VkRect2D>& deviceRenderAreas /* empty */,
    const std::vector<ClearValue>& clearValues /* empty */,
    VkSubpassContents contents /* VK_SUBPASS_CONTENTS_INLINE */) noexcept
{
    if (clearValues.empty()) {
        MAGMA_ASSERT(!renderPass->usesClear());
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
    MAGMA_INUSE(renderPass);
    MAGMA_INUSE(framebuffer);
    renderPass->begin(framebuffer->getAttachments());
    inRenderPass = VK_TRUE;
    renderPassState = {renderPass, framebuffer};
}

#ifdef VK_KHR_imageless_framebuffer
void CommandBuffer::beginDeviceGroupRenderPass(uint32_t deviceMask,
    const std::shared_ptr<RenderPass>& renderPass,
    const std::shared_ptr<ImagelessFramebuffer>& framebuffer,
    const std::vector<std::shared_ptr<ImageView>>& attachments,
    const std::vector<VkRect2D>& deviceRenderAreas /* empty */,
    const std::vector<ClearValue>& clearValues /* empty */,
    VkSubpassContents contents /* VK_SUBPASS_CONTENTS_INLINE */) noexcept
{
    if (clearValues.empty()) {
        MAGMA_ASSERT(!renderPass->usesClear());
    }
    MAGMA_STACK_ARRAY(VkImageView, dereferencedAttachments, attachments.size());
    for (auto const& attachment: attachments)
    {
        dereferencedAttachments.put(*attachment);
        MAGMA_INUSE(attachment);
    }
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
    MAGMA_INUSE(renderPass);
    MAGMA_INUSE(framebuffer);
    renderPass->begin(attachments);
    inRenderPass = VK_TRUE;
    renderPassState = {renderPass, framebuffer, attachments};
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
        MAGMA_INUSE(buffer);
        inConditionalRendering = VK_TRUE;
    }
}

void CommandBuffer::endConditionalRendering() noexcept
{
    MAGMA_ASSERT(inConditionalRendering);
    MAGMA_DEVICE_EXTENSION(vkCmdEndConditionalRenderingEXT);
    if (vkCmdEndConditionalRenderingEXT)
    {
        vkCmdEndConditionalRenderingEXT(handle);
        inConditionalRendering = VK_FALSE;
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
        for (auto const& buffer: counterBuffers)
        {
            dereferencedCounterBuffers.put(*buffer);
            MAGMA_INUSE(buffer);
        }
        vkCmdBeginTransformFeedbackEXT(handle, firstCounterBuffer, dereferencedCounterBuffers.size(), dereferencedCounterBuffers, counterBufferOffsets.begin());
        inTransformFeedback = VK_TRUE;
    }
}

void CommandBuffer::endTransformFeedback(uint32_t firstCounterBuffer, const std::initializer_list<std::shared_ptr<TransformFeedbackCounterBuffer>>& counterBuffers,
    const std::initializer_list<VkDeviceSize>& counterBufferOffsets /* empty */) noexcept
{
    MAGMA_ASSERT(inTransformFeedback);
    if (counterBufferOffsets.size() > 0) {
        MAGMA_ASSERT(counterBufferOffsets.size() >= counterBuffers.size());
    }
    MAGMA_DEVICE_EXTENSION(vkCmdEndTransformFeedbackEXT);
    if (vkCmdEndTransformFeedbackEXT)
    {
        MAGMA_STACK_ARRAY(VkBuffer, dereferencedCounterBuffers, counterBuffers.size());
        for (auto const& buffer: counterBuffers)
        {
            dereferencedCounterBuffers.put(*buffer);
            MAGMA_INUSE(buffer);
        }
        vkCmdEndTransformFeedbackEXT(handle, firstCounterBuffer, dereferencedCounterBuffers.size(), dereferencedCounterBuffers, counterBufferOffsets.begin());
        inTransformFeedback = VK_FALSE;
    }
}
#endif // VK_EXT_transform_feedback

#ifdef VK_KHR_acceleration_structure
void CommandBuffer::buildAccelerationStructure(const std::shared_ptr<BottomLevelAccelerationStructure>& accelerationStructure,
    const std::forward_list<AccelerationStructureGeometry>& geometries, const std::shared_ptr<Buffer>& scratchBuffer) noexcept
{
    std::vector<VkAccelerationStructureBuildRangeInfoKHR> buildRanges;
    for (auto const& geometry: geometries)
        buildRanges.push_back({geometry.primitiveCount});
    rebuildAccelerationStructure(VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR, accelerationStructure, geometries, buildRanges, scratchBuffer);
}

void CommandBuffer::updateAccelerationStructure(const std::shared_ptr<BottomLevelAccelerationStructure>& accelerationStructure,
    const std::forward_list<AccelerationStructureGeometry>& geometries, const std::shared_ptr<Buffer>& scratchBuffer) noexcept
{
    std::vector<VkAccelerationStructureBuildRangeInfoKHR> buildRanges;
    for (auto const& geometry: geometries)
        buildRanges.push_back({geometry.primitiveCount});
    rebuildAccelerationStructure(VK_BUILD_ACCELERATION_STRUCTURE_MODE_UPDATE_KHR, accelerationStructure, geometries, buildRanges, scratchBuffer);
}

// inline void CommandBuffer::buildAccelerationStructure
// inline void CommandBuffer::updateAccelerationStructure
// inline void CommandBuffer::buildAccelerationStructure
// inline void CommandBuffer::updateAccelerationStructure

void CommandBuffer::updateAccelerationStructureIndirect(const std::shared_ptr<AccelerationStructure>& accelerationStructure,
    const std::forward_list<AccelerationStructureGeometry>& geometries, const std::shared_ptr<Buffer>& indirectBuildRanges,
    const std::shared_ptr<Buffer>& scratchBuffer, uint32_t indirectStride /* sizeof(VkAccelerationStructureBuildRangeInfoKHR) */) noexcept
{
    const uint32_t geometryCount = (uint32_t)std::distance(geometries.begin(), geometries.end());
    MAGMA_STACK_ARRAY(const VkAccelerationStructureGeometryKHR *, geometryPointers, geometryCount);
    MAGMA_STACK_ARRAY(uint32_t, primitiveCounts, geometryCount);
    for (auto const& geometry: geometries)
    {
        geometryPointers.put(&geometry);
        primitiveCounts.put(geometry.primitiveCount);
    }
    VkAccelerationStructureBuildGeometryInfoKHR buildGeometryInfo;
    buildGeometryInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
    buildGeometryInfo.pNext = nullptr;
    buildGeometryInfo.type = accelerationStructure->getType();
    buildGeometryInfo.flags = accelerationStructure->getBuildFlags();
    buildGeometryInfo.mode = VK_BUILD_ACCELERATION_STRUCTURE_MODE_UPDATE_KHR;
    buildGeometryInfo.srcAccelerationStructure = VK_NULL_HANDLE;
    buildGeometryInfo.dstAccelerationStructure = *accelerationStructure;
    buildGeometryInfo.geometryCount = geometryCount;
    buildGeometryInfo.pGeometries = nullptr;
    buildGeometryInfo.ppGeometries = geometryPointers;
    buildGeometryInfo.scratchData.deviceAddress = scratchBuffer->getDeviceAddress();
    const VkDeviceAddress indirectDeviceAddress = indirectBuildRanges->getDeviceAddress();
    const uint32_t *maxPrimitiveCounts = primitiveCounts;
    MAGMA_DEVICE_EXTENSION(vkCmdBuildAccelerationStructuresIndirectKHR);
    if (vkCmdBuildAccelerationStructuresIndirectKHR)
    {
        vkCmdBuildAccelerationStructuresIndirectKHR(handle, 1, &buildGeometryInfo, &indirectDeviceAddress, &indirectStride, &maxPrimitiveCounts);
        MAGMA_INUSE(accelerationStructure);
        MAGMA_INUSE(scratchBuffer);
        MAGMA_INUSE(indirectBuildRanges);
    }
}

void CommandBuffer::copyAccelerationStructure(const std::shared_ptr<AccelerationStructure>& dstAccelerationStructure,
    const std::shared_ptr<AccelerationStructure>& srcAccelerationStructure)
{
    VkCopyAccelerationStructureInfoKHR copyInfo;
    copyInfo.sType = VK_STRUCTURE_TYPE_COPY_ACCELERATION_STRUCTURE_INFO_KHR;
    copyInfo.pNext = nullptr;
    copyInfo.src = *srcAccelerationStructure;
    copyInfo.dst = *dstAccelerationStructure;
    copyInfo.mode = VK_COPY_ACCELERATION_STRUCTURE_MODE_CLONE_KHR;
    MAGMA_DEVICE_EXTENSION(vkCmdCopyAccelerationStructureKHR);
    if (vkCmdCopyAccelerationStructureKHR)
        vkCmdCopyAccelerationStructureKHR(handle, &copyInfo);
}

void CommandBuffer::compactAccelerationStructure(const std::shared_ptr<AccelerationStructure>& dstAccelerationStructure,
    const std::shared_ptr<AccelerationStructure>& srcAccelerationStructure)
{
    VkCopyAccelerationStructureInfoKHR copyInfo;
    copyInfo.sType = VK_STRUCTURE_TYPE_COPY_ACCELERATION_STRUCTURE_INFO_KHR;
    copyInfo.pNext = nullptr;
    copyInfo.src = *srcAccelerationStructure;
    copyInfo.dst = *dstAccelerationStructure;
    copyInfo.mode = VK_COPY_ACCELERATION_STRUCTURE_MODE_COMPACT_KHR;
    MAGMA_DEVICE_EXTENSION(vkCmdCopyAccelerationStructureKHR);
    if (vkCmdCopyAccelerationStructureKHR)
        vkCmdCopyAccelerationStructureKHR(handle, &copyInfo);
}

void CommandBuffer::copyAccelerationStructureToBuffer(const std::shared_ptr<Buffer>& dstBuffer,
    const std::shared_ptr<AccelerationStructure>& srcAccelerationStructure)
{
    VkCopyAccelerationStructureToMemoryInfoKHR copyMemoryInfo;
    copyMemoryInfo.sType = VK_STRUCTURE_TYPE_COPY_ACCELERATION_STRUCTURE_TO_MEMORY_INFO_KHR;
    copyMemoryInfo.pNext = nullptr;
    copyMemoryInfo.src = *srcAccelerationStructure;
    copyMemoryInfo.dst.deviceAddress = dstBuffer->getDeviceAddress();
    copyMemoryInfo.mode = VK_COPY_ACCELERATION_STRUCTURE_MODE_CLONE_KHR;
    MAGMA_DEVICE_EXTENSION(vkCmdCopyAccelerationStructureToMemoryKHR);
    if (vkCmdCopyAccelerationStructureToMemoryKHR)
        vkCmdCopyAccelerationStructureToMemoryKHR(handle, &copyMemoryInfo);
}

void CommandBuffer::copyAccelerationStructureToMemory(void *dstBuffer,
    const std::shared_ptr<AccelerationStructure>& srcAccelerationStructure)
{
    VkCopyAccelerationStructureToMemoryInfoKHR copyMemoryInfo;
    copyMemoryInfo.sType = VK_STRUCTURE_TYPE_COPY_ACCELERATION_STRUCTURE_TO_MEMORY_INFO_KHR;
    copyMemoryInfo.pNext = nullptr;
    copyMemoryInfo.src = *srcAccelerationStructure;
    copyMemoryInfo.dst.hostAddress = dstBuffer;
    copyMemoryInfo.mode = VK_COPY_ACCELERATION_STRUCTURE_MODE_CLONE_KHR;
    MAGMA_DEVICE_EXTENSION(vkCmdCopyAccelerationStructureToMemoryKHR);
    if (vkCmdCopyAccelerationStructureToMemoryKHR)
        vkCmdCopyAccelerationStructureToMemoryKHR(handle, &copyMemoryInfo);
}

void CommandBuffer::copyBufferToAccelerationStructure(const std::shared_ptr<AccelerationStructure>& dstAccelerationStructure,
    const std::shared_ptr<Buffer>& srcBuffer)
{
    VkCopyMemoryToAccelerationStructureInfoKHR copyMemoryInfo;
    copyMemoryInfo.sType = VK_STRUCTURE_TYPE_COPY_MEMORY_TO_ACCELERATION_STRUCTURE_INFO_KHR;
    copyMemoryInfo.pNext = nullptr;
    copyMemoryInfo.src.deviceAddress = srcBuffer->getDeviceAddress();
    copyMemoryInfo.dst = *dstAccelerationStructure;
    copyMemoryInfo.mode = VK_COPY_ACCELERATION_STRUCTURE_MODE_CLONE_KHR;
    MAGMA_DEVICE_EXTENSION(vkCmdCopyMemoryToAccelerationStructureKHR);
    if (vkCmdCopyMemoryToAccelerationStructureKHR)
        vkCmdCopyMemoryToAccelerationStructureKHR(handle, &copyMemoryInfo);
}

void CommandBuffer::copyMemoryToAccelerationStructure(const std::shared_ptr<AccelerationStructure>& dstAccelerationStructure,
    const void *srcBuffer)
{
    VkCopyMemoryToAccelerationStructureInfoKHR copyMemoryInfo;
    copyMemoryInfo.sType = VK_STRUCTURE_TYPE_COPY_MEMORY_TO_ACCELERATION_STRUCTURE_INFO_KHR;
    copyMemoryInfo.pNext = nullptr;
    copyMemoryInfo.src.hostAddress = srcBuffer;
    copyMemoryInfo.dst = *dstAccelerationStructure;
    copyMemoryInfo.mode = VK_COPY_ACCELERATION_STRUCTURE_MODE_CLONE_KHR;
    MAGMA_DEVICE_EXTENSION(vkCmdCopyMemoryToAccelerationStructureKHR);
    if (vkCmdCopyMemoryToAccelerationStructureKHR)
        vkCmdCopyMemoryToAccelerationStructureKHR(handle, &copyMemoryInfo);
}

void CommandBuffer::serializeAccelerationStructure(const std::shared_ptr<Buffer>& dstBuffer,
    const std::shared_ptr<AccelerationStructure>& srcAccelerationStructure)
{
    VkCopyAccelerationStructureToMemoryInfoKHR copyMemoryInfo;
    copyMemoryInfo.sType = VK_STRUCTURE_TYPE_COPY_ACCELERATION_STRUCTURE_TO_MEMORY_INFO_KHR;
    copyMemoryInfo.pNext = nullptr;
    copyMemoryInfo.src = *srcAccelerationStructure;
    copyMemoryInfo.dst.deviceAddress = dstBuffer->getDeviceAddress();
    copyMemoryInfo.mode = VK_COPY_ACCELERATION_STRUCTURE_MODE_SERIALIZE_KHR;
    MAGMA_DEVICE_EXTENSION(vkCmdCopyAccelerationStructureToMemoryKHR);
    if (vkCmdCopyAccelerationStructureToMemoryKHR)
        vkCmdCopyAccelerationStructureToMemoryKHR(handle, &copyMemoryInfo);
}

void CommandBuffer::deserializeAccelerationStructure(const std::shared_ptr<AccelerationStructure>& dstAccelerationStructure,
    const std::shared_ptr<Buffer>& srcBuffer)
{
    VkCopyMemoryToAccelerationStructureInfoKHR copyMemoryInfo;
    copyMemoryInfo.sType = VK_STRUCTURE_TYPE_COPY_MEMORY_TO_ACCELERATION_STRUCTURE_INFO_KHR;
    copyMemoryInfo.pNext = nullptr;
    copyMemoryInfo.src.deviceAddress = srcBuffer->getDeviceAddress();
    copyMemoryInfo.dst = *dstAccelerationStructure;
    copyMemoryInfo.mode = VK_COPY_ACCELERATION_STRUCTURE_MODE_DESERIALIZE_KHR;
    MAGMA_DEVICE_EXTENSION(vkCmdCopyMemoryToAccelerationStructureKHR);
    if (vkCmdCopyMemoryToAccelerationStructureKHR)
        vkCmdCopyMemoryToAccelerationStructureKHR(handle, &copyMemoryInfo);
}

void CommandBuffer::writeAccelerationStructureProperties(const std::shared_ptr<AccelerationStructure>& accelerationStructure,
    const std::shared_ptr<QueryPool>& queryPool, uint32_t firstQuery /* 0 */)
{
    MAGMA_DEVICE_EXTENSION(vkCmdWriteAccelerationStructuresPropertiesKHR);
    if (vkCmdWriteAccelerationStructuresPropertiesKHR)
        vkCmdWriteAccelerationStructuresPropertiesKHR(handle, 1, accelerationStructure->getHandleAddress(), queryPool->getType(), *queryPool, firstQuery);
}

void CommandBuffer::rebuildAccelerationStructure(VkBuildAccelerationStructureModeKHR mode,
    const std::shared_ptr<BottomLevelAccelerationStructure>& accelerationStructure,
    const std::forward_list<AccelerationStructureGeometry>& geometries,
    const std::vector<VkAccelerationStructureBuildRangeInfoKHR>& buildRanges,
    const std::shared_ptr<Buffer>& scratchBuffer)
{
    const uint32_t geometryCount = (uint32_t)std::distance(geometries.begin(), geometries.end());
    MAGMA_ASSERT(buildRanges.size() >= geometryCount);
    MAGMA_STACK_ARRAY(const VkAccelerationStructureGeometryKHR *, geometryPointers, geometryCount);
    for (auto const& geometry: geometries)
        geometryPointers.put(&geometry);
    VkAccelerationStructureBuildGeometryInfoKHR buildGeometryInfo;
    buildGeometryInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
    buildGeometryInfo.pNext = nullptr;
    buildGeometryInfo.type = accelerationStructure->getType();
    buildGeometryInfo.flags = accelerationStructure->getBuildFlags();
    buildGeometryInfo.mode = mode;
    if (VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR == mode)
        buildGeometryInfo.srcAccelerationStructure = VK_NULL_HANDLE;
    else // VK_BUILD_ACCELERATION_STRUCTURE_MODE_UPDATE_KHR
        buildGeometryInfo.srcAccelerationStructure = *accelerationStructure;
    buildGeometryInfo.dstAccelerationStructure = *accelerationStructure;
    buildGeometryInfo.geometryCount = geometryCount;
    buildGeometryInfo.pGeometries = nullptr;
    buildGeometryInfo.ppGeometries = geometryPointers;
    buildGeometryInfo.scratchData.deviceAddress = scratchBuffer->getDeviceAddress();
    const VkAccelerationStructureBuildRangeInfoKHR *buildRangeInfos = buildRanges.data();
    MAGMA_DEVICE_EXTENSION(vkCmdBuildAccelerationStructuresKHR);
    if (vkCmdBuildAccelerationStructuresKHR)
    {
        vkCmdBuildAccelerationStructuresKHR(handle, 1, &buildGeometryInfo, &buildRangeInfos);
        MAGMA_INUSE(accelerationStructure);
        MAGMA_INUSE(scratchBuffer);
    }
}

void CommandBuffer::rebuildAccelerationStructure(VkBuildAccelerationStructureModeKHR mode,
    const std::shared_ptr<TopLevelAccelerationStructure>& accelerationStructure,
    const AccelerationStructureGeometryInstances& instances, const std::shared_ptr<Buffer>& scratchBuffer)
{
    VkAccelerationStructureBuildGeometryInfoKHR buildGeometryInfo;
    buildGeometryInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
    buildGeometryInfo.pNext = nullptr;
    buildGeometryInfo.type = accelerationStructure->getType();
    buildGeometryInfo.flags = accelerationStructure->getBuildFlags();
    buildGeometryInfo.mode = mode;
    if (VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR == mode)
        buildGeometryInfo.srcAccelerationStructure = VK_NULL_HANDLE;
    else // VK_BUILD_ACCELERATION_STRUCTURE_MODE_UPDATE_KHR
        buildGeometryInfo.srcAccelerationStructure = *accelerationStructure;
    buildGeometryInfo.dstAccelerationStructure = *accelerationStructure;
    buildGeometryInfo.geometryCount = 1; // If type is VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR, geometryCount must be 1
    buildGeometryInfo.pGeometries = &instances;
    buildGeometryInfo.ppGeometries = nullptr;
    buildGeometryInfo.scratchData.deviceAddress = scratchBuffer->getDeviceAddress();
    const VkAccelerationStructureBuildRangeInfoKHR buildRangeInfo = {instances.primitiveCount};
    const VkAccelerationStructureBuildRangeInfoKHR *buildRangeInfos = &buildRangeInfo;
    MAGMA_DEVICE_EXTENSION(vkCmdBuildAccelerationStructuresKHR);
    if (vkCmdBuildAccelerationStructuresKHR)
    {
        vkCmdBuildAccelerationStructuresKHR(handle, 1, &buildGeometryInfo, &buildRangeInfos);
        MAGMA_INUSE(accelerationStructure);
        MAGMA_INUSE(scratchBuffer);
    }
}
#endif // VK_KHR_acceleration_structure

#ifdef VK_KHR_ray_tracing_pipeline
void CommandBuffer::setRayTracingPipelineStackSize(uint32_t pipelineStackSize) const noexcept
{
    MAGMA_DEVICE_EXTENSION(vkCmdSetRayTracingPipelineStackSizeKHR);
    if (vkCmdSetRayTracingPipelineStackSizeKHR)
        vkCmdSetRayTracingPipelineStackSizeKHR(handle, pipelineStackSize);
}

void CommandBuffer::traceRays(const std::shared_ptr<const ShaderBindingTable>& shaderBindingTable,
    uint32_t width, uint32_t height, uint32_t depth) const noexcept
{
    MAGMA_DEVICE_EXTENSION(vkCmdTraceRaysKHR);
    if (vkCmdTraceRaysKHR)
    {
        vkCmdTraceRaysKHR(handle,
            shaderBindingTable->getRaygenStage(),
            shaderBindingTable->getMissStage(),
            shaderBindingTable->getHitStage(),
            shaderBindingTable->getCallableStage(),
            width, height, depth);
        MAGMA_INUSE(shaderBindingTable);
    }
}

void CommandBuffer::traceRaysIndirect(const std::shared_ptr<const ShaderBindingTable>& shaderBindingTable,
    const std::shared_ptr<const Buffer>& indirectBuffer) const noexcept
{
    MAGMA_DEVICE_EXTENSION(vkCmdTraceRaysIndirectKHR);
    if (vkCmdTraceRaysIndirectKHR)
    {
        vkCmdTraceRaysIndirectKHR(handle,
            shaderBindingTable->getRaygenStage(),
            shaderBindingTable->getMissStage(),
            shaderBindingTable->getHitStage(),
            shaderBindingTable->getCallableStage(),
            indirectBuffer->getDeviceAddress());
        MAGMA_INUSE(shaderBindingTable);
        MAGMA_INUSE(indirectBuffer);
    }
}
#endif // VK_KHR_ray_tracing_pipeline

#ifdef VK_KHR_ray_tracing_maintenance1
void CommandBuffer::traceRaysIndirect(const std::shared_ptr<Buffer>& indirectTraceRaysBuffer) const noexcept
{
    MAGMA_DEVICE_EXTENSION(vkCmdTraceRaysIndirect2KHR);
    if (vkCmdTraceRaysIndirect2KHR)
    {
        vkCmdTraceRaysIndirect2KHR(handle, indirectTraceRaysBuffer->getDeviceAddress());
        MAGMA_INUSE(indirectBuffer);
    }
}
#endif // VK_KHR_ray_tracing_maintenance1

void CommandBuffer::releaseResourcesInUse() const noexcept
{
#ifdef MAGMA_RETAIN_OBJECTS_IN_USE
    inUse.clear();
#endif
    pipelineBarriers.clear();
}

inline VkDevice CommandBuffer::getNativeDevice() const noexcept
{
    return device->getHandle();
}

CommandBuffer::PipelineBarrierBatch *CommandBuffer::lookupBarrierBatch(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags) noexcept
{
    const hash_t hash = core::hashArgs(srcStageMask, dstStageMask, dependencyFlags);
    auto it = pipelineBarriers.find(hash);
    if (it != pipelineBarriers.end())
        return &it->second;
    PipelineBarrierBatch batch{srcStageMask, dstStageMask, dependencyFlags};
    try
    {
        it = pipelineBarriers.emplace(hash, batch).first;
    }
    catch (...) {}
    if (it != pipelineBarriers.end())
        return &it->second;
    return nullptr;
}
} // namespace magma
