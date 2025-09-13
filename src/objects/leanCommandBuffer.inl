namespace magma
{
inline VkResult LeanCommandBuffer::end() noexcept
{
    return vkEndCommandBuffer(handle);
}

inline VkResult LeanCommandBuffer::reset(VkCommandBufferResetFlags flags /* 0 */) noexcept
{
    return vkResetCommandBuffer(handle, flags);
}

inline void LeanCommandBuffer::bindPipeline(const Pipeline *pipeline) noexcept
{
    vkCmdBindPipeline(handle, pipeline->getBindPoint(), *pipeline);
}

inline void LeanCommandBuffer::setViewport(const VkViewport& viewport) noexcept
{
    vkCmdSetViewport(handle, 0, 1, &viewport);
}

inline void LeanCommandBuffer::setViewports(uint32_t firstViewport, uint32_t viewportCount, const VkViewport *viewports) noexcept
{
    vkCmdSetViewport(handle, firstViewport, viewportCount, viewports);
}

inline void LeanCommandBuffer::setScissor(const VkRect2D& scissor) noexcept
{
    vkCmdSetScissor(handle, 0, 1, &scissor);
}

inline void LeanCommandBuffer::setScissors(uint32_t firstScissor, uint32_t scissorCount, const VkRect2D *scissors) noexcept
{
    vkCmdSetScissor(handle, firstScissor, scissorCount, scissors);
}

inline void LeanCommandBuffer::setLineWidth(float lineWidth) noexcept
{
    vkCmdSetLineWidth(handle, lineWidth);
}

inline void LeanCommandBuffer::setDepthBias(float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) noexcept
{
    vkCmdSetDepthBias(handle, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
}

inline void LeanCommandBuffer::setBlendConstants(const float blendConstants[4]) noexcept
{
    vkCmdSetBlendConstants(handle, blendConstants);
}

inline void LeanCommandBuffer::setDepthBounds(float minDepthBounds, float maxDepthBounds) noexcept
{
    vkCmdSetDepthBounds(handle, minDepthBounds, maxDepthBounds);
}

inline void LeanCommandBuffer::setStencilCompareMask(bool frontFace, bool backFace, uint32_t compareMask) noexcept
{
    vkCmdSetStencilCompareMask(handle, MAGMA_STENCIL_FACE_MASK(frontFace, backFace), compareMask);
}

inline void LeanCommandBuffer::setStencilWriteMask(bool frontFace, bool backFace, uint32_t writeMask) noexcept
{
    vkCmdSetStencilWriteMask(handle, MAGMA_STENCIL_FACE_MASK(frontFace, backFace), writeMask);
}

inline void LeanCommandBuffer::setStencilReference(bool frontFace, bool backFace, uint32_t reference) noexcept
{
    vkCmdSetStencilReference(handle, MAGMA_STENCIL_FACE_MASK(frontFace, backFace), reference);
}

inline void LeanCommandBuffer::bindDescriptorSet(VkPipelineBindPoint bindPoint, const PipelineLayout *layout, uint32_t firstSet, const DescriptorSet *descriptorSet,
    uint32_t dynamicOffsetCount /* 0 */, const uint32_t *dynamicOffsets /* nullptr */) noexcept
{
    vkCmdBindDescriptorSets(handle, bindPoint, *layout, firstSet, 1, descriptorSet->getHandleAddress(), dynamicOffsetCount, dynamicOffsets);
}

inline void LeanCommandBuffer::bindDescriptorSets(VkPipelineBindPoint bindPoint, const PipelineLayout *layout, uint32_t firstSet, uint32_t descriptorSetCount, const DescriptorSet **descriptorSets,
    uint32_t dynamicOffsetCount /* 0 */, const uint32_t *dynamicOffsets /* nullptr */) noexcept
{
    auto dereferencedDescriptorSets = stackalloc(VkDescriptorSet, descriptorSetCount);
    for (uint32_t i = 0; i < descriptorSetCount; ++i)
        dereferencedDescriptorSets[i] = *descriptorSets[i];
    vkCmdBindDescriptorSets(handle, bindPoint, *layout, firstSet, descriptorSetCount, dereferencedDescriptorSets, dynamicOffsetCount, dynamicOffsets);
}

inline void LeanCommandBuffer::bindIndexBuffer(const BaseIndexBuffer *indexBuffer, VkDeviceSize offset /* 0 */) noexcept
{
    vkCmdBindIndexBuffer(handle, indexBuffer->getHandle(), offset, indexBuffer->getIndexType());
}

inline void LeanCommandBuffer::bindVertexBuffer(uint32_t firstBinding, const Buffer *vertexBuffer, VkDeviceSize offset /* 0 */) noexcept
{
    MAGMA_ASSERT(vertexBuffer->getUsage() & VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    vkCmdBindVertexBuffers(handle, firstBinding, 1, vertexBuffer->getHandleAddress(), &offset);
}

inline void LeanCommandBuffer::bindVertexBuffers(uint32_t firstBinding, uint32_t bindingCount, const Buffer **vertexBuffers, const VkDeviceSize *offsets) noexcept
{
    auto dereferencedVertexBuffers = stackalloc(VkBuffer, bindingCount);
    for (uint32_t i = 0; i < bindingCount; ++i)
    {
        MAGMA_ASSERT(vertexBuffers[i]->getUsage() & VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
        dereferencedVertexBuffers[i] = *vertexBuffers[i];
    }
    vkCmdBindVertexBuffers(handle, firstBinding, bindingCount, dereferencedVertexBuffers, offsets);
}

inline void LeanCommandBuffer::draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) const noexcept
{
    vkCmdDraw(handle, vertexCount, instanceCount, firstVertex, firstInstance);
}

inline void LeanCommandBuffer::drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) const noexcept
{
    vkCmdDrawIndexed(handle, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

inline void LeanCommandBuffer::drawIndirect(const Buffer *buffer, uint32_t drawCount, VkDeviceSize offset /* 0 */) const noexcept
{
    MAGMA_ASSERT(buffer->getUsage() & VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT);
    vkCmdDrawIndirect(handle, *buffer, offset, drawCount, sizeof(VkDrawIndirectCommand));
}

inline void LeanCommandBuffer::drawIndexedIndirect(const Buffer *buffer, uint32_t drawCount, VkDeviceSize offset /* 0 */) const noexcept
{
    MAGMA_ASSERT(buffer->getUsage() & VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT);
    vkCmdDrawIndexedIndirect(handle, *buffer, offset, drawCount, sizeof(VkDrawIndexedIndirectCommand));
}

inline void LeanCommandBuffer::dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const noexcept
{
    vkCmdDispatch(handle, groupCountX, groupCountY, groupCountZ);
}

inline void LeanCommandBuffer::dispatchIndirect(const Buffer *buffer, VkDeviceSize offset /* 0 */) const noexcept
{
    MAGMA_ASSERT(buffer->getUsage() & VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT);
    vkCmdDispatchIndirect(handle, *buffer, offset);
}

inline void LeanCommandBuffer::copyBuffer(const Buffer *srcBuffer, Buffer *dstBuffer, const VkBufferCopy& region) const noexcept
{
    MAGMA_ASSERT(srcBuffer->getUsage() & VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
    MAGMA_ASSERT(dstBuffer->getUsage() & VK_BUFFER_USAGE_TRANSFER_DST_BIT);
    vkCmdCopyBuffer(handle, *srcBuffer, *dstBuffer, 1, &region);
}

inline void LeanCommandBuffer::copyImage(const Image *srcImage, Image *dstImage, const VkImageCopy& region) const noexcept
{
    VkImageLayout srcImageLayout = srcImage->getLayout(region.srcSubresource.mipLevel);
    VkImageLayout dstImageLayout = dstImage->getLayout(region.dstSubresource.mipLevel);
    vkCmdCopyImage(handle, *srcImage, srcImageLayout, *dstImage, dstImageLayout, 1, &region);
}

inline void LeanCommandBuffer::blitImage(const Image *srcImage, Image *dstImage, const VkImageBlit& region, VkFilter filter) const noexcept
{
    VkImageLayout srcImageLayout = srcImage->getLayout(region.srcSubresource.mipLevel);
    VkImageLayout dstImageLayout = dstImage->getLayout(region.dstSubresource.mipLevel);
    vkCmdBlitImage(handle, *srcImage, srcImageLayout, *dstImage, dstImageLayout, 1, &region, filter);
}

inline void LeanCommandBuffer::copyBufferToImage(const Buffer *srcBuffer, Image *dstImage, uint32_t regionCount, const VkBufferImageCopy *regions) const noexcept
{
    MAGMA_ASSERT(srcBuffer->getUsage() & VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
    const VkImageLayout dstImageLayout = dstImage->getLayout(regions[0].imageSubresource.mipLevel);
    vkCmdCopyBufferToImage(handle, *srcBuffer, *dstImage, dstImageLayout, regionCount, regions);
}

inline void LeanCommandBuffer::copyImageToBuffer(const Image *srcImage, Buffer *dstBuffer, uint32_t regionCount, const VkBufferImageCopy *regions) const noexcept
{
    MAGMA_ASSERT(dstBuffer->getUsage() & VK_BUFFER_USAGE_TRANSFER_DST_BIT);
    VkImageLayout srcImageLayout = srcImage->getLayout(regions[0].imageSubresource.mipLevel);
    vkCmdCopyImageToBuffer(handle, *srcImage, srcImageLayout, *dstBuffer, regionCount, regions);
}

inline void LeanCommandBuffer::updateBuffer(Buffer *buffer, VkDeviceSize dataSize, const void *data, VkDeviceSize offset /* 0 */) const noexcept
{   /* Buffer updates performed with vkCmdUpdateBuffer first copy the data
       into command buffer memory when the command is recorded
       (which requires additional storage and may incur an additional allocation),
       and then copy the data from the command buffer into dstBuffer
       when the command is executed on a device. */
    MAGMA_ASSERT(buffer->getUsage() & VK_BUFFER_USAGE_TRANSFER_DST_BIT);
    vkCmdUpdateBuffer(handle, *buffer, offset, dataSize,
        reinterpret_cast<const uint32_t *>(data)); // Cast to compile with old SDK
}

inline void LeanCommandBuffer::fillBuffer(Buffer *buffer, uint32_t data,
    VkDeviceSize size /* VK_WHOLE_SIZE */, VkDeviceSize offset /* 0 */) const noexcept
{
    vkCmdFillBuffer(handle, *buffer, offset, size, data);
}

inline void LeanCommandBuffer::clearColorImage(Image *image, const VkClearColorValue& color, const VkImageSubresourceRange& range) const noexcept
{
    vkCmdClearColorImage(handle, *image, image->getLayout(0), &color, 1, &range);
}

inline void LeanCommandBuffer::clearDepthStencilImage(Image *image, const VkClearDepthStencilValue& depthStencil, const VkImageSubresourceRange& range) const noexcept
{
    vkCmdClearDepthStencilImage(handle, *image, image->getLayout(0), &depthStencil, 1, &range);
}

inline void LeanCommandBuffer::clearAttachments(uint32_t attachmentCount, const VkClearAttachment *attachments, const VkClearRect& clearRect) const noexcept
{
    vkCmdClearAttachments(handle, attachmentCount, attachments, 1, &clearRect);
}

inline void LeanCommandBuffer::resolveImage(const Image *srcImage, Image *dstImage, const VkImageResolve& region) const noexcept
{
    VkImageLayout srcImageLayout = srcImage->getLayout(region.srcSubresource.mipLevel);
    VkImageLayout dstImageLayout = dstImage->getLayout(region.dstSubresource.mipLevel);
    vkCmdResolveImage(handle, *srcImage, srcImageLayout, *dstImage, dstImageLayout, 1, &region);
}

inline void LeanCommandBuffer::setEvent(Event *event, VkPipelineStageFlags stageMask) const noexcept
{
    vkCmdSetEvent(handle, *event, stageMask);
}

inline void LeanCommandBuffer::resetEvent(Event *event, VkPipelineStageFlags stageMask) const noexcept
{
    vkCmdResetEvent(handle, *event, stageMask);
}

inline void LeanCommandBuffer::waitEvent(const Event *event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask) const noexcept
{
    vkCmdWaitEvents(handle, 1, event->getHandleAddress(), srcStageMask, dstStageMask, 0, nullptr, 0, nullptr, 0, nullptr);
}

inline void LeanCommandBuffer::waitEvent(const Event *event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const VkMemoryBarrier& barrier) const noexcept
{
    vkCmdWaitEvents(handle, 1, event->getHandleAddress(), srcStageMask, dstStageMask, 1, &barrier, 0, nullptr, 0, nullptr);
}

inline void LeanCommandBuffer::waitEvent(const Event *event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const VkBufferMemoryBarrier& barrier) const noexcept
{
    vkCmdWaitEvents(handle, 1, event->getHandleAddress(), srcStageMask, dstStageMask, 0, nullptr, 1, &barrier, 0, nullptr);
}

inline void LeanCommandBuffer::waitEvent(const Event *event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const VkImageMemoryBarrier& barrier) const noexcept
{
    vkCmdWaitEvents(handle, 1, event->getHandleAddress(), srcStageMask, dstStageMask, 0, nullptr, 0, nullptr, 1, &barrier);
}

inline void LeanCommandBuffer::waitEvent(const Event *event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier *memoryBarriers) const noexcept
{
    vkCmdWaitEvents(handle, 1, event->getHandleAddress(), srcStageMask, dstStageMask, memoryBarrierCount, memoryBarriers, 0, nullptr, 0, nullptr);
}

inline void LeanCommandBuffer::waitEvent(const Event *event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier *bufferMemoryBarriers) const noexcept
{
    vkCmdWaitEvents(handle, 1, event->getHandleAddress(), srcStageMask, dstStageMask, 0, nullptr, bufferMemoryBarrierCount, bufferMemoryBarriers, 0, nullptr);
}

inline void LeanCommandBuffer::waitEvent(const Event *event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier *imageMemoryBarriers) const noexcept
{
    vkCmdWaitEvents(handle, 1, event->getHandleAddress(), srcStageMask, dstStageMask, 0, nullptr, 0, nullptr, imageMemoryBarrierCount, imageMemoryBarriers);
}

inline void LeanCommandBuffer::waitEvents(uint32_t eventCount, const Event **events, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
    uint32_t memoryBarrierCount, const VkMemoryBarrier *memoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier *bufferMemoryBarriers,
    uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier *imageMemoryBarriers) const noexcept
{
    auto dereferencedEvents = stackalloc(VkEvent, eventCount);
    for (uint32_t i = 0; i < eventCount; ++i)
        dereferencedEvents[i] = *events[i];
    vkCmdWaitEvents(handle, eventCount, dereferencedEvents, srcStageMask, dstStageMask,
        memoryBarrierCount, memoryBarriers,
        bufferMemoryBarrierCount, bufferMemoryBarriers,
        imageMemoryBarrierCount, imageMemoryBarriers);
}

inline void LeanCommandBuffer::pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags /* 0 */) noexcept
{
    vkCmdPipelineBarrier(handle, srcStageMask, dstStageMask, dependencyFlags, 0, nullptr, 0, nullptr, 0, nullptr);
}

inline void LeanCommandBuffer::pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const VkMemoryBarrier& barrier, VkDependencyFlags dependencyFlags /* 0 */) noexcept
{
    vkCmdPipelineBarrier(handle, srcStageMask, dstStageMask, dependencyFlags, 1, &barrier, 0, nullptr, 0, nullptr);
}

inline void LeanCommandBuffer::pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const VkBufferMemoryBarrier& barrier, VkDependencyFlags dependencyFlags /* 0 */) noexcept
{
    vkCmdPipelineBarrier(handle, srcStageMask, dstStageMask, dependencyFlags, 0, nullptr, 1, &barrier, 0, nullptr);
}

inline void LeanCommandBuffer::pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const VkImageMemoryBarrier& barrier, VkDependencyFlags dependencyFlags /* 0 */) noexcept
{
    vkCmdPipelineBarrier(handle, srcStageMask, dstStageMask, dependencyFlags, 0, nullptr, 0, nullptr, 1, &barrier);
}

inline void LeanCommandBuffer::pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier *memoryBarriers, VkDependencyFlags dependencyFlags /* 0 */) noexcept
{
    vkCmdPipelineBarrier(handle, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, memoryBarriers, 0, nullptr, 0, nullptr);
}

inline void LeanCommandBuffer::pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier *bufferMemoryBarriers, VkDependencyFlags dependencyFlags /* 0 */) noexcept
{
    vkCmdPipelineBarrier(handle, srcStageMask, dstStageMask, dependencyFlags, 0, nullptr, bufferMemoryBarrierCount, bufferMemoryBarriers, 0, nullptr);
}

