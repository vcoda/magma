/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2025 Victor Coda.

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
#include "model/dispatchable.h"
#include "descriptorSet.h"
#include "event.h"
#include "image.h"
#include "indexBuffer.h"
#include "pipeline.h"
#include "pipelineLayout.h"
#include "queryPool.h"
#include "clusterAccelerationStructure.h"
#include "../misc/extension.h"
#include "../allocator/allocator.h"

namespace magma
{
    class CommandPool;
    class Framebuffer;
#ifdef VK_KHR_imageless_framebuffer
    class ImagelessFramebuffer;
#endif
    class RenderPass;
    class ImageView;
#ifdef VK_KHR_acceleration_structure
    class AccelerationStructure;
    class BottomLevelAccelerationStructure;
    class TopLevelAccelerationStructure;
    struct AccelerationStructureInstances;
#endif
#ifdef VK_KHR_ray_tracing_pipeline
    class ShaderBindingTable;
#endif

    /* Lean-and-mean wrapper over VkCommandBuffer object to approach
       zero overhead (compared to native Vulkan API calls) when recording
       command buffer. Most of the class methods are inlined and take
       native C pointers to avoid the overhead of std::shared_ptr with
       atomic reference counting. It does not track the internal state
       of the command buffer nor retains bound objects until the command
       buffer execution completes. The user must ensure that bound objects
       remains alive during the command buffer submission and asynchronous
       execution by the command queue. The class lacks debug methods or
       useful getters to check the command buffer state. */

    class LeanCommandBuffer : public Dispatchable<VkCommandBuffer>
    {
    public:
        ~LeanCommandBuffer();
        VkResult begin(VkCommandBufferUsageFlags flags = 0) noexcept;
        VkResult beginInherited(const RenderPass *renderPass, uint32_t subpass, const Framebuffer *framebuffer,
            bool occlusionQueryEnable = false, VkQueryControlFlags queryFlags = 0, VkQueryPipelineStatisticFlags pipelineStatistics = 0,
            VkCommandBufferUsageFlags flags = 0, const StructureChain& extendedInfo = StructureChain()) noexcept;
        VkResult end() noexcept;
        VkResult reset(VkCommandBufferResetFlags flags = 0) noexcept;

        void bindPipeline(const Pipeline *pipeline) noexcept;

        void setViewport(const VkViewport& viewport) noexcept;
        void setViewports(uint32_t firstViewport, uint32_t viewportCount, const VkViewport *viewports) noexcept;
        void setScissor(const VkRect2D& scissor) noexcept;
        void setScissors(uint32_t firstScissor, uint32_t scissorCount, const VkRect2D *scissors) noexcept;
        void setLineWidth(float lineWidth) noexcept;
        void setDepthBias(float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) noexcept;
        void setBlendConstants(const float blendConstants[4]) noexcept;
        void setDepthBounds(float minDepthBounds, float maxDepthBounds) noexcept;
        void setStencilCompareMask(bool frontFace, bool backFace, uint32_t compareMask) noexcept;
        void setStencilWriteMask(bool frontFace, bool backFace, uint32_t writeMask) noexcept;
        void setStencilReference(bool frontFace, bool backFace, uint32_t reference) noexcept;

        void bindDescriptorSet(VkPipelineBindPoint bindPoint, const PipelineLayout *layout, uint32_t firstSet, const DescriptorSet *descriptorSet,
            uint32_t dynamicOffsetCount = 0, const uint32_t *dynamicOffsets = nullptr) noexcept;
        void bindDescriptorSets(VkPipelineBindPoint bindPoint, const PipelineLayout *layout, uint32_t firstSet, uint32_t descriptorSetCount, const DescriptorSet **descriptorSets,
            uint32_t dynamicOffsetCount = 0, const uint32_t *dynamicOffsets = nullptr) noexcept;
        void bindIndexBuffer(const BaseIndexBuffer *indexBuffer, VkDeviceSize offset = 0) noexcept;
        void bindVertexBuffer(uint32_t firstBinding, const Buffer *vertexBuffer, VkDeviceSize offset = 0) noexcept;
        void bindVertexBuffers(uint32_t firstBinding, uint32_t bindingCount, const Buffer **vertexBuffers, const VkDeviceSize *offsets) noexcept;

