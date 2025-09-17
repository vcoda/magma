namespace magma
{
inline void CommandBuffer::bindPipeline(lent_ptr<const Pipeline> pipeline) noexcept
{
    leanCmd.bindPipeline(pipeline.get());
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);
    MAGMA_INCR(stats.bindPipelineCount, 1);
    MAGMA_INUSE(pipeline);
}

inline void CommandBuffer::setViewport(const VkViewport& viewport) noexcept
{
    leanCmd.setViewport(viewport);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);
}

inline void CommandBuffer::setViewport(uint32_t x, uint32_t y, uint32_t width, int32_t height, float minDepth /* 0 */, float maxDepth /* 1 */) noexcept
{
    setViewport((float)x, (float)y, (float)width, (float)height, minDepth, maxDepth);
}

inline void CommandBuffer::setViewports(uint32_t firstViewport, const std::initializer_list<VkViewport>& viewports) noexcept
{
    leanCmd.setViewports(firstViewport, core::countof(viewports), viewports.begin());
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);
}

inline void CommandBuffer::setScissor(const VkRect2D& scissor) noexcept
{
    leanCmd.setScissor(scissor);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);
}

inline void CommandBuffer::setScissor(int32_t x, int32_t y, uint32_t width, uint32_t height) noexcept
{
    VkRect2D scissor = {x, y, width, height};
    setScissor(scissor);
}

inline void CommandBuffer::setScissors(uint32_t firstViewport, const std::initializer_list<VkRect2D>& scissors) noexcept
{
    leanCmd.setScissors(firstViewport, core::countof(scissors), scissors.begin());
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);
}

inline void CommandBuffer::setLineWidth(float lineWidth) noexcept
{
    leanCmd.setLineWidth(lineWidth);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);
}

inline void CommandBuffer::setDepthBias(float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) noexcept
{
    leanCmd.setDepthBias(depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT);
}

inline void CommandBuffer::setBlendConstants(const float blendConstants[4]) noexcept
{
    leanCmd.setBlendConstants(blendConstants);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT);
}

inline void CommandBuffer::setDepthBounds(float minDepthBounds, float maxDepthBounds) noexcept
{
    leanCmd.setDepthBounds(minDepthBounds, maxDepthBounds);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT);
}

inline void CommandBuffer::setStencilCompareMask(bool frontFace, bool backFace, uint32_t compareMask) noexcept
{
    leanCmd.setStencilCompareMask(frontFace, backFace, compareMask);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT);
}

inline void CommandBuffer::setStencilWriteMask(bool frontFace, bool backFace, uint32_t writeMask) noexcept
{
    leanCmd.setStencilWriteMask(frontFace, backFace, writeMask);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT);
}

inline void CommandBuffer::setStencilReference(bool frontFace, bool backFace, uint32_t reference) noexcept
{
    leanCmd.setStencilReference(frontFace, backFace, reference);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT);
}

inline void CommandBuffer::bindDescriptorSet(lent_ptr<const Pipeline> pipeline, uint32_t firstSet, lent_ptr<const DescriptorSet> descriptorSet,
    uint32_t dynamicOffset /* std::numeric_limits<uint32_t>::max() */) noexcept
{
    uint32_t dynamicOffsetCount = dynamicOffset < std::numeric_limits<uint32_t>::max() ? 1 : 0;
    leanCmd.bindDescriptorSet(pipeline->getBindPoint(), pipeline->getLayout().get(), firstSet, descriptorSet.get(), dynamicOffsetCount, &dynamicOffset);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);
    MAGMA_INCR(stats.bindDescriptorSetCount, 1);
    MAGMA_INUSE(descriptorSet);
}

inline void CommandBuffer::bindDescriptorSet(lent_ptr<const Pipeline> pipeline, uint32_t firstSet, lent_ptr<const DescriptorSet> descriptorSet, const std::initializer_list<uint32_t>& dynamicOffsets) noexcept
{
    leanCmd.bindDescriptorSet(pipeline->getBindPoint(), pipeline->getLayout().get(), firstSet, descriptorSet.get(), core::countof(dynamicOffsets), dynamicOffsets.begin());
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);
    MAGMA_INCR(stats.bindDescriptorSetCount, 1);
    MAGMA_INUSE(descriptorSet);
}

inline void CommandBuffer::bindIndexBuffer(lent_ptr<const BaseIndexBuffer> indexBuffer, VkDeviceSize offset /* 0 */) noexcept
{
    leanCmd.bindIndexBuffer(indexBuffer.get(), offset);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_VERTEX_INPUT_BIT);
    MAGMA_INCR(stats.bindIndexBufferCount, 1);
    MAGMA_INUSE(indexBuffer);
}

inline void CommandBuffer::bindVertexBuffer(uint32_t firstBinding, lent_ptr<const Buffer> vertexBuffer, VkDeviceSize offset /* 0 */) noexcept
{
    leanCmd.bindVertexBuffer(firstBinding, vertexBuffer.get(), offset);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_VERTEX_INPUT_BIT);
    MAGMA_INCR(stats.bindVertexBufferCount, 1);
    MAGMA_INUSE(vertexBuffer);
}

inline void CommandBuffer::draw(uint32_t vertexCount, uint32_t firstVertex /* 0 */) const noexcept
{
    leanCmd.draw(vertexCount, 1, firstVertex, 0);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
    MAGMA_INCR(stats.drawCount, 1);
}

inline void CommandBuffer::drawInstanced(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex /* 0 */, uint32_t firstInstance /* 0 */) const noexcept
{
    leanCmd.draw(vertexCount, instanceCount, firstVertex, firstInstance);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
    MAGMA_INCR(stats.drawCount, 1);
}

inline void CommandBuffer::drawIndexed(uint32_t indexCount, uint32_t firstIndex /* 0 */, int32_t vertexOffset /* 0 */) const noexcept
{
    leanCmd.drawIndexed(indexCount, 1, firstIndex, vertexOffset, 0);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
    MAGMA_INCR(stats.drawIndexedCount, 1);
}

inline void CommandBuffer::drawIndexedInstanced(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) const noexcept
{
    leanCmd.drawIndexed(indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
    MAGMA_INCR(stats.drawIndexedCount, 1);
}

inline void CommandBuffer::drawIndirect(lent_ptr<const Buffer> buffer, uint32_t drawCount, VkDeviceSize offset /* 0 */) const noexcept
{
    leanCmd.drawIndirect(buffer.get(), drawCount, offset);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT);
    MAGMA_INCR(stats.drawIndirectCount, 1);
    MAGMA_INUSE(buffer);
}

inline void CommandBuffer::drawIndirect(lent_ptr<const DrawIndirectBuffer> buffer) const noexcept
{
    leanCmd.drawIndirect(buffer.get(), buffer->getDrawCount());
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT);
    MAGMA_INCR(stats.drawIndirectCount, 1);
    MAGMA_INUSE(buffer);
}

inline void CommandBuffer::drawIndexedIndirect(lent_ptr<const Buffer> buffer, uint32_t drawCount, VkDeviceSize offset /* 0 */) const noexcept
{
    leanCmd.drawIndexedIndirect(buffer.get(), drawCount, offset);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT);
    MAGMA_INCR(stats.drawIndirectCount, 1);
    MAGMA_INUSE(buffer);
}

inline void CommandBuffer::drawIndexedIndirect(lent_ptr<const DrawIndexedIndirectBuffer> buffer) const noexcept
{
    leanCmd.drawIndexedIndirect(buffer.get(), buffer->getDrawCount());
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT);
    MAGMA_INCR(stats.drawIndexedIndirectCount, 1);
    MAGMA_INUSE(buffer);
}

