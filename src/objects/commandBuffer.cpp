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
#include "renderPass.h"
#include "fence.h"
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
    resetInternalState();
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
    resetInternalState();
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
        stats = DrawStatistics();
    #endif
    }
    usage = flags;
    return (VK_SUCCESS == result);
}

bool CommandBuffer::beginInherited(const std::shared_ptr<RenderPass>& renderPass, uint32_t subpass,
    const std::shared_ptr<Framebuffer>& framebuffer, bool occlusionQueryEnable /* false */, VkQueryControlFlags queryFlags /* 0 */,
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
        stats = DrawStatistics();
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
        resetInternalState();
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
}

void CommandBuffer::bindDescriptorSets(const std::shared_ptr<Pipeline>& pipeline, uint32_t firstSet, const std::initializer_list<std::shared_ptr<DescriptorSet>>& descriptorSets,
    const std::initializer_list<uint32_t>& dynamicOffsets /* empty */) noexcept
{
    MAGMA_STACK_ARRAY(const DescriptorSet*, unmanagedDescriptorSets, descriptorSets.size());
    for (auto const& descriptorSet: descriptorSets)
    {
        unmanagedDescriptorSets.put(descriptorSet.get());
        MAGMA_INUSE(descriptorSet);
    }
    leanCmd.bindDescriptorSets(pipeline->getBindPoint(), pipeline->getLayout().get(), firstSet, unmanagedDescriptorSets.count(), unmanagedDescriptorSets,
        core::countof(dynamicOffsets), dynamicOffsets.begin());
}

void CommandBuffer::bindVertexBuffers(uint32_t firstBinding, const std::initializer_list<std::shared_ptr<Buffer>>& vertexBuffers,
    std::initializer_list<VkDeviceSize> offsets /* empty */) noexcept
{
    MAGMA_STACK_ARRAY(const Buffer*, unmanagedVertexBuffers, vertexBuffers.size());
    for (auto const& buffer: vertexBuffers)
    {
        unmanagedVertexBuffers.put(buffer.get());
        MAGMA_INUSE(buffer);
    }
    leanCmd.bindVertexBuffers(firstBinding, unmanagedVertexBuffers.count(), unmanagedVertexBuffers, offsets.begin());
}

void CommandBuffer::copyBuffer(const std::shared_ptr<Buffer>& srcBuffer, const std::shared_ptr<Buffer>& dstBuffer,
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
    MAGMA_INUSE(srcBuffer);
    MAGMA_INUSE(dstBuffer);
}

 void CommandBuffer::copyImage(const std::shared_ptr<Image>& srcImage, const std::shared_ptr<Image>& dstImage, uint32_t mipLevel) const noexcept
{
    VkImageCopy region;
    region.srcSubresource = srcImage->getSubresourceLayers(mipLevel);
    region.srcOffset = VkOffset3D{0, 0, 0};
    region.dstSubresource = dstImage->getSubresourceLayers(mipLevel);
    region.dstOffset = VkOffset3D{0, 0, 0};
    region.extent = srcImage->calculateMipExtent(mipLevel);
    copyImage(srcImage, dstImage, region);
}

void CommandBuffer::blitImage(const std::shared_ptr<Image>& srcImage, const std::shared_ptr<Image>& dstImage,
    uint32_t srcMipLevel, uint32_t dstMipLevel, VkFilter filter) const noexcept
{
    VkExtent3D srcExtent = srcImage->calculateMipExtent(srcMipLevel);
    VkExtent3D dstExtent = dstImage->calculateMipExtent(dstMipLevel);
    VkImageBlit region;
    region.srcSubresource = srcImage->getSubresourceLayers(srcMipLevel);
    region.srcOffsets[0] = VkOffset3D{0, 0, 0};
    region.srcOffsets[1].x = srcExtent.width;
    region.srcOffsets[1].y = srcExtent.height,
    region.srcOffsets[1].z = 1;
    region.dstSubresource = dstImage->getSubresourceLayers(dstMipLevel);
    region.dstOffsets[0] = VkOffset3D{0, 0, 0};
    region.dstOffsets[1].x = dstExtent.width;
    region.dstOffsets[1].y = dstExtent.height,
    region.dstOffsets[1].z = 1;
    blitImage(srcImage, dstImage, region, filter);
}

