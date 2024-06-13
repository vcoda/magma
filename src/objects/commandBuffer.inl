namespace magma
{
#ifdef MAGMA_RETAIN_OBJECTS_IN_USE
#define MAGMA_INUSE(child)\
if (child) try {\
    CommandBuffer::inUse.insert(child);\
} catch(...) {}
#else
#define MAGMA_INUSE(child)
#endif // MAGMA_TRACK_INUSE

inline void CommandBuffer::bindPipeline(const std::shared_ptr<Pipeline>& pipeline) noexcept
{
    vkCmdBindPipeline(handle, pipeline->getBindPoint(), *pipeline);
    MAGMA_INUSE(pipeline);
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
    vkCmdSetViewport(handle, 0, MAGMA_COUNT(viewports), viewports.begin());
}

inline void CommandBuffer::setScissor(int32_t x, int32_t y, uint32_t width, uint32_t height) noexcept
{
    const VkRect2D scissor = {x, y, width, height};
    vkCmdSetScissor(handle, 0, 1, &scissor);
}

inline void CommandBuffer::setScissor(const VkRect2D& scissor) noexcept
{
    vkCmdSetScissor(handle, 0, 1, &scissor);
}

inline void CommandBuffer::setScissors(const std::initializer_list<VkRect2D>& scissors) noexcept
{
    vkCmdSetScissor(handle, 0, MAGMA_COUNT(scissors), scissors.begin());
}

inline void CommandBuffer::setLineWidth(float lineWidth) noexcept
{
    vkCmdSetLineWidth(handle, lineWidth);
}

#ifdef VK_EXT_line_rasterization
inline void CommandBuffer::setLineStipple(uint32_t stippleFactor, uint16_t stipplePattern) noexcept
{
    MAGMA_DEVICE_EXTENSION(vkCmdSetLineStippleEXT);
    if (vkCmdSetLineStippleEXT)
        vkCmdSetLineStippleEXT(handle, stippleFactor, stipplePattern);
}
#endif // VK_EXT_line_rasterization

inline void CommandBuffer::setDepthBias(float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) noexcept
{
    vkCmdSetDepthBias(handle, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
}

inline void CommandBuffer::setBlendConstants(const float blendConstants[4]) noexcept
{
    vkCmdSetBlendConstants(handle, blendConstants);
}

inline void CommandBuffer::setDepthBounds(float minDepthBounds, float maxDepthBounds) noexcept
{
    vkCmdSetDepthBounds(handle, minDepthBounds, maxDepthBounds);
}

inline void CommandBuffer::setStencilCompareMask(bool frontFace, bool backFace, uint32_t compareMask) noexcept
{
    vkCmdSetStencilCompareMask(handle, MAGMA_STENCIL_FACE_MASK(frontFace, backFace), compareMask);
}

inline void CommandBuffer::setStencilWriteMask(bool frontFace, bool backFace, uint32_t writeMask) noexcept
{
    vkCmdSetStencilWriteMask(handle, MAGMA_STENCIL_FACE_MASK(frontFace, backFace), writeMask);
}

inline void CommandBuffer::setStencilReference(bool frontFace, bool backFace, uint32_t reference) noexcept
{
    vkCmdSetStencilReference(handle, MAGMA_STENCIL_FACE_MASK(frontFace, backFace), reference);
}

inline void CommandBuffer::bindDescriptorSet(VkPipelineBindPoint bindPoint, const std::shared_ptr<PipelineLayout>& layout, uint32_t setIndex,
    const std::shared_ptr<DescriptorSet>& descriptorSet, uint32_t dynamicOffset /* std::numeric_limits<uint32_t>::max() */) noexcept
{
    MAGMA_ASSERT(layout->hasLayout(descriptorSet->getLayout()));
    MAGMA_ASSERT(!descriptorSet->dirty());
    const uint32_t dynamicOffsetCount = dynamicOffset < std::numeric_limits<uint32_t>::max() ? 1 : 0;
    vkCmdBindDescriptorSets(handle, bindPoint, *layout, setIndex, 1, descriptorSet->getHandleAddress(), dynamicOffsetCount, dynamicOffsetCount ? &dynamicOffset : nullptr);
    MAGMA_INUSE(layout);
    MAGMA_INUSE(descriptorSet);
}

inline void CommandBuffer::bindDescriptorSet(VkPipelineBindPoint bindPoint, const std::shared_ptr<PipelineLayout>& layout, uint32_t setIndex,
    const std::shared_ptr<DescriptorSet>& descriptorSet, const std::initializer_list<uint32_t>& dynamicOffsets) noexcept
{
    MAGMA_ASSERT(layout->hasLayout(descriptorSet->getLayout()));
    MAGMA_ASSERT(!descriptorSet->dirty());
    vkCmdBindDescriptorSets(handle, bindPoint, *layout, setIndex, 1, descriptorSet->getHandleAddress(), MAGMA_COUNT(dynamicOffsets), dynamicOffsets.begin());
    MAGMA_INUSE(layout);
    MAGMA_INUSE(descriptorSet);
}

inline void CommandBuffer::bindDescriptorSet(const std::shared_ptr<Pipeline>& pipeline, uint32_t setIndex,
    const std::shared_ptr<DescriptorSet>& descriptorSet, uint32_t dynamicOffset /* std::numeric_limits<uint32_t>::max() */) noexcept
{
    bindDescriptorSet(pipeline->getBindPoint(), pipeline->getLayout(), setIndex, descriptorSet, dynamicOffset);
}

inline void CommandBuffer::bindDescriptorSet(const std::shared_ptr<Pipeline>& pipeline, uint32_t setIndex,
    const std::shared_ptr<DescriptorSet>& descriptorSet, const std::initializer_list<uint32_t>& dynamicOffsets) noexcept
{
    bindDescriptorSet(pipeline->getBindPoint(), pipeline->getLayout(), setIndex, descriptorSet, dynamicOffsets);
}

inline void CommandBuffer::bindIndexBuffer(const std::shared_ptr<BaseIndexBuffer>& indexBuffer, VkDeviceSize offset /* 0 */) noexcept
{
    vkCmdBindIndexBuffer(handle, *indexBuffer, offset, indexBuffer->getIndexType());
    MAGMA_INUSE(indexBuffer);
}

inline void CommandBuffer::bindVertexBuffer(uint32_t firstBinding, const std::shared_ptr<Buffer>& vertexBuffer,
    VkDeviceSize offset /* 0 */) noexcept
{
    vkCmdBindVertexBuffers(handle, firstBinding, 1, vertexBuffer->getHandleAddress(), &offset);
    MAGMA_INUSE(vertexBuffer);
}

inline void CommandBuffer::bindVertexBuffers(uint32_t firstBinding, const std::vector<std::shared_ptr<Buffer>>& vertexBuffers,
    std::vector<VkDeviceSize> offsets /* {} */) noexcept
{
    MAGMA_ASSERT(vertexBuffers.size() > 0);
    MAGMA_STACK_ARRAY(VkBuffer, dereferencedBuffers, vertexBuffers.size());
    for (auto const& buffer: vertexBuffers)
    {
        dereferencedBuffers.put(*buffer);
        MAGMA_INUSE(buffer);
    }
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

inline void CommandBuffer::drawIndirect(const std::shared_ptr<DrawIndirectBuffer>& buffer,
    VkDeviceSize offset /* 0 */) const noexcept
{
    vkCmdDrawIndirect(handle, *buffer, offset, buffer->getDrawCount(), buffer->getStride());
    MAGMA_INUSE(buffer);
}

inline void CommandBuffer::drawIndexedIndirect(const std::shared_ptr<DrawIndexedIndirectBuffer>& buffer,
    VkDeviceSize offset /* 0 */) const noexcept
{
    vkCmdDrawIndexedIndirect(handle, *buffer, offset, buffer->getDrawCount(), buffer->getStride());
    MAGMA_INUSE(buffer);
}

#if defined(VK_KHR_draw_indirect_count) || defined(VK_AMD_draw_indirect_count)
inline void CommandBuffer::drawIndirectCount(const std::shared_ptr<DrawIndirectBuffer>& buffer, const std::shared_ptr<Buffer>& countBuffer,
    VkDeviceSize offset /* 0 */,
    VkDeviceSize countBufferOffset /* 0 */) const noexcept
{
#ifdef VK_KHR_draw_indirect_count
    MAGMA_DEVICE_EXTENSION(vkCmdDrawIndirectCountKHR);
    if (vkCmdDrawIndirectCountKHR)
        vkCmdDrawIndirectCountKHR(handle, *buffer, offset, *countBuffer, countBufferOffset, buffer->getDrawCount(), buffer->getStride());
    else
#endif // VK_KHR_draw_indirect_count
    {
    #ifdef VK_AMD_draw_indirect_count
        MAGMA_DEVICE_EXTENSION(vkCmdDrawIndirectCountAMD);
        if (vkCmdDrawIndirectCountAMD)
            vkCmdDrawIndirectCountAMD(handle, *buffer, offset, *countBuffer, countBufferOffset, buffer->getDrawCount(), buffer->getStride());
    #endif // VK_AMD_draw_indirect_count
    }
    MAGMA_INUSE(buffer);
    MAGMA_INUSE(countBuffer);
}

inline void CommandBuffer::drawIndexedIndirectCount(const std::shared_ptr<DrawIndexedIndirectBuffer>& buffer, const std::shared_ptr<Buffer>& countBuffer,
    VkDeviceSize offset /* 0 */,
    VkDeviceSize countBufferOffset /* 0 */) const noexcept
{
#ifdef VK_KHR_draw_indirect_count
    MAGMA_DEVICE_EXTENSION(vkCmdDrawIndexedIndirectCountKHR);
    if (vkCmdDrawIndexedIndirectCountKHR)
        vkCmdDrawIndexedIndirectCountKHR(handle, *buffer, offset, *countBuffer, countBufferOffset, buffer->getDrawCount(), buffer->getStride());
    else
#endif // VK_KHR_draw_indirect_count
    {
    #ifdef VK_AMD_draw_indirect_count
        MAGMA_DEVICE_EXTENSION(vkCmdDrawIndexedIndirectCountAMD);
        if (vkCmdDrawIndexedIndirectCountAMD)
            vkCmdDrawIndexedIndirectCountAMD(handle, *buffer, offset, *countBuffer, countBufferOffset, buffer->getDrawCount(), buffer->getStride());
    #endif // VK_AMD_draw_indirect_count
    }
    MAGMA_INUSE(buffer);
    MAGMA_INUSE(countBuffer);
}
#endif // VK_KHR_draw_indirect_count || VK_AMD_draw_indirect_count

#ifdef VK_EXT_multi_draw
inline void CommandBuffer::drawMulti(const std::vector<VkMultiDrawInfoEXT>& vertexInfo) const noexcept
{
    MAGMA_DEVICE_EXTENSION(vkCmdDrawMultiEXT);
    if (vkCmdDrawMultiEXT)
        vkCmdDrawMultiEXT(handle, MAGMA_COUNT(vertexInfo), vertexInfo.data(), 1, 0, sizeof(VkMultiDrawInfoEXT));
}

inline void CommandBuffer::drawMultiInstanced(const std::vector<VkMultiDrawInfoEXT>& vertexInfo, uint32_t instanceCount, uint32_t firstInstance) const noexcept
{
    MAGMA_DEVICE_EXTENSION(vkCmdDrawMultiEXT);
    if (vkCmdDrawMultiEXT)
        vkCmdDrawMultiEXT(handle, MAGMA_COUNT(vertexInfo), vertexInfo.data(), instanceCount, firstInstance, sizeof(VkMultiDrawInfoEXT));
}

inline void CommandBuffer::drawMultiIndexed(const std::vector<VkMultiDrawIndexedInfoEXT>& indexInfo,
    const std::vector<int32_t>& vertexOffsets /* {} */) const noexcept
{
    MAGMA_DEVICE_EXTENSION(vkCmdDrawMultiIndexedEXT);
    if (vkCmdDrawMultiIndexedEXT)
        vkCmdDrawMultiIndexedEXT(handle, MAGMA_COUNT(indexInfo), indexInfo.data(), 1, 0, sizeof(VkMultiDrawIndexedInfoEXT), vertexOffsets.data());
}

inline void CommandBuffer::drawMultiIndexedInstanced(const std::vector<VkMultiDrawIndexedInfoEXT>& indexInfo, uint32_t instanceCount, uint32_t firstInstance,
    const std::vector<int32_t>& vertexOffsets /* {} */) const noexcept
{
    MAGMA_DEVICE_EXTENSION(vkCmdDrawMultiIndexedEXT);
    if (vkCmdDrawMultiIndexedEXT)
        vkCmdDrawMultiIndexedEXT(handle, MAGMA_COUNT(indexInfo), indexInfo.data(), instanceCount, firstInstance, sizeof(VkMultiDrawIndexedInfoEXT), vertexOffsets.data());
}
#endif // VK_EXT_multi_draw

#ifdef VK_EXT_transform_feedback
inline void CommandBuffer::drawIndirectByteCount(uint32_t instanceCount, uint32_t firstInstance, const std::shared_ptr<TransformFeedbackCounterBuffer>& counterBuffer,
    VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride) const noexcept
{
    MAGMA_DEVICE_EXTENSION(vkCmdDrawIndirectByteCountEXT);
    if (vkCmdDrawIndirectByteCountEXT)
    {
        vkCmdDrawIndirectByteCountEXT(handle, instanceCount, firstInstance, *counterBuffer, counterBufferOffset, counterOffset, vertexStride);
        MAGMA_INUSE(counterBuffer);
    }
}
#endif // VK_EXT_transform_feedback

#if defined(VK_EXT_mesh_shader) || defined(VK_NV_mesh_shader)
inline void CommandBuffer::drawMeshTasks(uint32_t groupCountX,
    uint32_t groupCountY /* 1 */,
    uint32_t groupCountZ /* 1 */) const noexcept
{
#ifdef VK_EXT_mesh_shader
    MAGMA_DEVICE_EXTENSION(vkCmdDrawMeshTasksEXT);
    if (vkCmdDrawMeshTasksEXT)
        vkCmdDrawMeshTasksEXT(handle, groupCountX, groupCountY, groupCountZ);
    else
#endif // VK_EXT_mesh_shader
    {
        MAGMA_UNUSED(groupCountY);
        MAGMA_UNUSED(groupCountZ);
    #ifdef VK_NV_mesh_shader
        // Y and Z dimension are implicitly set to 1
        MAGMA_ASSERT(1 == groupCountY);
        MAGMA_ASSERT(1 == groupCountZ);
        MAGMA_DEVICE_EXTENSION(vkCmdDrawMeshTasksNV);
        if (vkCmdDrawMeshTasksNV)
            vkCmdDrawMeshTasksNV(handle, groupCountX, 0 /* firstTask */);
    #endif // VK_NV_mesh_shader
    }
}

inline void CommandBuffer::drawMeshTasksIndirect(const std::shared_ptr<DrawMeshTasksIndirectBuffer>& buffer,
    VkDeviceSize offset /* 0 */) const noexcept
{
#ifdef VK_EXT_mesh_shader
    MAGMA_DEVICE_EXTENSION(vkCmdDrawMeshTasksIndirectEXT);
    if (vkCmdDrawMeshTasksIndirectEXT)
        vkCmdDrawMeshTasksIndirectEXT(handle, *buffer, offset, buffer->getDrawCount(), buffer->getStride());
    else
#endif // VK_EXT_mesh_shader
    {
    #ifdef VK_NV_mesh_shader
        MAGMA_DEVICE_EXTENSION(vkCmdDrawMeshTasksIndirectNV);
        if (vkCmdDrawMeshTasksIndirectNV)
            vkCmdDrawMeshTasksIndirectNV(handle, *buffer, offset, buffer->getDrawCount(), buffer->getStride());
    #endif // VK_NV_mesh_shader
    }
    MAGMA_INUSE(buffer);
}

inline void CommandBuffer::drawMeshTasksIndirectCount(const std::shared_ptr<DrawMeshTasksIndirectBuffer>& buffer, const std::shared_ptr<Buffer>& countBuffer,
    VkDeviceSize offset /* 0 */,
    VkDeviceSize countBufferOffset /* 0 */) const noexcept
{
#ifdef VK_EXT_mesh_shader
    MAGMA_DEVICE_EXTENSION(vkCmdDrawMeshTasksIndirectCountEXT);
    if (vkCmdDrawMeshTasksIndirectCountEXT)
        vkCmdDrawMeshTasksIndirectCountEXT(handle, *buffer, offset, *countBuffer, countBufferOffset, buffer->getDrawCount(), buffer->getStride());
    else
#endif // VK_EXT_mesh_shader
    {
    #ifdef VK_NV_mesh_shader
        MAGMA_DEVICE_EXTENSION(vkCmdDrawMeshTasksIndirectCountNV);
        if (vkCmdDrawMeshTasksIndirectCountNV)
            vkCmdDrawMeshTasksIndirectCountNV(handle, *buffer, offset, *countBuffer, countBufferOffset, buffer->getDrawCount(), buffer->getStride());
    #endif // VK_NV_mesh_shader
    }
    MAGMA_INUSE(buffer);
    MAGMA_INUSE(countBuffer);
}
#endif // VK_EXT_mesh_shader || VK_NV_mesh_shader

inline void CommandBuffer::dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const noexcept
{
    vkCmdDispatch(handle, groupCountX, groupCountY, groupCountZ);
}

inline void CommandBuffer::dispatchIndirect(const std::shared_ptr<Buffer>& buffer,
    VkDeviceSize offset /* 0 */) const noexcept
{
    vkCmdDispatchIndirect(handle, *buffer, offset);
    MAGMA_INUSE(buffer);
}

inline void CommandBuffer::copyBuffer(const std::shared_ptr<const Buffer>& srcBuffer, const std::shared_ptr<Buffer>& dstBuffer, const VkBufferCopy& region) const noexcept
{
    vkCmdCopyBuffer(handle, *srcBuffer, *dstBuffer, 1, &region);
    MAGMA_INUSE(srcBuffer);
    MAGMA_INUSE(dstBuffer);
}

inline void CommandBuffer::copyImage(const std::shared_ptr<const Image>& srcImage, const std::shared_ptr<Image>& dstImage, const VkImageCopy& region) const noexcept
{
    vkCmdCopyImage(handle, *srcImage, srcImage->getLayout(), *dstImage, dstImage->getLayout(), 1, &region);
    MAGMA_INUSE(srcImage);
    MAGMA_INUSE(dstImage);
}

inline void CommandBuffer::blitImage(const std::shared_ptr<const Image>& srcImage, const std::shared_ptr<Image>& dstImage, const VkImageBlit& region, VkFilter filter) const noexcept
{
    vkCmdBlitImage(handle, *srcImage, srcImage->getLayout(), *dstImage, dstImage->getLayout(), 1, &region, filter);
    MAGMA_INUSE(srcImage);
    MAGMA_INUSE(dstImage);
}

inline void CommandBuffer::copyBufferToImage(const std::shared_ptr<const Buffer>& srcBuffer, const std::shared_ptr<Image>& dstImage, const VkBufferImageCopy& region) const noexcept
{
    vkCmdCopyBufferToImage(handle, *srcBuffer, *dstImage, dstImage->getLayout(), 1, &region);
    MAGMA_INUSE(srcBuffer);
    MAGMA_INUSE(dstImage);
}

inline void CommandBuffer::copyBufferToImage(const std::shared_ptr<const Buffer>& srcBuffer, const std::shared_ptr<Image>& dstImage, const std::vector<VkBufferImageCopy>& regions) const noexcept
{
    vkCmdCopyBufferToImage(handle, *srcBuffer, *dstImage, dstImage->getLayout(), MAGMA_COUNT(regions), regions.data());
    MAGMA_INUSE(srcBuffer);
    MAGMA_INUSE(dstImage);
}

inline void CommandBuffer::copyImageToBuffer(const std::shared_ptr<const Image>& srcImage, const std::shared_ptr<Buffer>& dstBuffer, const VkBufferImageCopy& region) const noexcept
{
    vkCmdCopyImageToBuffer(handle, *srcImage, srcImage->getLayout(), *dstBuffer, 1, &region);
    MAGMA_INUSE(srcImage);
    MAGMA_INUSE(dstBuffer);
}

inline void CommandBuffer::copyImageToBuffer(const std::shared_ptr<const Image>& srcImage, const std::shared_ptr<Buffer>& dstBuffer, const std::vector<VkBufferImageCopy>& regions) const noexcept
{
    vkCmdCopyImageToBuffer(handle, *srcImage, srcImage->getLayout(), *dstBuffer, MAGMA_COUNT(regions), regions.data());
    MAGMA_INUSE(srcImage);
    MAGMA_INUSE(dstBuffer);
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
        reinterpret_cast<const uint32_t *>(data)); // Compatibility with old SDK
    MAGMA_INUSE(buffer);
}

template<class Type>
inline void CommandBuffer::updateBuffer(const std::shared_ptr<Buffer>& buffer, const std::vector<Type>& data,
    VkDeviceSize offset /* 0 */) const noexcept
{
    const VkDeviceSize dataSize = static_cast<VkDeviceSize>(sizeof(Type) * data.size());
    udpateBuffer(buffer, dataSize, data.data(), offset);
}

inline void CommandBuffer::clearColorImage(const std::shared_ptr<Image>& image, const ClearColor& color, const VkImageSubresourceRange& range) const noexcept
{
    const VkClearValue& clearValue = color;
    vkCmdClearColorImage(handle, *image, VK_IMAGE_LAYOUT_GENERAL, &clearValue.color, 1, &range);
    MAGMA_INUSE(image);
}

inline void CommandBuffer::clearDepthStencilImage(const std::shared_ptr<Image>& image, const ClearDepthStencil& depthStencil, const VkImageSubresourceRange& range) const noexcept
{
    const VkClearValue& clearValue = depthStencil;
    vkCmdClearDepthStencilImage(handle, *image, VK_IMAGE_LAYOUT_GENERAL, &clearValue.depthStencil, 1, &range);
    MAGMA_INUSE(image);
}

inline void CommandBuffer::clearAttachments(const std::initializer_list<ClearAttachment>& attachments, const VkClearRect& clearRect) const noexcept
{
    vkCmdClearAttachments(handle, MAGMA_COUNT(attachments), attachments.begin(), 1, &clearRect);
}

inline void CommandBuffer::resolveImage(const std::shared_ptr<const Image>& srcImage, const std::shared_ptr<Image>& dstImage) const noexcept
{
    const ImageResolve region(srcImage, dstImage);
    resolveImage(srcImage, dstImage, region);
}

inline void CommandBuffer::resolveImage(const std::shared_ptr<const Image>& srcImage, const std::shared_ptr<Image>& dstImage, const VkImageResolve& region) const noexcept
{
    vkCmdResolveImage(handle, *srcImage, srcImage->getLayout(), *dstImage, dstImage->getLayout(), 1, &region);
    MAGMA_INUSE(srcImage);
    MAGMA_INUSE(dstImage);
}

inline void CommandBuffer::setEvent(std::shared_ptr<Event>& event, VkPipelineStageFlags stageMask) const noexcept
{
    vkCmdSetEvent(handle, *event, stageMask);
    MAGMA_INUSE(event);
}

inline void CommandBuffer::resetEvent(std::shared_ptr<Event>& event, VkPipelineStageFlags stageMask) const noexcept
{
    vkCmdResetEvent(handle, *event, stageMask);
    MAGMA_INUSE(event);
}

inline void CommandBuffer::waitEvent(const std::shared_ptr<Event>& event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask) const noexcept
{
    vkCmdWaitEvents(handle, 1, event->getHandleAddress(), srcStageMask, dstStageMask, 0, nullptr, 0, nullptr, 0, nullptr);
    MAGMA_INUSE(event);
}

inline void CommandBuffer::waitEvent(const std::shared_ptr<Event>& event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
    const MemoryBarrier& barrier) const noexcept
{
    vkCmdWaitEvents(handle, 1, event->getHandleAddress(), srcStageMask, dstStageMask, 1, &barrier, 0, nullptr, 0, nullptr);
    MAGMA_INUSE(event);
}

inline void CommandBuffer::waitEvent(const std::shared_ptr<Event>& event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
    const MemoryBarrier& memoryBarrier, const BufferMemoryBarrier& bufferMemoryBarrier, const ImageMemoryBarrier& imageMemoryBarrier) const noexcept
{
    vkCmdWaitEvents(handle, 1, event->getHandleAddress(), srcStageMask, dstStageMask, 1, &memoryBarrier, 0, &bufferMemoryBarrier, 0, &imageMemoryBarrier);
    MAGMA_INUSE(event);
    MAGMA_INUSE(bufferMemoryBarrier.buffer);
    MAGMA_INUSE(imageMemoryBarrier.image);
}

inline void CommandBuffer::pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
    VkDependencyFlags dependencyFlags /* 0 */) noexcept
{
    vkCmdPipelineBarrier(handle, srcStageMask, dstStageMask, dependencyFlags,
        0, nullptr,
        0, nullptr,
        0, nullptr);
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
    MAGMA_INUSE(barrier.buffer);
}

inline void CommandBuffer::pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const ImageMemoryBarrier& barrier,
    VkDependencyFlags dependencyFlags /* 0 */) noexcept
{
    vkCmdPipelineBarrier(handle, srcStageMask, dstStageMask, dependencyFlags,
        0, nullptr,
        0, nullptr,
        1, &barrier);
    MAGMA_INUSE(barrier.image);
    barrier.image->setLayout(barrier.newLayout);
}