inline void CommandBuffer::dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const noexcept
{
    leanCmd.dispatch(groupCountX, groupCountY, groupCountZ);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT);
    MAGMA_INCR(stats.dispatchCount, 1);
}

inline void CommandBuffer::dispatchIndirect(lent_ptr<const Buffer> buffer, VkDeviceSize offset /* 0 */) const noexcept
{
    leanCmd.dispatchIndirect(buffer.get(), offset);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT);
    MAGMA_INCR(stats.dispatchIndirectCount, 1);
}

inline void CommandBuffer::copyImage(lent_ptr<const Image> srcImage, lent_ptr<Image> dstImage, const VkImageCopy& region) const noexcept
{
    leanCmd.copyImage(srcImage.get(), dstImage.get(), region);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TRANSFER_BIT);
    MAGMA_INUSE(srcImage);
    MAGMA_INUSE(dstImage);
}

inline void CommandBuffer::blitImage(lent_ptr<const Image> srcImage, lent_ptr<Image> dstImage, const VkImageBlit& region, VkFilter filter) const noexcept
{
    leanCmd.blitImage(srcImage.get(), dstImage.get(), region, filter);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TRANSFER_BIT);
    MAGMA_INUSE(srcImage);
    MAGMA_INUSE(dstImage);
}

inline void CommandBuffer::copyBufferToImage(lent_ptr<const Buffer> srcBuffer, lent_ptr<Image> dstImage, const VkBufferImageCopy& region) const noexcept
{
    leanCmd.copyBufferToImage(srcBuffer.get(), dstImage.get(), 1, &region);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TRANSFER_BIT);
    MAGMA_INUSE(srcBuffer);
    MAGMA_INUSE(dstImage);
}

inline void CommandBuffer::copyBufferToImage(lent_ptr<const Buffer> srcBuffer, lent_ptr<Image> dstImage, const std::vector<VkBufferImageCopy>& regions) const noexcept
{
    leanCmd.copyBufferToImage(srcBuffer.get(), dstImage.get(), core::countof(regions), regions.data());
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TRANSFER_BIT);
    MAGMA_INUSE(srcBuffer);
    MAGMA_INUSE(dstImage);
}

inline void CommandBuffer::copyImageToBuffer(lent_ptr<const Image> srcImage, lent_ptr<Buffer> dstBuffer, const VkBufferImageCopy& region) const noexcept
{
    leanCmd.copyImageToBuffer(srcImage.get(), dstBuffer.get(), 1, &region);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TRANSFER_BIT);
    MAGMA_INUSE(srcImage);
    MAGMA_INUSE(dstBuffer);
}

inline void CommandBuffer::copyImageToBuffer(lent_ptr<const Image> srcImage, lent_ptr<Buffer> dstBuffer, const std::vector<VkBufferImageCopy>& regions) const noexcept
{
    leanCmd.copyImageToBuffer(srcImage.get(), dstBuffer.get(), core::countof(regions), regions.data());
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TRANSFER_BIT);
    MAGMA_INUSE(srcImage);
    MAGMA_INUSE(dstBuffer);
}

inline void CommandBuffer::updateBuffer(lent_ptr<Buffer> buffer, VkDeviceSize dataSize, const void *data, VkDeviceSize offset /* 0 */) const noexcept
{
    leanCmd.updateBuffer(buffer.get(), dataSize, data, offset);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TRANSFER_BIT);
    MAGMA_INUSE(buffer);
}

template<class Type>
inline void CommandBuffer::updateBuffer(lent_ptr<Buffer> buffer, const std::vector<Type>& data, VkDeviceSize offset /* 0 */) const noexcept
{
    VkDeviceSize dataSize = static_cast<VkDeviceSize>(sizeof(Type) * data.size());
    leanCmd.updateBuffer(buffer.get(), dataSize, data.data(), offset);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TRANSFER_BIT);
    MAGMA_INUSE(buffer);
}

inline void CommandBuffer::fillBuffer(lent_ptr<Buffer> buffer, uint32_t value, VkDeviceSize size /* VK_WHOLE_SIZE */, VkDeviceSize offset /* 0 */) const noexcept
{
    leanCmd.fillBuffer(buffer.get(), value, size, offset);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TRANSFER_BIT);
    MAGMA_INUSE(buffer);
}

inline void CommandBuffer::clearColorImage(lent_ptr<Image> image, const ClearColor& color, const VkImageSubresourceRange& range) const noexcept
{
    const VkClearValue& clearValue = color;
    leanCmd.clearColorImage(image.get(), clearValue.color, range);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TRANSFER_BIT);
    MAGMA_INUSE(image);
}

inline void CommandBuffer::clearDepthStencilImage(lent_ptr<Image> image, const ClearDepthStencil& depthStencil, const VkImageSubresourceRange& range) const noexcept
{
    const VkClearValue& clearValue = depthStencil;
    leanCmd.clearDepthStencilImage(image.get(), clearValue.depthStencil, range);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TRANSFER_BIT);
    MAGMA_INUSE(image);
}

inline void CommandBuffer::clearAttachments(const std::initializer_list<ClearAttachment>& attachments, const VkClearRect& clearRect) const noexcept
{
    leanCmd.clearAttachments(core::countof(attachments), attachments.begin(), clearRect);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
}

inline void CommandBuffer::resolveImage(lent_ptr<const Image> srcImage, lent_ptr<Image> dstImage) const noexcept
{
    ImageResolve region(srcImage.get(), dstImage.get());
    resolveImage(std::move(srcImage), std::move(dstImage), region);
}

inline void CommandBuffer::resolveImage(lent_ptr<const Image> srcImage, lent_ptr<Image> dstImage, const VkImageResolve& region) const noexcept
{
    leanCmd.resolveImage(srcImage.get(), dstImage.get(), region);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TRANSFER_BIT);
    MAGMA_INUSE(srcImage);
    MAGMA_INUSE(dstImage);
}

inline void CommandBuffer::setEvent(lent_ptr<Event> event, VkPipelineStageFlags stageMask) const noexcept
{
    leanCmd.setEvent(event.get(), stageMask);
    MAGMA_CHECKPOINT(stageMask);
    MAGMA_INUSE(event);
}

inline void CommandBuffer::resetEvent(lent_ptr<Event> event, VkPipelineStageFlags stageMask) const noexcept
{
    leanCmd.resetEvent(event.get(), stageMask);
    MAGMA_CHECKPOINT(stageMask);
    MAGMA_INUSE(event);
}

inline void CommandBuffer::waitEvent(lent_ptr<const Event> event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask) const noexcept
{
    leanCmd.waitEvent(event.get(), srcStageMask, dstStageMask);
    MAGMA_CHECKPOINT(dstStageMask);
    MAGMA_INUSE(event);
}

inline void CommandBuffer::waitEvent(lent_ptr<const Event> event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const MemoryBarrier& barrier) const noexcept
{
    leanCmd.waitEvent(event.get(), srcStageMask, dstStageMask, barrier);
    MAGMA_CHECKPOINT(dstStageMask);
    MAGMA_INUSE(event);
}

inline void CommandBuffer::waitEvent(lent_ptr<const Event> event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const BufferMemoryBarrier& barrier) const noexcept
{
    leanCmd.waitEvent(event.get(), srcStageMask, dstStageMask, barrier);
    MAGMA_CHECKPOINT(dstStageMask);
    MAGMA_INUSE(event);
}

inline void CommandBuffer::waitEvent(lent_ptr<const Event> event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const ImageMemoryBarrier& barrier) const noexcept
{
    leanCmd.waitEvent(event.get(), srcStageMask, dstStageMask, barrier);
    MAGMA_CHECKPOINT(dstStageMask);
    barrier.updateImageLayout();
    MAGMA_INUSE(event);
}

