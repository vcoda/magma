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
#pragma once
#include "leanCommandBuffer.h"
#include "indirectBuffer.h"
#include "transformFeedbackBuffer.h"
#include "../barriers/memoryBarrier.h"
#include "../barriers/bufferMemoryBarrier.h"
#include "../barriers/imageMemoryBarrier.h"
#include "../misc/imageResolve.h"
#include "../misc/clearValue.h"
#include "../misc/clearAttachment.h"

namespace magma
{
    class Fence;
#ifdef VK_KHR_acceleration_structure
    struct AccelerationStructureGeometry;
#endif

    /* Command buffers are objects used to record commands which
       can be subsequently submitted to a device queue for execution.
       Recorded commands include the following:

        * Bind pipelines and descriptor sets to the command buffer.
        * Modify dynamic state.
        * Draw (for graphics rendering), dispatch (for compute).
        * Execute secondary command buffers (for primary command buffers only).
        * Copy buffers and images.

       Unless otherwise specified, and without explicit synchronization,
       the various commands submitted to a queue via command buffers
       may execute in arbitrary order relative to each other, and/or
       concurrently. */

    class CommandBuffer
    {
    public:
        enum class State : uint8_t;
        ~CommandBuffer();
        bool begin(VkCommandBufferUsageFlags flags = 0) noexcept;
        bool beginInherited(const std::shared_ptr<RenderPass>& renderPass, uint32_t subpass, const std::shared_ptr<Framebuffer>& framebuffer,
            bool occlusionQueryEnable = false, VkQueryControlFlags queryFlags = 0, VkQueryPipelineStatisticFlags pipelineStatistics = 0,
            VkCommandBufferUsageFlags flags = 0, const StructureChain& extendedInfo = StructureChain()) noexcept;
        void end();
        bool reset(VkCommandBufferResetFlags flags = 0) noexcept;

        void bindPipeline(const std::shared_ptr<Pipeline>& pipeline) noexcept;

        void setViewport(const VkViewport& viewport) noexcept;
        void setViewport(float x, float y, float width, float height, float minDepth = 0.f, float maxDepth = 1.f) noexcept;
        void setViewport(uint32_t x, uint32_t y, uint32_t width, int32_t height, float minDepth = 0.f, float maxDepth = 1.f) noexcept;
        void setViewports(uint32_t firstViewport, const std::initializer_list<VkViewport>& viewports) noexcept;
        void setScissor(const VkRect2D& scissor) noexcept;
        void setScissor(int32_t x, int32_t y, uint32_t width, uint32_t height) noexcept;
        void setScissors(uint32_t firstScissor, const std::initializer_list<VkRect2D>& scissors) noexcept;
        void setLineWidth(float lineWidth) noexcept;
        void setDepthBias(float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) noexcept;
        void setBlendConstants(const float blendConstants[4]) noexcept;
        void setDepthBounds(float minDepthBounds, float maxDepthBounds) noexcept;
        void setStencilCompareMask(bool frontFace, bool backFace, uint32_t compareMask) noexcept;
        void setStencilWriteMask(bool frontFace, bool backFace, uint32_t writeMask) noexcept;
        void setStencilReference(bool frontFace, bool backFace, uint32_t reference) noexcept;

        void bindDescriptorSet(const std::shared_ptr<Pipeline>& pipeline, uint32_t firstSet, const std::shared_ptr<DescriptorSet>& descriptorSet,
            uint32_t dynamicOffset = std::numeric_limits<uint32_t>::max()) noexcept;
        void bindDescriptorSet(const std::shared_ptr<Pipeline>& pipeline, uint32_t firstSet, const std::shared_ptr<DescriptorSet>& descriptorSet,
            const std::initializer_list<uint32_t>& dynamicOffsets) noexcept;
        void bindDescriptorSets(const std::shared_ptr<Pipeline>& pipeline, uint32_t firstSet, const std::initializer_list<std::shared_ptr<DescriptorSet>>& descriptorSets,
            const std::initializer_list<uint32_t>& dynamicOffsets = {}) noexcept;

        void bindIndexBuffer(const std::shared_ptr<BaseIndexBuffer>& indexBuffer, VkDeviceSize offset = 0) noexcept;
        void bindVertexBuffer(uint32_t firstBinding, const std::shared_ptr<Buffer>& vertexBuffer, VkDeviceSize offset = 0) noexcept;
        void bindVertexBuffers(uint32_t firstBinding, const std::initializer_list<std::shared_ptr<Buffer>>& vertexBuffers, std::initializer_list<VkDeviceSize> offsets = {}) noexcept;