inline void CommandBuffer::batchPipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const MemoryBarrier& barrier,
    VkDependencyFlags dependencyFlags /* 0 */) noexcept
{
    if (auto batch = lookupBarrierBatch(srcStageMask, dstStageMask, dependencyFlags)) try
    {
        batch->memoryBarriers.push_back(barrier);
    }
    catch (...) {}
}

inline void CommandBuffer::batchPipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const BufferMemoryBarrier& barrier,
    VkDependencyFlags dependencyFlags /* 0 */) noexcept
{
    if (auto batch = lookupBarrierBatch(srcStageMask, dstStageMask, dependencyFlags)) try
    {
        batch->bufferMemoryBarriers.push_back(barrier);
    }
    catch (...) {}
}

inline void CommandBuffer::batchPipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const ImageMemoryBarrier& barrier,
    VkDependencyFlags dependencyFlags /* 0 */) noexcept
{
    if (auto batch = lookupBarrierBatch(srcStageMask, dstStageMask, dependencyFlags)) try
    {
        batch->imageMemoryBarriers.push_back(barrier);
    }
    catch (...) {}
}

inline void CommandBuffer::beginQuery(const std::shared_ptr<QueryPool>& queryPool, uint32_t queryIndex) noexcept
{
    vkCmdBeginQuery(handle, *queryPool, queryIndex, queryPool->getControlFlags());
    MAGMA_INUSE(queryPool);
}

