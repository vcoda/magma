/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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
#include "instance.h"
#include "device.h"
#include "physicalDevice.h"
#include "framebuffer.h"
#include "imagelessFramebuffer.h"
#include "imageView.h"
#include "renderPass.h"
#include "storageBuffer.h"
#include "fence.h"
#include "bottomLevelStructure.h"
#include "../shaders/shaderBindingTable.h"
#include "../raytracing/accelerationStructureGeometry.h"
#include "../exceptions/errorResult.h"

namespace magma
{
CommandBuffer::CommandBuffer(VkCommandBufferLevel level, VkCommandBuffer handle, const CommandPool *cmdPool):
    leanCmd(handle, cmdPool),
    level(level),
    cmdPool(cmdPool->getHandle()),
    queueFamilyIndex(cmdPool->getQueueFamilyIndex()),
    extensions(cmdPool->getDevice()),
    device(cmdPool->getDevice()),
    fence(std::make_unique<Fence>(device)),
    usage(0),
    state(State::Initial),
    resetCommandBuffer((cmdPool->getFlags() & VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT) != 0)
{
    putInitialState();
}

CommandBuffer::CommandBuffer(VkCommandBufferLevel level, const CommandPool *cmdPool):
    leanCmd(level, cmdPool),
    level(level),
    cmdPool(cmdPool->getHandle()),
    queueFamilyIndex(cmdPool->getQueueFamilyIndex()),
    extensions(cmdPool->getDevice()),
    device(cmdPool->getDevice()),
    fence(std::make_unique<Fence>(device)),
    usage(0),
    state(State::Initial),
    resetCommandBuffer((cmdPool->getFlags() & VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT) != 0)
{
    putInitialState();
}

CommandBuffer::~CommandBuffer()
{   // Command buffer must not be destroyed while queue is executing its commands
    MAGMA_ASSERT(state != State::Pending);
}

bool CommandBuffer::begin(VkCommandBufferUsageFlags flags /* 0 */) noexcept
{
    MAGMA_ASSERT(state != State::Recording);
    const VkResult result = leanCmd.begin(flags);
    MAGMA_ASSERT(VK_SUCCESS == result);
    if (VK_SUCCESS == result)
    {
        state = State::Recording;
    #ifdef MAGMA_STATS
        stats = Statistics();
    #endif
    }
    usage = flags;
    return (VK_SUCCESS == result);
}

bool CommandBuffer::beginInherited(lent_ptr<const RenderPass> renderPass, uint32_t subpass,
    lent_ptr<const Framebuffer> framebuffer, bool occlusionQueryEnable /* false */, VkQueryControlFlags queryFlags /* 0 */,
    VkQueryPipelineStatisticFlags pipelineStatistics /* 0 */, VkCommandBufferUsageFlags flags /* 0 */,
    const StructureChain& extendedInfo /* default */) noexcept
{
    MAGMA_ASSERT(state != State::Recording);
    const VkResult result = leanCmd.beginInherited(renderPass.get(), subpass, framebuffer.get(), occlusionQueryEnable,
        queryFlags, pipelineStatistics, flags, extendedInfo);
    MAGMA_ASSERT(VK_SUCCESS == result);
    if (VK_SUCCESS == result)
    {
        state = State::Recording;
    #ifdef MAGMA_STATS
        stats = Statistics();
    #endif
    }
    usage = flags;
    return (VK_SUCCESS == result);
}

void CommandBuffer::end()
{
    MAGMA_ASSERT(State::Recording == state);
    MAGMA_ASSERT(!renderingPass);
    MAGMA_ASSERT(!nonIndexedQuery);
    MAGMA_ASSERT(!indexedQuery);
    MAGMA_ASSERT(!conditionalRendering);
    MAGMA_ASSERT(!transformFeedback);
    if (State::Recording == state)
    {
        if (annotatedBegin)
        {
            popDebugMarker();
            annotatedBegin = VK_FALSE;
        }
        /* Performance - critical commands generally do not have return codes.
           If a run time error occurs in such commands, the implementation will defer
           reporting the error until a specified point. For commands that record
           into command buffers (vkCmd*), run time errors are reported by vkEndCommandBuffer. */
        const VkResult result = leanCmd.end();
        // This is the only place where command buffer may throw an exception.
        MAGMA_HANDLE_RESULT(result, "failed to record command buffer");
        state = State::Executable;
    }
}

bool CommandBuffer::reset(VkCommandBufferResetFlags flags /* 0 */) noexcept
{   // The command buffer can be in any state other than pending
    MAGMA_ASSERT(state != State::Pending);
    MAGMA_ASSERT(allowsReset());
    const VkResult result = leanCmd.reset(flags);
    MAGMA_ASSERT(VK_SUCCESS == result);
    if (VK_SUCCESS == result)
    {
        releaseObjectsInUse();
        putInitialState();
        return true;
    }
    return false;
}

void CommandBuffer::setViewport(float x, float y, float width, float height,
    float minDepth /* 0 */, float maxDepth /* 1 */) noexcept
{
    VkViewport viewport;
    viewport.x = x;
    viewport.y = y;
    if (height < 0)
    {
        if (extensions.KHR_maintenance1)
            viewport.y = -height - y; // Move origin to bottom left
    }
    viewport.width = width;
    viewport.height = height;
    if (height < 0)
    {
        if (!(extensions.KHR_maintenance1 || extensions.AMD_negative_viewport_height))
            viewport.height = -height; // Negative viewport height not supported
    }
    viewport.minDepth = minDepth;
    viewport.maxDepth = maxDepth;
    leanCmd.setViewport(viewport);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);
}

