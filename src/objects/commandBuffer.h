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
#include "topLevelStructure.h"
#include "../barriers/memoryBarrier.h"
#include "../barriers/bufferMemoryBarrier.h"
#include "../barriers/imageMemoryBarrier.h"
#include "../misc/imageResolve.h"
#include "../misc/clearValue.h"
#include "../misc/clearAttachment.h"

namespace magma
{
    class Fence;
    class Queue;
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
        bool beginInherited(lent_ptr<const RenderPass> renderPass, uint32_t subpass, lent_ptr<const Framebuffer> framebuffer,
            bool occlusionQueryEnable = false, VkQueryControlFlags queryFlags = 0, VkQueryPipelineStatisticFlags pipelineStatistics = 0,
            VkCommandBufferUsageFlags flags = 0, const StructureChain& extendedInfo = StructureChain()) noexcept;
        void end();
        bool reset(VkCommandBufferResetFlags flags = 0) noexcept;

        void bindPipeline(lent_ptr<const Pipeline> pipeline) noexcept;

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

        void bindDescriptorSet(lent_ptr<const Pipeline> pipeline, uint32_t firstSet, lent_ptr<const DescriptorSet> descriptorSet,
            uint32_t dynamicOffset = std::numeric_limits<uint32_t>::max()) noexcept;
        void bindDescriptorSet(lent_ptr<const Pipeline> pipeline, uint32_t firstSet, lent_ptr<const DescriptorSet> descriptorSet,
            const std::initializer_list<uint32_t>& dynamicOffsets) noexcept;
        void bindDescriptorSets(lent_ptr<const Pipeline> pipeline, uint32_t firstSet, const std::initializer_list<lent_ptr<const DescriptorSet>>& descriptorSets,
            const std::initializer_list<uint32_t>& dynamicOffsets = {}) noexcept;

        void bindIndexBuffer(lent_ptr<const BaseIndexBuffer> indexBuffer, VkDeviceSize offset = 0) noexcept;
        void bindVertexBuffer(uint32_t firstBinding, lent_ptr<const Buffer> vertexBuffer, VkDeviceSize offset = 0) noexcept;
        void bindVertexBuffers(uint32_t firstBinding, const std::initializer_list<lent_ptr<const Buffer>>& vertexBuffers, const std::initializer_list<VkDeviceSize>& offsets = {}) noexcept;

        void draw(uint32_t vertexCount, uint32_t firstVertex = 0) const noexcept;
        void drawInstanced(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex = 0, uint32_t firstInstance = 0) const noexcept;
        void drawIndexed(uint32_t indexCount, uint32_t firstIndex = 0, int32_t vertexOffset = 0) const noexcept;
        void drawIndexedInstanced(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) const noexcept;
        void drawIndirect(lent_ptr<const Buffer> buffer, uint32_t drawCount, VkDeviceSize offset = 0) const noexcept;
        void drawIndirect(lent_ptr<const DrawIndirectBuffer> buffer) const noexcept;
        void drawIndexedIndirect(lent_ptr<const Buffer> buffer, uint32_t drawCount, VkDeviceSize offset = 0) const noexcept;
        void drawIndexedIndirect(lent_ptr<const DrawIndexedIndirectBuffer> buffer) const noexcept;
        void dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const noexcept;
        void dispatchIndirect(lent_ptr<const Buffer> buffer, VkDeviceSize offset = 0) const noexcept;