inline void CommandBuffer::endQuery(const std::shared_ptr<QueryPool>& queryPool, uint32_t queryIndex) noexcept
{
    vkCmdEndQuery(handle, *queryPool, queryIndex);
    MAGMA_INUSE(queryPool);
}

#ifdef VK_EXT_transform_feedback
inline void CommandBuffer::beginQueryIndexed(const std::shared_ptr<QueryPool>& queryPool, uint32_t queryIndex, uint32_t vertexStream) noexcept
{
    MAGMA_DEVICE_EXTENSION(vkCmdBeginQueryIndexedEXT);
    if (vkCmdBeginQueryIndexedEXT)
    {
        vkCmdBeginQueryIndexedEXT(handle, *queryPool, queryIndex, queryPool->getControlFlags(), vertexStream);
        MAGMA_INUSE(queryPool);
    }
}

inline void CommandBuffer::endQueryIndexed(const std::shared_ptr<QueryPool>& queryPool, uint32_t queryIndex, uint32_t vertexStream) noexcept
{
    MAGMA_DEVICE_EXTENSION(vkCmdEndQueryIndexedEXT);
    if (vkCmdEndQueryIndexedEXT)
    {
        vkCmdEndQueryIndexedEXT(handle, *queryPool, queryIndex, vertexStream);
        MAGMA_INUSE(queryPool);
    }
}
#endif // VK_EXT_transform_feedback