void CommandBuffer::bindDescriptorSets(lent_ptr<const Pipeline> pipeline, uint32_t firstSet, const std::initializer_list<lent_ptr<const DescriptorSet>>& descriptorSets,
    const std::initializer_list<uint32_t>& dynamicOffsets /* empty */) noexcept
{
    auto unmanagedDescriptorSets = stackalloc(const DescriptorSet*, descriptorSets.size());
    uint32_t descriptorSetCount = 0;
    for (auto const& descriptorSet: descriptorSets)
    {
        unmanagedDescriptorSets[descriptorSetCount++] = descriptorSet.get();
        MAGMA_INUSE(descriptorSet);
    }
    leanCmd.bindDescriptorSets(pipeline->getBindPoint(), pipeline->getLayout().get(), firstSet, descriptorSetCount, unmanagedDescriptorSets,
        core::countof(dynamicOffsets), dynamicOffsets.begin());
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);
    MAGMA_INCR(stats.bindDescriptorSetCount, 1);
}

void CommandBuffer::bindVertexBuffers(uint32_t firstBinding, const std::initializer_list<lent_ptr<const Buffer>>& vertexBuffers,
    const std::initializer_list<VkDeviceSize>& offsets /* empty */) noexcept
{
    auto unmanagedVertexBuffers = stackalloc(const Buffer*, vertexBuffers.size());
    uint32_t bindingCount = 0;
    for (auto const& buffer: vertexBuffers)
    {
        unmanagedVertexBuffers[bindingCount++] = buffer.get();
        MAGMA_INUSE(buffer);
    }
    leanCmd.bindVertexBuffers(firstBinding, bindingCount, unmanagedVertexBuffers, offsets.begin());
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_VERTEX_INPUT_BIT);
    MAGMA_INCR(stats.bindVertexBufferCount, 1);
}

void CommandBuffer::copyBuffer(lent_ptr<const Buffer> srcBuffer, lent_ptr<Buffer> dstBuffer,
    VkDeviceSize srcOffset /* 0 */, VkDeviceSize dstOffset /* 0 */, VkDeviceSize size /* VK_WHOLE_SIZE */) const noexcept
{
    VkBufferCopy region;
    region.srcOffset = srcOffset;
    region.dstOffset = dstOffset;
    if (size < VK_WHOLE_SIZE)
        region.size = size;
    else
    {
        MAGMA_ASSERT(srcBuffer->getSize() >= dstBuffer->getSize());
        region.size = dstBuffer->getSize();
    }
    leanCmd.copyBuffer(srcBuffer.get(), dstBuffer.get(), region);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TRANSFER_BIT);
    MAGMA_INUSE(srcBuffer);
    MAGMA_INUSE(dstBuffer);
}

 void CommandBuffer::copyImage(lent_ptr<const Image> srcImage, lent_ptr<Image> dstImage, uint32_t mipLevel) const noexcept
{
    VkImageCopy region;
    region.srcSubresource = srcImage->getSubresourceLayers(mipLevel);
    region.srcOffset = VkOffset3D{0, 0, 0};
    region.dstSubresource = dstImage->getSubresourceLayers(mipLevel);
    region.dstOffset = VkOffset3D{0, 0, 0};
    region.extent = srcImage->calculateMipExtent(mipLevel);
    copyImage(std::move(srcImage), std::move(dstImage), region);
}