inline void CommandBuffer::waitEvent(lent_ptr<const Event> event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::initializer_list<MemoryBarrier>& memoryBarriers) const noexcept
{
    leanCmd.waitEvent(event.get(), srcStageMask, dstStageMask, core::countof(memoryBarriers), memoryBarriers.begin());
    MAGMA_CHECKPOINT(dstStageMask);
    MAGMA_INUSE(event);
}

inline void CommandBuffer::waitEvent(lent_ptr<const Event> event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::vector<MemoryBarrier>& memoryBarriers) const noexcept
{
    leanCmd.waitEvent(event.get(), srcStageMask, dstStageMask, core::countof(memoryBarriers), memoryBarriers.data());
    MAGMA_CHECKPOINT(dstStageMask);
    MAGMA_INUSE(event);
}

inline void CommandBuffer::waitEvent(lent_ptr<const Event> event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::initializer_list<BufferMemoryBarrier>& bufferMemoryBarriers) const noexcept
{
    leanCmd.waitEvent(event.get(), srcStageMask, dstStageMask, core::countof(bufferMemoryBarriers), bufferMemoryBarriers.begin());
    MAGMA_CHECKPOINT(dstStageMask);
    MAGMA_INUSE(event);
}

inline void CommandBuffer::waitEvent(lent_ptr<const Event> event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::vector<BufferMemoryBarrier>& bufferMemoryBarriers) const noexcept
{
    leanCmd.waitEvent(event.get(), srcStageMask, dstStageMask, core::countof(bufferMemoryBarriers), bufferMemoryBarriers.data());
    MAGMA_CHECKPOINT(dstStageMask);
    MAGMA_INUSE(event);
}

inline void CommandBuffer::waitEvent(lent_ptr<const Event> event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::initializer_list<ImageMemoryBarrier>& imageMemoryBarriers) const noexcept
{
    leanCmd.waitEvent(event.get(), srcStageMask, dstStageMask, core::countof(imageMemoryBarriers), imageMemoryBarriers.begin());
    for (auto const& barrier: imageMemoryBarriers)
        barrier.updateImageLayout();
    MAGMA_CHECKPOINT(dstStageMask);
    MAGMA_INUSE(event);
}

inline void CommandBuffer::waitEvent(lent_ptr<const Event> event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::vector<ImageMemoryBarrier>& imageMemoryBarriers) const noexcept
{
    leanCmd.waitEvent(event.get(), srcStageMask, dstStageMask, core::countof(imageMemoryBarriers), imageMemoryBarriers.data());
    for (auto const& barrier: imageMemoryBarriers)
        barrier.updateImageLayout();
    MAGMA_CHECKPOINT(dstStageMask);
    MAGMA_INUSE(event);
}

inline void CommandBuffer::pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags /* 0 */) noexcept
{
    leanCmd.pipelineBarrier(srcStageMask, dstStageMask, dependencyFlags);
    MAGMA_CHECKPOINT(dstStageMask);
    MAGMA_INCR(stats.pipelineBarrierCount, 1);
}

inline void CommandBuffer::pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const MemoryBarrier& barrier, VkDependencyFlags dependencyFlags /* 0 */) noexcept
{
    leanCmd.pipelineBarrier(srcStageMask, dstStageMask, barrier, dependencyFlags);
    MAGMA_CHECKPOINT(dstStageMask);
    MAGMA_INCR(stats.pipelineBarrierCount, 1);
}

inline void CommandBuffer::pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const BufferMemoryBarrier& barrier, VkDependencyFlags dependencyFlags /* 0 */) noexcept
{
    leanCmd.pipelineBarrier(srcStageMask, dstStageMask, barrier, dependencyFlags);
    MAGMA_CHECKPOINT(dstStageMask);
    MAGMA_INCR(stats.pipelineBarrierCount, 1);
}

inline void CommandBuffer::pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const ImageMemoryBarrier& barrier, VkDependencyFlags dependencyFlags /* 0 */) noexcept
{
    leanCmd.pipelineBarrier(srcStageMask, dstStageMask, barrier, dependencyFlags);
    MAGMA_CHECKPOINT(dstStageMask);
    barrier.updateImageLayout();
    MAGMA_INCR(stats.pipelineBarrierCount, 1);
}

inline void CommandBuffer::pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::initializer_list<MemoryBarrier>& memoryBarriers, VkDependencyFlags dependencyFlags /* 0 */) noexcept
{
    leanCmd.pipelineBarrier(srcStageMask, dstStageMask, core::countof(memoryBarriers), memoryBarriers.begin(), dependencyFlags);
    MAGMA_CHECKPOINT(dstStageMask);
    MAGMA_INCR(stats.pipelineBarrierCount, 1);
}

inline void CommandBuffer::pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::vector<MemoryBarrier>& memoryBarriers, VkDependencyFlags dependencyFlags /* 0 */) noexcept
{
    leanCmd.pipelineBarrier(srcStageMask, dstStageMask, core::countof(memoryBarriers), memoryBarriers.data(), dependencyFlags);
    MAGMA_CHECKPOINT(dstStageMask);
    MAGMA_INCR(stats.pipelineBarrierCount, 1);
}

inline void CommandBuffer::pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::initializer_list<BufferMemoryBarrier>& bufferMemoryBarriers, VkDependencyFlags dependencyFlags /* 0 */) noexcept
{
    leanCmd.pipelineBarrier(srcStageMask, dstStageMask, core::countof(bufferMemoryBarriers), bufferMemoryBarriers.begin(), dependencyFlags);
    MAGMA_CHECKPOINT(dstStageMask);
    MAGMA_INCR(stats.pipelineBarrierCount, 1);
}

inline void CommandBuffer::pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::vector<BufferMemoryBarrier>& bufferMemoryBarriers, VkDependencyFlags dependencyFlags /* 0 */) noexcept
{
    leanCmd.pipelineBarrier(srcStageMask, dstStageMask, core::countof(bufferMemoryBarriers), bufferMemoryBarriers.data(), dependencyFlags);
    MAGMA_CHECKPOINT(dstStageMask);
    MAGMA_INCR(stats.pipelineBarrierCount, 1);
}

inline void CommandBuffer::pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::initializer_list<ImageMemoryBarrier>& imageMemoryBarriers, VkDependencyFlags dependencyFlags /* 0 */) noexcept
{
    leanCmd.pipelineBarrier(srcStageMask, dstStageMask, core::countof(imageMemoryBarriers), imageMemoryBarriers.begin(), dependencyFlags);
    for (auto const& barrier: imageMemoryBarriers)
        barrier.updateImageLayout();
    MAGMA_CHECKPOINT(dstStageMask);
    MAGMA_INCR(stats.pipelineBarrierCount, 1);
}

inline void CommandBuffer::pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::vector<ImageMemoryBarrier>& imageMemoryBarriers, VkDependencyFlags dependencyFlags /* 0 */) noexcept
{
    leanCmd.pipelineBarrier(srcStageMask, dstStageMask, core::countof(imageMemoryBarriers), imageMemoryBarriers.data(), dependencyFlags);
    for (auto const& barrier: imageMemoryBarriers)
        barrier.updateImageLayout();
    MAGMA_CHECKPOINT(dstStageMask);
    MAGMA_INCR(stats.pipelineBarrierCount, 1);
}