inline void LeanCommandBuffer::pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier *imageMemoryBarriers, VkDependencyFlags dependencyFlags /* 0 */) noexcept
{
    vkCmdPipelineBarrier(handle, srcStageMask, dstStageMask, dependencyFlags, 0, nullptr, 0, nullptr, imageMemoryBarrierCount, imageMemoryBarriers);
}

inline void LeanCommandBuffer::pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier *memoryBarriers,
    uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier *bufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier *imageMemoryBarriers, VkDependencyFlags dependencyFlags /* 0 */) noexcept
{
    vkCmdPipelineBarrier(handle, srcStageMask, dstStageMask, dependencyFlags,
        memoryBarrierCount, memoryBarriers,
        bufferMemoryBarrierCount, bufferMemoryBarriers,
        imageMemoryBarrierCount, imageMemoryBarriers);
}

inline void LeanCommandBuffer::beginQuery(QueryPool *queryPool, uint32_t query) noexcept
{
    vkCmdBeginQuery(handle, *queryPool, query, queryPool->getControlFlags());
}

inline void LeanCommandBuffer::endQuery(QueryPool *queryPool, uint32_t query) noexcept
{
    vkCmdEndQuery(handle, *queryPool, query);
}

inline void LeanCommandBuffer::resetQueryPool(QueryPool *queryPool, uint32_t  firstQuery, uint32_t queryCount) const noexcept
{
    vkCmdResetQueryPool(handle, *queryPool, firstQuery, queryCount);
}