void CommandBuffer::blitImage(lent_ptr<const Image> srcImage, lent_ptr<Image> dstImage,
    uint32_t srcMipLevel, uint32_t dstMipLevel, VkFilter filter) const noexcept
{
    VkExtent3D srcExtent = srcImage->calculateMipExtent(srcMipLevel);
    VkExtent3D dstExtent = dstImage->calculateMipExtent(dstMipLevel);
    VkImageBlit region;
    region.srcSubresource = srcImage->getSubresourceLayers(srcMipLevel);
    region.srcOffsets[0] = VkOffset3D{0, 0, 0};
    region.srcOffsets[1].x = srcExtent.width;
    region.srcOffsets[1].y = srcExtent.height;
    region.srcOffsets[1].z = 1;
    region.dstSubresource = dstImage->getSubresourceLayers(dstMipLevel);
    region.dstOffsets[0] = VkOffset3D{0, 0, 0};
    region.dstOffsets[1].x = dstExtent.width;
    region.dstOffsets[1].y = dstExtent.height;
    region.dstOffsets[1].z = 1;
    blitImage(std::move(srcImage), std::move(dstImage), region, filter);
}

void CommandBuffer::waitEvents(const std::initializer_list<lent_ptr<const Event>>& events, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
    const std::initializer_list<MemoryBarrier>& memoryBarriers /* empty */,
    const std::initializer_list<BufferMemoryBarrier>& bufferMemoryBarriers /* empty */,
    const std::initializer_list<ImageMemoryBarrier>& imageMemoryBarriers /* empty */) const noexcept
{
    auto unmanagedEvents = stackalloc(const Event*, events.size());
    uint32_t eventCount = 0;
    for (auto const& event: events)
    {
        unmanagedEvents[eventCount++] = event.get();
        MAGMA_INUSE(event);
    }
    leanCmd.waitEvents(eventCount, unmanagedEvents, srcStageMask, dstStageMask,
        core::countof(memoryBarriers), memoryBarriers.begin(),
        core::countof(bufferMemoryBarriers), bufferMemoryBarriers.begin(),
        core::countof(imageMemoryBarriers), imageMemoryBarriers.begin());
    for (auto const& barrier: imageMemoryBarriers)
        barrier.updateImageLayout();
    MAGMA_CHECKPOINT(dstStageMask);
}

void CommandBuffer::waitEvents(const std::vector<lent_ptr<const Event>>& events, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
    const std::vector<MemoryBarrier>& memoryBarriers, const std::vector<BufferMemoryBarrier>& bufferMemoryBarriers,
    const std::vector<ImageMemoryBarrier>& imageMemoryBarriers) const noexcept
{
    auto unmanagedEvents = stackalloc(const Event*, events.size());
    uint32_t eventCount = 0;
    for (auto const& event: events)
    {
        unmanagedEvents[eventCount++] = event.get();
        MAGMA_INUSE(event);
    }
    leanCmd.waitEvents(eventCount, unmanagedEvents, srcStageMask, dstStageMask,
        core::countof(memoryBarriers), memoryBarriers.data(),
        core::countof(bufferMemoryBarriers), bufferMemoryBarriers.data(),
        core::countof(imageMemoryBarriers), imageMemoryBarriers.data());
    for (auto const& barrier: imageMemoryBarriers)
        barrier.updateImageLayout();
    MAGMA_CHECKPOINT(dstStageMask);
}

void CommandBuffer::pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
    const std::initializer_list<MemoryBarrier>& memoryBarriers /* empty */,
    const std::initializer_list<BufferMemoryBarrier>& bufferMemoryBarriers /* empty */,
    const std::initializer_list<ImageMemoryBarrier>& imageMemoryBarriers /* empty */,
    VkDependencyFlags dependencyFlags /* 0 */) noexcept
{
    leanCmd.pipelineBarrier(srcStageMask, dstStageMask,
        core::countof(memoryBarriers), memoryBarriers.begin(),
        core::countof(bufferMemoryBarriers), bufferMemoryBarriers.begin(),
        core::countof(imageMemoryBarriers), imageMemoryBarriers.begin(),
        dependencyFlags);
    for (auto const& barrier: imageMemoryBarriers)
        barrier.updateImageLayout();
    MAGMA_CHECKPOINT(dstStageMask);
    MAGMA_INCR(stats.pipelineBarrierCount, 1);
}

void CommandBuffer::pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
    const std::vector<MemoryBarrier>& memoryBarriers,
    const std::vector<BufferMemoryBarrier>& bufferMemoryBarriers,
    const std::vector<ImageMemoryBarrier>& imageMemoryBarriers,
    VkDependencyFlags dependencyFlags /* 0 */) noexcept
{
    leanCmd.pipelineBarrier(srcStageMask, dstStageMask,
        core::countof(memoryBarriers), memoryBarriers.data(),
        core::countof(bufferMemoryBarriers), bufferMemoryBarriers.data(),
        core::countof(imageMemoryBarriers), imageMemoryBarriers.data(),
        dependencyFlags);
    for (auto const& barrier: imageMemoryBarriers)
        barrier.updateImageLayout();
    MAGMA_CHECKPOINT(dstStageMask);
    MAGMA_INCR(stats.pipelineBarrierCount, 1);
}