void CommandBuffer::waitEvents(const std::initializer_list<std::shared_ptr<Event>>& events, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
    const std::initializer_list<VkMemoryBarrier>& memoryBarriers /* empty */,
    const std::initializer_list<VkBufferMemoryBarrier>& bufferMemoryBarriers /* empty */,
    const std::initializer_list<ImageMemoryBarrier>& imageMemoryBarriers_ /* empty */) const noexcept
{
    MAGMA_STACK_ARRAY(const Event*, unmanagedEvents, events.size());
    for (auto const& event: events)
    {
        unmanagedEvents.put(event.get());
        MAGMA_INUSE(event);
    }
    MAGMA_STACK_ARRAY(VkImageMemoryBarrier, imageMemoryBarriers, imageMemoryBarriers_.size());
    for (auto const& barrier: imageMemoryBarriers_)
    {
        imageMemoryBarriers.put(barrier);
        changeImageLayout(barrier);
    }
    leanCmd.waitEvents(unmanagedEvents.count(), unmanagedEvents, srcStageMask, dstStageMask,
        core::countof(memoryBarriers), memoryBarriers.begin(),
        core::countof(bufferMemoryBarriers), bufferMemoryBarriers.begin(),
        imageMemoryBarriers.count(), imageMemoryBarriers);
}

void CommandBuffer::waitEvents(const std::vector<std::shared_ptr<Event>>& events, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
    const std::vector<VkMemoryBarrier>& memoryBarriers, const std::vector<VkBufferMemoryBarrier>& bufferMemoryBarriers,
    const std::vector<ImageMemoryBarrier>& imageMemoryBarriers_) const noexcept
{
    MAGMA_STACK_ARRAY(const Event*, unmanagedEvents, events.size());
    for (auto const& event: events)
    {
        unmanagedEvents.put(event.get());
        MAGMA_INUSE(event);
    }
    MAGMA_STACK_ARRAY(VkImageMemoryBarrier, imageMemoryBarriers, imageMemoryBarriers_.size());
    for (auto const& barrier: imageMemoryBarriers_)
    {
        imageMemoryBarriers.put(barrier);
        changeImageLayout(barrier);
    }
    leanCmd.waitEvents(unmanagedEvents.count(), unmanagedEvents, srcStageMask, dstStageMask,
        core::countof(memoryBarriers), memoryBarriers.data(),
        core::countof(bufferMemoryBarriers), bufferMemoryBarriers.data(),
        imageMemoryBarriers.count(), imageMemoryBarriers);
}

void CommandBuffer::pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
    const std::initializer_list<VkMemoryBarrier>& memoryBarriers /* empty */,
    const std::initializer_list<VkBufferMemoryBarrier>& bufferMemoryBarriers /* empty */,
    const std::initializer_list<ImageMemoryBarrier>& imageMemoryBarriers_ /* empty */,
    VkDependencyFlags dependencyFlags /* 0 */) noexcept
{
    MAGMA_STACK_ARRAY(VkImageMemoryBarrier, imageMemoryBarriers, imageMemoryBarriers_.size());
    for (auto const& barrier: imageMemoryBarriers_)
    {
        imageMemoryBarriers.put(barrier);
        changeImageLayout(barrier);
    }
    leanCmd.pipelineBarrier(srcStageMask, dstStageMask,
        core::countof(memoryBarriers), memoryBarriers.begin(),
        core::countof(bufferMemoryBarriers), bufferMemoryBarriers.begin(),
        imageMemoryBarriers.count(), imageMemoryBarriers, dependencyFlags);
}