inline void CommandBuffer::beginQuery(lent_ptr<QueryPool> queryPool, uint32_t query) noexcept
{
    MAGMA_ASSERT(!nonIndexedQuery);
    leanCmd.beginQuery(queryPool.get(), query);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);
    MAGMA_INCR(stats.queryCount, 1);
    MAGMA_INUSE(queryPool);
    nonIndexedQuery = VK_TRUE;
}

inline void CommandBuffer::endQuery(lent_ptr<QueryPool> queryPool, uint32_t query) noexcept
{
    MAGMA_ASSERT(nonIndexedQuery);
    leanCmd.endQuery(queryPool.get(), query);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);
    MAGMA_INUSE(queryPool);
    nonIndexedQuery = VK_FALSE;
}

inline void CommandBuffer::resetQueryPool(lent_ptr<QueryPool> queryPool, uint32_t firstQuery, uint32_t queryCount) noexcept
{
    leanCmd.resetQueryPool(queryPool.get(), firstQuery, queryCount);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);
    MAGMA_INUSE(queryPool);
}

inline void CommandBuffer::writeTimestamp(VkPipelineStageFlagBits pipelineStage, lent_ptr<QueryPool> queryPool, uint32_t query) noexcept
{
    leanCmd.writeTimestamp(pipelineStage, queryPool.get(), query);
    MAGMA_CHECKPOINT(pipelineStage);
    MAGMA_INUSE(queryPool);
}

template<class Type>
inline void CommandBuffer::copyQueryResults(lent_ptr<const QueryPool> /* queryPool */, lent_ptr<Buffer> /* dstBuffer */, bool /* wait */,
    uint32_t /* firstQuery */, uint32_t /* queryCount */, VkDeviceSize /* dstOffset */) noexcept
{
    static_assert(std::is_same<Type, uint32_t>::value || std::is_same<Type, uint64_t>::value,
        "CommandBuffer::copyQueryResults() specialized only for uint32_t and uint64_t types");
}

template<>
inline void CommandBuffer::copyQueryResults<uint32_t>(lent_ptr<const QueryPool> queryPool, lent_ptr<Buffer> dstBuffer, bool wait,
    uint32_t firstQuery /* 0 */, uint32_t queryCount /* std::numeric_limits<uint32_t>::max() */, VkDeviceSize dstOffset /* 0 */) noexcept
{
    if (std::numeric_limits<uint32_t>::max() == queryCount)
        queryCount = queryPool->getQueryCount();
    VkQueryResultFlags flags = wait ? VK_QUERY_RESULT_WAIT_BIT : 0;
    leanCmd.copyQueryPoolResults(queryPool.get(), firstQuery, queryCount, dstBuffer.get(), dstOffset, sizeof(uint32_t), flags);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TRANSFER_BIT);
    MAGMA_INUSE(queryPool);
    MAGMA_INUSE(dstBuffer);
}

template<>
inline void CommandBuffer::copyQueryResults<uint64_t>(lent_ptr<const QueryPool> queryPool, lent_ptr<Buffer> dstBuffer, bool wait,
    uint32_t firstQuery /* 0 */, uint32_t queryCount /* std::numeric_limits<uint32_t>::max() */, VkDeviceSize dstOffset /* 0 */) noexcept
{
    if (std::numeric_limits<uint32_t>::max() == queryCount)
        queryCount = queryPool->getQueryCount();
    const VkQueryResultFlags flags = VK_QUERY_RESULT_64_BIT | (wait ? VK_QUERY_RESULT_WAIT_BIT : 0);
    leanCmd.copyQueryPoolResults(queryPool.get(), firstQuery, queryCount, dstBuffer.get(), dstOffset, sizeof(uint64_t), flags);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TRANSFER_BIT);
    MAGMA_INUSE(queryPool);
    MAGMA_INUSE(dstBuffer);
}

template<class Type>
inline void CommandBuffer::copyQueryResultsWithAvailability(lent_ptr<const QueryPool> /* queryPool */, lent_ptr<Buffer> /* dstBuffer */,
    uint32_t /* firstQuery */, uint32_t /* queryCount */, VkDeviceSize /* dstOffset */) noexcept
{
    static_assert(std::is_same<Type, uint32_t>::value || std::is_same<Type, uint64_t>::value,
        "CommandBuffer::copyQueryResultsWithAvailability() specialized only for uint32_t and uint64_t types");
}

template<>
inline void CommandBuffer::copyQueryResultsWithAvailability<uint32_t>(lent_ptr<const QueryPool> queryPool, lent_ptr<Buffer> dstBuffer,
    uint32_t firstQuery /* 0 */, uint32_t queryCount /* std::numeric_limits<uint32_t>::max() */, VkDeviceSize dstOffset /* 0 */) noexcept
{
    if (std::numeric_limits<uint32_t>::max() == queryCount)
        queryCount = queryPool->getQueryCount();
    constexpr VkQueryResultFlags flags = VK_QUERY_RESULT_WITH_AVAILABILITY_BIT;
    leanCmd.copyQueryPoolResults(queryPool.get(), firstQuery, queryCount, dstBuffer.get(), dstOffset, sizeof(uint32_t) * 2, flags);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TRANSFER_BIT);
    MAGMA_INUSE(queryPool);
    MAGMA_INUSE(dstBuffer);
}

template<>
inline void CommandBuffer::copyQueryResultsWithAvailability<uint64_t>(lent_ptr<const QueryPool> queryPool, lent_ptr<Buffer> dstBuffer,
    uint32_t firstQuery /* 0 */, uint32_t queryCount /* std::numeric_limits<uint32_t>::max() */, VkDeviceSize dstOffset /* 0 */) noexcept
{
    if (std::numeric_limits<uint32_t>::max() == queryCount)
        queryCount = queryPool->getQueryCount();
    constexpr VkQueryResultFlags flags = VK_QUERY_RESULT_64_BIT | VK_QUERY_RESULT_WITH_AVAILABILITY_BIT;
    leanCmd.copyQueryPoolResults(queryPool.get(), firstQuery, queryCount, dstBuffer.get(), dstOffset, sizeof(uint64_t) * 2, flags);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TRANSFER_BIT);
    MAGMA_INUSE(queryPool);
    MAGMA_INUSE(dstBuffer);
}

template<class Type>
inline void CommandBuffer::pushConstant(lent_ptr<const PipelineLayout> layout, VkShaderStageFlags stageFlags, const Type& constant,
    uint32_t offset /* 0 */) noexcept
{
    leanCmd.pushConstants(layout.get(), stageFlags, offset, static_cast<uint32_t>(sizeof(Type)), &constant);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);
    MAGMA_INCR(stats.pushConstantCount, 1);
}

template<class Type, uint32_t PushConstantCount>
inline void CommandBuffer::pushConstants(lent_ptr<const PipelineLayout> layout, VkShaderStageFlags stageFlags, const Type(&constants)[PushConstantCount],
    uint32_t offset /* 0 */) noexcept
{
    leanCmd.pushConstants(layout.get(), stageFlags, offset, static_cast<uint32_t>(sizeof(Type) * PushConstantCount), constants);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);
    MAGMA_INCR(stats.pushConstantCount, 1);
}

template<class Type>
inline void CommandBuffer::pushConstants(lent_ptr<const PipelineLayout> layout, VkShaderStageFlags stageFlags, const std::vector<Type>& constants,
    uint32_t offset /* 0 */) noexcept
{
    leanCmd.pushConstants(layout.get(), stageFlags, offset, static_cast<uint32_t>(sizeof(Type) * constants.size()), constants.data());
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);
    MAGMA_INCR(stats.pushConstantCount, 1);
}