void CommandBuffer::beginRenderPass(lent_ptr<const RenderPass> renderPass, lent_ptr<Framebuffer> framebuffer,
    const std::initializer_list<ClearValue> clearValues /* empty */, const VkRect2D& renderArea_ /* {0, 0, 0, 0} */,
    VkSubpassContents contents /* VK_SUBPASS_CONTENTS_INLINE */) noexcept
{
    MAGMA_ASSERT(!renderingPass);
    VkRect2D renderArea;
    if (renderArea_.extent.width && renderArea_.extent.height)
        renderArea = renderArea_;
    else
    {
        renderArea.offset = VkOffset2D{0, 0};
        renderArea.extent = framebuffer->getExtent();
    }
    leanCmd.beginRenderPass(renderPass.get(), framebuffer.get(), core::countof(clearValues), (const VkClearValue *)clearValues.begin(), renderArea, contents);
    renderingPass = VK_TRUE;
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);
    MAGMA_INCR(stats.renderPassCount, 1);
    MAGMA_INUSE(renderPass);
    MAGMA_INUSE(framebuffer);
    renderPass->begin(framebuffer->getAttachments());
    renderPassState.renderPass = renderPass.get();
    renderPassState.framebuffer = framebuffer.get();
}

void CommandBuffer::endRenderPass() noexcept
{
    MAGMA_ASSERT(renderingPass);
    if (annotatedRenderPass)
    {
        popDebugMarker();
        annotatedRenderPass = VK_FALSE;
    }
    leanCmd.endRenderPass();
    renderingPass = VK_FALSE;
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);
    if (!renderPassState.attachments.empty())
        renderPassState.renderPass->end(renderPassState.attachments);
    else if (renderPassState.framebuffer)
        renderPassState.renderPass->end(renderPassState.framebuffer->getAttachments());
    renderPassState.renderPass = nullptr;
    renderPassState.framebuffer = nullptr;
    renderPassState.attachments.clear();
}

#ifdef VK_KHR_device_group
bool CommandBuffer::beginDeviceGroup(uint32_t deviceMask, VkCommandBufferUsageFlags flags /* 0 */) noexcept
{
    MAGMA_ASSERT(extensions.KHR_device_group);
    if (extensions.KHR_device_group)
    {
        const VkResult result = leanCmd.beginDeviceGroup(deviceMask, flags);
        MAGMA_ASSERT(VK_SUCCESS == result);
        if (VK_SUCCESS == result)
        {
            MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);
            state = State::Recording;
        }
        return (VK_SUCCESS == result);
    }
    return false;
}

void CommandBuffer::beginDeviceGroupRenderPass(uint32_t deviceMask, lent_ptr<const RenderPass> renderPass, lent_ptr<Framebuffer> framebuffer,
    const std::initializer_list<ClearValue>& clearValues /* empty */, const std::initializer_list<VkRect2D>& deviceRenderAreas /* empty */,
    VkSubpassContents contents /* VK_SUBPASS_CONTENTS_INLINE */) noexcept
{
    MAGMA_ASSERT(!renderingPass);
    MAGMA_ASSERT(extensions.KHR_device_group);
    if (extensions.KHR_device_group)
    {
        leanCmd.beginDeviceGroupRenderPass(deviceMask, renderPass.get(), framebuffer.get(),
            core::countof(clearValues), reinterpret_cast<const VkClearValue *>(clearValues.begin()),
            core::countof(deviceRenderAreas), deviceRenderAreas.begin(), contents);
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);
        MAGMA_INCR(stats.renderPassCount, 1);
        MAGMA_INUSE(renderPass);
        MAGMA_INUSE(framebuffer);
        renderPass->begin(framebuffer->getAttachments());
        renderPassState.renderPass = renderPass.get();
        renderPassState.framebuffer = framebuffer.get();
        renderingPass = VK_TRUE;
    }
}
#endif // VK_KHR_device_group

#ifdef VK_EXT_conditional_rendering
void CommandBuffer::beginConditionalRendering(lent_ptr<const Buffer> buffer, VkDeviceSize offset /* 0 */, bool inverted /* false */) noexcept
{
    MAGMA_ASSERT(!conditionalRendering);
    MAGMA_ASSERT(extensions.EXT_conditional_rendering);
    if (extensions.EXT_conditional_rendering)
    {
        leanCmd.beginConditionalRendering(buffer.get(), offset, inverted);
        conditionalRendering = VK_TRUE;
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_CONDITIONAL_RENDERING_BIT_EXT);
        MAGMA_INCR(stats.conditionalRenderingCount, 1);
        MAGMA_INUSE(buffer);
    }
}