void CommandBuffer::pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
    const std::vector<VkMemoryBarrier>& memoryBarriers,
    const std::vector<VkBufferMemoryBarrier>& bufferMemoryBarriers,
    const std::vector<ImageMemoryBarrier>& imageMemoryBarriers_,
    VkDependencyFlags dependencyFlags /* 0 */) noexcept
{
    MAGMA_STACK_ARRAY(VkImageMemoryBarrier, imageMemoryBarriers, imageMemoryBarriers_.size());
    for (auto const& barrier: imageMemoryBarriers_)
    {
        imageMemoryBarriers.put(barrier);
        changeImageLayout(barrier);
    }
    leanCmd.pipelineBarrier(srcStageMask, dstStageMask,
        core::countof(memoryBarriers), memoryBarriers.data(),
        core::countof(bufferMemoryBarriers), bufferMemoryBarriers.data(),
        imageMemoryBarriers.count(), imageMemoryBarriers, dependencyFlags);
}

void CommandBuffer::beginRenderPass(const std::shared_ptr<RenderPass>& renderPass, const std::shared_ptr<Framebuffer>& framebuffer,
    const std::initializer_list<ClearValue>& clearValues /* empty */, const VkRect2D& renderArea_ /* {0, 0, 0, 0} */,
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
    MAGMA_INUSE(renderPass);
    MAGMA_INUSE(framebuffer);
    renderPass->begin(framebuffer->getAttachments());
    renderPassState.renderPass = renderPass;
    renderPassState.framebuffer = framebuffer;
    renderingPass = VK_TRUE;
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
    if (!renderPassState.attachments.empty())
        renderPassState.renderPass->end(renderPassState.attachments);
    else if (renderPassState.framebuffer)
        renderPassState.renderPass->end(renderPassState.framebuffer->getAttachments());
    renderPassState.renderPass.reset();
    renderPassState.framebuffer.reset();
    renderPassState.attachments.clear();
    renderingPass = VK_FALSE;
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
            state = State::Recording;
        return (VK_SUCCESS == result);
    }
    return false;
}

void CommandBuffer::beginDeviceGroupRenderPass(uint32_t deviceMask, const std::shared_ptr<RenderPass>& renderPass, const std::shared_ptr<Framebuffer>& framebuffer,
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
        MAGMA_INUSE(renderPass);
        MAGMA_INUSE(framebuffer);
        renderPass->begin(framebuffer->getAttachments());
        renderPassState.renderPass = renderPass;
        renderPassState.framebuffer = framebuffer;
        renderingPass = VK_TRUE;
    }
}
#endif // VK_KHR_device_group

#ifdef VK_EXT_conditional_rendering
void CommandBuffer::beginConditionalRendering(const std::shared_ptr<Buffer>& buffer, VkDeviceSize offset /* 0 */, bool inverted /* false */) noexcept
{
    MAGMA_ASSERT(!conditionalRendering);
    MAGMA_ASSERT(extensions.EXT_conditional_rendering);
    if (extensions.EXT_conditional_rendering)
    {
        leanCmd.beginConditionalRendering(buffer.get(), offset, inverted);
        MAGMA_INUSE(buffer);
        conditionalRendering = VK_TRUE;
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
    }
}
#endif // VK_EXT_conditional_rendering

#ifdef VK_EXT_transform_feedback
void CommandBuffer::bindTransformFeedbackBuffers(uint32_t firstBinding, const std::initializer_list<std::shared_ptr<Buffer>>& transformFeedbackBuffers,
    const std::initializer_list<VkDeviceSize>& offsets, const std::initializer_list<VkDeviceSize>& sizes /* empty */) noexcept
{
    MAGMA_ASSERT(extensions.EXT_transform_feedback);
    if (extensions.EXT_transform_feedback)
    {
        MAGMA_STACK_ARRAY(const Buffer*, unmanagedTransformFeedbackBuffers, transformFeedbackBuffers.size());
        for (auto const& buffer: transformFeedbackBuffers)
        {
            unmanagedTransformFeedbackBuffers.put(buffer.get());
            MAGMA_INUSE(buffer);
        }
        leanCmd.bindTransformFeedbackBuffers(firstBinding, unmanagedTransformFeedbackBuffers.count(), unmanagedTransformFeedbackBuffers, offsets.begin(), sizes.begin());
    }
}