inline void CommandBuffer::resetQueryPool(const std::shared_ptr<QueryPool>& queryPool, uint32_t firstQuery, uint32_t queryCount) noexcept
{
    vkCmdResetQueryPool(handle, *queryPool, firstQuery, queryCount);
    MAGMA_INUSE(queryPool);
}

inline void CommandBuffer::writeTimestamp(VkPipelineStageFlagBits pipelineStage, const std::shared_ptr<QueryPool>& queryPool, uint32_t queryIndex) noexcept
{
    vkCmdWriteTimestamp(handle, pipelineStage, *queryPool, queryIndex);
    MAGMA_INUSE(queryPool);
}

template<class Type>
void CommandBuffer::copyQueryResults(const std::shared_ptr<QueryPool>& /* queryPool */, const std::shared_ptr<Buffer>& /* dstBuffer */, bool /* wait */,
    uint32_t /* firstQuery */, uint32_t /* queryCount */, VkDeviceSize /* dstOffset */) noexcept
{
    static_assert(std::is_same<Type, uint32_t>::value || std::is_same<Type, uint64_t>::value,
        "CommandBuffer::copyQueryResults() specialized only for uint32_t and uint64_t types");
}

template<>
inline void CommandBuffer::copyQueryResults<uint32_t>(const std::shared_ptr<QueryPool>& queryPool, const std::shared_ptr<Buffer>& dstBuffer, bool wait,
    uint32_t firstQuery /* 0 */, uint32_t queryCount /* std::numeric_limits<uint32_t>::max() */, VkDeviceSize dstOffset /* 0 */) noexcept
{
    if (std::numeric_limits<uint32_t>::max() == queryCount)
        queryCount = queryPool->getQueryCount();
    const VkQueryResultFlags flags = wait ? VK_QUERY_RESULT_WAIT_BIT : 0;
    vkCmdCopyQueryPoolResults(handle, *queryPool, firstQuery, queryCount, *dstBuffer, dstOffset, sizeof(uint32_t), flags);
    MAGMA_INUSE(queryPool);
    MAGMA_INUSE(dstBuffer);
}