void CommandBuffer::endConditionalRendering() noexcept
{
    MAGMA_ASSERT(conditionalRendering);
    MAGMA_ASSERT(extensions.EXT_conditional_rendering);
    if (extensions.EXT_conditional_rendering)
    {
        if (annotatedConditionalRendering)
        {
            popDebugMarker();
            annotatedConditionalRendering = VK_FALSE;
        }
        leanCmd.endConditionalRendering();
        conditionalRendering = VK_FALSE;
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_CONDITIONAL_RENDERING_BIT_EXT);
    }
}
#endif // VK_EXT_conditional_rendering

#ifdef VK_EXT_transform_feedback
void CommandBuffer::bindTransformFeedbackBuffers(uint32_t firstBinding, const std::initializer_list<lent_ptr<Buffer>>& transformFeedbackBuffers,
    const std::initializer_list<VkDeviceSize> offsets, const std::initializer_list<VkDeviceSize> sizes /* empty */) noexcept
{
    MAGMA_ASSERT(extensions.EXT_transform_feedback);
    if (extensions.EXT_transform_feedback)
    {
        auto unmanagedTransformFeedbackBuffers = stackalloc(Buffer*, transformFeedbackBuffers.size());
        uint32_t bindingCount = 0;
        for (auto const& buffer: transformFeedbackBuffers)
        {
            unmanagedTransformFeedbackBuffers[bindingCount++] = buffer.get();
            MAGMA_INUSE(buffer);
        }
        leanCmd.bindTransformFeedbackBuffers(firstBinding, bindingCount, unmanagedTransformFeedbackBuffers, offsets.begin(), sizes.begin());
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TRANSFORM_FEEDBACK_BIT_EXT);
        MAGMA_INCR(stats.bindTransformFeedbackBufferCount, 1);
    }
}

void CommandBuffer::beginTransformFeedback(uint32_t firstCounterBuffer, const std::initializer_list<lent_ptr<const Buffer>>& counterBuffers,
    const std::initializer_list<VkDeviceSize>& counterBufferOffsets /* empty */) noexcept
{
    MAGMA_ASSERT(!transformFeedback);
    MAGMA_ASSERT(extensions.EXT_transform_feedback);
    if (extensions.EXT_transform_feedback)
    {
        auto unmanagedCounterBuffers = stackalloc(const Buffer*, counterBuffers.size());
        uint32_t counterBufferCount = 0;
        for (auto const& buffer: counterBuffers)
        {
            unmanagedCounterBuffers[counterBufferCount++] = buffer.get();
            MAGMA_INUSE(buffer);
        }
        leanCmd.beginTransformFeedback(firstCounterBuffer, counterBufferCount, unmanagedCounterBuffers, counterBufferOffsets.begin());
        transformFeedback = VK_TRUE;
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TRANSFORM_FEEDBACK_BIT_EXT);
        MAGMA_INCR(stats.transformFeedbackCount, 1);
    }
}

void CommandBuffer::endTransformFeedback(uint32_t firstCounterBuffer, const std::initializer_list<lent_ptr<Buffer>>& counterBuffers,
    const std::initializer_list<VkDeviceSize> counterBufferOffsets /* empty */) noexcept
{
    MAGMA_ASSERT(transformFeedback);
    MAGMA_ASSERT(extensions.EXT_transform_feedback);
    if (extensions.EXT_transform_feedback)
    {
        if (annotatedTransformFeedback)
        {
            popDebugMarker();
            annotatedTransformFeedback = VK_FALSE;
        }
        auto unmanagedCounterBuffers = stackalloc(Buffer*, counterBuffers.size());
        uint32_t counterBufferCount = 0;
        for (auto const& buffer: counterBuffers)
        {
            unmanagedCounterBuffers[counterBufferCount++] = buffer.get();
            MAGMA_INUSE(buffer);
        }
        leanCmd.endTransformFeedback(firstCounterBuffer, counterBufferCount, unmanagedCounterBuffers, counterBufferOffsets.begin());
        transformFeedback = VK_FALSE;
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TRANSFORM_FEEDBACK_BIT_EXT);
    }
}
#endif // VK_EXT_transform_feedback