inline void LeanCommandBuffer::writeTimestamp(VkPipelineStageFlagBits pipelineStage, QueryPool *queryPool, uint32_t query) const noexcept
{
    vkCmdWriteTimestamp(handle, pipelineStage, *queryPool, query);
}

inline void LeanCommandBuffer::copyQueryPoolResults(const QueryPool *queryPool, uint32_t firstQuery, uint32_t queryCount,
    Buffer *dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags) const noexcept
{
    vkCmdCopyQueryPoolResults(handle, *queryPool, firstQuery, queryCount, *dstBuffer, dstOffset, stride, flags);
}

inline void LeanCommandBuffer::pushConstants(const PipelineLayout *layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void *values) noexcept
{
    vkCmdPushConstants(handle, *layout, stageFlags, offset, size, values);
}

inline void LeanCommandBuffer::nextSubpass(VkSubpassContents contents /* VK_SUBPASS_CONTENTS_INLINE */) noexcept
{
    vkCmdNextSubpass(handle, contents);
}

inline void LeanCommandBuffer::endRenderPass() noexcept
{
    vkCmdEndRenderPass(handle);
}

inline void LeanCommandBuffer::executeCommands(uint32_t cmdBufferCount, const LeanCommandBuffer **cmdBuffers) noexcept
{
    auto dereferencedCmdBuffers = stackalloc(VkCommandBuffer, cmdBufferCount);
    for (uint32_t i = 0; i < cmdBufferCount; ++i)
        dereferencedCmdBuffers[i] = *cmdBuffers[i];
    vkCmdExecuteCommands(handle, cmdBufferCount, dereferencedCmdBuffers);
}