template<class BlockType>
inline void CommandBuffer::pushConstantBlock(lent_ptr<const PipelineLayout> layout, VkShaderStageFlags stageFlags, const BlockType& block,
    uint32_t offset /* 0 */) noexcept
{
    leanCmd.pushConstants(layout.get(), stageFlags, offset, static_cast<uint32_t>(sizeof(BlockType)), &block);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);
    MAGMA_INCR(stats.pushConstantCount, 1);
}

inline void CommandBuffer::nextSubpass(VkSubpassContents contents /* VK_SUBPASS_CONTENTS_INLINE */) noexcept
{
    leanCmd.nextSubpass(contents);
    MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
}

#ifdef VK_KHR_device_group
inline void CommandBuffer::setDeviceMask(uint32_t deviceMask) noexcept
{
    MAGMA_ASSERT(extensions.KHR_device_group);
    if (extensions.KHR_device_group)
    {
        leanCmd.setDeviceMask(deviceMask);
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);
    }
}

inline void CommandBuffer::dispatchBase(uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const noexcept
{
    MAGMA_ASSERT(extensions.KHR_device_group);
    if (extensions.KHR_device_group)
    {
        leanCmd.dispatchBase(baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT);
        MAGMA_INCR(stats.dispatchCount, 1);
    }
}
#endif // VK_KHR_device_group

#ifdef VK_EXT_line_rasterization
inline void CommandBuffer::setLineStipple(uint32_t stippleFactor, uint16_t stipplePattern) noexcept
{
    MAGMA_ASSERT(extensions.EXT_line_rasterization);
    if (extensions.EXT_line_rasterization)
    {
        leanCmd.setLineStipple(stippleFactor, stipplePattern);
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);
    }
}
#endif // VK_EXT_line_rasterization

#ifdef VK_KHR_push_descriptor
inline void CommandBuffer::pushDescriptorSet(lent_ptr<const Pipeline> pipeline, uint32_t set, lent_ptr<const DescriptorSet> descriptorSet) noexcept
{
    MAGMA_ASSERT(extensions.KHR_push_descriptor);
    if (extensions.KHR_push_descriptor)
    {
        leanCmd.pushDescriptorSet(pipeline->getBindPoint(), pipeline->getLayout().get(), set, descriptorSet.get());
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);
        MAGMA_INCR(stats.pushDescriptorSetCount, 1);
        MAGMA_INUSE(descriptorSet);
    }
}
#endif // VK_KHR_push_descriptor

#ifdef VK_EXT_multi_draw
inline void CommandBuffer::drawMulti(const std::vector<VkMultiDrawInfoEXT>& vertexInfo) const noexcept
{
    MAGMA_ASSERT(extensions.EXT_multi_draw);
    if (extensions.EXT_multi_draw)
    {
        const uint32_t drawCount = core::countof(vertexInfo);
        leanCmd.drawMulti(drawCount, vertexInfo.data(), 1, 0);
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
        MAGMA_INCR(stats.drawCount, drawCount);
    }
}

inline void CommandBuffer::drawMultiInstanced(const std::vector<VkMultiDrawInfoEXT>& vertexInfo, uint32_t instanceCount, uint32_t firstInstance) const noexcept
{
    MAGMA_ASSERT(extensions.EXT_multi_draw);
    if (extensions.EXT_multi_draw)
    {
        const uint32_t drawCount = core::countof(vertexInfo);
        leanCmd.drawMulti(drawCount, vertexInfo.data(), instanceCount, firstInstance);
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
        MAGMA_INCR(stats.drawCount, drawCount);
    }
}

inline void CommandBuffer::drawMultiIndexed(const std::vector<VkMultiDrawIndexedInfoEXT>& indexInfo, const std::vector<int32_t>& vertexOffsets /* empty */) const noexcept
{
    MAGMA_ASSERT(extensions.EXT_multi_draw);
    if (extensions.EXT_multi_draw)
    {
        const uint32_t drawIndexedCount = core::countof(indexInfo);
        leanCmd.drawMultiIndexed(drawIndexedCount, indexInfo.data(), 0, 1, vertexOffsets.data());
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
        MAGMA_INCR(stats.drawIndexedCount, drawIndexedCount);
    }
}

inline void CommandBuffer::drawMultiIndexedInstanced(const std::vector<VkMultiDrawIndexedInfoEXT>& indexInfo, uint32_t instanceCount, uint32_t firstInstance,
    const std::vector<int32_t>& vertexOffsets /* {} */) const noexcept
{
    MAGMA_ASSERT(extensions.EXT_multi_draw);
    if (extensions.EXT_multi_draw)
    {
        const uint32_t drawIndexedCount = core::countof(indexInfo);
        leanCmd.drawMultiIndexed(drawIndexedCount, indexInfo.data(), instanceCount, firstInstance, vertexOffsets.data());
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
        MAGMA_INCR(stats.drawIndexedCount, drawIndexedCount);
    }
}
#endif // VK_EXT_multi_draw

#if defined(VK_KHR_draw_indirect_count) || defined(VK_AMD_draw_indirect_count)
inline void CommandBuffer::drawIndirectCount(lent_ptr<const Buffer> buffer, lent_ptr<const Buffer> countBuffer, uint32_t maxDrawCount,
    VkDeviceSize offset /* 0 */, VkDeviceSize countBufferOffset /* 0 */) const noexcept
{
    MAGMA_ASSERT(extensions.KHR_draw_indirect_count || extensions.AMD_draw_indirect_count);
    if (extensions.KHR_draw_indirect_count || extensions.AMD_draw_indirect_count)
    {
        leanCmd.drawIndirectCount(buffer.get(), countBuffer.get(), maxDrawCount, offset, countBufferOffset);
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT);
        MAGMA_INCR(stats.drawIndirectCount, 1);
        MAGMA_INUSE(buffer);
        MAGMA_INUSE(countBuffer);
    }
}

inline void CommandBuffer::drawIndexedIndirectCount(lent_ptr<const Buffer> buffer, lent_ptr<const Buffer> countBuffer, uint32_t maxDrawCount,
    VkDeviceSize offset /* 0 */, VkDeviceSize countBufferOffset /* 0 */) const noexcept
{
    MAGMA_ASSERT(extensions.KHR_draw_indirect_count || extensions.AMD_draw_indirect_count);
    if (extensions.KHR_draw_indirect_count || extensions.AMD_draw_indirect_count)
    {
        leanCmd.drawIndexedIndirectCount(buffer.get(), countBuffer.get(), maxDrawCount, offset, countBufferOffset);
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT);
        MAGMA_INCR(stats.drawIndexedIndirectCount, 1);
        MAGMA_INUSE(buffer);
        MAGMA_INUSE(countBuffer);
    }
}
#endif // VK_KHR_draw_indirect_count || VK_AMD_draw_indirect_count

#if defined(VK_EXT_mesh_shader) || defined(VK_NV_mesh_shader)
inline void CommandBuffer::drawMeshTasks(uint32_t groupCountX, uint32_t groupCountY /* 1 */, uint32_t groupCountZ /* 1 */) const noexcept
{
    MAGMA_ASSERT(extensions.EXT_mesh_shader || extensions.NV_mesh_shader);
    if (extensions.EXT_mesh_shader || extensions.NV_mesh_shader)
    {
        leanCmd.drawMeshTasks(groupCountX, groupCountY, groupCountZ);
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TASK_SHADER_BIT_NV);
        MAGMA_INCR(stats.drawMeshTasksCount, 1);
    }
}