template<>
inline void CommandBuffer::copyQueryResults<uint64_t>(const std::shared_ptr<QueryPool>& queryPool, const std::shared_ptr<Buffer>& dstBuffer, bool wait,
    uint32_t firstQuery /* 0 */, uint32_t queryCount /* std::numeric_limits<uint32_t>::max() */, VkDeviceSize dstOffset /* 0 */) noexcept
{
    if (std::numeric_limits<uint32_t>::max() == queryCount)
        queryCount = queryPool->getQueryCount();
    const VkQueryResultFlags flags = VK_QUERY_RESULT_64_BIT | (wait ? VK_QUERY_RESULT_WAIT_BIT : 0);
    vkCmdCopyQueryPoolResults(handle, *queryPool, firstQuery, queryCount, *dstBuffer, dstOffset, sizeof(uint64_t), flags);
    MAGMA_INUSE(queryPool);
    MAGMA_INUSE(dstBuffer);
}

template<class Type>
inline void CommandBuffer::copyQueryResultsWithAvailability(const std::shared_ptr<QueryPool>& /* queryPool */, const std::shared_ptr<Buffer>& /* dstBuffer */,
    uint32_t /* firstQuery */, uint32_t /* queryCount */, VkDeviceSize /* dstOffset */) noexcept
{
    static_assert(std::is_same<Type, uint32_t>::value || std::is_same<Type, uint64_t>::value,
        "CommandBuffer::copyQueryResultsWithAvailability() specialized only for uint32_t and uint64_t types");
}