#ifdef VK_KHR_device_group
inline void LeanCommandBuffer::setDeviceMask(uint32_t deviceMask) noexcept
{
    MAGMA_DEVICE_EXTENSION(vkCmdSetDeviceMaskKHR);
    vkCmdSetDeviceMaskKHR(handle, deviceMask);
}

inline void LeanCommandBuffer::dispatchBase(uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const noexcept
{
    MAGMA_DEVICE_EXTENSION(vkCmdDispatchBaseKHR);
    vkCmdDispatchBaseKHR(handle, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
}
#endif // VK_KHR_device_group

#ifdef VK_EXT_line_rasterization
inline void LeanCommandBuffer::setLineStipple(uint32_t stippleFactor, uint16_t stipplePattern) noexcept
{
    MAGMA_DEVICE_EXTENSION(vkCmdSetLineStippleEXT);
    vkCmdSetLineStippleEXT(handle, stippleFactor, stipplePattern);
}
#endif // VK_EXT_line_rasterization

#ifdef VK_KHR_push_descriptor
inline void LeanCommandBuffer::pushDescriptorSet(VkPipelineBindPoint bindPoint, const PipelineLayout *layout, uint32_t set, const DescriptorSet *descriptorSet) noexcept
{
    auto descriptorWrites = stackalloc(VkWriteDescriptorSet, descriptorSet->getDescriptorCount());
    uint32_t descriptorWriteCount = descriptorSet->writeDescriptors(descriptorWrites);
    MAGMA_DEVICE_EXTENSION(vkCmdPushDescriptorSetKHR);
    vkCmdPushDescriptorSetKHR(handle, bindPoint, *layout, set, descriptorWriteCount, descriptorWrites);
}
#endif // VK_KHR_push_descriptor

#ifdef VK_EXT_multi_draw
inline void LeanCommandBuffer::drawMulti(uint32_t drawCount, const VkMultiDrawInfoEXT *vertexInfo, uint32_t instanceCount, uint32_t firstInstance) const noexcept
{
    MAGMA_DEVICE_EXTENSION(vkCmdDrawMultiEXT);
    vkCmdDrawMultiEXT(handle, drawCount, vertexInfo, instanceCount, firstInstance, sizeof(VkMultiDrawInfoEXT));
}

inline void LeanCommandBuffer::drawMultiIndexed(uint32_t drawCount, const VkMultiDrawIndexedInfoEXT *indexInfo, uint32_t instanceCount, uint32_t firstInstance, const int32_t *vertexOffsets /* nullptr */) const noexcept
{
    MAGMA_DEVICE_EXTENSION(vkCmdDrawMultiIndexedEXT);
    vkCmdDrawMultiIndexedEXT(handle, drawCount, indexInfo, instanceCount, firstInstance, sizeof(VkMultiDrawIndexedInfoEXT), vertexOffsets);
}
#endif // VK_EXT_multi_draw

#if defined(VK_KHR_draw_indirect_count) || defined(VK_AMD_draw_indirect_count)
inline void LeanCommandBuffer::drawIndirectCount(const Buffer *buffer, const Buffer *countBuffer, uint32_t maxDrawCount, VkDeviceSize offset /* 0 */, VkDeviceSize countBufferOffset /* 0 */) const noexcept
{
    MAGMA_ASSERT(buffer->getUsage() & VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT);
    MAGMA_ASSERT(countBuffer->getUsage() & VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT);
#ifdef VK_KHR_draw_indirect_count
    MAGMA_DEVICE_EXTENSION(vkCmdDrawIndirectCountKHR);
    if (vkCmdDrawIndirectCountKHR)
        return vkCmdDrawIndirectCountKHR(handle, *buffer, offset, *countBuffer, countBufferOffset, maxDrawCount, sizeof(VkDrawIndirectCommand));
#endif // VK_KHR_draw_indirect_count
#ifdef VK_AMD_draw_indirect_count
    MAGMA_DEVICE_EXTENSION(vkCmdDrawIndirectCountAMD);
    vkCmdDrawIndirectCountAMD(handle, *buffer, offset, *countBuffer, countBufferOffset, maxDrawCount, sizeof(VkDrawIndirectCommand));
#endif // VK_AMD_draw_indirect_count
}

inline void LeanCommandBuffer::drawIndexedIndirectCount(const Buffer *buffer, const Buffer *countBuffer, uint32_t maxDrawCount, VkDeviceSize offset /* 0 */, VkDeviceSize countBufferOffset /* 0 */) const noexcept
{
    MAGMA_ASSERT(buffer->getUsage() & VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT);
    MAGMA_ASSERT(countBuffer->getUsage() & VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT);
#ifdef VK_KHR_draw_indirect_count
    MAGMA_DEVICE_EXTENSION(vkCmdDrawIndexedIndirectCountKHR);
    if (vkCmdDrawIndexedIndirectCountKHR)
        return vkCmdDrawIndexedIndirectCountKHR(handle, *buffer, offset, *countBuffer, countBufferOffset, maxDrawCount, sizeof(VkDrawIndexedIndirectCommand));
#endif // VK_KHR_draw_indirect_count
#ifdef VK_AMD_draw_indirect_count
    MAGMA_DEVICE_EXTENSION(vkCmdDrawIndexedIndirectCountAMD);
    vkCmdDrawIndexedIndirectCountAMD(handle, *buffer, offset, *countBuffer, countBufferOffset, maxDrawCount, sizeof(VkDrawIndexedIndirectCommand));
#endif // VK_AMD_draw_indirect_count
}
#endif // VK_KHR_draw_indirect_count || VK_AMD_draw_indirect_count

#if defined(VK_EXT_mesh_shader) || defined(VK_NV_mesh_shader)
inline void LeanCommandBuffer::drawMeshTasks(uint32_t groupCountX, uint32_t groupCountY /* 1 */, uint32_t groupCountZ /* 1 */) const noexcept
{
#ifdef VK_EXT_mesh_shader
    MAGMA_DEVICE_EXTENSION(vkCmdDrawMeshTasksEXT);
    if (vkCmdDrawMeshTasksEXT)
        return vkCmdDrawMeshTasksEXT(handle, groupCountX, groupCountY, groupCountZ);
#endif // VK_EXT_mesh_shader
#ifdef VK_NV_mesh_shader
    MAGMA_ASSERT(1 == groupCountY);
    MAGMA_ASSERT(1 == groupCountZ);
    MAGMA_UNUSED(groupCountY);
    MAGMA_UNUSED(groupCountZ);
    MAGMA_DEVICE_EXTENSION(vkCmdDrawMeshTasksNV);
    vkCmdDrawMeshTasksNV(handle, groupCountX, /* firstTask */ 0);
#endif // VK_NV_mesh_shader
}

inline void LeanCommandBuffer::drawMeshTasksIndirect(const Buffer *buffer, uint32_t drawCount, VkDeviceSize offset /* 0 */) const noexcept
{
    MAGMA_ASSERT(buffer->getUsage() & VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT);
#ifdef VK_EXT_mesh_shader
    MAGMA_DEVICE_EXTENSION(vkCmdDrawMeshTasksIndirectEXT);
    if (vkCmdDrawMeshTasksIndirectEXT)
        return vkCmdDrawMeshTasksIndirectEXT(handle, *buffer, offset, drawCount, sizeof(VkDrawMeshTasksIndirectCommandEXT));
#endif // VK_EXT_mesh_shader
#ifdef VK_NV_mesh_shader
    MAGMA_DEVICE_EXTENSION(vkCmdDrawMeshTasksIndirectNV);
    vkCmdDrawMeshTasksIndirectNV(handle, *buffer, offset, drawCount, sizeof(VkDrawMeshTasksIndirectCommandNV));
#endif // VK_NV_mesh_shader
}

inline void LeanCommandBuffer::drawMeshTasksIndirectCount(const Buffer *buffer, const Buffer *countBuffer, uint32_t maxDrawCount, VkDeviceSize offset /* 0 */, VkDeviceSize countBufferOffset /* 0 */) const noexcept
{
    MAGMA_ASSERT(buffer->getUsage() & VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT);
    MAGMA_ASSERT(countBuffer->getUsage() & VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT);
#ifdef VK_EXT_mesh_shader
    MAGMA_DEVICE_EXTENSION(vkCmdDrawMeshTasksIndirectCountEXT);
    if (vkCmdDrawMeshTasksIndirectCountEXT)
        return vkCmdDrawMeshTasksIndirectCountEXT(handle, *buffer, offset, *countBuffer, countBufferOffset, maxDrawCount, sizeof(VkDrawMeshTasksIndirectCommandEXT));
#endif // VK_EXT_mesh_shader
#ifdef VK_NV_mesh_shader
    MAGMA_DEVICE_EXTENSION(vkCmdDrawMeshTasksIndirectCountNV);
    vkCmdDrawMeshTasksIndirectCountNV(handle, *buffer, offset, *countBuffer, countBufferOffset, maxDrawCount, sizeof(VkDrawMeshTasksIndirectCommandNV));
#endif // VK_NV_mesh_shader
}
#endif // VK_EXT_mesh_shader || VK_NV_mesh_shader

#ifdef VK_EXT_conditional_rendering
inline void LeanCommandBuffer::endConditionalRendering() noexcept
{
    MAGMA_DEVICE_EXTENSION(vkCmdEndConditionalRenderingEXT);
    vkCmdEndConditionalRenderingEXT(handle);
}
#endif // VK_EXT_conditional_rendering

#ifdef VK_EXT_transform_feedback
inline void LeanCommandBuffer::bindTransformFeedbackBuffer(uint32_t firstBinding, Buffer *buffer, VkDeviceSize offset /* 0 */, VkDeviceSize size /* VK_WHOLE_SIZE */) noexcept
{
    MAGMA_ASSERT(buffer->getUsage() & VK_BUFFER_USAGE_TRANSFORM_FEEDBACK_BUFFER_BIT_EXT);
    MAGMA_DEVICE_EXTENSION(vkCmdBindTransformFeedbackBuffersEXT);
    vkCmdBindTransformFeedbackBuffersEXT(handle, firstBinding, 1, buffer->getHandleAddress(), &offset, &size);
}

inline void LeanCommandBuffer::bindTransformFeedbackBuffers(uint32_t firstBinding, uint32_t bindingCount, Buffer **buffers, const VkDeviceSize *offsets, const VkDeviceSize *sizes /* nullptr */) noexcept
{
    auto dereferencedBuffers = stackalloc(VkBuffer, bindingCount);
    for (uint32_t i = 0; i < bindingCount; ++i)
    {
        MAGMA_ASSERT(buffers[i]->getUsage() & VK_BUFFER_USAGE_TRANSFORM_FEEDBACK_BUFFER_BIT_EXT);
        dereferencedBuffers[i] = *buffers[i];
    }
    MAGMA_DEVICE_EXTENSION(vkCmdBindTransformFeedbackBuffersEXT);
    vkCmdBindTransformFeedbackBuffersEXT(handle, firstBinding, bindingCount, dereferencedBuffers, offsets, sizes);
}

inline void LeanCommandBuffer::beginQueryIndexed(QueryPool *queryPool, uint32_t query, uint32_t index) noexcept
{
    MAGMA_DEVICE_EXTENSION(vkCmdBeginQueryIndexedEXT);
    vkCmdBeginQueryIndexedEXT(handle, *queryPool, query, queryPool->getControlFlags(), index);
}

inline void LeanCommandBuffer::endQueryIndexed(QueryPool *queryPool, uint32_t query, uint32_t index) noexcept
{
    MAGMA_DEVICE_EXTENSION(vkCmdEndQueryIndexedEXT);
    vkCmdEndQueryIndexedEXT(handle, *queryPool, query, index);
}

inline void LeanCommandBuffer::drawIndirectByteCount(uint32_t instanceCount, uint32_t firstInstance, const Buffer *counterBuffer, uint32_t vertexStride,
    VkDeviceSize counterBufferOffset /* 0 */, uint32_t counterOffset /* 0 */) const noexcept
{
    MAGMA_ASSERT(counterBuffer->getUsage() & VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT);
    MAGMA_DEVICE_EXTENSION(vkCmdDrawIndirectByteCountEXT);
    vkCmdDrawIndirectByteCountEXT(handle, instanceCount, firstInstance, *counterBuffer, counterBufferOffset, counterOffset, vertexStride);
}
#endif // VK_EXT_transform_feedback

#ifdef VK_KHR_acceleration_structure
inline void LeanCommandBuffer::buildAccelerationStructure(BottomLevelAccelerationStructure *accelerationStructure, uint32_t geometryCount, const VkAccelerationStructureGeometryKHR **geometries,
    const VkAccelerationStructureBuildRangeInfoKHR *buildRanges, Buffer *scratchBuffer) const noexcept
{
    rebuildAccelerationStructure(VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR, accelerationStructure, geometryCount, geometries, buildRanges, scratchBuffer);
}

inline void LeanCommandBuffer::updateAccelerationStructure(BottomLevelAccelerationStructure *accelerationStructure, uint32_t geometryCount, const VkAccelerationStructureGeometryKHR **geometries,
    const VkAccelerationStructureBuildRangeInfoKHR *buildRanges, Buffer *scratchBuffer) const noexcept
{
    rebuildAccelerationStructure(VK_BUILD_ACCELERATION_STRUCTURE_MODE_UPDATE_KHR, accelerationStructure, geometryCount, geometries, buildRanges, scratchBuffer);
}

inline void LeanCommandBuffer::buildAccelerationStructure(TopLevelAccelerationStructure *accelerationStructure, const AccelerationStructureInstances& instances, Buffer *scratchBuffer) const noexcept
{
    rebuildAccelerationStructure(VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR, accelerationStructure, instances, scratchBuffer);
}

inline void LeanCommandBuffer::updateAccelerationStructure(TopLevelAccelerationStructure *accelerationStructure, const AccelerationStructureInstances& instances, Buffer *scratchBuffer) const noexcept
{
    rebuildAccelerationStructure(VK_BUILD_ACCELERATION_STRUCTURE_MODE_UPDATE_KHR, accelerationStructure, instances, scratchBuffer);
}
#endif // VK_KHR_acceleration_structure

#ifdef VK_NV_cluster_acceleration_structure
inline void LeanCommandBuffer::moveObjectsIndirect(ClusterAccelerationStructure *clusterAccelerationStructure, uint32_t maxAccelerationStructureCount, Buffer *scratchBuffer) const noexcept
{
    buildClusterAccelerationStructureIndirect(VK_CLUSTER_ACCELERATION_STRUCTURE_OP_TYPE_MOVE_OBJECTS_NV,
        clusterAccelerationStructure, maxAccelerationStructureCount, scratchBuffer, 0); // VkBuildAccelerationStructureFlagsKHR
}

inline void LeanCommandBuffer::buildBottomLevelClustersIndirect(ClusterAccelerationStructure *clusterAccelerationStructure, uint32_t maxAccelerationStructureCount, Buffer *scratchBuffer) const noexcept
{
    buildClusterAccelerationStructureIndirect(VK_CLUSTER_ACCELERATION_STRUCTURE_OP_TYPE_BUILD_CLUSTERS_BOTTOM_LEVEL_NV,
        clusterAccelerationStructure, maxAccelerationStructureCount, scratchBuffer, 0);
}

inline void LeanCommandBuffer::buildTriangleClusterIndirect(ClusterAccelerationStructure *clusterAccelerationStructure, uint32_t maxAccelerationStructureCount, Buffer *scratchBuffer) const noexcept
{
    buildClusterAccelerationStructureIndirect(VK_CLUSTER_ACCELERATION_STRUCTURE_OP_TYPE_BUILD_TRIANGLE_CLUSTER_NV,
        clusterAccelerationStructure, maxAccelerationStructureCount, scratchBuffer, 0);
}

inline void LeanCommandBuffer::buildTriangleClusterTemplateIndirect(ClusterAccelerationStructure *clusterAccelerationStructure, uint32_t maxAccelerationStructureCount, Buffer *scratchBuffer) const noexcept
{
    buildClusterAccelerationStructureIndirect(VK_CLUSTER_ACCELERATION_STRUCTURE_OP_TYPE_BUILD_TRIANGLE_CLUSTER_TEMPLATE_NV,
        clusterAccelerationStructure, maxAccelerationStructureCount, scratchBuffer, 0);
}

inline void LeanCommandBuffer::instantiateTriangleClusterIndirect(ClusterAccelerationStructure *clusterAccelerationStructure, uint32_t maxAccelerationStructureCount, Buffer *scratchBuffer) const noexcept
{
    buildClusterAccelerationStructureIndirect(VK_CLUSTER_ACCELERATION_STRUCTURE_OP_TYPE_INSTANTIATE_TRIANGLE_CLUSTER_NV,
        clusterAccelerationStructure, maxAccelerationStructureCount, scratchBuffer, 0);
}

inline void LeanCommandBuffer::getClusterTemplateIndicesIndirect(ClusterAccelerationStructure *clusterAccelerationStructure, uint32_t maxAccelerationStructureCount, Buffer *scratchBuffer) const noexcept
{
    buildClusterAccelerationStructureIndirect(VK_CLUSTER_ACCELERATION_STRUCTURE_OP_TYPE_GET_CLUSTER_TEMPLATE_INDICES_NV,
        clusterAccelerationStructure, maxAccelerationStructureCount, scratchBuffer, 0);
}
#endif // VK_NV_cluster_acceleration_structure

#ifdef VK_KHR_ray_tracing_pipeline
inline void LeanCommandBuffer::setRayTracingPipelineStackSize(uint32_t pipelineStackSize) const noexcept
{
    MAGMA_DEVICE_EXTENSION(vkCmdSetRayTracingPipelineStackSizeKHR);
    vkCmdSetRayTracingPipelineStackSizeKHR(handle, pipelineStackSize);
}
#endif // VK_KHR_ray_tracing_pipeline

#ifdef VK_KHR_ray_tracing_maintenance1
inline void LeanCommandBuffer::traceRaysIndirect(const Buffer *indirectBuffer) const noexcept
{
    MAGMA_ASSERT(indirectBuffer->getUsage() & VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT);
    MAGMA_DEVICE_EXTENSION(vkCmdTraceRaysIndirect2KHR);
    vkCmdTraceRaysIndirect2KHR(handle, indirectBuffer->getDeviceAddress());
}
#endif // VK_KHR_ray_tracing_maintenance1
} // namespace magma
