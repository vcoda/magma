namespace magma
{
inline void CommandBuffer::bindPipeline(const std::shared_ptr<Pipeline>& pipeline) noexcept
{
    vkCmdBindPipeline(handle, pipeline->getBindPoint(), *pipeline);
}

inline void CommandBuffer::setViewport(float x, float y, float width, float height,
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

inline void CommandBuffer::setViewport(uint32_t x, uint32_t y, uint32_t width, int32_t height,
    float minDepth /* 0 */, float maxDepth /* 1 */) noexcept
{
    setViewport(static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height), minDepth, maxDepth);
}

inline void CommandBuffer::setViewport(const VkViewport& viewport) noexcept
{
    vkCmdSetViewport(handle, 0, 1, &viewport);
}

inline void CommandBuffer::setViewports(const std::initializer_list<Viewport>& viewports) noexcept
{
    MAGMA_ASSERT(viewports.size());
    vkCmdSetViewport(handle, 0, MAGMA_COUNT(viewports), viewports.begin());
}

inline void CommandBuffer::setScissor(int32_t x, int32_t y, uint32_t width, uint32_t height) noexcept
{
    VkRect2D scissor;
    scissor.offset.x = x;
    scissor.offset.y = y;
    scissor.extent.width = width;
    scissor.extent.height = height;
    vkCmdSetScissor(handle, 0, 1, &scissor);
}

inline void CommandBuffer::setScissor(const VkRect2D& scissor) noexcept
{
    vkCmdSetScissor(handle, 0, 1, &scissor);
}

inline void CommandBuffer::setScissors(const std::initializer_list<VkRect2D>& scissors) noexcept
{
    MAGMA_ASSERT(scissors.size());
    vkCmdSetScissor(handle, 0, MAGMA_COUNT(scissors), scissors.begin());
}

inline void CommandBuffer::setLineWidth(float lineWidth) noexcept
{
    vkCmdSetLineWidth(handle, lineWidth);
}

inline void CommandBuffer::setDepthBias(float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) noexcept
{
    vkCmdSetDepthBias(handle, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
}

inline void CommandBuffer::setBlendConstants(const float blendConstants[4]) noexcept
{
    MAGMA_ASSERT(blendConstants);
    vkCmdSetBlendConstants(handle, blendConstants);
}

inline void CommandBuffer::setDepthBounds(float minDepthBounds, float maxDepthBounds) noexcept
{
    MAGMA_ASSERT(minDepthBounds <= maxDepthBounds);
    vkCmdSetDepthBounds(handle, minDepthBounds, maxDepthBounds);
}

inline void CommandBuffer::setStencilCompareMask(bool frontFace, bool backFace, uint32_t compareMask) noexcept
{
    MAGMA_ASSERT(frontFace || backFace);
    vkCmdSetStencilCompareMask(handle, MAGMA_STENCIL_FACE_MASK(frontFace, backFace), compareMask);
}

inline void CommandBuffer::setStencilWriteMask(bool frontFace, bool backFace, uint32_t writeMask) noexcept
{
    MAGMA_ASSERT(frontFace || backFace);
    vkCmdSetStencilWriteMask(handle, MAGMA_STENCIL_FACE_MASK(frontFace, backFace), writeMask);
}

inline void CommandBuffer::setStencilReference(bool frontFace, bool backFace, uint32_t reference) noexcept
{
    MAGMA_ASSERT(frontFace || backFace);
    vkCmdSetStencilReference(handle, MAGMA_STENCIL_FACE_MASK(frontFace, backFace), reference);
}

inline void CommandBuffer::bindDescriptorSet(const std::shared_ptr<Pipeline>& pipeline, uint32_t setIndex, const std::shared_ptr<DescriptorSet>& descriptorSet,
    uint32_t dynamicOffset /* -1 */) noexcept
{
    MAGMA_ASSERT(pipeline->getLayout()->hasSetLayout(descriptorSet->getLayout()));
    const VkDescriptorSet dereferencedDescriptorSet[1] = {*descriptorSet};
    if (descriptorSet->dirty())
        descriptorSet->update();
    vkCmdBindDescriptorSets(handle, pipeline->getBindPoint(), *pipeline->getLayout(), setIndex, 1, dereferencedDescriptorSet,
        (0xFFFFFFFF == dynamicOffset) ? 0 : 1,
        (0xFFFFFFFF == dynamicOffset) ? nullptr : &dynamicOffset);
}

inline void CommandBuffer::bindDescriptorSet(const std::shared_ptr<Pipeline>& pipeline, uint32_t setIndex, const std::shared_ptr<DescriptorSet>& descriptorSet,
    const std::initializer_list<uint32_t>& dynamicOffsets) noexcept
{
    MAGMA_ASSERT(pipeline->getLayout()->hasSetLayout(descriptorSet->getLayout()));
    const VkDescriptorSet dereferencedDescriptorSet[1] = {*descriptorSet};
    if (descriptorSet->dirty())
        descriptorSet->update();
    vkCmdBindDescriptorSets(handle, pipeline->getBindPoint(), *pipeline->getLayout(), setIndex, 1, dereferencedDescriptorSet, MAGMA_COUNT(dynamicOffsets), dynamicOffsets.begin());
}

inline void CommandBuffer::bindIndexBuffer(const std::shared_ptr<BaseIndexBuffer>& indexBuffer, VkDeviceSize offset /* 0 */) noexcept
{
    vkCmdBindIndexBuffer(handle, *indexBuffer, offset, indexBuffer->getIndexType());
}

inline void CommandBuffer::bindVertexBuffer(uint32_t firstBinding, const std::shared_ptr<BaseVertexBuffer>& vertexBuffer,
    VkDeviceSize offset /* 0 */) noexcept
{
    const VkBuffer dereferencedBuffers[1] = {*vertexBuffer};
    vkCmdBindVertexBuffers(handle, firstBinding, 1, dereferencedBuffers, &offset);
}

inline void CommandBuffer::bindVertexBuffers(uint32_t firstBinding, const std::vector<std::shared_ptr<BaseVertexBuffer>>& vertexBuffers,
    std::vector<VkDeviceSize> offsets /* {} */) noexcept
{
    MAGMA_ASSERT(vertexBuffers.size() > 0);
    MAGMA_STACK_ARRAY(VkBuffer, dereferencedBuffers, vertexBuffers.size());
    for (const auto& buffer : vertexBuffers)
        dereferencedBuffers.put(*buffer);
    if (offsets.empty())
        offsets.resize(vertexBuffers.size(), 0);
    else
        MAGMA_ASSERT(offsets.size() >= vertexBuffers.size());
    vkCmdBindVertexBuffers(handle, firstBinding, dereferencedBuffers.size(), dereferencedBuffers, offsets.data());
}

inline void CommandBuffer::draw(uint32_t vertexCount,
    uint32_t firstVertex /* 0 */) const noexcept
{
    vkCmdDraw(handle, vertexCount, 1, firstVertex, 0);
}

inline void CommandBuffer::drawInstanced(uint32_t vertexCount, uint32_t instanceCount,
    uint32_t firstVertex /* 0 */, uint32_t firstInstance /* 0 */) const noexcept
{
    vkCmdDraw(handle, vertexCount, instanceCount, firstVertex, firstInstance);
}

inline void CommandBuffer::drawIndexed(uint32_t indexCount,
    uint32_t firstIndex /* 0 */, int32_t vertexOffset /* 0 */) const noexcept
{
    vkCmdDrawIndexed(handle, indexCount, 1, firstIndex, vertexOffset, 0);
}

inline void CommandBuffer::drawIndexedInstanced(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) const noexcept
{
    vkCmdDrawIndexed(handle, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

#ifdef VK_EXT_multi_draw
inline void CommandBuffer::drawMultiIndexed(const std::vector<VkMultiDrawIndexedInfoEXT>& indexInfo, uint32_t stride,
    const std::vector<int32_t>& vertexOffsets /* {} */) const noexcept
{
    vkCmdDrawMultiIndexedEXT(handle, MAGMA_COUNT(indexInfo), indexInfo.data(), 1, 0, stride, vertexOffsets.data());
}

inline void CommandBuffer::drawMultiIndexedInstanced(const std::vector<VkMultiDrawIndexedInfoEXT>& indexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride,
    const std::vector<int32_t>& vertexOffsets /* {} */) const noexcept
{
    vkCmdDrawMultiIndexedEXT(handle, MAGMA_COUNT(indexInfo), indexInfo.data(), instanceCount, firstInstance, stride, vertexOffsets.data());
}
#endif // VK_EXT_multi_draw

inline void CommandBuffer::drawIndirect(const std::shared_ptr<DrawIndirectBuffer>& buffer) const noexcept
{
    vkCmdDrawIndirect(handle, *buffer, 0, buffer->getDrawCommandCount(), buffer->getStride());
}

inline void CommandBuffer::drawIndexedIndirect(const std::shared_ptr<DrawIndexedIndirectBuffer>& buffer) const noexcept
{
    vkCmdDrawIndexedIndirect(handle, *buffer, 0, buffer->getDrawCommandCount(), buffer->getStride());
}

inline void CommandBuffer::dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const noexcept
{
    vkCmdDispatch(handle, groupCountX, groupCountY, groupCountZ);
}

inline void CommandBuffer::dispatchIndirect(const std::shared_ptr<Buffer>& buffer, VkDeviceSize offset) const noexcept
{
    vkCmdDispatchIndirect(handle, *buffer, offset);
}

inline void CommandBuffer::copyBuffer(const std::shared_ptr<const Buffer>& srcBuffer, const std::shared_ptr<Buffer>& dstBuffer, const VkBufferCopy& region) const noexcept
{
    vkCmdCopyBuffer(handle, *srcBuffer, *dstBuffer, 1, &region);
}

inline void CommandBuffer::copyImage(const std::shared_ptr<const Image>& srcImage, const std::shared_ptr<Image>& dstImage, const VkImageCopy& region) const noexcept
{
    vkCmdCopyImage(handle, *srcImage, srcImage->getLayout(), *dstImage, dstImage->getLayout(), 1, &region);
}

inline void CommandBuffer::blitImage(const std::shared_ptr<const Image>& srcImage, const std::shared_ptr<Image>& dstImage, const VkImageBlit& region, VkFilter filter) const noexcept
{
    vkCmdBlitImage(handle, *srcImage, srcImage->getLayout(), *dstImage, dstImage->getLayout(), 1, &region, filter);
}

inline void CommandBuffer::copyBufferToImage(const std::shared_ptr<const Buffer>& srcBuffer, const std::shared_ptr<Image>& dstImage, const VkBufferImageCopy& region) const noexcept
{
    vkCmdCopyBufferToImage(handle, *srcBuffer, *dstImage, dstImage->getLayout(), 1, &region);
}

inline void CommandBuffer::copyBufferToImage(const std::shared_ptr<const Buffer>& srcBuffer, const std::shared_ptr<Image>& dstImage, const std::vector<VkBufferImageCopy>& regions) const noexcept
{
    vkCmdCopyBufferToImage(handle, *srcBuffer, *dstImage, dstImage->getLayout(), MAGMA_COUNT(regions), regions.data());
}

inline void CommandBuffer::copyImageToBuffer(const std::shared_ptr<const Image>& srcImage, const std::shared_ptr<Buffer>& dstBuffer, const VkBufferImageCopy& region) const noexcept
{
    vkCmdCopyImageToBuffer(handle, *srcImage, srcImage->getLayout(), *dstBuffer, 1, &region);
}

inline void CommandBuffer::copyImageToBuffer(const std::shared_ptr<const Image>& srcImage, const std::shared_ptr<Buffer>& dstBuffer, const std::vector<VkBufferImageCopy>& regions) const noexcept
{
    vkCmdCopyImageToBuffer(handle, *srcImage, srcImage->getLayout(), *dstBuffer, MAGMA_COUNT(regions), regions.data());
}

inline void CommandBuffer::updateBuffer(const std::shared_ptr<Buffer>& buffer, VkDeviceSize dataSize, const void *data,
    VkDeviceSize offset /* 0 */) const noexcept
{
    /* Buffer updates performed with vkCmdUpdateBuffer first copy the data
       into command buffer memory when the command is recorded
       (which requires additional storage and may incur an additional allocation),
       and then copy the data from the command buffer into dstBuffer
       when the command is executed on a device. */
    vkCmdUpdateBuffer(handle, *buffer, offset, dataSize,
        (const uint32_t *)data); // Compatibility with old SDK
}

template<typename Type>
inline void CommandBuffer::updateBuffer(const std::shared_ptr<Buffer>& buffer, const std::vector<Type>& data,
    VkDeviceSize offset /* 0 */) const noexcept
{
    const VkDeviceSize dataSize = static_cast<VkDeviceSize>(sizeof(Type) * data.size());
    udpateBuffer(buffer, dataSize, data.data(), offset);
}

inline void CommandBuffer::clearColorImage(const std::shared_ptr<Image>& image, const ClearColor& color, const VkImageSubresourceRange& range) const noexcept
{
    const VkClearColorValue& clearColor = static_cast<const VkClearValue&>(color).color;
    vkCmdClearColorImage(handle, *image, VK_IMAGE_LAYOUT_GENERAL, &clearColor, 1, &range);
}

inline void CommandBuffer::clearDepthStencilImage(const std::shared_ptr<Image>& image, const ClearDepthStencil& depthStencil, const VkImageSubresourceRange& range) const noexcept
{
    const VkClearDepthStencilValue& clearDepthStencil = static_cast<const VkClearValue&>(depthStencil).depthStencil;
    vkCmdClearDepthStencilImage(handle, *image, VK_IMAGE_LAYOUT_GENERAL, &clearDepthStencil, 1, &range);
}

inline void CommandBuffer::clearAttachments(const std::initializer_list<ClearAttachment>& attachments, const VkClearRect& clearRect) const noexcept
{
    vkCmdClearAttachments(handle, MAGMA_COUNT(attachments), attachments.begin(), 1, &clearRect);
}

inline void CommandBuffer::resolveImage(const std::shared_ptr<Image>& srcImage, const std::shared_ptr<Image>& dstImage, const VkImageResolve& region) const noexcept
{
    vkCmdResolveImage(handle, *srcImage, srcImage->getLayout(), *dstImage, dstImage->getLayout(), 1, &region);
}

inline void CommandBuffer::setEvent(const std::shared_ptr<Event>& event, VkPipelineStageFlags stageMask) noexcept
{
    MAGMA_ASSERT(!(stageMask & VK_PIPELINE_STAGE_HOST_BIT));
    vkCmdSetEvent(handle, *event, stageMask);
}

inline void CommandBuffer::resetEvent(const std::shared_ptr<Event>& event, VkPipelineStageFlags stageMask) noexcept
{
    MAGMA_ASSERT(!(stageMask & VK_PIPELINE_STAGE_HOST_BIT));
    vkCmdResetEvent(handle, *event, stageMask);
}

inline void CommandBuffer::pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const MemoryBarrier& barrier,
    VkDependencyFlags dependencyFlags /* 0 */) noexcept
{
    vkCmdPipelineBarrier(handle, srcStageMask, dstStageMask, dependencyFlags,
        1, &barrier,
        0, nullptr,
        0, nullptr);
}

inline void CommandBuffer::pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const BufferMemoryBarrier& barrier,
    VkDependencyFlags dependencyFlags /* 0 */) noexcept
{
    vkCmdPipelineBarrier(handle, srcStageMask, dstStageMask, dependencyFlags,
        0, nullptr,
        1, &barrier,
        0, nullptr);
}

inline void CommandBuffer::pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const ImageMemoryBarrier& barrier,
    VkDependencyFlags dependencyFlags /* 0 */) noexcept
{
    vkCmdPipelineBarrier(handle, srcStageMask, dstStageMask, dependencyFlags,
        0, nullptr,
        0, nullptr,
        1, &barrier);
    barrier.resource->setLayout(barrier.newLayout);
}

inline void CommandBuffer::beginQuery(const std::shared_ptr<QueryPool>& queryPool, uint32_t queryIndex, bool precise) noexcept
{
    MAGMA_ASSERT(queryIndex < queryPool->getQueryCount());
    VkQueryControlFlags flags = 0;
    if (precise)
        flags |= VK_QUERY_CONTROL_PRECISE_BIT;
    vkCmdBeginQuery(handle, *queryPool, queryIndex, flags);
}

inline void CommandBuffer::endQuery(const std::shared_ptr<QueryPool>& queryPool, uint32_t queryIndex) noexcept
{
    MAGMA_ASSERT(queryIndex < queryPool->getQueryCount());
    vkCmdEndQuery(handle, *queryPool, queryIndex);
}

inline void CommandBuffer::resetQueryPool(const std::shared_ptr<QueryPool>& queryPool) noexcept
{
    vkCmdResetQueryPool(handle, *queryPool, 0, queryPool->getQueryCount());
}

inline void CommandBuffer::writeTimestamp(VkPipelineStageFlagBits pipelineStage, const std::shared_ptr<QueryPool>& queryPool, uint32_t queryIndex) noexcept
{
    vkCmdWriteTimestamp(handle, pipelineStage, *queryPool, queryIndex);
}

template<typename Type>
inline void CommandBuffer::pushConstant(const std::shared_ptr<PipelineLayout>& layout, VkShaderStageFlags stageFlags, const Type& constant,
    uint32_t offset /* 0 */) noexcept
{
    vkCmdPushConstants(handle, *layout, stageFlags, offset, static_cast<uint32_t>(sizeof(Type)), &constant);
}

template<typename Type, uint32_t pushConstantCount>
inline void CommandBuffer::pushConstants(const std::shared_ptr<PipelineLayout>& layout, VkShaderStageFlags stageFlags, const Type(&constants)[pushConstantCount],
    uint32_t offset /* 0 */) noexcept
{
    vkCmdPushConstants(handle, *layout, stageFlags, offset, static_cast<uint32_t>(sizeof(Type) * pushConstantCount), constants);
}

template<typename Type>
inline void CommandBuffer::pushConstants(const std::shared_ptr<PipelineLayout>& layout, VkShaderStageFlags stageFlags, const std::vector<Type>& constants,
    uint32_t offset /* 0 */) noexcept
{
    vkCmdPushConstants(handle, *layout, stageFlags, offset, static_cast<uint32_t>(sizeof(Type) * constants.size()), constants.data());
}

template<typename BlockType>
inline void CommandBuffer::pushConstantBlock(const std::shared_ptr<PipelineLayout>& layout, VkShaderStageFlags stageFlags, const BlockType& block,
    uint32_t offset /* 0 */) noexcept
{
    vkCmdPushConstants(handle, *layout, stageFlags, offset, static_cast<uint32_t>(sizeof(BlockType)), &block);
}

inline void CommandBuffer::nextSubpass(VkSubpassContents contents /* VK_SUBPASS_CONTENTS_INLINE */) noexcept
{
    vkCmdNextSubpass(handle, contents);
}

inline void CommandBuffer::endRenderPass() noexcept
{
#ifdef MAGMA_DEBUG_LABEL
    if (beginRenderPassMarked)
    {
        endDebugLabel();
        beginRenderPassMarked = VK_FALSE;
    }
#endif // MAGMA_DEBUG_LABEL
    vkCmdEndRenderPass(handle);
}

inline void CommandBuffer::enableOcclusionQuery(bool enable, VkQueryControlFlags queryFlags) noexcept
{
    occlusionQueryEnable = MAGMA_BOOLEAN(enable);
    this->queryFlags = queryFlags;
}

inline void CommandBuffer::enableConditionalRendering(bool enable) noexcept
{
    conditionalRenderingEnable = MAGMA_BOOLEAN(enable);
}

inline void CommandBuffer::queryPipelineStatistics(VkQueryPipelineStatisticFlags pipelineStatistics) noexcept
{
    this->pipelineStatistics = pipelineStatistics;
}
} // namespace magma