#ifdef VK_KHR_imageless_framebuffer
void CommandBuffer::beginRenderPass(lent_ptr<const RenderPass> renderPass, lent_ptr<ImagelessFramebuffer> framebuffer,
    const std::vector<lent_ptr<ImageView>>& attachments, const std::initializer_list<ClearValue> clearValues /* empty */,
    const VkRect2D& renderArea_ /* {0, 0, 0, 0} */, VkSubpassContents contents /* VK_SUBPASS_CONTENTS_INLINE */) noexcept
{
    MAGMA_ASSERT(!renderingPass);
    MAGMA_ASSERT(extensions.KHR_imageless_framebuffer);
    if (extensions.KHR_imageless_framebuffer)
    {
        auto unmanagedAttachments = stackalloc(ImageView*, attachments.size());
        uint32_t attachmentCount = 0;
        for (auto const& attachment: attachments)
        {
            unmanagedAttachments[attachmentCount++] = attachment.get();
            MAGMA_INUSE(attachment);
        }
        VkRect2D renderArea;
        if (renderArea_.extent.width && renderArea_.extent.height)
            renderArea = renderArea_;
        else
        {
            renderArea.offset = VkOffset2D{0, 0};
            renderArea.extent = framebuffer->getExtent();
        }
        leanCmd.beginRenderPass(renderPass.get(), framebuffer.get(), attachmentCount, unmanagedAttachments,
            core::countof(clearValues), reinterpret_cast<const VkClearValue *>(clearValues.begin()), renderArea, contents);
        renderingPass = VK_TRUE;
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);
        MAGMA_INCR(stats.renderPassCount, 1);
        MAGMA_INUSE(renderPass);
        MAGMA_INUSE(framebuffer);
        renderPass->begin(framebuffer->getAttachments());
        renderPassState.renderPass = renderPass.get();
        renderPassState.framebuffer = framebuffer.get();
        MAGMA_ASSERT(renderPassState.attachments.empty());
        for (auto const& attachment: attachments)
            renderPassState.attachments.push_back(attachment.get());
    }
}

#ifdef VK_KHR_device_group
void CommandBuffer::beginDeviceGroupRenderPass(uint32_t deviceMask, lent_ptr<const RenderPass> renderPass,
    lent_ptr<ImagelessFramebuffer> framebuffer, const std::vector<lent_ptr<ImageView>>& attachments,
    const std::initializer_list<ClearValue> clearValues /* empty */, const std::initializer_list<VkRect2D> deviceRenderAreas /* empty */,
    VkSubpassContents contents /* VK_SUBPASS_CONTENTS_INLINE */) noexcept
{
    MAGMA_ASSERT(!renderingPass);
    MAGMA_ASSERT(extensions.KHR_imageless_framebuffer && extensions.KHR_device_group);
    if (extensions.KHR_imageless_framebuffer && extensions.KHR_device_group)
    {
        auto unmanagedAttachments = stackalloc(ImageView*, attachments.size());
        uint32_t attachmentCount = 0;
        for (auto const& attachment: attachments)
        {
            unmanagedAttachments[attachmentCount++] = attachment.get();
            MAGMA_INUSE(attachment);
        }
        leanCmd.beginDeviceGroupRenderPass(deviceMask, renderPass.get(), framebuffer.get(), attachmentCount, unmanagedAttachments,
            core::countof(clearValues), reinterpret_cast<const VkClearValue *>(clearValues.begin()),
            core::countof(deviceRenderAreas), deviceRenderAreas.begin(), contents);
        renderingPass = VK_TRUE;
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);
        MAGMA_INCR(stats.renderPassCount, 1);
        MAGMA_INUSE(renderPass);
        MAGMA_INUSE(framebuffer);
        renderPass->begin(framebuffer->getAttachments());
        renderPassState.renderPass = renderPass.get();
        renderPassState.framebuffer = framebuffer.get();
        //renderPassState.attachments = attachments;
    }
}
#endif // VK_KHR_device_group
#endif // VK_KHR_imageless_framebuffer

#ifdef VK_KHR_acceleration_structure
void CommandBuffer::buildAccelerationStructure(lent_ptr<BottomLevelAccelerationStructure> accelerationStructure,
    const std::list<AccelerationStructureGeometry>& geometries, lent_ptr<Buffer> scratchBuffer) const noexcept
{
    MAGMA_ASSERT(extensions.KHR_acceleration_structure);
    if (extensions.KHR_acceleration_structure)
    {
        const uint32_t geometryCount = (uint32_t)std::distance(geometries.begin(), geometries.end());
        auto geometryPointers = stackalloc(const VkAccelerationStructureGeometryKHR*, geometryCount);
        auto buildRanges = stackalloc(VkAccelerationStructureBuildRangeInfoKHR, geometryCount);
        uint32_t i = 0;
        for (auto const& geometry: geometries)
        {
            geometryPointers[i] = &geometry;
            buildRanges[i++] = {geometry.primitiveCount};
        }
        leanCmd.buildAccelerationStructure(accelerationStructure.get(), geometryCount, geometryPointers, buildRanges, scratchBuffer.get());
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR);
        MAGMA_INUSE(accelerationStructure);
        MAGMA_INUSE(scratchBuffer);
    }
}