void CommandBuffer::beginTransformFeedback(uint32_t firstCounterBuffer, const std::initializer_list<std::shared_ptr<Buffer>>& counterBuffers,
    const std::initializer_list<VkDeviceSize>& counterBufferOffsets /* empty */) noexcept
{
    MAGMA_ASSERT(!transformFeedback);
    MAGMA_ASSERT(extensions.EXT_transform_feedback);
    if (extensions.EXT_transform_feedback)
    {
        MAGMA_STACK_ARRAY(const Buffer*, unmanagedCounterBuffers, counterBuffers.size());
        for (auto const& buffer: counterBuffers)
        {
            unmanagedCounterBuffers.put(buffer.get());
            MAGMA_INUSE(buffer);
        }
        leanCmd.beginTransformFeedback(firstCounterBuffer, unmanagedCounterBuffers.count(), unmanagedCounterBuffers, counterBufferOffsets.begin());
        transformFeedback = VK_TRUE;
    }
}

void CommandBuffer::endTransformFeedback(uint32_t firstCounterBuffer, const std::initializer_list<std::shared_ptr<Buffer>>& counterBuffers,
    const std::initializer_list<VkDeviceSize>& counterBufferOffsets /* empty */) noexcept
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
        MAGMA_STACK_ARRAY(const Buffer*, unmanagedCounterBuffers, counterBuffers.size());
        for (auto const& buffer: counterBuffers)
        {
            unmanagedCounterBuffers.put(buffer.get());
            MAGMA_INUSE(buffer);
        }
        leanCmd.endTransformFeedback(firstCounterBuffer, unmanagedCounterBuffers.count(), unmanagedCounterBuffers, counterBufferOffsets.begin());
        transformFeedback = VK_FALSE;
    }
}
#endif // VK_EXT_transform_feedback