        void copyBuffer(lent_ptr<const Buffer> srcBuffer, lent_ptr<Buffer> dstBuffer,
            VkDeviceSize srcOffset = 0, VkDeviceSize dstOffset = 0, VkDeviceSize size = VK_WHOLE_SIZE) const noexcept;
        void copyImage(lent_ptr<const Image> srcImage, lent_ptr<Image> dstImage, const VkImageCopy& region) const noexcept;
        void copyImage(lent_ptr<const Image> srcImage, lent_ptr<Image> dstImage, uint32_t mipLevel) const noexcept;
        void blitImage(lent_ptr<const Image> srcImage, lent_ptr<Image> dstImage, const VkImageBlit& region, VkFilter filter) const noexcept;
        void blitImage(lent_ptr<const Image> srcImage, lent_ptr<Image> dstImage, uint32_t srcMipLevel, uint32_t dstMipLevel, VkFilter filter) const noexcept;
        void copyBufferToImage(lent_ptr<const Buffer> srcBuffer, lent_ptr<Image> dstImage, const VkBufferImageCopy& region) const noexcept;
        void copyBufferToImage(lent_ptr<const Buffer> srcBuffer, lent_ptr<Image> dstImage, const std::vector<VkBufferImageCopy>& regions) const noexcept;
        void copyImageToBuffer(lent_ptr<const Image> srcImage, lent_ptr<Buffer> dstBuffer, const VkBufferImageCopy& region) const noexcept;
        void copyImageToBuffer(lent_ptr<const Image> srcImage, lent_ptr<Buffer> dstBuffer, const std::vector<VkBufferImageCopy>& regions) const noexcept;
        void updateBuffer(lent_ptr<Buffer> buffer, VkDeviceSize dataSize, const void *data, VkDeviceSize offset = 0) const noexcept;
        template<class Type> void updateBuffer(lent_ptr<Buffer> buffer, const std::vector<Type>& data, VkDeviceSize offset = 0) const noexcept;
        void fillBuffer(lent_ptr<Buffer> buffer, uint32_t value, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) const noexcept;

        void clearColorImage(lent_ptr<Image> image, const ClearColor& color, const VkImageSubresourceRange& range) const noexcept;
        void clearDepthStencilImage(lent_ptr<Image> image, const ClearDepthStencil& depthStencil, const VkImageSubresourceRange& range) const noexcept;
        void clearAttachments(const std::initializer_list<ClearAttachment>& attachments, const VkClearRect& clearRect) const noexcept;
        void resolveImage(lent_ptr<const Image> srcImage, lent_ptr<Image> dstImage) const noexcept;
        void resolveImage(lent_ptr<const Image> srcImage, lent_ptr<Image> dstImage, const VkImageResolve& region) const noexcept;

        void setEvent(lent_ptr<Event> event, VkPipelineStageFlags stageMask) const noexcept;
        void resetEvent(lent_ptr<Event> event, VkPipelineStageFlags stageMask) const noexcept;
        void waitEvent(lent_ptr<const Event> event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask) const noexcept;
        void waitEvent(lent_ptr<const Event> event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const MemoryBarrier& barrier) const noexcept;
        void waitEvent(lent_ptr<const Event> event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const BufferMemoryBarrier& barrier) const noexcept;
        void waitEvent(lent_ptr<const Event> event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const ImageMemoryBarrier& barrier) const noexcept;
        void waitEvent(lent_ptr<const Event> event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::initializer_list<MemoryBarrier>& memoryBarriers) const noexcept;
        void waitEvent(lent_ptr<const Event> event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::vector<MemoryBarrier>& memoryBarriers) const noexcept;
        void waitEvent(lent_ptr<const Event> event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::initializer_list<BufferMemoryBarrier>& bufferMemoryBarriers) const noexcept;
        void waitEvent(lent_ptr<const Event> event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::vector<BufferMemoryBarrier>& bufferMemoryBarriers) const noexcept;
        void waitEvent(lent_ptr<const Event> event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::initializer_list<ImageMemoryBarrier>& imageMemoryBarriers) const noexcept;
        void waitEvent(lent_ptr<const Event> event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::vector<ImageMemoryBarrier>& imageMemoryBarriers) const noexcept;
        void waitEvents(const std::initializer_list<lent_ptr<const Event>>& events, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::initializer_list<MemoryBarrier>& memoryBarriers = {},
            const std::initializer_list<BufferMemoryBarrier>& bufferMemoryBarriers = {}, const std::initializer_list<ImageMemoryBarrier>& imageMemoryBarriers = {}) const noexcept;
        void waitEvents(const std::vector<lent_ptr<const Event>>& events, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::vector<MemoryBarrier>& memoryBarriers,
            const std::vector<BufferMemoryBarrier>& bufferMemoryBarriers, const std::vector<ImageMemoryBarrier>& imageMemoryBarriers) const noexcept;