void CommandBuffer::buildAccelerationStructure(lent_ptr<BottomLevelAccelerationStructure> accelerationStructure,
    const std::list<AccelerationStructureGeometry>& geometries, const std::vector<VkAccelerationStructureBuildRangeInfoKHR>& buildRanges,
    lent_ptr<Buffer> scratchBuffer) const noexcept
{
    MAGMA_ASSERT(extensions.KHR_acceleration_structure);
    if (extensions.KHR_acceleration_structure)
    {
        const uint32_t geometryCount = (uint32_t)std::distance(geometries.begin(), geometries.end());
        auto geometryPointers = stackalloc(const VkAccelerationStructureGeometryKHR*, geometryCount);
        uint32_t i = 0;
        for (auto const& geometry: geometries)
            geometryPointers[i++] = &geometry;
        leanCmd.buildAccelerationStructure(accelerationStructure.get(), geometryCount, geometryPointers, buildRanges.data(), scratchBuffer.get());
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR);
        MAGMA_INUSE(accelerationStructure);
        MAGMA_INUSE(scratchBuffer);
    }
}

void CommandBuffer::updateAccelerationStructure(lent_ptr<BottomLevelAccelerationStructure> accelerationStructure,
    const std::list<AccelerationStructureGeometry>& geometries, lent_ptr<Buffer> scratchBuffer) const noexcept
{
    MAGMA_ASSERT(extensions.KHR_acceleration_structure);
    if (extensions.KHR_acceleration_structure)
    {
        const uint32_t geometryCount = (uint32_t)std::distance(geometries.begin(), geometries.end());
        auto geometryPointers = stackalloc(const VkAccelerationStructureGeometryKHR*, geometryCount);
        auto buildRanges = stackalloc(VkAccelerationStructureBuildRangeInfoKHR, geometryCount);
        uint32_t i = 0;
        for (auto const& geometry: geometries)
        {
            geometryPointers[i] = &geometry;
            buildRanges[i++] = {geometry.primitiveCount};
        }
        leanCmd.updateAccelerationStructure(accelerationStructure.get(), geometryCount, geometryPointers, buildRanges, scratchBuffer.get());
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR);
        MAGMA_INUSE(accelerationStructure);
        MAGMA_INUSE(scratchBuffer);
    }
}

void CommandBuffer::updateAccelerationStructure(lent_ptr<BottomLevelAccelerationStructure> accelerationStructure,
    const std::list<AccelerationStructureGeometry>& geometries, const std::vector<VkAccelerationStructureBuildRangeInfoKHR>& buildRanges,
    lent_ptr<Buffer> scratchBuffer) const noexcept
{
    MAGMA_ASSERT(extensions.KHR_acceleration_structure);
    if (extensions.KHR_acceleration_structure)
    {
        const uint32_t geometryCount = (uint32_t)std::distance(geometries.begin(), geometries.end());
        auto geometryPointers = stackalloc(const VkAccelerationStructureGeometryKHR*, geometryCount);
        uint32_t i = 0;
        for (auto const& geometry: geometries)
            geometryPointers[i++] = &geometry;
        leanCmd.updateAccelerationStructure(accelerationStructure.get(), geometryCount, geometryPointers, buildRanges.data(), scratchBuffer.get());
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR);
        MAGMA_INUSE(accelerationStructure);
        MAGMA_INUSE(scratchBuffer);
    }
}

void CommandBuffer::updateAccelerationStructureIndirect(lent_ptr<AccelerationStructure> accelerationStructure,
    const std::list<AccelerationStructureGeometry> geometries, lent_ptr<const Buffer> indirectBuildRanges,
    lent_ptr<Buffer> scratchBuffer) const noexcept
{
    MAGMA_ASSERT(extensions.KHR_acceleration_structure);
    if (extensions.KHR_acceleration_structure)
    {
        const uint32_t geometryCount = (uint32_t)std::distance(geometries.begin(), geometries.end());
        auto geometryPointers = stackalloc(const VkAccelerationStructureGeometryKHR*, geometryCount);
        auto maxPrimitiveCounts = stackalloc(uint32_t, geometryCount);
        uint32_t i = 0;
        for (auto const& geometry: geometries)
        {
            geometryPointers[i] = &geometry;
            maxPrimitiveCounts[i++] = geometry.primitiveCount;
        }
        leanCmd.updateAccelerationStructureIndirect(accelerationStructure.get(), geometryCount, geometryPointers, maxPrimitiveCounts,
            indirectBuildRanges.get(), scratchBuffer.get());
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR);
        MAGMA_INUSE(accelerationStructure);
        MAGMA_INUSE(indirectBuildRanges);
        MAGMA_INUSE(scratchBuffer);
    }
}
#endif // VK_KHR_acceleration_structure