        void draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) const noexcept;
        void drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) const noexcept;
        void drawIndirect(const Buffer *buffer, uint32_t drawCount, VkDeviceSize offset = 0) const noexcept;
        void drawIndexedIndirect(const Buffer *buffer, uint32_t drawCount, VkDeviceSize offset = 0) const noexcept;
        void dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const noexcept;
        void dispatchIndirect(const Buffer *buffer, VkDeviceSize offset = 0) const noexcept;

        void copyBuffer(const Buffer *srcBuffer, Buffer *dstBuffer, const VkBufferCopy& region) const noexcept;
        void copyImage(const Image *srcImage, Image *dstImage, const VkImageCopy& region) const noexcept;
        void blitImage(const Image *srcImage, Image *dstImage, const VkImageBlit& region, VkFilter filter) const noexcept;
        void copyBufferToImage(const Buffer *srcBuffer, Image *dstImage, uint32_t regionCount, const VkBufferImageCopy *regions) const noexcept;
        void copyImageToBuffer(const Image *srcImage, Buffer *dstBuffer, uint32_t regionCount, const VkBufferImageCopy *regions) const noexcept;
        void updateBuffer(Buffer *buffer, VkDeviceSize dataSize, const void *data, VkDeviceSize offset = 0) const noexcept;
        void fillBuffer(Buffer *buffer, uint32_t data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) const noexcept;

        void clearColorImage(Image *image, const VkClearColorValue& color, const VkImageSubresourceRange& range) const noexcept;
        void clearDepthStencilImage(Image *image, const VkClearDepthStencilValue& depthStencil, const VkImageSubresourceRange& range) const noexcept;
        void clearAttachments(uint32_t attachmentCount, const VkClearAttachment *attachments, const VkClearRect& clearRect) const noexcept;
        void resolveImage(const Image *srcImage, Image *dstImage, const VkImageResolve& region) const noexcept;

        void setEvent(Event *event, VkPipelineStageFlags stageMask) const noexcept;
        void resetEvent(Event *event, VkPipelineStageFlags stageMask) const noexcept;
        void waitEvent(const Event *event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask) const noexcept;
        void waitEvent(const Event *event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const VkMemoryBarrier& barrier) const noexcept;
        void waitEvent(const Event *event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const VkBufferMemoryBarrier& barrier) const noexcept;
        void waitEvent(const Event *event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const VkImageMemoryBarrier& barrier) const noexcept;
        void waitEvent(const Event *event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier *memoryBarriers) const noexcept;
        void waitEvent(const Event *event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier *bufferMemoryBarriers) const noexcept;
        void waitEvent(const Event *event, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier *imageMemoryBarriers) const noexcept;
        void waitEvents(uint32_t eventCount, const Event **events, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier *memoryBarriers,
            uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier *bufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier *imageMemoryBarriers) const noexcept;

        void pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags = 0) noexcept;
        void pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const VkMemoryBarrier& barrier, VkDependencyFlags dependencyFlags = 0) noexcept;
        void pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const VkBufferMemoryBarrier& barrier, VkDependencyFlags dependencyFlags = 0) noexcept;
        void pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const VkImageMemoryBarrier& imageMemoryBarrier, VkDependencyFlags dependencyFlags = 0) noexcept;
        void pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier *memoryBarriers, VkDependencyFlags dependencyFlags = 0) noexcept;
        void pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier *bufferMemoryBarriers, VkDependencyFlags dependencyFlags = 0) noexcept;
        void pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier *imageMemoryBarriers, VkDependencyFlags dependencyFlags = 0) noexcept;
        void pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier *memoryBarriers, uint32_t bufferMemoryBarrierCount,
            const VkBufferMemoryBarrier *bufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier *imageMemoryBarriers, VkDependencyFlags dependencyFlags = 0) noexcept;

        void beginQuery(QueryPool *queryPool, uint32_t query) noexcept;
        void endQuery(QueryPool *queryPool, uint32_t query) noexcept;
        void resetQueryPool(QueryPool *queryPool, uint32_t  firstQuery, uint32_t queryCount) const noexcept;
        void writeTimestamp(VkPipelineStageFlagBits pipelineStage, QueryPool *queryPool, uint32_t query) const noexcept;
        void copyQueryPoolResults(const QueryPool *queryPool, uint32_t firstQuery, uint32_t queryCount, Buffer *dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags) const noexcept;

        void pushConstants(const PipelineLayout *layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void *values) noexcept;

        void beginRenderPass(const RenderPass *renderPass, Framebuffer *framebuffer, uint32_t clearValueCount, const VkClearValue *clearValues, const VkRect2D& renderArea,
            VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE) noexcept;
        void nextSubpass(VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE) noexcept;
        void endRenderPass() noexcept;

        void executeCommands(uint32_t cmdBufferCount, const LeanCommandBuffer **cmdBuffers) noexcept;

    #ifdef VK_KHR_device_group
        VkResult beginDeviceGroup(uint32_t deviceMask, VkCommandBufferUsageFlags flags = 0) noexcept;
        void beginDeviceGroupRenderPass(uint32_t deviceMask, const RenderPass *renderPass, Framebuffer *framebuffer, uint32_t clearValueCount, const VkClearValue *clearValues,
            uint32_t deviceRenderAreaCount = 0, const VkRect2D *deviceRenderAreas = nullptr, VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE) noexcept;
        void setDeviceMask(uint32_t deviceMask) noexcept;
        void dispatchBase(uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const noexcept;
    #endif // VK_KHR_device_group

    #ifdef VK_EXT_line_rasterization
        void setLineStipple(uint32_t stippleFactor, uint16_t stipplePattern) noexcept;
    #endif // VK_EXT_line_rasterization

    #ifdef VK_KHR_push_descriptor
        void pushDescriptorSet(VkPipelineBindPoint bindPoint, const PipelineLayout *layout, uint32_t set, const DescriptorSet *descriptorSet) noexcept;
    #endif // VK_KHR_push_descriptor

    #ifdef VK_EXT_multi_draw
        void drawMulti(uint32_t drawCount, const VkMultiDrawInfoEXT *vertexInfo, uint32_t instanceCount, uint32_t firstInstance) const noexcept;
        void drawMultiIndexed(uint32_t drawCount, const VkMultiDrawIndexedInfoEXT *indexInfo, uint32_t instanceCount, uint32_t firstInstance, const int32_t *vertexOffsets = nullptr) const noexcept;
    #endif // VK_EXT_multi_draw

    #if defined(VK_KHR_draw_indirect_count) || defined(VK_AMD_draw_indirect_count)
        void drawIndirectCount(const Buffer *buffer, const Buffer *countBuffer, uint32_t maxDrawCount, VkDeviceSize offset = 0, VkDeviceSize countBufferOffset = 0) const noexcept;
        void drawIndexedIndirectCount(const Buffer *buffer, const Buffer *countBuffer, uint32_t maxDrawCount, VkDeviceSize offset = 0, VkDeviceSize countBufferOffset = 0) const noexcept;
    #endif // VK_KHR_draw_indirect_count || VK_AMD_draw_indirect_count

    #if defined(VK_EXT_mesh_shader) || defined(VK_NV_mesh_shader)
        void drawMeshTasks(uint32_t groupCountX, uint32_t groupCountY = 1, uint32_t groupCountZ = 1) const noexcept;
        void drawMeshTasksIndirect(const Buffer *buffer, uint32_t drawCount, VkDeviceSize offset = 0) const noexcept;
        void drawMeshTasksIndirectCount(const Buffer *buffer, const Buffer *countBuffer, uint32_t maxDrawCount, VkDeviceSize offset = 0, VkDeviceSize countBufferOffset = 0) const noexcept;
    #endif // VK_EXT_mesh_shader || VK_NV_mesh_shader

    #ifdef VK_EXT_conditional_rendering
        void beginConditionalRendering(const Buffer *buffer, VkDeviceSize offset = 0, bool inverted = false) noexcept;
        void endConditionalRendering() noexcept;
    #endif // VK_EXT_conditional_rendering

    #ifdef VK_EXT_transform_feedback
        void bindTransformFeedbackBuffer(uint32_t firstBinding, Buffer *buffer, VkDeviceSize offset = 0, VkDeviceSize size = VK_WHOLE_SIZE) noexcept;
        void bindTransformFeedbackBuffers(uint32_t firstBinding, uint32_t bindingCount, Buffer **buffers, const VkDeviceSize *offsets, const VkDeviceSize *sizes = nullptr) noexcept;
        void beginTransformFeedback(uint32_t firstCounterBuffer, uint32_t counterBufferCount, const Buffer **counterBuffers, const VkDeviceSize *counterBufferOffsets = nullptr) noexcept;
        void endTransformFeedback(uint32_t firstCounterBuffer, uint32_t counterBufferCount, Buffer **counterBuffers, const VkDeviceSize *counterBufferOffsets = nullptr) noexcept;
        void beginQueryIndexed(QueryPool *queryPool, uint32_t query, uint32_t index) noexcept;
        void endQueryIndexed(QueryPool *queryPool, uint32_t query, uint32_t index) noexcept;
        void drawIndirectByteCount(uint32_t instanceCount, uint32_t firstInstance, const Buffer *counterBuffer, uint32_t vertexStride, VkDeviceSize counterBufferOffset = 0, uint32_t counterOffset = 0) const noexcept;
    #endif // VK_EXT_transform_feedback

    #ifdef VK_KHR_imageless_framebuffer
        void beginRenderPass(const RenderPass *renderPass, ImagelessFramebuffer *framebuffer, uint32_t attachmentCount, ImageView **attachments, uint32_t clearValueCount, const VkClearValue *clearValues,
            const VkRect2D& renderArea, VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE) noexcept;
    #ifdef VK_KHR_device_group
        void beginDeviceGroupRenderPass(uint32_t deviceMask, const RenderPass *renderPass, ImagelessFramebuffer *framebuffer, uint32_t attachmentCount, ImageView **attachments, uint32_t clearValueCount, const VkClearValue *clearValues,
            uint32_t deviceRenderAreaCount = 0, const VkRect2D *deviceRenderAreas = nullptr, VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE) noexcept;
    #endif // VK_KHR_device_group
    #endif // VK_KHR_imageless_framebuffer

    #ifdef VK_KHR_acceleration_structure
        void buildAccelerationStructure(BottomLevelAccelerationStructure *accelerationStructure, uint32_t geometryCount, const VkAccelerationStructureGeometryKHR **geometries,
            const VkAccelerationStructureBuildRangeInfoKHR *buildRanges, Buffer *scratchBuffer) const noexcept;
        void updateAccelerationStructure(BottomLevelAccelerationStructure *accelerationStructure, uint32_t geometryCount, const VkAccelerationStructureGeometryKHR **geometries,
            const VkAccelerationStructureBuildRangeInfoKHR *buildRanges, Buffer *scratchBuffer) const noexcept;
        void buildAccelerationStructure(TopLevelAccelerationStructure *accelerationStructure, const AccelerationStructureInstances& instances, Buffer *scratchBuffer) const noexcept;
        void updateAccelerationStructure(TopLevelAccelerationStructure *accelerationStructure, const AccelerationStructureInstances& instances, Buffer *scratchBuffer) const noexcept;
        void updateAccelerationStructureIndirect(AccelerationStructure *accelerationStructure, uint32_t geometryCount, const VkAccelerationStructureGeometryKHR **geometries,
            const uint32_t *maxPrimitiveCounts, const Buffer *indirectBuildRanges, Buffer *scratchBuffer) const noexcept;
        void copyAccelerationStructure(const AccelerationStructure *srcAccelerationStructure, AccelerationStructure *dstAccelerationStructure) const noexcept;
        void compactAccelerationStructure(const AccelerationStructure *srcAccelerationStructure, AccelerationStructure *dstAccelerationStructure) const noexcept;
        void copyAccelerationStructureToBuffer(const AccelerationStructure *srcAccelerationStructure, Buffer *dstBuffer) const noexcept;
        void copyBufferToAccelerationStructure(const Buffer *srcBuffer, AccelerationStructure *dstAccelerationStructure) const noexcept;
        void serializeAccelerationStructure(const AccelerationStructure *srcAccelerationStructure, Buffer *dstBuffer, VkDeviceAddress bufferOffset = 0) const noexcept;
        void deserializeAccelerationStructure(const Buffer *srcBuffer, AccelerationStructure *dstAccelerationStructure, VkDeviceAddress bufferOffset = 0) const noexcept;
        void writeAccelerationStructureProperties(const AccelerationStructure *accelerationStructure, QueryPool *queryPool, uint32_t firstQuery = 0) const noexcept;
    #endif // VK_KHR_acceleration_structure

    #ifdef VK_NV_cluster_acceleration_structure
        void moveObjects(ClusterAccelerationStructure *accelerationStructure, Buffer *scratchBuffer) const noexcept;
        void buildClustersBottomLevel(BottomLevelClusterAccelerationStructure *accelerationStructure, Buffer *scratchBuffer) const noexcept;
        void buildTriangleCluster(TriangleClusterAccelerationStructure *accelerationStructure, Buffer *scratchBuffer) const noexcept;
        void buildTriangleClusterTemplate(TriangleClusterAccelerationStructureTemplate *accelerationStructure, Buffer *scratchBuffer) const noexcept;
        void instantiateTriangleCluster(TriangleClusterAccelerationStructure *accelerationStructure, Buffer *scratchBuffer) const noexcept;
        void getClusterTemplateIndices(ClusterAccelerationStructure *accelerationStructure, Buffer *scratchBuffer) const noexcept;
    #endif // VK_NV_cluster_acceleration_structure

    #ifdef VK_KHR_ray_tracing_pipeline
        void setRayTracingPipelineStackSize(uint32_t pipelineStackSize) const noexcept;
        void traceRays(const ShaderBindingTable& shaderBindingTable, uint32_t width, uint32_t height, uint32_t depth = 1) const noexcept;
        void traceRaysIndirect(const ShaderBindingTable& shaderBindingTable, const Buffer *indirectBuffer) const noexcept;
    #endif // VK_KHR_ray_tracing_pipeline
    #ifdef VK_KHR_ray_tracing_maintenance1
        void traceRaysIndirect(const Buffer *indirectBuffer) const noexcept;
    #endif // VK_KHR_ray_tracing_maintenance1

    protected:
        LeanCommandBuffer(VkCommandBuffer handle, const CommandPool *cmdPool) noexcept;
        LeanCommandBuffer(VkCommandBufferLevel level, const CommandPool *cmdPool);
        VkDevice getNativeDevice() const noexcept { return device; }

    private:
    #ifdef VK_KHR_acceleration_structure
        void rebuildAccelerationStructure(VkBuildAccelerationStructureModeKHR mode, BottomLevelAccelerationStructure *accelerationStructure, uint32_t geometryCount,
            const VkAccelerationStructureGeometryKHR **geometries, const VkAccelerationStructureBuildRangeInfoKHR *buildRanges, Buffer *scratchBuffer) const noexcept;
        void rebuildAccelerationStructure(VkBuildAccelerationStructureModeKHR mode, TopLevelAccelerationStructure *accelerationStructure,
            const AccelerationStructureInstances& instances, Buffer *scratchBuffer) const noexcept;
    #endif // VK_KHR_acceleration_structure
    #ifdef VK_NV_cluster_acceleration_structure
        void buildClusterAccelerationStructureIndirect(VkClusterAccelerationStructureOpTypeNV opType,
            ClusterAccelerationStructure *clusterAccelerationStructure, Buffer *scratchBuffer) const noexcept;
    #endif // VK_NV_cluster_acceleration_structure

        VkDevice device;
        VkCommandPool cmdPool;
        friend CommandPool;
        friend CommandBuffer;
    };
} // namespace magma

#include "leanCommandBuffer.inl"