template<>
inline void CommandBuffer::copyQueryResultsWithAvailability<uint32_t>(const std::shared_ptr<QueryPool>& queryPool, const std::shared_ptr<Buffer>& dstBuffer,
    uint32_t firstQuery /* 0 */, uint32_t queryCount /* std::numeric_limits<uint32_t>::max() */, VkDeviceSize dstOffset /* 0 */) noexcept
{
    if (std::numeric_limits<uint32_t>::max() == queryCount)
        queryCount = queryPool->getQueryCount();
    constexpr VkQueryResultFlags flags = VK_QUERY_RESULT_WITH_AVAILABILITY_BIT;
    vkCmdCopyQueryPoolResults(handle, *queryPool, firstQuery, queryCount, *dstBuffer, dstOffset, sizeof(uint32_t) * 2, flags);
    MAGMA_INUSE(queryPool);
    MAGMA_INUSE(dstBuffer);
}

template<>
inline void CommandBuffer::copyQueryResultsWithAvailability<uint64_t>(const std::shared_ptr<QueryPool>& queryPool, const std::shared_ptr<Buffer>& dstBuffer,
    uint32_t firstQuery /* 0 */, uint32_t queryCount /* std::numeric_limits<uint32_t>::max() */, VkDeviceSize dstOffset /* 0 */) noexcept
{
    if (std::numeric_limits<uint32_t>::max() == queryCount)
        queryCount = queryPool->getQueryCount();
    constexpr VkQueryResultFlags flags = VK_QUERY_RESULT_64_BIT | VK_QUERY_RESULT_WITH_AVAILABILITY_BIT;
    vkCmdCopyQueryPoolResults(handle, *queryPool, firstQuery, queryCount, *dstBuffer, dstOffset, sizeof(uint64_t) * 2, flags);
    MAGMA_INUSE(queryPool);
    MAGMA_INUSE(dstBuffer);
}