        void pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags = 0) noexcept;
        void pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const MemoryBarrier& barrier, VkDependencyFlags dependencyFlags = 0) noexcept;
        void pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const BufferMemoryBarrier& barrier, VkDependencyFlags dependencyFlags = 0) noexcept;
        void pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const ImageMemoryBarrier& barrier, VkDependencyFlags dependencyFlags = 0) noexcept;
        void pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::initializer_list<MemoryBarrier>& memoryBarriers, VkDependencyFlags dependencyFlags = 0) noexcept;
        void pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::vector<MemoryBarrier>& memoryBarriers, VkDependencyFlags dependencyFlags = 0) noexcept;
        void pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::initializer_list<BufferMemoryBarrier>& bufferMemoryBarriers, VkDependencyFlags dependencyFlags = 0) noexcept;
        void pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::vector<BufferMemoryBarrier>& bufferMemoryBarriers, VkDependencyFlags dependencyFlags = 0) noexcept;
        void pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::initializer_list<ImageMemoryBarrier>& imageMemoryBarriers, VkDependencyFlags dependencyFlags = 0) noexcept;
        void pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::vector<ImageMemoryBarrier>& imageMemoryBarriers, VkDependencyFlags dependencyFlags = 0) noexcept;
        void pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::initializer_list<MemoryBarrier>& memoryBarriers = {},
            const std::initializer_list<BufferMemoryBarrier>& bufferMemoryBarriers = {}, const std::initializer_list<ImageMemoryBarrier>& imageMemoryBarriers = {}, VkDependencyFlags dependencyFlags = 0) noexcept;
        void pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::vector<MemoryBarrier>& memoryBarriers,
            const std::vector<BufferMemoryBarrier>& bufferMemoryBarriers, const std::vector<ImageMemoryBarrier>& imageMemoryBarriers, VkDependencyFlags dependencyFlags = 0) noexcept;

        void beginQuery(lent_ptr<QueryPool> queryPool, uint32_t query) noexcept;
        void endQuery(lent_ptr<QueryPool> queryPool, uint32_t query) noexcept;
        void resetQueryPool(lent_ptr<QueryPool> queryPool, uint32_t firstQuery, uint32_t queryCount) noexcept;
        void writeTimestamp(VkPipelineStageFlagBits pipelineStage, lent_ptr<QueryPool> queryPool, uint32_t query) noexcept;
        template<class Type>
        void copyQueryResults(lent_ptr<const QueryPool> queryPool, lent_ptr<Buffer> dstBuffer, bool wait,
            uint32_t firstQuery = 0, uint32_t queryCount = std::numeric_limits<uint32_t>::max(), VkDeviceSize dstOffset = 0) noexcept;
        template<class Type>
        void copyQueryResultsWithAvailability(lent_ptr<const QueryPool> queryPool, lent_ptr<Buffer> dstBuffer,
            uint32_t firstQuery = 0, uint32_t queryCount = std::numeric_limits<uint32_t>::max(), VkDeviceSize dstOffset = 0) noexcept;

        template<class Type> void pushConstant(lent_ptr<const PipelineLayout> layout, VkShaderStageFlags stageFlags, const Type& constant, uint32_t offset = 0) noexcept;
        template<class Type, uint32_t PushConstantCount> void pushConstants(lent_ptr<const PipelineLayout> layout, VkShaderStageFlags stageFlags, const Type(&constants)[PushConstantCount], uint32_t offset = 0) noexcept;
        template<class Type> void pushConstants(lent_ptr<const PipelineLayout> layout, VkShaderStageFlags stageFlags, const std::vector<Type>& constants, uint32_t offset = 0) noexcept;
        template<class BlockType> void pushConstantBlock(lent_ptr<const PipelineLayout> layout, VkShaderStageFlags stageFlags, const BlockType& block, uint32_t offset = 0) noexcept;

        void beginRenderPass(lent_ptr<const RenderPass> renderPass, lent_ptr<Framebuffer> framebuffer, const std::initializer_list<ClearValue> clearValues = {},
            const VkRect2D& renderArea = {0, 0, 0, 0}, VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE) noexcept;
        void nextSubpass(VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE) noexcept;
        void endRenderPass() noexcept;

    #ifdef VK_KHR_device_group
        bool beginDeviceGroup(uint32_t deviceMask, VkCommandBufferUsageFlags flags = 0) noexcept;
        void beginDeviceGroupRenderPass(uint32_t deviceMask, lent_ptr<const RenderPass> renderPass, lent_ptr<Framebuffer> framebuffer,
            const std::initializer_list<ClearValue>& clearValues = {}, const std::initializer_list<VkRect2D>& deviceRenderAreas = {},
            VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE) noexcept;
        void setDeviceMask(uint32_t deviceMask) noexcept;
        void dispatchBase(uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const noexcept;
    #endif // VK_KHR_device_group

    #ifdef VK_EXT_line_rasterization
        void setLineStipple(uint32_t stippleFactor, uint16_t stipplePattern) noexcept;
    #endif // VK_EXT_line_rasterization

    #ifdef VK_KHR_push_descriptor
        void pushDescriptorSet(lent_ptr<const Pipeline> pipeline, uint32_t set, lent_ptr<const DescriptorSet> descriptorSet) noexcept;
    #endif // VK_KHR_push_descriptor

    #ifdef VK_EXT_multi_draw
        void drawMulti(const std::vector<VkMultiDrawInfoEXT>& vertexInfo) const noexcept;
        void drawMultiInstanced(const std::vector<VkMultiDrawInfoEXT>& vertexInfo, uint32_t instanceCount, uint32_t firstInstance) const noexcept;
        void drawMultiIndexed(const std::vector<VkMultiDrawIndexedInfoEXT>& indexInfo, const std::vector<int32_t>& vertexOffsets = {}) const noexcept;
        void drawMultiIndexedInstanced(const std::vector<VkMultiDrawIndexedInfoEXT>& indexInfo, uint32_t instanceCount, uint32_t firstInstance,
            const std::vector<int32_t>& vertexOffsets = {}) const noexcept;
    #endif // VK_EXT_multi_draw

    #if defined(VK_KHR_draw_indirect_count) || defined(VK_AMD_draw_indirect_count)
        void drawIndirectCount(lent_ptr<const Buffer> buffer, lent_ptr<const Buffer> countBuffer, uint32_t maxDrawCount,
            VkDeviceSize offset = 0, VkDeviceSize countBufferOffset = 0) const noexcept;
        void drawIndexedIndirectCount(lent_ptr<const Buffer> buffer, lent_ptr<const Buffer> countBuffer, uint32_t maxDrawCount,
            VkDeviceSize offset = 0, VkDeviceSize countBufferOffset = 0) const noexcept;
    #endif // VK_KHR_draw_indirect_count || VK_AMD_draw_indirect_count

    #if defined(VK_EXT_mesh_shader) || defined(VK_NV_mesh_shader)
        void drawMeshTasks(uint32_t groupCountX, uint32_t groupCountY = 1, uint32_t groupCountZ = 1) const noexcept;
        void drawMeshTasksIndirect(lent_ptr<const DrawMeshTasksIndirectBuffer> buffer, VkDeviceSize offset = 0) const noexcept;
        void drawMeshTasksIndirectCount(lent_ptr<const DrawMeshTasksIndirectBuffer> buffer, lent_ptr<const Buffer> countBuffer,
            VkDeviceSize offset = 0, VkDeviceSize countBufferOffset = 0) const noexcept;
    #endif // VK_EXT_mesh_shader || VK_NV_mesh_shader

    #ifdef VK_EXT_conditional_rendering
        void beginConditionalRendering(lent_ptr<const Buffer> buffer, VkDeviceSize offset = 0, bool inverted = false) noexcept;
        void endConditionalRendering() noexcept;
    #endif // VK_EXT_conditional_rendering

    #ifdef VK_EXT_transform_feedback
        void bindTransformFeedbackBuffer(uint32_t firstBinding, lent_ptr<Buffer> transformFeedbackBuffer, VkDeviceSize offset = 0, VkDeviceSize size = VK_WHOLE_SIZE) noexcept;
        void bindTransformFeedbackBuffers(uint32_t firstBinding, const std::initializer_list<lent_ptr<Buffer>>& transformFeedbackBuffers, const std::initializer_list<VkDeviceSize> offsets,
            const std::initializer_list<VkDeviceSize> sizes = {}) noexcept;
        void beginTransformFeedback(uint32_t firstCounterBuffer, const std::initializer_list<lent_ptr<const Buffer>>& counterBuffers, const std::initializer_list<VkDeviceSize>& counterBufferOffsets = {}) noexcept;
        void endTransformFeedback(uint32_t firstCounterBuffer, const std::initializer_list<lent_ptr<Buffer>>& counterBuffers, const std::initializer_list<VkDeviceSize> counterBufferOffsets = {}) noexcept;
        void beginQueryIndexed(lent_ptr<QueryPool> queryPool, uint32_t query, uint32_t index) noexcept;
        void endQueryIndexed(lent_ptr<QueryPool> queryPool, uint32_t query, uint32_t index) noexcept;
        void drawIndirectByteCount(lent_ptr<const Buffer> counterBuffer, uint32_t vertexStride, VkDeviceSize counterBufferOffset = 0, uint32_t counterOffset = 0) const noexcept;
        void drawIndirectByteCountInstanced(uint32_t instanceCount, uint32_t firstInstance, lent_ptr<const Buffer> counterBuffer, uint32_t vertexStride,
            VkDeviceSize counterBufferOffset = 0, uint32_t counterOffset = 0) const noexcept;
    #endif // VK_EXT_transform_feedback

    #ifdef VK_KHR_imageless_framebuffer
        void beginRenderPass(lent_ptr<const RenderPass> renderPass, lent_ptr<ImagelessFramebuffer> framebuffer, const std::vector<lent_ptr<ImageView>>& attachments,
            const std::initializer_list<ClearValue> clearValues = {}, const VkRect2D& renderArea = {0, 0, 0, 0}, VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE) noexcept;
    #ifdef VK_KHR_device_group
        void beginDeviceGroupRenderPass(uint32_t deviceMask, lent_ptr<const RenderPass> renderPass, lent_ptr<ImagelessFramebuffer> framebuffer,
            const std::vector<lent_ptr<ImageView>>& attachments,  const std::initializer_list<ClearValue> clearValues = {}, const std::initializer_list<VkRect2D> deviceRenderAreas = {},
            VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE) noexcept;
    #endif // VK_KHR_device_group
    #endif // VK_KHR_imageless_framebuffer

    #ifdef VK_KHR_acceleration_structure
        void buildAccelerationStructure(lent_ptr<BottomLevelAccelerationStructure> accelerationStructure, const std::list<AccelerationStructureGeometry>& geometries,
            lent_ptr<Buffer> scratchBuffer) const noexcept;
        void buildAccelerationStructure(lent_ptr<BottomLevelAccelerationStructure> accelerationStructure, const std::list<AccelerationStructureGeometry>& geometries,
            const std::vector<VkAccelerationStructureBuildRangeInfoKHR>& buildRanges, lent_ptr<Buffer> scratchBuffer) const noexcept;
        void updateAccelerationStructure(lent_ptr<BottomLevelAccelerationStructure> accelerationStructure, const std::list<AccelerationStructureGeometry>& geometries,
            lent_ptr<Buffer> scratchBuffer) const noexcept;
        void updateAccelerationStructure(lent_ptr<BottomLevelAccelerationStructure> accelerationStructure, const std::list<AccelerationStructureGeometry>& geometries,
            const std::vector<VkAccelerationStructureBuildRangeInfoKHR>& buildRanges, lent_ptr<Buffer> scratchBuffer) const noexcept;
        void buildAccelerationStructure(lent_ptr<TopLevelAccelerationStructure> accelerationStructure, const AccelerationStructureInstances& instances,
            lent_ptr<Buffer> scratchBuffer) const noexcept;
        void updateAccelerationStructure(lent_ptr<TopLevelAccelerationStructure> accelerationStructure, const AccelerationStructureInstances& instances,
            lent_ptr<Buffer> scratchBuffer) const noexcept;
        void updateAccelerationStructureIndirect(lent_ptr<AccelerationStructure> accelerationStructure, const std::list<AccelerationStructureGeometry> geometries,
            lent_ptr<const Buffer> indirectBuildRanges, lent_ptr<Buffer> scratchBuffer) const noexcept;
        void copyAccelerationStructure(lent_ptr<const AccelerationStructure> srcAccelerationStructure, lent_ptr<AccelerationStructure> dstAccelerationStructure) const noexcept;
        void compactAccelerationStructure(lent_ptr<const AccelerationStructure> srcAccelerationStructure, lent_ptr<AccelerationStructure> dstAccelerationStructure) const noexcept;
        void copyAccelerationStructureToBuffer(lent_ptr<const AccelerationStructure> srcAccelerationStructure, lent_ptr<Buffer> dstBuffer) const noexcept;
        void copyBufferToAccelerationStructure(lent_ptr<const Buffer> srcBuffer, lent_ptr<AccelerationStructure> dstAccelerationStructure) const noexcept;
        void serializeAccelerationStructure(lent_ptr<const AccelerationStructure> srcAccelerationStructure, lent_ptr<Buffer> dstBuffer, VkDeviceAddress bufferOffset = 0) const noexcept;
        void deserializeAccelerationStructure(lent_ptr<const Buffer> srcBuffer, lent_ptr<AccelerationStructure> dstAccelerationStructure, VkDeviceAddress bufferOffset = 0) const noexcept;
        void writeAccelerationStructureProperties(lent_ptr<const AccelerationStructure> accelerationStructure, lent_ptr<QueryPool> queryPool, uint32_t firstQuery = 0) const noexcept;
    #endif // VK_KHR_acceleration_structure

    #ifdef VK_NV_cluster_acceleration_structure
        void moveObjects(lent_ptr<ClusterAccelerationStructure> accelerationStructure, lent_ptr<Buffer> scratchBuffer) const noexcept;
        void buildClustersBottomLevel(lent_ptr<BottomLevelClusterAcccelerationStructure> accelerationStructure, lent_ptr<Buffer> scratchBuffer) const noexcept;
        void buildTriangleCluster(lent_ptr<TriangleClusterAccelerationStructure> accelerationStructure, lent_ptr<Buffer> scratchBuffer) const noexcept;
        void buildTriangleClusterTemplate(lent_ptr<TriangleClusterAccelerationStructureTemplate> accelerationStructure, lent_ptr<Buffer> scratchBuffer) const noexcept;
        void instantiateTriangleCluster(lent_ptr<TriangleClusterAccelerationStructure> accelerationStructure, lent_ptr<Buffer> scratchBuffer) const noexcept;
        void getClusterTemplateIndices(lent_ptr<ClusterAccelerationStructure> accelerationStructure,lent_ptr<Buffer> scratchBuffer) const noexcept;
    #endif // VK_NV_cluster_acceleration_structure

    #ifdef VK_KHR_ray_tracing_pipeline
        void setRayTracingPipelineStackSize(uint32_t pipelineStackSize) const noexcept;
        void traceRays(const ShaderBindingTable& shaderBindingTable, uint32_t width, uint32_t height, uint32_t depth = 1) const noexcept;
        void traceRaysIndirect(const ShaderBindingTable& shaderBindingTable, lent_ptr<const Buffer> indirectBuffer) const noexcept;
    #endif // VK_KHR_ray_tracing_pipeline
    #ifdef VK_KHR_ray_tracing_maintenance1
        void traceRaysIndirect(lent_ptr<const Buffer> indirectBuffer) const noexcept;
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
        void writeBufferMarker(VkPipelineStageFlagBits /* pipelineStage */, lent_ptr<Buffer> /* dstBuffer */, VkDeviceSize /* dstOffset */, uint32_t /* marker */) const noexcept MAGMA_NOOP;
    #endif
    #ifdef VK_NV_device_diagnostic_checkpoints
        void setCheckpoint(const char* /* name */) const noexcept MAGMA_NOOP;
    #endif

        bool beginAnnotated(const char *name, uint32_t color, VkCommandBufferUsageFlags flags = 0) noexcept;
        bool beginInheritedAnnotated(const char *name, uint32_t color, lent_ptr<const RenderPass> renderPass, uint32_t subpass, lent_ptr<Framebuffer> framebuffer,
            bool occlusionQueryEnable = false, VkQueryControlFlags queryFlags = 0, VkQueryPipelineStatisticFlags pipelineStatistics = 0, VkCommandBufferUsageFlags flags = 0,
            const StructureChain& extendedInfo = StructureChain()) noexcept;
        void beginRenderPassAnnotated(const char *name, uint32_t color, lent_ptr<const RenderPass> renderPass, lent_ptr<Framebuffer> framebuffer,
            const std::initializer_list<ClearValue>& clearValues = {}, const VkRect2D& renderArea = {0, 0, 0, 0}, VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE) noexcept;
    #ifdef VK_KHR_imageless_framebuffer
        void beginRenderPassAnnotated(const char *name, uint32_t color, lent_ptr<const RenderPass> renderPass, lent_ptr<ImagelessFramebuffer> framebuffer,
            const std::vector<lent_ptr<ImageView>>& attachments, const std::initializer_list<ClearValue>& clearValues = {}, const VkRect2D& renderArea = {0, 0, 0, 0},
            VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE) noexcept;
    #endif // VK_KHR_imageless_framebuffer
    #ifdef VK_KHR_device_group
        bool beginDeviceGroupAnnotated(const char *name, uint32_t color, uint32_t deviceMask, VkCommandBufferUsageFlags flags = 0) noexcept;
        void beginDeviceGroupRenderPassAnnotated(const char *name, uint32_t color, uint32_t deviceMask,
            lent_ptr<const RenderPass> renderPass, lent_ptr<Framebuffer> framebuffer,
            const std::initializer_list<ClearValue>& clearValues = {}, const std::initializer_list<VkRect2D>& deviceRenderAreas = {},
            VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE) noexcept;
        #ifdef VK_KHR_imageless_framebuffer
        void beginDeviceGroupRenderPassAnnotated(const char *name, uint32_t color, uint32_t deviceMask, lent_ptr<const RenderPass> renderPass, lent_ptr<ImagelessFramebuffer> framebuffer,
            const std::vector<lent_ptr<ImageView>>& attachments, const std::initializer_list<ClearValue>& clearValues = {}, const std::initializer_list<VkRect2D>& deviceRenderAreas = {},
            VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE) noexcept;
        #endif // VK_KHR_imageless_framebuffer
    #endif // VK_KHR_device_group
    #ifdef VK_EXT_conditional_rendering
        void beginConditionalRenderingAnnotated(const char *name, uint32_t color, lent_ptr<const Buffer> buffer, VkDeviceSize offset = 0, bool inverted = false) noexcept;
    #endif // VK_EXT_conditional_rendering
    #ifdef VK_EXT_transform_feedback
        void beginTransformFeedbackAnnotated(const char *name, uint32_t color, uint32_t firstCounterBuffer, const std::initializer_list<lent_ptr<const Buffer>>& counterBuffers,
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
        const std::unique_ptr<Buffer>& getMarkerBuffer() const noexcept;
        const std::vector<const char *>& getCheckpoints() const noexcept { return checkpoints; }
        bool allowsReset() const noexcept { return VK_TRUE == resetCommandBuffer; }
        bool insideRenderPass() const noexcept { return renderingPass; }
        bool insideQuery() const noexcept { return nonIndexedQuery; }
        bool insideQueryIndexed() const noexcept { return indexedQuery; }
        bool insideConditionalRendering() const noexcept { return conditionalRendering; }
        bool insideTransformFeedback() const noexcept { return transformFeedback; }
        uint32_t inUseObjectCount() const noexcept;
        void releaseObjectsInUse() noexcept;

    protected:
        MAGMA_MAKE_SHARED(CommandBuffer)
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
            const VkBool32 NV_cluster_acceleration_structure: 1;
            const VkBool32 NV_device_diagnostic_checkpoints: 1;
            const VkBool32 NV_mesh_shader: 1;
            const VkBool32 EXT_debug_utils: 1;

            explicit Extensions(lent_ptr<const Device> device);
        };

        struct RenderPassState
        {
            const RenderPass *renderPass = nullptr;
            const Framebuffer *framebuffer = nullptr;
            std::vector<ImageView *> attachments;
        };

        VkDevice getNativeDevice() const noexcept { return leanCmd.device; }
        void queueSubmissionFinished() noexcept;
        void executionFinished() noexcept;
        void pushDebugMarker(const char* /* name */, uint32_t /* color */) noexcept MAGMA_NOOP;
        void popDebugMarker() noexcept MAGMA_NOOP;
        void insertDebugCheckpoint(const char *command, VkPipelineStageFlagBits pipelineStage) const noexcept;
        void putInitialState() noexcept;

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
        // Only arguments passed as std::shared_ptr<T> where T is
        // derived from DeviceChild can be stored in the in-use list.
        mutable std::vector<std::shared_ptr<const DeviceChild>> inUse;
    #endif // MAGMA_RETAIN_OBJECTS_IN_USE
        friend CommandPool;
        friend Fence;
        friend Queue;
    };

    /* See 6.1. Command Buffer Lifecycle
       Each command buffer is always in one of the following states: */

    enum class CommandBuffer::State : uint8_t
    {
        Initial, Recording, Executable, Pending, Invalid
    };
} // namespace magma

#include "commandBuffer.inl"