        void draw(uint32_t vertexCount, uint32_t firstVertex = 0) const noexcept;
        void drawInstanced(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex = 0, uint32_t firstInstance = 0) const noexcept;
        void drawIndexed(uint32_t indexCount, uint32_t firstIndex = 0, int32_t vertexOffset = 0) const noexcept;
        void drawIndexedInstanced(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) const noexcept;
        void drawIndirect(const std::shared_ptr<Buffer>& buffer, uint32_t drawCount, VkDeviceSize offset = 0) const noexcept;
        void drawIndirect(const std::shared_ptr<DrawIndirectBuffer>& buffer) const noexcept;
        void drawIndexedIndirect(const std::shared_ptr<Buffer>& buffer, uint32_t drawCount, VkDeviceSize offset = 0) const noexcept;
        void drawIndexedIndirect(const std::shared_ptr<DrawIndexedIndirectBuffer>& buffer) const noexcept;
        void dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const noexcept;
        void dispatchIndirect(const std::shared_ptr<Buffer>& buffer, VkDeviceSize offset = 0) const noexcept;

        void copyBuffer(const std::shared_ptr<Buffer>& srcBuffer, const std::shared_ptr<Buffer>& dstBuffer,
            VkDeviceSize srcOffset = 0, VkDeviceSize dstOffset = 0, VkDeviceSize size = VK_WHOLE_SIZE) const noexcept;
        void copyImage(const std::shared_ptr<Image>& srcImage, const std::shared_ptr<Image>& dstImage, const VkImageCopy& region) const noexcept;
        void copyImage(const std::shared_ptr<Image>& srcImage, const std::shared_ptr<Image>& dstImage, uint32_t mipLevel) const noexcept;
        void blitImage(const std::shared_ptr<Image>& srcImage, const std::shared_ptr<Image>& dstImage, const VkImageBlit& region, VkFilter filter) const noexcept;
        void blitImage(const std::shared_ptr<Image>& srcImage, const std::shared_ptr<Image>& dstImage, uint32_t srcMipLevel, uint32_t dstMipLevel, VkFilter filter) const noexcept;
        void copyBufferToImage(const std::shared_ptr<Buffer>& srcBuffer, const std::shared_ptr<Image>& dstImage, const VkBufferImageCopy& region) const noexcept;
        void copyBufferToImage(const std::shared_ptr<Buffer>& srcBuffer, const std::shared_ptr<Image>& dstImage, const std::vector<VkBufferImageCopy>& regions) const noexcept;
        void copyImageToBuffer(const std::shared_ptr<Image>& srcImage, const std::shared_ptr<Buffer>& dstBuffer, const VkBufferImageCopy& region) const noexcept;
        void copyImageToBuffer(const std::shared_ptr<Image>& srcImage, const std::shared_ptr<Buffer>& dstBuffer, const std::vector<VkBufferImageCopy>& regions) const noexcept;
        void updateBuffer(const std::shared_ptr<Buffer>& buffer, VkDeviceSize dataSize, const void *data, VkDeviceSize offset = 0) const noexcept;
        template<class Type> void updateBuffer(const std::shared_ptr<Buffer>& buffer, const std::vector<Type>& data, VkDeviceSize offset = 0) const noexcept;
        void fillBuffer(const std::shared_ptr<Buffer>& buffer, uint32_t value, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) const noexcept;

        void clearColorImage(const std::shared_ptr<Image>& image, const ClearColor& color, const VkImageSubresourceRange& range) const noexcept;
        void clearDepthStencilImage(const std::shared_ptr<Image>& image, const ClearDepthStencil& depthStencil, const VkImageSubresourceRange& range) const noexcept;
        void clearAttachments(const std::initializer_list<ClearAttachment>& attachments, const VkClearRect& clearRect) const noexcept;
        void resolveImage(const std::shared_ptr<Image>& srcImage, const std::shared_ptr<Image>& dstImage) const noexcept;
        void resolveImage(const std::shared_ptr<Image>& srcImage, const std::shared_ptr<Image>& dstImage, const VkImageResolve& region) const noexcept;