inline void CommandBuffer::drawMeshTasksIndirect(lent_ptr<const DrawMeshTasksIndirectBuffer> buffer, VkDeviceSize offset /* 0 */) const noexcept
{
    MAGMA_ASSERT(extensions.EXT_mesh_shader || extensions.NV_mesh_shader);
    if (extensions.EXT_mesh_shader || extensions.NV_mesh_shader)
    {
        leanCmd.drawMeshTasksIndirect(buffer.get(), buffer->getDrawCount(), offset);
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TASK_SHADER_BIT_NV);
        MAGMA_INCR(stats.drawMeshTasksIndirectCount, 1);
        MAGMA_INUSE(buffer);
    }
}

inline void CommandBuffer::drawMeshTasksIndirectCount(lent_ptr<const DrawMeshTasksIndirectBuffer> buffer, lent_ptr<const Buffer> countBuffer,
    VkDeviceSize offset /* 0 */, VkDeviceSize countBufferOffset /* 0 */) const noexcept
{
    MAGMA_ASSERT(extensions.EXT_mesh_shader || extensions.NV_mesh_shader);
    if (extensions.EXT_mesh_shader || extensions.NV_mesh_shader)
    {
        leanCmd.drawMeshTasksIndirectCount(buffer.get(), countBuffer.get(), buffer->getDrawCount(), offset, countBufferOffset);
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TASK_SHADER_BIT_NV);
        MAGMA_INCR(stats.drawMeshTasksIndirectCount, 1);
        MAGMA_INUSE(buffer);
        MAGMA_INUSE(countBuffer);
    }
}
#endif // VK_EXT_mesh_shader || VK_NV_mesh_shader

#ifdef VK_EXT_transform_feedback
inline void CommandBuffer::bindTransformFeedbackBuffer(uint32_t firstBinding, lent_ptr<Buffer> transformFeedbackBuffer, VkDeviceSize offset /* 0 */, VkDeviceSize size /* VK_WHOLE_SIZE */) noexcept
{
    MAGMA_ASSERT(extensions.EXT_transform_feedback);
    if (extensions.EXT_transform_feedback)
    {
        leanCmd.bindTransformFeedbackBuffer(firstBinding, transformFeedbackBuffer.get(), offset, size);
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TRANSFORM_FEEDBACK_BIT_EXT);
        MAGMA_INCR(stats.bindTransformFeedbackBufferCount, 1);
        MAGMA_INUSE(transformFeedbackBuffer);
    }
}

inline void CommandBuffer::beginQueryIndexed(lent_ptr<QueryPool> queryPool, uint32_t query, uint32_t index) noexcept
{
    MAGMA_ASSERT(!indexedQuery);
    MAGMA_ASSERT(extensions.EXT_transform_feedback);
    if (extensions.EXT_transform_feedback)
    {
        leanCmd.beginQueryIndexed(queryPool.get(), query, index);
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);
        MAGMA_INCR(stats.indexedQueryCount, 1);
        MAGMA_INUSE(queryPool);
        indexedQuery = VK_TRUE;
    }
}

inline void CommandBuffer::endQueryIndexed(lent_ptr<QueryPool> queryPool, uint32_t query, uint32_t index) noexcept
{
    MAGMA_ASSERT(indexedQuery);
    MAGMA_ASSERT(extensions.EXT_transform_feedback);
    if (extensions.EXT_transform_feedback)
    {
        leanCmd.endQueryIndexed(queryPool.get(), query, index);
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);
        MAGMA_INUSE(queryPool);
        indexedQuery = VK_FALSE;
    }
}

inline void CommandBuffer::drawIndirectByteCount(lent_ptr<const Buffer> counterBuffer, uint32_t vertexStride,
    VkDeviceSize counterBufferOffset /* 0 */, uint32_t counterOffset /* 0 */) const noexcept
{
    MAGMA_ASSERT(extensions.EXT_transform_feedback);
    if (extensions.EXT_transform_feedback)
    {
        leanCmd.drawIndirectByteCount(1, 0, counterBuffer.get(), vertexStride, counterBufferOffset, counterOffset);
        MAGMA_INCR(stats.drawIndirectCount, 1);
        MAGMA_INUSE(counterBuffer);
    }
}

inline void CommandBuffer::drawIndirectByteCountInstanced(uint32_t instanceCount, uint32_t firstInstance, lent_ptr<const Buffer> counterBuffer, uint32_t vertexStride,
    VkDeviceSize counterBufferOffset /* 0 */, uint32_t counterOffset /* 0 */) const noexcept
{
    MAGMA_ASSERT(extensions.EXT_transform_feedback);
    if (extensions.EXT_transform_feedback)
    {
        leanCmd.drawIndirectByteCount(instanceCount, firstInstance, counterBuffer.get(), vertexStride, counterBufferOffset, counterOffset);
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT);
        MAGMA_INCR(stats.drawIndirectCount, 1);
        MAGMA_INUSE(counterBuffer);
    }
}
#endif // VK_EXT_transform_feedback

#ifdef VK_KHR_acceleration_structure
inline void CommandBuffer::buildAccelerationStructure(lent_ptr<TopLevelAccelerationStructure> accelerationStructure, const AccelerationStructureInstances& instances, lent_ptr<Buffer> scratchBuffer) const noexcept
{
    MAGMA_ASSERT(extensions.KHR_acceleration_structure);
    if (extensions.KHR_acceleration_structure)
    {
        leanCmd.buildAccelerationStructure(accelerationStructure.get(), instances, scratchBuffer.get());
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR);
        MAGMA_INUSE(accelerationStructure);
        MAGMA_INUSE(scratchBuffer);
    }
}

inline void CommandBuffer::updateAccelerationStructure(lent_ptr<TopLevelAccelerationStructure> accelerationStructure, const AccelerationStructureInstances& instances, lent_ptr<Buffer> scratchBuffer) const noexcept
{
    MAGMA_ASSERT(extensions.KHR_acceleration_structure);
    if (extensions.KHR_acceleration_structure)
    {
        leanCmd.updateAccelerationStructure(accelerationStructure.get(), instances, scratchBuffer.get());
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR);
        MAGMA_INUSE(accelerationStructure);
        MAGMA_INUSE(scratchBuffer);
    }
}

inline void CommandBuffer::copyAccelerationStructure(lent_ptr<const AccelerationStructure> srcAccelerationStructure, lent_ptr<AccelerationStructure> dstAccelerationStructure) const noexcept
{
    MAGMA_ASSERT(extensions.KHR_acceleration_structure);
    if (extensions.KHR_acceleration_structure)
    {
        leanCmd.copyAccelerationStructure(srcAccelerationStructure.get(), dstAccelerationStructure.get());
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TRANSFER_BIT);
        MAGMA_INUSE(srcAccelerationStructure);
        MAGMA_INUSE(dstAccelerationStructure);
    }
}

inline void CommandBuffer::compactAccelerationStructure(lent_ptr<const AccelerationStructure> srcAccelerationStructure, lent_ptr<AccelerationStructure> dstAccelerationStructure) const noexcept
{
    MAGMA_ASSERT(extensions.KHR_acceleration_structure);
    if (extensions.KHR_acceleration_structure)
    {
        leanCmd.compactAccelerationStructure(srcAccelerationStructure.get(), dstAccelerationStructure.get());
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TRANSFER_BIT);
        MAGMA_INUSE(srcAccelerationStructure);
        MAGMA_INUSE(dstAccelerationStructure);
    }
}

inline void CommandBuffer::copyAccelerationStructureToBuffer(lent_ptr<const AccelerationStructure> srcAccelerationStructure, lent_ptr<Buffer> dstBuffer) const noexcept
{
    MAGMA_ASSERT(extensions.KHR_acceleration_structure);
    if (extensions.KHR_acceleration_structure)
    {
        leanCmd.copyAccelerationStructureToBuffer(srcAccelerationStructure.get(), dstBuffer.get());
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TRANSFER_BIT);
        MAGMA_INUSE(srcAccelerationStructure);
        MAGMA_INUSE(dstBuffer);
    }
}