const std::unique_ptr<Buffer>& CommandBuffer::getMarkerBuffer() const noexcept
{
    constexpr uint32_t MaxBufferMarkers = 1024;
    if (!markerBuffer) try
    {   /* Implementations may only support a limited number of
           pipelined marker write operations in flight at a given time,
           thus excessive number of marker write operations may degrade
           command execution performance. */
        constexpr VkDeviceSize size = MaxBufferMarkers * sizeof(uint32_t);
        markerBuffer = std::make_unique<DynamicStorageBuffer>(device, size, false);
    }
    catch (...) {}
    return markerBuffer;
}

void CommandBuffer::putInitialState() noexcept
{
    state = State::Initial;
    stats = Statistics();
    renderingPass = VK_FALSE;
    nonIndexedQuery = VK_FALSE;
    indexedQuery = VK_FALSE;
    conditionalRendering = VK_FALSE;
    transformFeedback = VK_FALSE;
    annotatedBegin = VK_FALSE;
    annotatedRenderPass = VK_FALSE;
    annotatedConditionalRendering = VK_FALSE;
    annotatedTransformFeedback = VK_FALSE;
    renderPassState.renderPass = nullptr;
    renderPassState.framebuffer = nullptr;
    renderPassState.attachments.clear();
    if (markerBuffer)
        markerBuffer->setPrivateData(0ull);
    checkpoints.clear();
#ifdef MAGMA_RETAIN_OBJECTS_IN_USE
    inUse.reserve(1000);
#endif
}

#define MAGMA_DEVICE_EXTENSION_ENABLED(name) name(device->extensionEnabled(MAGMA_EXTENSION_PREFIX #name))
#define MAGMA_INSTANCE_EXTENSION_ENABLED(name) name(device->getPhysicalDevice()->getInstance()->extensionEnabled(MAGMA_EXTENSION_PREFIX #name))

CommandBuffer::Extensions::Extensions(lent_ptr<const Device> device):
    MAGMA_DEVICE_EXTENSION_ENABLED(AMD_buffer_marker),
    MAGMA_DEVICE_EXTENSION_ENABLED(AMD_draw_indirect_count),
    MAGMA_DEVICE_EXTENSION_ENABLED(AMD_negative_viewport_height),
    MAGMA_DEVICE_EXTENSION_ENABLED(EXT_debug_marker),
    MAGMA_DEVICE_EXTENSION_ENABLED(EXT_conditional_rendering),
    MAGMA_DEVICE_EXTENSION_ENABLED(EXT_line_rasterization),
    MAGMA_DEVICE_EXTENSION_ENABLED(EXT_mesh_shader),
    MAGMA_DEVICE_EXTENSION_ENABLED(EXT_multi_draw),
    MAGMA_DEVICE_EXTENSION_ENABLED(EXT_transform_feedback),
    MAGMA_DEVICE_EXTENSION_ENABLED(KHR_acceleration_structure),
    MAGMA_DEVICE_EXTENSION_ENABLED(KHR_device_group),
    MAGMA_DEVICE_EXTENSION_ENABLED(KHR_draw_indirect_count),
    MAGMA_DEVICE_EXTENSION_ENABLED(KHR_imageless_framebuffer),
    MAGMA_DEVICE_EXTENSION_ENABLED(KHR_maintenance1),
    MAGMA_DEVICE_EXTENSION_ENABLED(KHR_push_descriptor),
    MAGMA_DEVICE_EXTENSION_ENABLED(KHR_ray_tracing_pipeline),
    MAGMA_DEVICE_EXTENSION_ENABLED(KHR_ray_tracing_maintenance1),
    MAGMA_DEVICE_EXTENSION_ENABLED(NV_cluster_acceleration_structure),
    MAGMA_DEVICE_EXTENSION_ENABLED(NV_device_diagnostic_checkpoints),
    MAGMA_DEVICE_EXTENSION_ENABLED(NV_mesh_shader),
    MAGMA_INSTANCE_EXTENSION_ENABLED(EXT_debug_utils)
{}
} // namespace magma