        void setEvent(const std::shared_ptr<Event>& event, VkPipelineStageFlags stageMask) const noexcept;
        void resetEvent(const std::shared_ptr<Event>& event, VkPipelineStageFlags stageMask) const noexcept;
        void waitEvent(const std::shared_ptr<Event>& event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask) const noexcept;
        void waitEvent(const std::shared_ptr<Event>& event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const VkMemoryBarrier& barrier) const noexcept;
        void waitEvent(const std::shared_ptr<Event>& event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const VkBufferMemoryBarrier& barrier) const noexcept;
        void waitEvent(const std::shared_ptr<Event>& event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const ImageMemoryBarrier& barrier) const noexcept;
        void waitEvent(const std::shared_ptr<Event>& event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::initializer_list<VkMemoryBarrier>& memoryBarriers) const noexcept;
        void waitEvent(const std::shared_ptr<Event>& event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::vector<VkMemoryBarrier>& memoryBarriers) const noexcept;
        void waitEvent(const std::shared_ptr<Event>& event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::initializer_list<VkBufferMemoryBarrier>& bufferMemoryBarriers) const noexcept;
        void waitEvent(const std::shared_ptr<Event>& event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::vector<VkBufferMemoryBarrier>& bufferMemoryBarriers) const noexcept;
        void waitEvent(const std::shared_ptr<Event>& event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::initializer_list<ImageMemoryBarrier>& imageMemoryBarriers) const noexcept;
        void waitEvent(const std::shared_ptr<Event>& event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::vector<ImageMemoryBarrier>& imageMemoryBarriers) const noexcept;
        void waitEvents(const std::initializer_list<std::shared_ptr<Event>>& events, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::initializer_list<VkMemoryBarrier>& memoryBarriers = {},
            const std::initializer_list<VkBufferMemoryBarrier>& bufferMemoryBarriers = {}, const std::initializer_list<ImageMemoryBarrier>& imageMemoryBarriers = {}) const noexcept;
        void waitEvents(const std::vector<std::shared_ptr<Event>>& events, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::vector<VkMemoryBarrier>& memoryBarriers,
            const std::vector<VkBufferMemoryBarrier>& bufferMemoryBarriers, const std::vector<ImageMemoryBarrier>& imageMemoryBarriers) const noexcept;