inline void CommandBuffer::copyBufferToAccelerationStructure(lent_ptr<const Buffer> srcBuffer, lent_ptr<AccelerationStructure> dstAccelerationStructure) const noexcept
{
    MAGMA_ASSERT(extensions.KHR_acceleration_structure);
    if (extensions.KHR_acceleration_structure)
    {
        leanCmd.copyBufferToAccelerationStructure(srcBuffer.get(), dstAccelerationStructure.get());
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TRANSFER_BIT);
        MAGMA_INUSE(srcBuffer);
        MAGMA_INUSE(dstAccelerationStructure);
    }
}

inline void CommandBuffer::serializeAccelerationStructure(lent_ptr<const AccelerationStructure> srcAccelerationStructure, lent_ptr<Buffer> dstBuffer, VkDeviceAddress bufferOffset /* 0 */) const noexcept
{
    MAGMA_ASSERT(extensions.KHR_acceleration_structure);
    if (extensions.KHR_acceleration_structure)
    {
        leanCmd.serializeAccelerationStructure(srcAccelerationStructure.get(), dstBuffer.get(), bufferOffset);
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TRANSFER_BIT);
        MAGMA_INUSE(srcAccelerationStructure);
        MAGMA_INUSE(dstBuffer);
    }
}

inline void CommandBuffer::deserializeAccelerationStructure(lent_ptr<const Buffer> srcBuffer, lent_ptr<AccelerationStructure> dstAccelerationStructure, VkDeviceAddress bufferOffset /* 0 */) const noexcept
{
    MAGMA_ASSERT(extensions.KHR_acceleration_structure);
    if (extensions.KHR_acceleration_structure)
    {
        leanCmd.deserializeAccelerationStructure(srcBuffer.get(), dstAccelerationStructure.get(), bufferOffset);
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TRANSFER_BIT);
        MAGMA_INUSE(srcBuffer);
        MAGMA_INUSE(dstAccelerationStructure);
    }
}

inline void CommandBuffer::writeAccelerationStructureProperties(lent_ptr<const AccelerationStructure> accelerationStructure, lent_ptr<QueryPool> queryPool, uint32_t firstQuery /* 0 */) const noexcept
{
    MAGMA_ASSERT(extensions.KHR_acceleration_structure);
    if (extensions.KHR_acceleration_structure)
    {
        leanCmd.writeAccelerationStructureProperties(accelerationStructure.get(), queryPool.get(), firstQuery);
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TRANSFER_BIT);
        MAGMA_INUSE(accelerationStructure);
        MAGMA_INUSE(queryPool);
    }
}
#endif // VK_KHR_acceleration_structure

#ifdef VK_NV_cluster_acceleration_structure
inline void CommandBuffer::moveObjects(lent_ptr<ClusterAccelerationStructure> accelerationStructure, lent_ptr<Buffer> scratchBuffer) const noexcept
{
    MAGMA_ASSERT(extensions.NV_cluster_acceleration_structure);
    if (extensions.NV_cluster_acceleration_structure)
    {
        leanCmd.moveObjects(accelerationStructure.get(), scratchBuffer.get());
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_TRANSFER_BIT);
        MAGMA_INUSE(scratchBuffer);
    }
}

inline void CommandBuffer::buildClustersBottomLevel(lent_ptr<BottomLevelClusterAcccelerationStructure> accelerationStructure, lent_ptr<Buffer> scratchBuffer) const noexcept
{
    MAGMA_ASSERT(extensions.NV_cluster_acceleration_structure);
    if (extensions.NV_cluster_acceleration_structure)
    {
        leanCmd.buildClustersBottomLevel(accelerationStructure.get(), scratchBuffer.get());
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR);
        MAGMA_INUSE(scratchBuffer);
    }
}

inline void CommandBuffer::buildTriangleCluster(lent_ptr<TriangleClusterAccelerationStructure> accelerationStructure, lent_ptr<Buffer> scratchBuffer) const noexcept
{
    MAGMA_ASSERT(extensions.NV_cluster_acceleration_structure);
    if (extensions.NV_cluster_acceleration_structure)
    {
        leanCmd.buildTriangleCluster(accelerationStructure.get(), scratchBuffer.get());
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR);
        MAGMA_INUSE(scratchBuffer);
    }
}

inline void CommandBuffer::buildTriangleClusterTemplate(lent_ptr<TriangleClusterAccelerationStructureTemplate> accelerationStructure, lent_ptr<Buffer> scratchBuffer) const noexcept
{
    MAGMA_ASSERT(extensions.NV_cluster_acceleration_structure);
    if (extensions.NV_cluster_acceleration_structure)
    {
        leanCmd.buildTriangleClusterTemplate(accelerationStructure.get(), scratchBuffer.get());
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR);
        MAGMA_INUSE(scratchBuffer);
    }
}

inline void CommandBuffer::instantiateTriangleCluster(lent_ptr<TriangleClusterAccelerationStructure> accelerationStructure, lent_ptr<Buffer> scratchBuffer) const noexcept
{
    MAGMA_ASSERT(extensions.NV_cluster_acceleration_structure);
    if (extensions.NV_cluster_acceleration_structure)
    {
        leanCmd.instantiateTriangleCluster(accelerationStructure.get(), scratchBuffer.get());
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR);
        MAGMA_INUSE(scratchBuffer);
    }
}

inline void CommandBuffer::getClusterTemplateIndices(lent_ptr<ClusterAccelerationStructure> accelerationStructure, lent_ptr<Buffer> scratchBuffer) const noexcept
{
    MAGMA_ASSERT(extensions.NV_cluster_acceleration_structure);
    if (extensions.NV_cluster_acceleration_structure)
    {
        leanCmd.getClusterTemplateIndices(accelerationStructure.get(), scratchBuffer.get());
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR);
        MAGMA_INUSE(scratchBuffer);
    }
}
#endif // VK_NV_cluster_acceleration_structure

#ifdef VK_KHR_ray_tracing_pipeline
inline void CommandBuffer::setRayTracingPipelineStackSize(uint32_t pipelineStackSize) const noexcept
{
    MAGMA_ASSERT(extensions.KHR_ray_tracing_pipeline);
    if (extensions.KHR_ray_tracing_pipeline)
    {
        leanCmd.setRayTracingPipelineStackSize(pipelineStackSize);
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_KHR);
    }
}

inline void CommandBuffer::traceRays(const ShaderBindingTable& shaderBindingTable, uint32_t width, uint32_t height, uint32_t depth /* 1 */) const noexcept
{
    MAGMA_ASSERT(extensions.KHR_ray_tracing_pipeline);
    if (extensions.KHR_ray_tracing_pipeline)
    {
        leanCmd.traceRays(shaderBindingTable, width, height, depth);
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_KHR);
    }
}

inline void CommandBuffer::traceRaysIndirect(const ShaderBindingTable& shaderBindingTable, lent_ptr<const Buffer> indirectBuffer) const noexcept
{
    MAGMA_ASSERT(extensions.KHR_ray_tracing_pipeline);
    if (extensions.KHR_ray_tracing_pipeline)
    {
        leanCmd.traceRaysIndirect(shaderBindingTable, indirectBuffer.get());
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_KHR);
        MAGMA_INUSE(indirectBuffer);
    }
}
#endif // VK_KHR_ray_tracing_pipeline