template<class Type>
inline void CommandBuffer::pushConstant(const std::shared_ptr<PipelineLayout>& layout, VkShaderStageFlags stageFlags, const Type& constant,
    uint32_t offset /* 0 */) noexcept
{
    vkCmdPushConstants(handle, *layout, stageFlags, offset, static_cast<uint32_t>(sizeof(Type)), &constant);
    MAGMA_INUSE(layout);
}

template<class Type, uint32_t pushConstantCount>
inline void CommandBuffer::pushConstants(const std::shared_ptr<PipelineLayout>& layout, VkShaderStageFlags stageFlags, const Type(&constants)[pushConstantCount],
    uint32_t offset /* 0 */) noexcept
{
    vkCmdPushConstants(handle, *layout, stageFlags, offset, static_cast<uint32_t>(sizeof(Type) * pushConstantCount), constants);
    MAGMA_INUSE(layout);
}

template<class Type>
inline void CommandBuffer::pushConstants(const std::shared_ptr<PipelineLayout>& layout, VkShaderStageFlags stageFlags, const std::vector<Type>& constants,
    uint32_t offset /* 0 */) noexcept
{
    vkCmdPushConstants(handle, *layout, stageFlags, offset, static_cast<uint32_t>(sizeof(Type) * constants.size()), constants.data());
    MAGMA_INUSE(layout);
}

