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
namespace magma
{
MAGMA_INLINE void CommandBuffer::setViewport(float x, float y, float width, float height,
    float minDepth /* 0 */, float maxDepth /* 1 */) noexcept
{
    VkViewport viewport;
    viewport.x = x;
    viewport.y = y;
    viewport.width = width;
    viewport.height = height;
    viewport.minDepth = minDepth;
    viewport.maxDepth = maxDepth;
    vkCmdSetViewport(handle, 0, 1, &viewport);
}

MAGMA_INLINE void CommandBuffer::setViewport(uint32_t x, uint32_t y, uint32_t width, int32_t height,
    float minDepth /* 0 */, float maxDepth /* 1 */) noexcept
{
    VkViewport viewport;
    viewport.x = static_cast<float>(x);
    viewport.y = static_cast<float>(y);
    viewport.width = static_cast<float>(width);
    viewport.height = static_cast<float>(height);
    viewport.minDepth = minDepth;
    viewport.maxDepth = maxDepth;
    vkCmdSetViewport(handle, 0, 1, &viewport);
}

MAGMA_INLINE void CommandBuffer::setViewport(const Viewport& viewport) noexcept
{
    vkCmdSetViewport(handle, 0, 1, &viewport);
}

MAGMA_INLINE void CommandBuffer::setViewports(const std::initializer_list<Viewport>& viewports) noexcept
{
    vkCmdSetViewport(handle, 0, MAGMA_COUNT(viewports), viewports.begin());
}

MAGMA_INLINE void CommandBuffer::setScissor(int32_t x, int32_t y, uint32_t width, uint32_t height) noexcept
{
    VkRect2D scissor;
    scissor.offset.x = x;
    scissor.offset.y = y;
    scissor.extent.width = width;
    scissor.extent.height = height;
    vkCmdSetScissor(handle, 0, 1, &scissor);
}    

MAGMA_INLINE void CommandBuffer::setScissor(const Scissor& scissor) noexcept
{
    vkCmdSetScissor(handle, 0, 1, &scissor);
}

MAGMA_INLINE void CommandBuffer::setScissors(const std::initializer_list<Scissor>& scissors) noexcept
{
    vkCmdSetScissor(handle, 0, MAGMA_COUNT(scissors), scissors.begin());
}

MAGMA_INLINE void CommandBuffer::setLineWidth(float lineWidth) noexcept
{
    vkCmdSetLineWidth(handle, lineWidth);
}

MAGMA_INLINE void CommandBuffer::setDepthBias(float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) noexcept
{
    vkCmdSetDepthBias(handle, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
}

MAGMA_INLINE void CommandBuffer::setBlendConstants(const float blendConstants[4]) noexcept
{
    vkCmdSetBlendConstants(handle, blendConstants);
}

MAGMA_INLINE void CommandBuffer::setDepthBounds(float minDepthBounds, float maxDepthBounds) noexcept
{
    vkCmdSetDepthBounds(handle, minDepthBounds, maxDepthBounds);
}

MAGMA_INLINE void CommandBuffer::setStencilCompareMask(bool frontFace, bool backFace, uint32_t compareMask) noexcept
{
    MAGMA_ASSERT(frontFace || backFace);
    vkCmdSetStencilCompareMask(handle, MAGMA_STENCIL_FACE_MASK(frontFace, backFace), compareMask);
}

MAGMA_INLINE void CommandBuffer::setStencilWriteMask(bool frontFace, bool backFace, uint32_t writeMask) noexcept
{
    MAGMA_ASSERT(frontFace || backFace);
    vkCmdSetStencilWriteMask(handle, MAGMA_STENCIL_FACE_MASK(frontFace, backFace), writeMask);
}

MAGMA_INLINE void CommandBuffer::setStencilReference(bool frontFace, bool backFace, uint32_t reference) noexcept
{
    MAGMA_ASSERT(frontFace || backFace);
    vkCmdSetStencilReference(handle, MAGMA_STENCIL_FACE_MASK(frontFace, backFace), reference);
}

MAGMA_INLINE void CommandBuffer::bindDescriptorSet(const std::shared_ptr<PipelineLayout>& pipelineLayout, const std::shared_ptr<DescriptorSet>& descriptorSet, 
    VkPipelineBindPoint pipelineBindPoint /* VK_PIPELINE_BIND_POINT_GRAPHICS */) noexcept
{
    const VkDescriptorSet dereferencedDescriptorSets[1] = {*descriptorSet};
    vkCmdBindDescriptorSets(handle, pipelineBindPoint, *pipelineLayout, 0, 1, dereferencedDescriptorSets, 0, nullptr);
}

MAGMA_INLINE void CommandBuffer::bindDescriptorSet(const std::shared_ptr<PipelineLayout>& pipelineLayout, const std::shared_ptr<DescriptorSet>& descriptorSet, uint32_t dynamic0ffset,
    VkPipelineBindPoint pipelineBindPoint /* VK_PIPELINE_BIND_POINT_GRAPHICS */) noexcept
{
    const VkDescriptorSet dereferencedDescriptorSets[1] = {*descriptorSet};
    vkCmdBindDescriptorSets(handle, pipelineBindPoint, *pipelineLayout, 0, 1, dereferencedDescriptorSets, 1, &dynamic0ffset);
}

template<uint32_t descriptorSetCount>
MAGMA_INLINE void CommandBuffer::bindDescriptorSets(const std::shared_ptr<PipelineLayout>& pipelineLayout, const std::shared_ptr<magma::DescriptorSet>(&descriptorSets)[descriptorSetCount],
    const std::initializer_list<uint32_t>& offsets /* {} */,
    VkPipelineBindPoint pipelineBindPoint /* VK_PIPELINE_BIND_POINT_GRAPHICS */) noexcept
{
    MAGMA_STACK_ARRAY(VkDescriptorSet, dereferencedDescriptorSets, descriptorSetCount);
    for (uint32_t i = 0; i < descriptorSetCount; ++i)
        dereferencedDescriptorSets.put(*descriptorSets[i]);
    vkCmdBindDescriptorSets(handle, pipelineBindPoint, *pipelineLayout, 0, descriptorSetCount, dereferencedDescriptorSets,
        MAGMA_COUNT(offsets), offsets.begin());
}

MAGMA_INLINE void CommandBuffer::bindDescriptorSets(const std::shared_ptr<PipelineLayout>& pipelineLayout, const std::initializer_list<std::shared_ptr<DescriptorSet>>& descriptorSets,
    const std::initializer_list<uint32_t>& offsets /* {} */,
    VkPipelineBindPoint pipelineBindPoint /* VK_PIPELINE_BIND_POINT_GRAPHICS */) noexcept
{
    MAGMA_STACK_ARRAY(VkDescriptorSet, dereferencedDescriptorSets, descriptorSets.size());
    for (const auto& descriptorSet : descriptorSets)
        dereferencedDescriptorSets.put(*descriptorSet);
    vkCmdBindDescriptorSets(handle, pipelineBindPoint, *pipelineLayout, 0, dereferencedDescriptorSets.size(), dereferencedDescriptorSets,
        MAGMA_COUNT(offsets), offsets.begin());
}

MAGMA_INLINE void CommandBuffer::bindIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer, VkDeviceSize offset /* 0 */) noexcept
{
    vkCmdBindIndexBuffer(handle, *indexBuffer, offset, indexBuffer->getIndexType());
}

MAGMA_INLINE void CommandBuffer::bindVertexBuffer(uint32_t firstBinding, const std::shared_ptr<VertexBuffer>& vertexBuffer, VkDeviceSize offset /* 0 */) noexcept
{
    const VkBuffer dereferencedBuffers[1] = {*vertexBuffer};
    vkCmdBindVertexBuffers(handle, firstBinding, 1, dereferencedBuffers, &offset);
}

MAGMA_INLINE void CommandBuffer::bindVertexBuffers(uint32_t firstBinding, const std::vector<std::shared_ptr<VertexBuffer>>& vertexBuffers, const std::vector<VkDeviceSize>& offsets) noexcept
{
    MAGMA_ASSERT(vertexBuffers.size() > 0);
    MAGMA_ASSERT(vertexBuffers.size() == offsets.size());
    MAGMA_STACK_ARRAY(VkBuffer, dereferencedBuffers, vertexBuffers.size());
    for (const auto& buffer : vertexBuffers)
        dereferencedBuffers.put(*buffer);
    vkCmdBindVertexBuffers(handle, firstBinding, dereferencedBuffers.size(), dereferencedBuffers, offsets.data());
}

MAGMA_INLINE void CommandBuffer::draw(uint32_t vertexCount, uint32_t firstVertex) const noexcept
{
    vkCmdDraw(handle, vertexCount, 1, firstVertex, 0);
}

MAGMA_INLINE void CommandBuffer::drawInstanced(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) const noexcept
{
    vkCmdDraw(handle, vertexCount, instanceCount, firstVertex, firstInstance);
}

MAGMA_INLINE void CommandBuffer::drawIndexed(uint32_t indexCount, uint32_t firstIndex, int32_t vertexOffset /* 0 */) const noexcept
{
    vkCmdDrawIndexed(handle, indexCount, 1, firstIndex, vertexOffset, 0);
}

MAGMA_INLINE void CommandBuffer::drawIndexedInstanced(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) const noexcept
{
    vkCmdDrawIndexed(handle, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

MAGMA_INLINE void CommandBuffer::drawIndirect(const std::shared_ptr<Buffer>& buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) const noexcept
{
    vkCmdDrawIndirect(handle, *buffer, offset, drawCount, stride);
}

MAGMA_INLINE void CommandBuffer::drawIndexedIndirect(const std::shared_ptr<Buffer>& buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) const noexcept
{
    vkCmdDrawIndexedIndirect(handle, *buffer, offset, drawCount, stride);
}

MAGMA_INLINE void CommandBuffer::dispatch(uint32_t x, uint32_t y, uint32_t z) const noexcept
{
    vkCmdDispatch(handle, x, y, z);
}

MAGMA_INLINE void CommandBuffer::dispatchIndirect(const std::shared_ptr<Buffer>& buffer, VkDeviceSize offset) const noexcept
{
    vkCmdDispatchIndirect(handle, *buffer, offset);
}

template<typename Type>
MAGMA_INLINE void CommandBuffer::updateBuffer(const std::shared_ptr<Buffer>& buffer, const std::vector<Type>& data,
    VkDeviceSize offset /* 0 */) const noexcept
{
    /* Buffer updates performed with vkCmdUpdateBuffer first copy the data 
       into command buffer memory when the command is recorded 
       (which requires additional storage and may incur an additional allocation), 
       and then copy the data from the command buffer into dstBuffer 
       when the command is executed on a device. */
    vkCmdUpdateBuffer(handle, *buffer, offset, static_cast<uint32_t>(sizeof(Type) * data.size()), data.data());
}

template<typename Type>
MAGMA_INLINE void CommandBuffer::fillBuffer(const std::shared_ptr<Buffer>& buffer, const std::vector<Type>& data,
    VkDeviceSize offset /* 0 */) const noexcept
{
    vkCmdFillBuffer(handle, *buffer, offset, static_cast<uint32_t>(sizeof(Type) * data.size()), data.data());
}

template<typename Type, uint32_t pushConstantCount>
MAGMA_INLINE void CommandBuffer::pushConstants(const std::shared_ptr<PipelineLayout>& layout, VkShaderStageFlags stageFlags, const Type(&values)[pushConstantCount],
    uint32_t offset /* 0 */) noexcept
{
    vkCmdPushConstants(handle, *layout, stageFlags, offset, static_cast<uint32_t>(sizeof(Type) * pushConstantCount), values);
}

template<typename Type>
MAGMA_INLINE void CommandBuffer::pushConstants(const std::shared_ptr<PipelineLayout>& layout, VkShaderStageFlags stageFlags, const std::vector<Type>& values,
    uint32_t offset /* 0 */) noexcept
{
    vkCmdPushConstants(handle, *layout, stageFlags, offset, static_cast<uint32_t>(sizeof(Type) * values.size()), values.data());
}

template<typename Type>
MAGMA_INLINE void CommandBuffer::pushConstantBlock(const std::shared_ptr<PipelineLayout>& layout, VkShaderStageFlags stageFlags, const Type& block,
    uint32_t offset /* 0 */) noexcept
{
    vkCmdPushConstants(handle, *layout, stageFlags, offset, static_cast<uint32_t>(sizeof(Type)), &block);
}

MAGMA_INLINE void CommandBuffer::nextSubpass(VkSubpassContents contents /* VK_SUBPASS_CONTENTS_INLINE */) noexcept
{
    vkCmdNextSubpass(handle, contents);
}

MAGMA_INLINE void CommandBuffer::setRenderArea(const VkRect2D& rc) noexcept
{
    renderArea = rc;
}

MAGMA_INLINE void CommandBuffer::setRenderArea(int32_t x, int32_t y, const VkExtent2D& extent) noexcept
{
    renderArea.offset.x = x;
    renderArea.offset.y = y;
    renderArea.extent = extent;
}

MAGMA_INLINE void CommandBuffer::setRenderArea(int32_t x, int32_t y, uint32_t width, uint32_t height) noexcept
{
    renderArea.offset.x = x;
    renderArea.offset.y = y;
    renderArea.extent.width = width;
    renderArea.extent.height = height;
}

MAGMA_INLINE void CommandBuffer::enableOcclusionQuery(bool enable, VkQueryControlFlags queryFlags) noexcept
{
    occlusionQueryEnable = enable;
    this->queryFlags = queryFlags;
}

MAGMA_INLINE void CommandBuffer::queryPipelineStatistics(VkQueryPipelineStatisticFlags pipelineStatistics) noexcept
{
    this->pipelineStatistics = pipelineStatistics;
}
} // namespace magma