#ifdef VK_KHR_ray_tracing_maintenance1
inline void CommandBuffer::traceRaysIndirect(lent_ptr<const Buffer> indirectBuffer) const noexcept
{
    MAGMA_ASSERT(extensions.KHR_ray_tracing_maintenance1);
    if (extensions.KHR_ray_tracing_maintenance1)
    {
        leanCmd.traceRaysIndirect(indirectBuffer.get());
        MAGMA_CHECKPOINT(VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_KHR);
        MAGMA_INUSE(indirectBuffer);
    }
}
#endif // VK_KHR_ray_tracing_maintenance1

inline bool CommandBuffer::beginAnnotated(const char *name, uint32_t color, VkCommandBufferUsageFlags flags /* 0 */) noexcept
{
    if (begin(flags))
    {
        pushDebugMarker(name, color);
        annotatedBegin = VK_TRUE;
        return true;
    }
    return false;
}

inline bool CommandBuffer::beginInheritedAnnotated(const char *name, uint32_t color, lent_ptr<const RenderPass> renderPass, uint32_t subpass,
    lent_ptr<Framebuffer> framebuffer, bool occlusionQueryEnable, VkQueryControlFlags queryFlags /* 0 */, VkQueryPipelineStatisticFlags pipelineStatistics /* 0 */,
    VkCommandBufferUsageFlags flags /* 0 */, const StructureChain& extendedInfo /* default */) noexcept
{
    if (beginInherited(std::move(renderPass), subpass, std::move(framebuffer), occlusionQueryEnable, queryFlags, pipelineStatistics, flags, extendedInfo))
    {
        pushDebugMarker(name, color);
        annotatedBegin = VK_TRUE;
        return true;
    }
    return false;
}

inline void CommandBuffer::beginRenderPassAnnotated(const char *name, uint32_t color, lent_ptr<const RenderPass> renderPass, lent_ptr<Framebuffer> framebuffer,
    const std::initializer_list<ClearValue>& clearValues /* empty */, const VkRect2D& renderArea /* {0, 0, 0, 0} */, VkSubpassContents contents /* VK_SUBPASS_CONTENTS_INLINE */) noexcept
{
    beginRenderPass(std::move(renderPass), std::move(framebuffer), clearValues, renderArea, contents);
    pushDebugMarker(name, color);
    annotatedRenderPass = VK_TRUE;
}

#ifdef VK_KHR_imageless_framebuffer
inline void CommandBuffer::beginRenderPassAnnotated(const char *name, uint32_t color, lent_ptr<const RenderPass> renderPass, lent_ptr<ImagelessFramebuffer> framebuffer,
    const std::vector<lent_ptr<ImageView>>& attachments, const std::initializer_list<ClearValue>& clearValues /* empty */,
    const VkRect2D& renderArea /* {0, 0, 0, 0} */, VkSubpassContents contents /* VK_SUBPASS_CONTENTS_INLINE */) noexcept
{
    beginRenderPass(std::move(renderPass), std::move(framebuffer), attachments, clearValues, renderArea, contents);
    pushDebugMarker(name, color);
    annotatedRenderPass = VK_TRUE;
}
#endif // VK_KHR_imageless_framebuffer

#ifdef VK_KHR_device_group
inline bool CommandBuffer::beginDeviceGroupAnnotated(const char *name, uint32_t color, uint32_t deviceMask, VkCommandBufferUsageFlags flags /* 0 */) noexcept
{
    if (extensions.KHR_device_group)
    {
        bool result = beginDeviceGroup(deviceMask, flags);
        pushDebugMarker(name, color);
        annotatedBegin = VK_TRUE;
        return result;
    }
    return false;
}

inline void CommandBuffer::beginDeviceGroupRenderPassAnnotated(const char *name, uint32_t color, uint32_t deviceMask,
    lent_ptr<const RenderPass> renderPass, lent_ptr<Framebuffer> framebuffer, const std::initializer_list<ClearValue>& clearValues /* empty */,
    const std::initializer_list<VkRect2D>& deviceRenderAreas /* empty */, VkSubpassContents contents /* VK_SUBPASS_CONTENTS_INLINE */) noexcept
{
    if (extensions.KHR_device_group)
    {
        beginDeviceGroupRenderPass(deviceMask, std::move(renderPass), std::move(framebuffer), clearValues, deviceRenderAreas, contents);
        pushDebugMarker(name, color);
        annotatedRenderPass = VK_TRUE;
    }
}

#ifdef VK_KHR_imageless_framebuffer
inline void CommandBuffer::beginDeviceGroupRenderPassAnnotated(const char *name, uint32_t color, uint32_t deviceMask,
    lent_ptr<const RenderPass> renderPass, lent_ptr<ImagelessFramebuffer> framebuffer, const std::vector<lent_ptr<ImageView>>& attachments,
    const std::initializer_list<ClearValue>& clearValues /* empty */, const std::initializer_list<VkRect2D>& deviceRenderAreas /* empty */,
    VkSubpassContents contents /* VK_SUBPASS_CONTENTS_INLINE */) noexcept
{
    beginDeviceGroupRenderPass(deviceMask, std::move(renderPass), std::move(framebuffer), attachments, clearValues, deviceRenderAreas, contents);
    if (renderingPass)
    {
        pushDebugMarker(name, color);
        annotatedRenderPass = VK_TRUE;
    }
}
#endif // VK_KHR_imageless_framebuffer
#endif // VK_KHR_device_group

#ifdef VK_EXT_conditional_rendering
inline void CommandBuffer::beginConditionalRenderingAnnotated(const char *name, uint32_t color, lent_ptr<const Buffer> buffer, VkDeviceSize offset /* 0 */, bool inverted /* false */) noexcept
{
    beginConditionalRendering(std::move(buffer), offset, inverted);
    if (conditionalRendering)
    {
        pushDebugMarker(name, color);
        annotatedConditionalRendering = VK_TRUE;
    }
}
#endif // VK_EXT_conditional_rendering

#ifdef VK_EXT_transform_feedback
inline void CommandBuffer::beginTransformFeedbackAnnotated(const char *name, uint32_t color, uint32_t firstCounterBuffer, const std::initializer_list<lent_ptr<const Buffer>>& counterBuffers,
    const std::initializer_list<VkDeviceSize>& counterBufferOffsets /* empty */) noexcept
{
    beginTransformFeedback(firstCounterBuffer, counterBuffers, counterBufferOffsets);
    if (transformFeedback)
    {
        pushDebugMarker(name, color);
        annotatedTransformFeedback = VK_TRUE;
    }
}
#endif // VK_EXT_transform_feedback

inline uint32_t CommandBuffer::inUseObjectCount() const noexcept
{
#ifdef MAGMA_RETAIN_OBJECTS_IN_USE
    return core::countof(inUse);
#endif
    return 0;
}

inline void CommandBuffer::releaseObjectsInUse() noexcept
{
#ifdef MAGMA_RETAIN_OBJECTS_IN_USE
    while (!inUse.empty())
        inUse.pop_back();
#endif
}

inline void CommandBuffer::queueSubmissionFinished() noexcept
{
    MAGMA_ASSERT(State::Executable == state);
    if (State::Executable == state)
    {   // If any command buffer submitted to the queue is in the
        // executable state, it is moved to the pending state.
        state = State::Pending;
    }
}

inline void CommandBuffer::executionFinished() noexcept
{
    /* Once execution of all submissions of a command buffer complete,
       it moves from the pending state, back to the executable state.
       If a command buffer was recorded with the VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
       flag, it instead moves to the invalid state. */
    MAGMA_ASSERT(State::Pending == state);
    if (usage & VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT)
    {
        state = State::Invalid;
        releaseObjectsInUse();
    }
    else
    {
        state = State::Executable;
    }
}
} // namespace magma