        void pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags = 0) noexcept;
        void pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const VkMemoryBarrier& barrier, VkDependencyFlags dependencyFlags = 0) noexcept;
        void pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const VkBufferMemoryBarrier& barrier, VkDependencyFlags dependencyFlags = 0) noexcept;
        void pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const ImageMemoryBarrier& barrier, VkDependencyFlags dependencyFlags = 0) noexcept;
        void pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::initializer_list<VkMemoryBarrier>& memoryBarriers, VkDependencyFlags dependencyFlags = 0) noexcept;
        void pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::vector<VkMemoryBarrier>& memoryBarriers, VkDependencyFlags dependencyFlags = 0) noexcept;
        void pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::initializer_list<VkBufferMemoryBarrier>& bufferMemoryBarriers, VkDependencyFlags dependencyFlags = 0) noexcept;
        void pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::vector<VkBufferMemoryBarrier>& bufferMemoryBarriers, VkDependencyFlags dependencyFlags = 0) noexcept;
        void pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::initializer_list<ImageMemoryBarrier>& imageMemoryBarriers, VkDependencyFlags dependencyFlags = 0) noexcept;
        void pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::vector<ImageMemoryBarrier>& imageMemoryBarriers, VkDependencyFlags dependencyFlags = 0) noexcept;
        void pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::initializer_list<VkMemoryBarrier>& memoryBarriers = {},
            const std::initializer_list<VkBufferMemoryBarrier>& bufferMemoryBarriers = {}, const std::initializer_list<ImageMemoryBarrier>& imageMemoryBarriers = {}, VkDependencyFlags dependencyFlags = 0) noexcept;
        void pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::vector<VkMemoryBarrier>& memoryBarriers,
            const std::vector<VkBufferMemoryBarrier>& bufferMemoryBarriers, const std::vector<ImageMemoryBarrier>& imageMemoryBarriers, VkDependencyFlags dependencyFlags = 0) noexcept;

        void beginQuery(const std::shared_ptr<QueryPool>& queryPool, uint32_t query) noexcept;
        void endQuery(const std::shared_ptr<QueryPool>& queryPool, uint32_t query) noexcept;
        void resetQueryPool(const std::shared_ptr<QueryPool>& queryPool, uint32_t firstQuery, uint32_t queryCount) noexcept;
        void writeTimestamp(VkPipelineStageFlagBits pipelineStage, const std::shared_ptr<QueryPool>& queryPool, uint32_t query) noexcept;
        template<class Type>
        void copyQueryResults(const std::shared_ptr<QueryPool>& queryPool, const std::shared_ptr<Buffer>& dstBuffer, bool wait,
            uint32_t firstQuery = 0, uint32_t queryCount = std::numeric_limits<uint32_t>::max(), VkDeviceSize dstOffset = 0) noexcept;
        template<class Type>
        void copyQueryResultsWithAvailability(const std::shared_ptr<QueryPool>& queryPool, const std::shared_ptr<Buffer>& dstBuffer,
            uint32_t firstQuery = 0, uint32_t queryCount = std::numeric_limits<uint32_t>::max(), VkDeviceSize dstOffset = 0) noexcept;

        template<class Type> void pushConstant(const PipelineLayout& layout, VkShaderStageFlags stageFlags, const Type& constant, uint32_t offset = 0) noexcept;
        template<class Type, uint32_t PushConstantCount> void pushConstants(const PipelineLayout& layout, VkShaderStageFlags stageFlags, const Type(&constants)[PushConstantCount], uint32_t offset = 0) noexcept;
        template<class Type> void pushConstants(const PipelineLayout& layout, VkShaderStageFlags stageFlags, const std::vector<Type>& constants, uint32_t offset = 0) noexcept;
        template<class BlockType> void pushConstantBlock(const PipelineLayout& layout, VkShaderStageFlags stageFlags, const BlockType& block, uint32_t offset = 0) noexcept;

        void beginRenderPass(const std::shared_ptr<RenderPass>& renderPass, const std::shared_ptr<Framebuffer>& framebuffer, const std::initializer_list<ClearValue>& clearValues = {},
            const VkRect2D& renderArea = {0, 0, 0, 0}, VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE) noexcept;
        void nextSubpass(VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE) noexcept;
        void endRenderPass() noexcept;

    #ifdef VK_KHR_device_group
        bool beginDeviceGroup(uint32_t deviceMask, VkCommandBufferUsageFlags flags = 0) noexcept;
        void beginDeviceGroupRenderPass(uint32_t deviceMask, const std::shared_ptr<RenderPass>& renderPass, const std::shared_ptr<Framebuffer>& framebuffer,
            const std::initializer_list<ClearValue>& clearValues = {}, const std::initializer_list<VkRect2D>& deviceRenderAreas = {},
            VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE) noexcept;
        void setDeviceMask(uint32_t deviceMask) noexcept;
        void dispatchBase(uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const noexcept;
    #endif // VK_KHR_device_group

    #ifdef VK_EXT_line_rasterization
        void setLineStipple(uint32_t stippleFactor, uint16_t stipplePattern) noexcept;
    #endif // VK_EXT_line_rasterization

    #ifdef VK_KHR_push_descriptor
        void pushDescriptorSet(const std::shared_ptr<Pipeline>& pipeline, uint32_t set, const std::shared_ptr<DescriptorSet>& descriptorSet) noexcept;
    #endif // VK_KHR_push_descriptor

    #ifdef VK_EXT_multi_draw
        void drawMulti(const std::vector<VkMultiDrawInfoEXT>& vertexInfo) const noexcept;
        void drawMultiInstanced(const std::vector<VkMultiDrawInfoEXT>& vertexInfo, uint32_t instanceCount, uint32_t firstInstance) const noexcept;
        void drawMultiIndexed(const std::vector<VkMultiDrawIndexedInfoEXT>& indexInfo, const std::vector<int32_t>& vertexOffsets = {}) const noexcept;
        void drawMultiIndexedInstanced(const std::vector<VkMultiDrawIndexedInfoEXT>& indexInfo, uint32_t instanceCount, uint32_t firstInstance,
            const std::vector<int32_t>& vertexOffsets = {}) const noexcept;
    #endif // VK_EXT_multi_draw

    #if defined(VK_KHR_draw_indirect_count) || defined(VK_AMD_draw_indirect_count)
        void drawIndirectCount(const std::shared_ptr<Buffer>& buffer, const std::shared_ptr<Buffer>& countBuffer, uint32_t maxDrawCount,
            VkDeviceSize offset = 0, VkDeviceSize countBufferOffset = 0) const noexcept;
        void drawIndexedIndirectCount(const std::shared_ptr<Buffer>& buffer, const std::shared_ptr<Buffer>& countBuffer, uint32_t maxDrawCount,
            VkDeviceSize offset = 0, VkDeviceSize countBufferOffset = 0) const noexcept;
    #endif // VK_KHR_draw_indirect_count || VK_AMD_draw_indirect_count

    #if defined(VK_EXT_mesh_shader) || defined(VK_NV_mesh_shader)
        void drawMeshTasks(uint32_t groupCountX, uint32_t groupCountY = 1, uint32_t groupCountZ = 1) const noexcept;
        void drawMeshTasksIndirect(const std::shared_ptr<DrawMeshTasksIndirectBuffer>& buffer, VkDeviceSize offset = 0) const noexcept;
        void drawMeshTasksIndirectCount(const std::shared_ptr<DrawMeshTasksIndirectBuffer>& buffer, const std::shared_ptr<Buffer>& countBuffer,
            VkDeviceSize offset = 0, VkDeviceSize countBufferOffset = 0) const noexcept;
    #endif // VK_EXT_mesh_shader || VK_NV_mesh_shader

    #ifdef VK_EXT_conditional_rendering
        void beginConditionalRendering(const std::shared_ptr<Buffer>& buffer, VkDeviceSize offset = 0, bool inverted = false) noexcept;
        void endConditionalRendering() noexcept;
    #endif // VK_EXT_conditional_rendering

    #ifdef VK_EXT_transform_feedback
        void bindTransformFeedbackBuffer(uint32_t firstBinding, const std::shared_ptr<Buffer>& transformFeedbackBuffer, VkDeviceSize offset = 0, VkDeviceSize size = VK_WHOLE_SIZE) noexcept;
        void bindTransformFeedbackBuffers(uint32_t firstBinding, const std::initializer_list<std::shared_ptr<Buffer>>& transformFeedbackBuffers, const std::initializer_list<VkDeviceSize>& offsets,
            const std::initializer_list<VkDeviceSize>& sizes = {}) noexcept;
        void beginTransformFeedback(uint32_t firstCounterBuffer, const std::initializer_list<std::shared_ptr<Buffer>>& counterBuffers, const std::initializer_list<VkDeviceSize>& counterBufferOffsets = {}) noexcept;
        void endTransformFeedback(uint32_t firstCounterBuffer, const std::initializer_list<std::shared_ptr<Buffer>>& counterBuffers, const std::initializer_list<VkDeviceSize>& counterBufferOffsets = {}) noexcept;
        void beginQueryIndexed(const std::shared_ptr<QueryPool>& queryPool, uint32_t query, uint32_t index) noexcept;
        void endQueryIndexed(const std::shared_ptr<QueryPool>& queryPool, uint32_t query, uint32_t index) noexcept;
        void drawIndirectByteCount(const std::shared_ptr<Buffer>& counterBuffer, uint32_t vertexStride, VkDeviceSize counterBufferOffset = 0, uint32_t counterOffset = 0) const noexcept;
        void drawIndirectByteCountInstanced(uint32_t instanceCount, uint32_t firstInstance, const std::shared_ptr<Buffer>& counterBuffer, uint32_t vertexStride,
            VkDeviceSize counterBufferOffset = 0, uint32_t counterOffset = 0) const noexcept;
    #endif // VK_EXT_transform_feedback

    #ifdef VK_KHR_imageless_framebuffer
        void beginRenderPass(const std::shared_ptr<RenderPass>& renderPass, const std::shared_ptr<ImagelessFramebuffer>& framebuffer, const std::vector<std::shared_ptr<ImageView>>& attachments,
            const std::initializer_list<ClearValue>& clearValues = {}, const VkRect2D& renderArea = {0, 0, 0, 0}, VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE) noexcept;
    #ifdef VK_KHR_device_group
        void beginDeviceGroupRenderPass(uint32_t deviceMask, const std::shared_ptr<RenderPass>& renderPass, const std::shared_ptr<ImagelessFramebuffer>& framebuffer,
            const std::vector<std::shared_ptr<ImageView>>& attachments,  const std::initializer_list<ClearValue>& clearValues = {}, const std::initializer_list<VkRect2D>& deviceRenderAreas = {},
            VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE) noexcept;
    #endif // VK_KHR_device_group
    #endif // VK_KHR_imageless_framebuffer

    #ifdef VK_KHR_acceleration_structure
        void buildAccelerationStructure(const std::shared_ptr<BottomLevelAccelerationStructure>& accelerationStructure, const std::list<AccelerationStructureGeometry>& geometries,
            const std::shared_ptr<Buffer>& scratchBuffer) const noexcept;
        void buildAccelerationStructure(const std::shared_ptr<BottomLevelAccelerationStructure>& accelerationStructure, const std::list<AccelerationStructureGeometry>& geometries,
            const std::vector<VkAccelerationStructureBuildRangeInfoKHR>& buildRanges, const std::shared_ptr<Buffer>& scratchBuffer) const noexcept;
        void updateAccelerationStructure(const std::shared_ptr<BottomLevelAccelerationStructure>& accelerationStructure, const std::list<AccelerationStructureGeometry>& geometries,
            const std::shared_ptr<Buffer>& scratchBuffer) const noexcept;
        void updateAccelerationStructure(const std::shared_ptr<BottomLevelAccelerationStructure>& accelerationStructure, const std::list<AccelerationStructureGeometry>& geometries,
            const std::vector<VkAccelerationStructureBuildRangeInfoKHR>& buildRanges, const std::shared_ptr<Buffer>& scratchBuffer) const noexcept;
        void buildAccelerationStructure(const std::shared_ptr<TopLevelAccelerationStructure>& accelerationStructure, const AccelerationStructureGeometryInstances& instances,
            const std::shared_ptr<Buffer>& scratchBuffer) const noexcept;
        void updateAccelerationStructure(const std::shared_ptr<TopLevelAccelerationStructure>& accelerationStructure, const AccelerationStructureGeometryInstances& instances,
            const std::shared_ptr<Buffer>& scratchBuffer) const noexcept;
        void updateAccelerationStructureIndirect(const std::shared_ptr<AccelerationStructure>& accelerationStructure, const std::list<AccelerationStructureGeometry>& geometries,
            const std::shared_ptr<Buffer>& indirectBuildRanges, const std::shared_ptr<Buffer>& scratchBuffer) const noexcept;
        void copyAccelerationStructure(const std::shared_ptr<AccelerationStructure>& srcAccelerationStructure, const std::shared_ptr<AccelerationStructure>& dstAccelerationStructure) const noexcept;
        void compactAccelerationStructure(const std::shared_ptr<AccelerationStructure>& srcAccelerationStructure, const std::shared_ptr<AccelerationStructure>& dstAccelerationStructure) const noexcept;
        void copyAccelerationStructureToBuffer(const std::shared_ptr<AccelerationStructure>& srcAccelerationStructure, const std::shared_ptr<Buffer>& dstBuffer) const noexcept;
        void copyBufferToAccelerationStructure(const std::shared_ptr<Buffer>& srcBuffer, const std::shared_ptr<AccelerationStructure>& dstAccelerationStructure) const noexcept;
        void serializeAccelerationStructure(const std::shared_ptr<AccelerationStructure>& srcAccelerationStructure, const std::shared_ptr<Buffer>& dstBuffer, VkDeviceAddress bufferOffset = 0) const noexcept;
        void deserializeAccelerationStructure(const std::shared_ptr<Buffer>& srcBuffer, const std::shared_ptr<AccelerationStructure>& dstAccelerationStructure, VkDeviceAddress bufferOffset = 0) const noexcept;
        void writeAccelerationStructureProperties(const std::shared_ptr<AccelerationStructure>& accelerationStructure, const std::shared_ptr<QueryPool>& queryPool, uint32_t firstQuery = 0) const noexcept;
    #endif // VK_KHR_acceleration_structure

    #ifdef VK_KHR_ray_tracing_pipeline
        void setRayTracingPipelineStackSize(uint32_t pipelineStackSize) const noexcept;
        void traceRays(const ShaderBindingTable& shaderBindingTable, uint32_t width, uint32_t height, uint32_t depth = 1) const noexcept;
        void traceRaysIndirect(const ShaderBindingTable& shaderBindingTable, const std::shared_ptr<Buffer>& indirectBuffer) const noexcept;
    #endif // VK_KHR_ray_tracing_pipeline
    #ifdef VK_KHR_ray_tracing_maintenance1
        void traceRaysIndirect(const std::shared_ptr<Buffer>& indirectBuffer) const noexcept;
    #endif // VK_KHR_ray_tracing_maintenance1

    #ifdef VK_EXT_debug_marker
        void beginDebugMarker(const char* /* name */, float /* r */, float /* g */, float /* b */, float /* a */ = 1.f) noexcept MAGMA_NOOP;
        void endDebugMarker() noexcept MAGMA_NOOP;
        void insertDebugMarker(const char* /* name */, float /* r */, float /* g */, float /* b */, float /* a */ = 1.f) noexcept MAGMA_NOOP;
    #endif // VK_EXT_debug_marker
    #ifdef VK_EXT_debug_utils
        void beginDebugLabel(const char* /* name */, float /* r */, float /* g */, float /* b */, float /* a */ = 1.f) noexcept MAGMA_NOOP;
        void endDebugLabel() noexcept MAGMA_NOOP;
        void insertDebugLabel(const char* /* name */, float /* r */, float /* g */, float /* b */, float /* a */ = 1.f) noexcept MAGMA_NOOP;
    #endif // VK_EXT_debug_utils
    #ifdef VK_AMD_buffer_marker
        void writeBufferMarker(VkPipelineStageFlagBits /* pipelineStage */, const std::unique_ptr<Buffer>& /* dstBuffer */, VkDeviceSize /* dstOffset */, uint32_t /* marker */) const noexcept MAGMA_NOOP;
    #endif
    #ifdef VK_NV_device_diagnostic_checkpoints
        void setCheckpoint(const char* /* name */) const noexcept MAGMA_NOOP;
    #endif

        bool beginAnnotated(const char *name, uint32_t color, VkCommandBufferUsageFlags flags = 0) noexcept;
        bool beginInheritedAnnotated(const char *name, uint32_t color, const std::shared_ptr<RenderPass>& renderPass, uint32_t subpass, const std::shared_ptr<Framebuffer>& framebuffer,
            bool occlusionQueryEnable = false, VkQueryControlFlags queryFlags = 0, VkQueryPipelineStatisticFlags pipelineStatistics = 0, VkCommandBufferUsageFlags flags = 0,
            const StructureChain& extendedInfo = StructureChain()) noexcept;
        void beginRenderPassAnnotated(const char *name, uint32_t color, const std::shared_ptr<RenderPass>& renderPass, const std::shared_ptr<Framebuffer>& framebuffer,
            const std::initializer_list<ClearValue>& clearValues = {}, const VkRect2D& renderArea = {0, 0, 0, 0}, VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE) noexcept;
    #ifdef VK_KHR_imageless_framebuffer
        void beginRenderPassAnnotated(const char *name, uint32_t color, const std::shared_ptr<RenderPass>& renderPass, const std::shared_ptr<ImagelessFramebuffer>& framebuffer,
            const std::vector<std::shared_ptr<ImageView>>& attachments, const std::initializer_list<ClearValue>& clearValues = {}, const VkRect2D& renderArea = {0, 0, 0, 0},
            VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE) noexcept;
    #endif // VK_KHR_imageless_framebuffer
    #ifdef VK_KHR_device_group
        bool beginDeviceGroupAnnotated(const char *name, uint32_t color, uint32_t deviceMask, VkCommandBufferUsageFlags flags = 0) noexcept;
        void beginDeviceGroupRenderPassAnnotated(const char *name, uint32_t color, uint32_t deviceMask,
            const std::shared_ptr<RenderPass>& renderPass, const std::shared_ptr<Framebuffer>& framebuffer,
            const std::initializer_list<ClearValue>& clearValues = {}, const std::initializer_list<VkRect2D>& deviceRenderAreas = {},
            VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE) noexcept;
        #ifdef VK_KHR_imageless_framebuffer
        void beginDeviceGroupRenderPassAnnotated(const char *name, uint32_t color, uint32_t deviceMask, const std::shared_ptr<RenderPass>& renderPass, const std::shared_ptr<ImagelessFramebuffer>& framebuffer,
            const std::vector<std::shared_ptr<ImageView>>& attachments,  const std::initializer_list<ClearValue>& clearValues = {}, const std::initializer_list<VkRect2D>& deviceRenderAreas = {},
            VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE) noexcept;
        #endif // VK_KHR_imageless_framebuffer
    #endif // VK_KHR_device_group
    #ifdef VK_EXT_conditional_rendering
        void beginConditionalRenderingAnnotated(const char *name, uint32_t color, const std::shared_ptr<Buffer>& buffer, VkDeviceSize offset = 0, bool inverted = false) noexcept;
    #endif // VK_EXT_conditional_rendering
    #ifdef VK_EXT_transform_feedback
        void beginTransformFeedbackAnnotated(const char *name, uint32_t color, uint32_t firstCounterBuffer, const std::initializer_list<std::shared_ptr<Buffer>>& counterBuffers,
            const std::initializer_list<VkDeviceSize>& counterBufferOffsets = {}) noexcept;
    #endif // VK_EXT_transform_feedback

        struct Statistics
        {
            uint16_t bindPipelineCount = 0;
            uint16_t bindDescriptorSetCount = 0;
            uint16_t pushDescriptorSetCount = 0;
            uint16_t bindIndexBufferCount = 0;
            uint16_t bindVertexBufferCount = 0;
            uint16_t bindTransformFeedbackBufferCount = 0;
            uint16_t pipelineBarrierCount = 0;
            uint16_t queryCount = 0;
            uint16_t indexedQueryCount = 0;
            uint16_t pushConstantCount = 0;
            uint16_t renderPassCount = 0;
            uint16_t conditionalRenderingCount = 0;
            uint16_t transformFeedbackCount = 0;
            uint16_t drawCount = 0;
            uint16_t drawIndexedCount = 0;
            uint16_t drawIndirectCount = 0;
            uint16_t drawIndexedIndirectCount = 0;
            uint16_t dispatchCount = 0;
            uint16_t dispatchIndirectCount = 0;
            uint16_t drawMeshTasksCount = 0;
            uint16_t drawMeshTasksIndirectCount = 0;
        };

        LeanCommandBuffer& getLean() noexcept { return leanCmd; }
        bool primary() const noexcept { return VK_COMMAND_BUFFER_LEVEL_PRIMARY == level; }
        uint32_t getQueueFamilyIndex() const noexcept { return queueFamilyIndex; }
        const std::shared_ptr<Device>& getDevice() const noexcept { return device; }
        const std::unique_ptr<Fence>& getFence() const noexcept { return fence; }
        VkCommandBufferUsageFlags getUsage() const noexcept { return usage; }
        State getState() const noexcept { return state; }
        const Statistics& getStats() const noexcept { return stats; }
        const std::unique_ptr<Buffer>& getMarkerBuffer() const noexcept { return markerBuffer; }
        bool allowsReset() const noexcept { return VK_TRUE == resetCommandBuffer; }
        bool insideRenderPass() const noexcept { return renderingPass; }
        bool insideQuery() const noexcept { return nonIndexedQuery; }
        bool insideQueryIndexed() const noexcept { return indexedQuery; }
        bool insideConditionalRendering() const noexcept { return conditionalRendering; }
        bool insideTransformFeedback() const noexcept { return transformFeedback; }
        uint32_t inUseObjectCount() const noexcept;
        void releaseObjectsInUse() noexcept;

    protected:
        CommandBuffer(VkCommandBufferLevel level, VkCommandBuffer handle, const CommandPool *cmdPool);
        CommandBuffer(VkCommandBufferLevel level, const CommandPool *cmdPool);

        LeanCommandBuffer leanCmd;

    private:
        struct Extensions
        {
            const VkBool32 AMD_buffer_marker: 1;
            const VkBool32 AMD_draw_indirect_count: 1;
            const VkBool32 AMD_negative_viewport_height: 1;

            const VkBool32 EXT_debug_marker: 1;
            const VkBool32 EXT_conditional_rendering: 1;
            const VkBool32 EXT_line_rasterization: 1;
            const VkBool32 EXT_mesh_shader: 1;
            const VkBool32 EXT_multi_draw: 1;
            const VkBool32 EXT_transform_feedback: 1;

            const VkBool32 KHR_acceleration_structure: 1;
            const VkBool32 KHR_device_group: 1;
            const VkBool32 KHR_draw_indirect_count: 1;
            const VkBool32 KHR_imageless_framebuffer: 1;
            const VkBool32 KHR_maintenance1: 1;
            const VkBool32 KHR_push_descriptor: 1;
            const VkBool32 KHR_ray_tracing_pipeline: 1;
            const VkBool32 KHR_ray_tracing_maintenance1: 1;

            const VkBool32 NV_device_diagnostic_checkpoints: 1;
            const VkBool32 NV_mesh_shader: 1;

            const VkBool32 EXT_debug_utils: 1;

            explicit Extensions(std::shared_ptr<const Device> device);
        };

        struct RenderPassState
        {
            std::shared_ptr<RenderPass> renderPass;
            std::shared_ptr<Framebuffer> framebuffer;
            std::vector<std::shared_ptr<ImageView>> attachments;
        };

        VkDevice getNativeDevice() const noexcept { return leanCmd.device; }
        void queueSubmissionFinished() noexcept;
        void executionFinished() noexcept;
        void pushDebugMarker(const char* /* name */, uint32_t /* color */) noexcept MAGMA_NOOP;
        void popDebugMarker() noexcept MAGMA_NOOP;
        void insertDebugCheckpoint(const char *command, VkPipelineStageFlagBits pipelineStage) const noexcept;
        void resetInternalState() noexcept;

        const VkCommandBufferLevel level;
        const VkCommandPool cmdPool;
        const uint32_t queueFamilyIndex;
        const Extensions extensions;
        std::shared_ptr<Device> device;
        std::unique_ptr<Fence> fence;
        VkCommandBufferUsageFlags usage;
        State state;
        mutable Statistics stats;
        const VkBool32 resetCommandBuffer: 1;
        VkBool32 renderingPass: 1;
        VkBool32 nonIndexedQuery: 1;
        VkBool32 indexedQuery: 1;
        VkBool32 conditionalRendering: 1;
        VkBool32 transformFeedback: 1;
        VkBool32 annotatedBegin: 1;
        VkBool32 annotatedRenderPass: 1;
        VkBool32 annotatedConditionalRendering: 1;
        VkBool32 annotatedTransformFeedback: 1;
        RenderPassState renderPassState;
        mutable std::unique_ptr<Buffer> markerBuffer;
        mutable std::vector<const char *> checkpoints;
    #ifdef MAGMA_RETAIN_OBJECTS_IN_USE
        // User may optionally compile library with this define to allow
        // deferred release of resources bound to command buffer.
        mutable std::unordered_set<std::shared_ptr<DeviceChild>> inUse;
    #endif
        friend class Queue;
    };

    /* See 6.1. Command Buffer Lifecycle
       Each command buffer is always in one of the following states: */

    enum class CommandBuffer::State : uint8_t
    {
        Initial, Recording, Executable, Pending, Invalid
    };
} // namespace magma

#include "commandBuffer.inl"