#ifdef VK_KHR_imageless_framebuffer
void CommandBuffer::beginRenderPass(const std::shared_ptr<RenderPass>& renderPass, const std::shared_ptr<ImagelessFramebuffer>& framebuffer,
    const std::vector<std::shared_ptr<ImageView>>& attachments, const std::initializer_list<ClearValue>& clearValues /* empty */,
    const VkRect2D& renderArea_ /* {0, 0, 0, 0} */, VkSubpassContents contents /* VK_SUBPASS_CONTENTS_INLINE */) noexcept
{
    MAGMA_ASSERT(!renderingPass);
    MAGMA_ASSERT(extensions.KHR_imageless_framebuffer);
    if (extensions.KHR_imageless_framebuffer)
    {
        MAGMA_STACK_ARRAY(const ImageView*, unmanagedAttachments, attachments.size());
        for (auto const& attachment: attachments)
        {
            unmanagedAttachments.put(attachment.get());
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
        leanCmd.beginRenderPass(renderPass.get(), framebuffer.get(), unmanagedAttachments.count(), unmanagedAttachments,
            core::countof(clearValues), reinterpret_cast<const VkClearValue *>(clearValues.begin()), renderArea, contents);
        MAGMA_INUSE(renderPass);
        MAGMA_INUSE(framebuffer);
        renderPass->begin(framebuffer->getAttachments());
        renderPassState.renderPass = renderPass;
        renderPassState.framebuffer = framebuffer;
        renderPassState.attachments = attachments;
        renderingPass = VK_TRUE;
    }
}

#ifdef VK_KHR_device_group
void CommandBuffer::beginDeviceGroupRenderPass(uint32_t deviceMask, const std::shared_ptr<RenderPass>& renderPass,
    const std::shared_ptr<ImagelessFramebuffer>& framebuffer, const std::vector<std::shared_ptr<ImageView>>& attachments,
    const std::initializer_list<ClearValue>& clearValues /* empty */, const std::initializer_list<VkRect2D>& deviceRenderAreas /* empty */,
    VkSubpassContents contents /* VK_SUBPASS_CONTENTS_INLINE */) noexcept
{
    MAGMA_ASSERT(!renderingPass);
    MAGMA_ASSERT(extensions.KHR_imageless_framebuffer && extensions.KHR_device_group);
    if (extensions.KHR_imageless_framebuffer && extensions.KHR_device_group)
    {
        MAGMA_STACK_ARRAY(const ImageView*, unmanagedAttachments, attachments.size());
        for (auto const& attachment: attachments)
        {
            unmanagedAttachments.put(attachment.get());
            MAGMA_INUSE(attachment);
        }
        leanCmd.beginDeviceGroupRenderPass(deviceMask, renderPass.get(), framebuffer.get(), unmanagedAttachments.count(), unmanagedAttachments,
            core::countof(clearValues), reinterpret_cast<const VkClearValue *>(clearValues.begin()),
            core::countof(deviceRenderAreas), deviceRenderAreas.begin(), contents);
        MAGMA_INUSE(renderPass);
        MAGMA_INUSE(framebuffer);
        renderPass->begin(framebuffer->getAttachments());
        renderPassState.renderPass = renderPass;
        renderPassState.framebuffer = framebuffer;
        renderPassState.attachments = attachments;
        renderingPass = VK_TRUE;
    }
}
#endif // VK_KHR_device_group
#endif // VK_KHR_imageless_framebuffer

#ifdef VK_KHR_acceleration_structure
void CommandBuffer::buildAccelerationStructure(const std::shared_ptr<BottomLevelAccelerationStructure>& accelerationStructure,
    const std::list<AccelerationStructureGeometry>& geometries, const std::shared_ptr<Buffer>& scratchBuffer) const noexcept
{
    MAGMA_ASSERT(extensions.KHR_acceleration_structure);
    if (extensions.KHR_acceleration_structure)
    {
        const uint32_t geometryCount = (uint32_t)std::distance(geometries.begin(), geometries.end());
        MAGMA_STACK_ARRAY(const VkAccelerationStructureGeometryKHR*, geometryPointers, geometryCount);
        MAGMA_STACK_ARRAY(VkAccelerationStructureBuildRangeInfoKHR, buildRanges, geometryCount);
        for (auto const& geometry: geometries)
        {
            geometryPointers.put(&geometry);
            buildRanges.put({geometry.primitiveCount});
        }
        leanCmd.buildAccelerationStructure(accelerationStructure.get(), geometryCount, geometryPointers, buildRanges, scratchBuffer.get());
        MAGMA_INUSE(accelerationStructure);
        MAGMA_INUSE(scratchBuffer);
    }
}

void CommandBuffer::buildAccelerationStructure(const std::shared_ptr<BottomLevelAccelerationStructure>& accelerationStructure,
    const std::list<AccelerationStructureGeometry>& geometries, const std::vector<VkAccelerationStructureBuildRangeInfoKHR>& buildRanges,
    const std::shared_ptr<Buffer>& scratchBuffer) const noexcept
{
    MAGMA_ASSERT(extensions.KHR_acceleration_structure);
    if (extensions.KHR_acceleration_structure)
    {
        const uint32_t geometryCount = (uint32_t)std::distance(geometries.begin(), geometries.end());
        MAGMA_STACK_ARRAY(const VkAccelerationStructureGeometryKHR*, geometryPointers, geometryCount);
        for (auto const& geometry: geometries)
            geometryPointers.put(&geometry);
        leanCmd.buildAccelerationStructure(accelerationStructure.get(), geometryCount, geometryPointers, buildRanges.data(), scratchBuffer.get());
        MAGMA_INUSE(accelerationStructure);
        MAGMA_INUSE(scratchBuffer);
    }
}

void CommandBuffer::updateAccelerationStructure(const std::shared_ptr<BottomLevelAccelerationStructure>& accelerationStructure,
    const std::list<AccelerationStructureGeometry>& geometries, const std::shared_ptr<Buffer>& scratchBuffer) const noexcept
{
    MAGMA_ASSERT(extensions.KHR_acceleration_structure);
    if (extensions.KHR_acceleration_structure)
    {
        const uint32_t geometryCount = (uint32_t)std::distance(geometries.begin(), geometries.end());
        MAGMA_STACK_ARRAY(const VkAccelerationStructureGeometryKHR*, geometryPointers, geometryCount);
        MAGMA_STACK_ARRAY(VkAccelerationStructureBuildRangeInfoKHR, buildRanges, geometryCount);
        for (auto const& geometry: geometries)
        {
            geometryPointers.put(&geometry);
            buildRanges.put({geometry.primitiveCount});
        }
        leanCmd.updateAccelerationStructure(accelerationStructure.get(), geometryCount, geometryPointers, buildRanges, scratchBuffer.get());
        MAGMA_INUSE(accelerationStructure);
        MAGMA_INUSE(scratchBuffer);
    }
}

void CommandBuffer::updateAccelerationStructure(const std::shared_ptr<BottomLevelAccelerationStructure>& accelerationStructure,
    const std::list<AccelerationStructureGeometry>& geometries, const std::vector<VkAccelerationStructureBuildRangeInfoKHR>& buildRanges,
    const std::shared_ptr<Buffer>& scratchBuffer) const noexcept
{
    MAGMA_ASSERT(extensions.KHR_acceleration_structure);
    if (extensions.KHR_acceleration_structure)
    {
        const uint32_t geometryCount = (uint32_t)std::distance(geometries.begin(), geometries.end());
        MAGMA_STACK_ARRAY(const VkAccelerationStructureGeometryKHR*, geometryPointers, geometryCount);
        for (auto const& geometry: geometries)
            geometryPointers.put(&geometry);
        leanCmd.updateAccelerationStructure(accelerationStructure.get(), geometryCount, geometryPointers, buildRanges.data(), scratchBuffer.get());
        MAGMA_INUSE(accelerationStructure);
        MAGMA_INUSE(scratchBuffer);
    }
}

void CommandBuffer::updateAccelerationStructureIndirect(const std::shared_ptr<AccelerationStructure>& accelerationStructure,
    const std::list<AccelerationStructureGeometry>& geometries, const std::shared_ptr<Buffer>& indirectBuildRanges,
    const std::shared_ptr<Buffer>& scratchBuffer) const noexcept
{
    MAGMA_ASSERT(extensions.KHR_acceleration_structure);
    if (extensions.KHR_acceleration_structure)
    {
        const uint32_t geometryCount = (uint32_t)std::distance(geometries.begin(), geometries.end());
        MAGMA_STACK_ARRAY(const VkAccelerationStructureGeometryKHR*, geometryPointers, geometryCount);
        MAGMA_STACK_ARRAY(uint32_t, maxPrimitiveCounts, geometryCount);
        for (auto const& geometry: geometries)
        {
            geometryPointers.put(&geometry);
            maxPrimitiveCounts.put(geometry.primitiveCount);
        }
        leanCmd.updateAccelerationStructureIndirect(accelerationStructure.get(), geometryCount, geometryPointers, maxPrimitiveCounts,
            indirectBuildRanges.get(), scratchBuffer.get());
        MAGMA_INUSE(accelerationStructure);
        MAGMA_INUSE(indirectBuildRanges);
        MAGMA_INUSE(scratchBuffer);
    }
}
#endif // VK_KHR_acceleration_structure

void CommandBuffer::resetInternalState() noexcept
{
    state = State::Initial;
    stats = DrawStatistics();
    renderingPass = VK_FALSE;
    nonIndexedQuery = VK_FALSE;
    indexedQuery = VK_FALSE;
    conditionalRendering = VK_FALSE;
    transformFeedback = VK_FALSE;
    annotatedBegin = VK_FALSE;
    annotatedRenderPass = VK_FALSE;
    annotatedConditionalRendering = VK_FALSE;
    annotatedTransformFeedback = VK_FALSE;
    renderPassState.renderPass.reset();
    renderPassState.framebuffer.reset();
    renderPassState.attachments.clear();
    markerOffset = 0ull;
}

#define MAGMA_DEVICE_EXTENSION_ENABLED(name) name(device->extensionEnabled(MAGMA_EXTENSION_PREFIX #name))
#define MAGMA_INSTANCE_EXTENSION_ENABLED(name) name(device->getPhysicalDevice()->getInstance()->extensionEnabled(MAGMA_EXTENSION_PREFIX #name))

CommandBuffer::Extensions::Extensions(std::shared_ptr<const Device> device):
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

    MAGMA_DEVICE_EXTENSION_ENABLED(NV_device_diagnostic_checkpoints),
    MAGMA_DEVICE_EXTENSION_ENABLED(NV_mesh_shader),

    MAGMA_INSTANCE_EXTENSION_ENABLED(EXT_debug_utils)
{}
} // namespace magma
