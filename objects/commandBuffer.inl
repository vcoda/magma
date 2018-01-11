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
inline void CommandBuffer::setViewport(const VkViewport& viewport) noexcept
{
    vkCmdSetViewport(handle, 0, 1, &viewport);
}

inline void CommandBuffer::setViewport(float x, float y, float width, float height,
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

inline void CommandBuffer::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height,
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

inline void CommandBuffer::setScissor(const VkRect2D& scissor) noexcept
{
    vkCmdSetScissor(handle, 0, 1, &scissor);
}

inline void CommandBuffer::setScissor(int32_t x, int32_t y, uint32_t width, uint32_t height) noexcept
{
    VkRect2D scissor;
    scissor.offset.x = x;
    scissor.offset.y = y;
    scissor.extent.width = width;
    scissor.extent.width = height;
    vkCmdSetScissor(handle, 0, 1, &scissor);
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
    vkCmdSetBlendConstants(handle, blendConstants);
}

inline void CommandBuffer::setDepthBounds(float minDepthBounds, float maxDepthBounds) noexcept
{
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

inline void CommandBuffer::draw(uint32_t vertexCount, uint32_t firstVertex) const noexcept
{
    vkCmdDraw(handle, vertexCount, 1, firstVertex, 0);
}

inline void CommandBuffer::drawInstanced(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) const noexcept
{
    vkCmdDraw(handle, vertexCount, instanceCount, firstVertex, firstInstance);
}

inline void CommandBuffer::drawIndexed(uint32_t indexCount, uint32_t firstIndex, int32_t vertexOffset /* 0 */) const noexcept
{
    vkCmdDrawIndexed(handle, indexCount, 1, firstIndex, vertexOffset, 0);
}

inline void CommandBuffer::drawIndexedInstanced(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) const noexcept
{
    vkCmdDrawIndexed(handle, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

inline void CommandBuffer::setClear(const ClearValue& value)
{
    clearValues = std::vector<ClearValue>{value};
}

inline void CommandBuffer::setClears(const std::vector<ClearValue>& values)
{
    clearValues = values;
}

inline void CommandBuffer::setRenderArea(const VkRect2D& rc) noexcept
{
    renderArea = rc;
}

inline void CommandBuffer::setRenderArea(int32_t x, int32_t y, const VkExtent2D& extent) noexcept
{
    renderArea.offset.x = x;
    renderArea.offset.y = y;
    renderArea.extent = extent;
}

inline void CommandBuffer::setRenderArea(int32_t x, int32_t y, uint32_t width, uint32_t height) noexcept
{
    renderArea.offset.x = x;
    renderArea.offset.y = y;
    renderArea.extent.width = width;
    renderArea.extent.height = height;
}

inline void CommandBuffer::enableOcclusionQuery(bool enable, VkQueryControlFlags queryFlags) noexcept
{
    occlusionQueryEnable = enable;
    this->queryFlags = queryFlags;
}

inline void CommandBuffer::queryPipelineStatistics(VkQueryPipelineStatisticFlags pipelineStatistics) noexcept
{
    this->pipelineStatistics = pipelineStatistics;
}
} // namespace magma