template<class BlockType>
inline void CommandBuffer::pushConstantBlock(const std::shared_ptr<PipelineLayout>& layout, VkShaderStageFlags stageFlags, const BlockType& block,
    uint32_t offset /* 0 */) noexcept
{
    vkCmdPushConstants(handle, *layout, stageFlags, offset, static_cast<uint32_t>(sizeof(BlockType)), &block);
    MAGMA_INUSE(layout);
}

inline void CommandBuffer::nextSubpass(VkSubpassContents contents /* VK_SUBPASS_CONTENTS_INLINE */) noexcept
{
    vkCmdNextSubpass(handle, contents);
}

#ifdef VK_KHR_device_group
inline void CommandBuffer::setDeviceMask(uint32_t deviceMask) noexcept
{
    MAGMA_DEVICE_EXTENSION(vkCmdSetDeviceMaskKHR);
    if (vkCmdSetDeviceMaskKHR)
        vkCmdSetDeviceMaskKHR(handle, deviceMask);
}

inline void CommandBuffer::dispatchBase(uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ,
    uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const noexcept
{
    MAGMA_DEVICE_EXTENSION(vkCmdDispatchBaseKHR);
    if (vkCmdDispatchBaseKHR)
        vkCmdDispatchBaseKHR(handle, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
}
#endif // VK_KHR_device_group

#ifdef VK_EXT_transform_feedback
inline void CommandBuffer::beginTransformFeedback() noexcept
{
    MAGMA_DEVICE_EXTENSION(vkCmdBeginTransformFeedbackEXT);
    if (vkCmdBeginTransformFeedbackEXT)
    {
        vkCmdBeginTransformFeedbackEXT(handle, 0, 0, nullptr, nullptr);
        inTransformFeedback = VK_TRUE;
    }
}

inline void CommandBuffer::endTransformFeedback() noexcept
{
    MAGMA_ASSERT(inTransformFeedback);
    MAGMA_DEVICE_EXTENSION(vkCmdEndTransformFeedbackEXT);
    if (vkCmdEndTransformFeedbackEXT)
    {
        vkCmdEndTransformFeedbackEXT(handle, 0, 0, nullptr, nullptr);
        inTransformFeedback = VK_FALSE;
    }
}
#endif // VK_EXT_transform_feedback

inline void CommandBuffer::enableOcclusionQuery(bool enable, VkQueryControlFlags queryFlags_) noexcept
{
    occlusionQueryEnable = MAGMA_BOOLEAN(enable);
    queryFlags = queryFlags_;
}

inline void CommandBuffer::enableConditionalRendering(bool enable) noexcept
{
    conditionalRenderingEnable = MAGMA_BOOLEAN(enable);
}

inline void CommandBuffer::queryPipelineStatistics(VkQueryPipelineStatisticFlags pipelineStatistics_) noexcept
{
    pipelineStatistics = pipelineStatistics_;
}

inline void CommandBuffer::finishedQueueSubmission() noexcept
{
    state = State::Pending;
}

inline void CommandBuffer::finishedExecution() noexcept
{
    if (usageFlags & VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT)
    {
        state = State::Invalid;
        releaseResourcesInUse();
    }
    else
    {
        state = State::Executable;
    }
}
} // namespace magma
