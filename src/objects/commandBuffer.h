/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

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
#include "vertexBuffer.h"
#include "indexBuffer.h"
#include "indirectBuffer.h"
#include "transformFeedbackBuffer.h"
#include "image.h"
#include "pipeline.h"
#include "pipelineLayout.h"
#include "queryPool.h"
#include "event.h"
#include "../barriers/memoryBarrier.h"
#include "../barriers/bufferMemoryBarrier.h"
#include "../barriers/imageMemoryBarrier.h"
#include "../misc/extension.h"
#include "../misc/clearValue.h"
#include "../misc/clearAttachment.h"
#include "../misc/imageResolve.h"
#include "../misc/viewport.h"
#include "../misc/scissor.h"
#include "../helpers/stackArray.h"

namespace magma
{
    class Device;
    class CommandPool;
    class Queue;
    class Framebuffer;
#ifdef VK_KHR_imageless_framebuffer
    class ImagelessFramebuffer;
#endif
    class RenderPass;
    class Image;
    class ImageView;
    class Event;
    class Fence;
    class QueryPool;
#ifdef VK_KHR_acceleration_structure
    class AccelerationStructure;
    class BottomLevelAccelerationStructure;
    class TopLevelAccelerationStructure;
    class AccelerationStructureQuery;
    struct AccelerationStructureGeometry;
    struct AccelerationStructureGeometryInstances;
    struct AccelerationStructureBuildRange;
#endif
#ifdef VK_KHR_ray_tracing_pipeline
    class ShaderBindingTable;
#endif

    struct MemoryBarrier;
    struct BufferMemoryBarrier;
    struct ImageMemoryBarrier;

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

    class CommandBuffer : public Dispatchable<VkCommandBuffer>
    {
    public:
        enum class State : uint8_t;
        ~CommandBuffer();
        bool begin(VkCommandBufferUsageFlags flags = 0) noexcept;
        bool beginInherited(const std::shared_ptr<RenderPass>& renderPass,
            uint32_t subpass,
            const std::shared_ptr<Framebuffer>& framebuffer,
            VkCommandBufferUsageFlags flags = 0) noexcept;
        void end();
        bool reset(bool releaseResources = false) noexcept;
        void bindPipeline(const std::shared_ptr<Pipeline>& pipeline) noexcept;

        void setViewport(float x, float y,
            float width,
            float height,
            float minDepth = 0.f,
            float maxDepth = 1.f) noexcept;
        void setViewport(uint32_t x, uint32_t y,
            uint32_t width,
            int32_t height, // Viewport height can be negative (if supported)
            float minDepth = 0.f,
            float maxDepth = 1.f) noexcept;
        void setViewport(const VkViewport& viewport) noexcept;
        void setViewports(const std::initializer_list<Viewport>& viewports) noexcept;
        void setScissor(int32_t x, int32_t y,
            uint32_t width,
            uint32_t height) noexcept;
        void setScissor(const VkRect2D& scissor) noexcept;
        void setScissors(const std::initializer_list<VkRect2D>& scissors) noexcept;

        void setLineWidth(float lineWidth) noexcept;
    #ifdef VK_EXT_line_rasterization
        void setLineStipple(uint32_t stippleFactor,
            uint16_t stipplePattern) noexcept;
    #endif // VK_EXT_line_rasterization
        void setDepthBias(float depthBiasConstantFactor,
            float depthBiasClamp,
            float depthBiasSlopeFactor) noexcept;
        void setBlendConstants(const float blendConstants[4]) noexcept;
        void setDepthBounds(float minDepthBounds,
            float maxDepthBounds) noexcept;
        void setStencilCompareMask(bool frontFace,
            bool backFace,
            uint32_t compareMask) noexcept;
        void setStencilWriteMask(bool frontFace,
            bool backFace,
            uint32_t writeMask) noexcept;
        void setStencilReference(bool frontFace,
            bool backFace,
            uint32_t reference) noexcept;


        void bindDescriptorSet(VkPipelineBindPoint bindPoint,
            const PipelineLayout& layout,
            uint32_t setIndex,
            const std::shared_ptr<DescriptorSet>& descriptorSet,
            uint32_t dynamicOffset = std::numeric_limits<uint32_t>::max()) noexcept;
        void bindDescriptorSet(VkPipelineBindPoint bindPoint,
            const PipelineLayout& layout,
            uint32_t setIndex,
            const std::shared_ptr<DescriptorSet>& descriptorSet,
            const std::initializer_list<uint32_t>& dynamicOffsets) noexcept;


        void bindDescriptorSet(const std::shared_ptr<Pipeline>& pipeline,
            uint32_t setIndex,
            const std::shared_ptr<DescriptorSet>& descriptorSet,
            uint32_t dynamicOffset = std::numeric_limits<uint32_t>::max()) noexcept;
        void bindDescriptorSet(const std::shared_ptr<Pipeline>& pipeline,
            uint32_t setIndex,
            const std::shared_ptr<DescriptorSet>& descriptorSet,
            const std::initializer_list<uint32_t>& dynamicOffsets) noexcept;
        void bindDescriptorSets(const std::shared_ptr<Pipeline>& pipeline,
            uint32_t firstSet,
            const std::initializer_list<std::shared_ptr<DescriptorSet>>& descriptorSets,
            const std::initializer_list<uint32_t>& dynamicOffsets = {}) noexcept;
    #ifdef VK_KHR_push_descriptor
        void pushDescriptorSet(VkPipelineBindPoint bindPoint,
            const PipelineLayout& layout,
            uint32_t setIndex,
            const std::shared_ptr<DescriptorSet>& descriptorSet);
        void pushDescriptorSet(const std::shared_ptr<Pipeline>& pipeline,
            uint32_t setIndex,
            const std::shared_ptr<DescriptorSet>& descriptorSet);
    #endif // VK_KHR_push_descriptor

        void bindIndexBuffer(const std::shared_ptr<BaseIndexBuffer>& indexBuffer,
            VkDeviceSize offset = 0) noexcept;
        void bindVertexBuffer(uint32_t firstBinding,
            const std::shared_ptr<Buffer>& vertexBuffer,
            VkDeviceSize offset = 0) noexcept;
        void bindVertexBuffers(uint32_t firstBinding,
            const std::vector<std::shared_ptr<Buffer>>& vertexBuffers,
            std::vector<VkDeviceSize> offsets = {}) noexcept;
    #ifdef VK_EXT_transform_feedback
        void bindTransformFeedbackBuffer(uint32_t firstBinding,
            const std::shared_ptr<TransformFeedbackBuffer>& transformFeedbackBuffer,
            VkDeviceSize offset = 0,
            VkDeviceSize size = VK_WHOLE_SIZE);
        void bindTransformFeedbackBuffers(uint32_t firstBinding,
            const std::initializer_list<std::shared_ptr<TransformFeedbackBuffer>>& transformFeedbackBuffers,
            std::vector<VkDeviceSize> offsets = {},
            const std::initializer_list<VkDeviceSize>& sizes = {});
    #endif // VK_EXT_transform_feedback

        void draw(uint32_t vertexCount,
            uint32_t firstVertex = 0) const noexcept;
        void drawInstanced(uint32_t vertexCount,
            uint32_t instanceCount,
            uint32_t firstVertex = 0,
            uint32_t firstInstance = 0) const noexcept;
        void drawIndexed(uint32_t indexCount,
            uint32_t firstIndex = 0,
            int32_t vertexOffset = 0) const noexcept;
        void drawIndexedInstanced(uint32_t indexCount,
            uint32_t instanceCount,
            uint32_t firstIndex,
            int32_t vertexOffset,
            uint32_t firstInstance) const noexcept;
        void drawIndirect(const std::shared_ptr<DrawIndirectBuffer>& buffer,
            VkDeviceSize offset = 0) const noexcept;
        void drawIndexedIndirect(const std::shared_ptr<DrawIndexedIndirectBuffer>& buffer,
            VkDeviceSize offset = 0) const noexcept;
    #if defined(VK_KHR_draw_indirect_count) || defined(VK_AMD_draw_indirect_count)
        void drawIndirectCount(const std::shared_ptr<DrawIndirectBuffer>& buffer,
            const std::shared_ptr<Buffer>& countBuffer,
            VkDeviceSize offset = 0,
            VkDeviceSize countBufferOffset = 0) const noexcept;
        void drawIndexedIndirectCount(const std::shared_ptr<DrawIndexedIndirectBuffer>& buffer,
            const std::shared_ptr<Buffer>& countBuffer,
            VkDeviceSize offset = 0,
            VkDeviceSize countBufferOffset = 0) const noexcept;
    #endif // VK_KHR_draw_indirect_count || VK_AMD_draw_indirect_count
    #ifdef VK_EXT_multi_draw
        void drawMulti(const std::vector<VkMultiDrawInfoEXT>& vertexInfo) const noexcept;
        void drawMultiInstanced(const std::vector<VkMultiDrawInfoEXT>& vertexInfo,
            uint32_t instanceCount,
            uint32_t firstInstance) const noexcept;
        void drawMultiIndexed(const std::vector<VkMultiDrawIndexedInfoEXT>& indexInfo,
            const std::vector<int32_t>& vertexOffsets = {}) const noexcept;
        void drawMultiIndexedInstanced(const std::vector<VkMultiDrawIndexedInfoEXT>& indexInfo,
            uint32_t instanceCount,
            uint32_t firstInstance,
            const std::vector<int32_t>& vertexOffsets = {}) const noexcept;
    #endif // VK_EXT_multi_draw
    #ifdef VK_EXT_transform_feedback
        void drawIndirectByteCount(uint32_t instanceCount,
            uint32_t firstInstance,
            const std::shared_ptr<TransformFeedbackCounterBuffer>& counterBuffer,
            VkDeviceSize counterBufferOffset,
            uint32_t counterOffset,
            uint32_t vertexStride) const noexcept;
    #endif // VK_EXT_transform_feedback
    #if defined(VK_EXT_mesh_shader) || defined(VK_NV_mesh_shader)
        void drawMeshTasks(uint32_t groupCountX,
            uint32_t groupCountY = 1,
            uint32_t groupCountZ = 1) const noexcept;
        void drawMeshTasksIndirect(const std::shared_ptr<DrawMeshTasksIndirectBuffer>& buffer,
            VkDeviceSize offset = 0) const noexcept;
        void drawMeshTasksIndirectCount(const std::shared_ptr<DrawMeshTasksIndirectBuffer>& buffer,
            const std::shared_ptr<Buffer>& countBuffer,
            VkDeviceSize offset = 0,
            VkDeviceSize countBufferOffset = 0) const noexcept;
    #endif // VK_EXT_mesh_shader || VK_NV_mesh_shader
        void dispatch(uint32_t groupCountX,
            uint32_t groupCountY,
            uint32_t groupCountZ) const noexcept;
        void dispatchIndirect(const std::shared_ptr<Buffer>& buffer,
            VkDeviceSize offset = 0) const noexcept;

        void copyBuffer(const std::shared_ptr<const Buffer>& srcBuffer,
            const std::shared_ptr<Buffer>& dstBuffer,
            VkDeviceSize srcOffset = 0,
            VkDeviceSize dstOffset = 0,
            VkDeviceSize size = VK_WHOLE_SIZE) const noexcept;
        void copyBuffer(const std::shared_ptr<const Buffer>& srcBuffer,
            const std::shared_ptr<Buffer>& dstBuffer,
            const VkBufferCopy& region) const noexcept;

        void copyImage(const std::shared_ptr<const Image>& srcImage,
            const std::shared_ptr<Image>& dstImage,
            const VkImageCopy& region) const noexcept;
        void copyImage(const std::shared_ptr<const Image>& srcImage,
            const std::shared_ptr<Image>& dstImage,
            uint32_t mipLevel = 0,
            const VkOffset3D& srcOffset = VkOffset3D{0, 0, 0},
            const VkOffset3D& dstOffset = VkOffset3D{0, 0, 0}) const noexcept;

        void blitImage(const std::shared_ptr<const Image>& srcImage,
            const std::shared_ptr<Image>& dstImage,
            const VkImageBlit& region,
            VkFilter filter) const noexcept;
        void blitImage(const std::shared_ptr<const Image>& srcImage,
            const std::shared_ptr<Image>& dstImage,
            VkFilter filter,
            uint32_t mipLevel = 0,
            const VkOffset2D& srcOffset = VkOffset2D{0, 0},
            const VkOffset2D& dstOffset = VkOffset2D{0, 0}) const noexcept;

        void copyBufferToImage(const std::shared_ptr<const Buffer>& srcBuffer,
            const std::shared_ptr<Image>& dstImage,
            const VkBufferImageCopy& region) const noexcept;
        void copyBufferToImage(const std::shared_ptr<const Buffer>& srcBuffer,
            const std::shared_ptr<Image>& dstImage,
            const std::vector<VkBufferImageCopy>& regions) const noexcept;
        void copyImageToBuffer(const std::shared_ptr<const Image>& srcImage,
            const std::shared_ptr<Buffer>& dstBuffer,
            const VkBufferImageCopy& region) const noexcept;
        void copyImageToBuffer(const std::shared_ptr<const Image>& srcImage,
            const std::shared_ptr<Buffer>& dstBuffer,
            const std::vector<VkBufferImageCopy>& regions) const noexcept;

        void updateBuffer(const std::shared_ptr<Buffer>& buffer,
            VkDeviceSize dataSize,
            const void *data,
            VkDeviceSize offset = 0) const noexcept;
        template<class Type>
        void updateBuffer(const std::shared_ptr<Buffer>& buffer,
            const std::vector<Type>& data,
            VkDeviceSize offset = 0) const noexcept;
        void fillBuffer(const std::shared_ptr<Buffer>& buffer,
            uint32_t value,
            VkDeviceSize size = VK_WHOLE_SIZE,
            VkDeviceSize offset = 0) const noexcept;

        void clearColorImage(const std::shared_ptr<Image>& image,
            const ClearColor& color,
            const VkImageSubresourceRange& range) const noexcept;
        void clearDepthStencilImage(const std::shared_ptr<Image>& image,
            const ClearDepthStencil& depthStencil,
            const VkImageSubresourceRange& range) const noexcept;
        void clearAttachments(const std::initializer_list<ClearAttachment>& attachments,
            const VkClearRect& clearRect) const noexcept;
        void resolveImage(const std::shared_ptr<const Image>& srcImage,
            const std::shared_ptr<Image>& dstImage) const noexcept;
        void resolveImage(const std::shared_ptr<const Image>& srcImage,
            const std::shared_ptr<Image>& dstImage,
            const VkImageResolve& region) const noexcept;

        void setEvent(std::shared_ptr<Event>& event,
            VkPipelineStageFlags stageMask) const noexcept;
        void resetEvent(std::shared_ptr<Event>& event,
            VkPipelineStageFlags stageMask) const noexcept;
        void waitEvent(const std::shared_ptr<Event>& event,
            VkPipelineStageFlags srcStageMask,
            VkPipelineStageFlags dstStageMask) const noexcept;
        void waitEvent(const std::shared_ptr<Event>& event,
            VkPipelineStageFlags srcStageMask,
            VkPipelineStageFlags dstStageMask,
            const MemoryBarrier& barrier) const noexcept;
        void waitEvent(const std::shared_ptr<Event>& event,
            VkPipelineStageFlags srcStageMask,
            VkPipelineStageFlags dstStageMask,
            const BufferMemoryBarrier& barrier) const noexcept;
        void waitEvent(const std::shared_ptr<Event>& event,
            VkPipelineStageFlags srcStageMask,
            VkPipelineStageFlags dstStageMask,
            const ImageMemoryBarrier& barrier) const noexcept;
        void waitEvent(const std::shared_ptr<Event>& event,
            VkPipelineStageFlags srcStageMask,
            VkPipelineStageFlags dstStageMask,
            const std::initializer_list<MemoryBarrier>& barriers) const noexcept;
        void waitEvent(const std::shared_ptr<Event>& event,
            VkPipelineStageFlags srcStageMask,
            VkPipelineStageFlags dstStageMask,
            const std::initializer_list<BufferMemoryBarrier>& barriers) const noexcept;
        void waitEvent(const std::shared_ptr<Event>& event,
            VkPipelineStageFlags srcStageMask,
            VkPipelineStageFlags dstStageMask,
            const std::initializer_list<ImageMemoryBarrier>& barriers) const noexcept;
        void waitEvents(const std::vector<std::shared_ptr<Event>>& events,
            VkPipelineStageFlags srcStageMask,
            VkPipelineStageFlags dstStageMask,
            const std::vector<MemoryBarrier>& memoryBarriers = {},
            const std::vector<BufferMemoryBarrier>& bufferMemoryBarriers = {},
            const std::vector<ImageMemoryBarrier>& imageMemoryBarriers = {}) const noexcept;

        void pipelineBarrier(VkPipelineStageFlags srcStageMask,
            VkPipelineStageFlags dstStageMask,
            VkDependencyFlags dependencyFlags = 0) noexcept;
        void pipelineBarrier(VkPipelineStageFlags srcStageMask,
            VkPipelineStageFlags dstStageMask,
            const MemoryBarrier& barrier,
            VkDependencyFlags dependencyFlags = 0) noexcept;
        void pipelineBarrier(VkPipelineStageFlags srcStageMask,
            VkPipelineStageFlags dstStageMask,
            const BufferMemoryBarrier& barrier,
            VkDependencyFlags dependencyFlags = 0) noexcept;
        void pipelineBarrier(VkPipelineStageFlags srcStageMask,
            VkPipelineStageFlags dstStageMask,
            const ImageMemoryBarrier& imageMemoryBarrier,
            VkDependencyFlags dependencyFlags = 0) noexcept;
        void pipelineBarrier(VkPipelineStageFlags srcStageMask,
            VkPipelineStageFlags dstStageMask,
            const std::initializer_list<MemoryBarrier>& barriers,
            VkDependencyFlags dependencyFlags = 0) noexcept;
        void pipelineBarrier(VkPipelineStageFlags srcStageMask,
            VkPipelineStageFlags dstStageMask,
            const std::initializer_list<BufferMemoryBarrier>& barriers,
            VkDependencyFlags dependencyFlags = 0) noexcept;
        void pipelineBarrier(VkPipelineStageFlags srcStageMask,
            VkPipelineStageFlags dstStageMask,
            const std::initializer_list<ImageMemoryBarrier>& barriers,
            VkDependencyFlags dependencyFlags = 0) noexcept;
        void pipelineBarrier(VkPipelineStageFlags srcStageMask,
            VkPipelineStageFlags dstStageMask,
            const std::vector<MemoryBarrier>& memoryBarriers = {},
            const std::vector<BufferMemoryBarrier>& bufferMemoryBarriers = {},
            const std::vector<ImageMemoryBarrier>& imageMemoryBarriers = {},
            VkDependencyFlags dependencyFlags = 0) noexcept;
        void batchPipelineBarrier(VkPipelineStageFlags srcStageMask,
            VkPipelineStageFlags dstStageMask,
            const MemoryBarrier& barrier,
            VkDependencyFlags dependencyFlags = 0) noexcept;
        void batchPipelineBarrier(VkPipelineStageFlags srcStageMask,
            VkPipelineStageFlags dstStageMask,
            const BufferMemoryBarrier& barrier,
            VkDependencyFlags dependencyFlags = 0) noexcept;
        void batchPipelineBarrier(VkPipelineStageFlags srcStageMask,
            VkPipelineStageFlags dstStageMask,
            const ImageMemoryBarrier& imageMemoryBarrier,
            VkDependencyFlags dependencyFlags = 0) noexcept;

        void beginQuery(const std::shared_ptr<QueryPool>& queryPool,
            uint32_t queryIndex) noexcept;
        void endQuery(const std::shared_ptr<QueryPool>& queryPool,
            uint32_t queryIndex) noexcept;
    #ifdef VK_EXT_transform_feedback
        void beginQueryIndexed(const std::shared_ptr<QueryPool>& queryPool,
            uint32_t queryIndex,
            uint32_t vertexStream) noexcept;
        void endQueryIndexed(const std::shared_ptr<QueryPool>& queryPool,
            uint32_t queryIndex,
            uint32_t vertexStream) noexcept;
    #endif // VK_EXT_transform_feedback
        void resetQueryPool(const std::shared_ptr<QueryPool>& queryPool,
            uint32_t firstQuery,
            uint32_t queryCount) noexcept;
        void writeTimestamp(VkPipelineStageFlagBits pipelineStage,
            const std::shared_ptr<QueryPool>& queryPool,
            uint32_t queryIndex) noexcept;
        template<class Type>
        void copyQueryResults(const std::shared_ptr<QueryPool>& queryPool,
            const std::shared_ptr<Buffer>& dstBuffer,
            bool wait,
            uint32_t firstQuery = 0,
            uint32_t queryCount = std::numeric_limits<uint32_t>::max(),
            VkDeviceSize dstOffset = 0) noexcept;
        template<class Type>
        void copyQueryResultsWithAvailability(const std::shared_ptr<QueryPool>& queryPool,
            const std::shared_ptr<Buffer>& dstBuffer,
            uint32_t firstQuery = 0,
            uint32_t queryCount = std::numeric_limits<uint32_t>::max(),
            VkDeviceSize dstOffset = 0) noexcept;

        template<class Type>
        void pushConstant(const PipelineLayout& layout,
            VkShaderStageFlags stageFlags,
            const Type& constant,
            uint32_t offset = 0) noexcept;
        template<class Type, uint32_t PushConstantCount>
        void pushConstants(const PipelineLayout& layout,
            VkShaderStageFlags stageFlags,
            const Type(&constants)[PushConstantCount],
            uint32_t offset = 0) noexcept;
        template<class Type>
        void pushConstants(const PipelineLayout& layout,
            VkShaderStageFlags stageFlags,
            const std::vector<Type>& constants,
            uint32_t offset = 0) noexcept;
        template<class BlockType>
        void pushConstantBlock(const PipelineLayout& layout,
            VkShaderStageFlags stageFlags,
            const BlockType& block,
            uint32_t offset = 0) noexcept;

        void beginRenderPass(const std::shared_ptr<RenderPass>& renderPass,
            const std::shared_ptr<Framebuffer>& framebuffer,
            const std::vector<ClearValue>& clearValues = {},
            const VkRect2D& renderArea = {0, 0, 0, 0},
            VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE) noexcept;
    #ifdef VK_KHR_imageless_framebuffer
        void beginRenderPass(const std::shared_ptr<RenderPass>& renderPass,
            const std::shared_ptr<ImagelessFramebuffer>& framebuffer,
            const std::vector<std::shared_ptr<ImageView>>& attachments,
            const std::vector<ClearValue>& clearValues = {},
            const VkRect2D& renderArea = {0, 0, 0, 0},
            VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE) noexcept;
    #endif // VK_KHR_imageless_framebuffer
        void nextSubpass(VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE) noexcept;
        void endRenderPass() noexcept;

    #ifdef VK_KHR_device_group
        void setDeviceMask(uint32_t deviceMask) noexcept;
        void dispatchBase(uint32_t baseGroupX,
            uint32_t baseGroupY,
            uint32_t baseGroupZ,
            uint32_t groupCountX,
            uint32_t groupCountY,
            uint32_t groupCountZ) const noexcept;
        bool beginDeviceGroup(uint32_t deviceMask,
            VkCommandBufferUsageFlags flags = 0) noexcept;
        void beginDeviceGroupRenderPass(uint32_t deviceMask,
            const std::shared_ptr<RenderPass>& renderPass,
            const std::shared_ptr<Framebuffer>& framebuffer,
            const std::vector<VkRect2D>& deviceRenderAreas = {},
            const std::vector<ClearValue>& clearValues = {},
            VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE) noexcept;
    #ifdef VK_KHR_imageless_framebuffer
        void beginDeviceGroupRenderPass(uint32_t deviceMask,
            const std::shared_ptr<RenderPass>& renderPass,
            const std::shared_ptr<ImagelessFramebuffer>& framebuffer,
            const std::vector<std::shared_ptr<ImageView>>& attachments,
            const std::vector<VkRect2D>& deviceRenderAreas = {},
            const std::vector<ClearValue>& clearValues = {},
            VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE) noexcept;
    #endif // VK_KHR_imageless_framebuffer
    #endif // VK_KHR_device_group

    #ifdef VK_EXT_conditional_rendering
        void beginConditionalRendering(const std::shared_ptr<Buffer>& buffer,
            VkDeviceSize offset = 0,
            bool inverted = false) noexcept;
        void endConditionalRendering() noexcept;
    #endif // VK_EXT_conditional_rendering

    #ifdef VK_EXT_transform_feedback
        void beginTransformFeedback() noexcept;
        void endTransformFeedback() noexcept;
        void beginTransformFeedback(uint32_t firstCounterBuffer,
            const std::initializer_list<std::shared_ptr<TransformFeedbackCounterBuffer>>& counterBuffers,
            const std::initializer_list<VkDeviceSize>& counterBufferOffsets = {}) noexcept;
        void endTransformFeedback(uint32_t firstCounterBuffer,
            const std::initializer_list<std::shared_ptr<TransformFeedbackCounterBuffer>>& counterBuffers,
            const std::initializer_list<VkDeviceSize>& counterBufferOffsets = {}) noexcept;
    #endif // VK_EXT_transform_feedback

    #ifdef VK_KHR_acceleration_structure
        // BLAS
        void buildAccelerationStructure(const std::shared_ptr<BottomLevelAccelerationStructure>& accelerationStructure,
            const std::list<AccelerationStructureGeometry>& geometries,
            const std::shared_ptr<Buffer>& scratchBuffer) noexcept;
        void buildAccelerationStructure(const std::shared_ptr<BottomLevelAccelerationStructure>& accelerationStructure,
            const std::list<AccelerationStructureGeometry>& geometries,
            const std::vector<VkAccelerationStructureBuildRangeInfoKHR>& buildRanges,
            const std::shared_ptr<Buffer>& scratchBuffer) noexcept;
        void updateAccelerationStructure(const std::shared_ptr<BottomLevelAccelerationStructure>& accelerationStructure,
            const std::list<AccelerationStructureGeometry>& geometries,
            const std::shared_ptr<Buffer>& scratchBuffer) noexcept;
        void updateAccelerationStructure(const std::shared_ptr<BottomLevelAccelerationStructure>& accelerationStructure,
            const std::list<AccelerationStructureGeometry>& geometries,
            const std::vector<VkAccelerationStructureBuildRangeInfoKHR>& buildRanges,
            const std::shared_ptr<Buffer>& scratchBuffer) noexcept;
        // TLAS
        void buildAccelerationStructure(const std::shared_ptr<TopLevelAccelerationStructure>& accelerationStructure,
            const AccelerationStructureGeometryInstances& instances,
            const std::shared_ptr<Buffer>& scratchBuffer) noexcept;
        void updateAccelerationStructure(const std::shared_ptr<TopLevelAccelerationStructure>& accelerationStructure,
            const AccelerationStructureGeometryInstances& instances,
            const std::shared_ptr<Buffer>& scratchBuffer) noexcept;
        void updateAccelerationStructureIndirect(const std::shared_ptr<AccelerationStructure>& accelerationStructure,
            const std::list<AccelerationStructureGeometry>& geometries,
            const std::shared_ptr<Buffer>& indirectBuildRanges,
            const std::shared_ptr<Buffer>& scratchBuffer,
            uint32_t indirectStride = sizeof(VkAccelerationStructureBuildRangeInfoKHR)) noexcept;
        void copyAccelerationStructure(const std::shared_ptr<const AccelerationStructure>& srcAccelerationStructure,
            const std::shared_ptr<AccelerationStructure>& dstAccelerationStructure);
        void compactAccelerationStructure(const std::shared_ptr<const AccelerationStructure>& srcAccelerationStructure,
            const std::shared_ptr<AccelerationStructure>& dstAccelerationStructure);
        void copyAccelerationStructureToBuffer(const std::shared_ptr<const AccelerationStructure>& srcAccelerationStructure,
            const std::shared_ptr<Buffer>& dstBuffer);
        void copyBufferToAccelerationStructure(const std::shared_ptr<const Buffer>& srcBuffer,
            const std::shared_ptr<AccelerationStructure>& dstAccelerationStructure);
        void serializeAccelerationStructure(const std::shared_ptr<const AccelerationStructure>& accelerationStructure,
            const std::shared_ptr<Buffer>& buffer,
            VkDeviceAddress bufferOffset = 0);
        void deserializeAccelerationStructure(const std::shared_ptr<const Buffer>& buffer,
            const std::shared_ptr<AccelerationStructure>& accelerationStructure,
            VkDeviceAddress bufferOffset = 0);
        void writeAccelerationStructureProperties(const std::shared_ptr<const AccelerationStructure>& accelerationStructure,
            const std::shared_ptr<QueryPool>& queryPool,
            uint32_t firstQuery = 0);
    #endif // VK_KHR_acceleration_structure

    #ifdef VK_KHR_ray_tracing_pipeline
        void setRayTracingPipelineStackSize(uint32_t pipelineStackSize) const noexcept;
        void traceRays(const ShaderBindingTable& shaderBindingTable,
            uint32_t width,
            uint32_t height,
            uint32_t depth) const noexcept;
        void traceRaysIndirect(const ShaderBindingTable& shaderBindingTable,
            const std::shared_ptr<const Buffer>& indirectBuffer) const noexcept;
    #endif // VK_KHR_ray_tracing_pipeline
    #ifdef VK_KHR_ray_tracing_maintenance1
        void traceRaysIndirect(const std::shared_ptr<Buffer>& indirectTraceRaysBuffer) const noexcept;
    #endif

    #ifdef VK_EXT_debug_marker
        void beginDebugMarker(const char *MAGMA_NOOP(name),
            uint32_t MAGMA_NOOP(color)) noexcept MAGMA_NOOP_METHOD;
        void endDebugMarker() noexcept MAGMA_NOOP_METHOD;
        void insertDebugMarker(const char *MAGMA_NOOP(name),
            uint32_t MAGMA_NOOP(color)) noexcept MAGMA_NOOP_METHOD;
    #endif // VK_EXT_debug_marker
    #ifdef VK_EXT_debug_utils
        void beginDebugLabel(const char *MAGMA_NOOP(name),
            uint32_t MAGMA_NOOP(color)) noexcept MAGMA_NOOP_METHOD;
        void endDebugLabel() noexcept MAGMA_NOOP_METHOD;
        void insertDebugLabel(const char *MAGMA_NOOP(name),
            uint32_t MAGMA_NOOP(color)) noexcept MAGMA_NOOP_METHOD;
    #endif // VK_EXT_debug_utils

        void writeBufferMarker(VkPipelineStageFlagBits MAGMA_NOOP(pipelineStage),
            uint32_t MAGMA_NOOP(marker)) MAGMA_NOOP_METHOD;
        const std::shared_ptr<Buffer>& getMarkerBuffer() const noexcept { return markerBuffer; }
        VkDeviceSize getMarkerOffset() const noexcept { return markerOffset; }
    #ifdef VK_NV_device_diagnostic_checkpoints
        void setCheckpoint(const char *MAGMA_NOOP(name)) noexcept MAGMA_NOOP_METHOD;
    #endif

        const std::shared_ptr<Device>& getDevice() const noexcept { return device; }
        const std::unique_ptr<Fence>& getFence() const noexcept { return fence; }
        bool primary() const noexcept { return VK_COMMAND_BUFFER_LEVEL_PRIMARY == level; }
        bool secondary() const noexcept { return VK_COMMAND_BUFFER_LEVEL_SECONDARY == level; }
        uint32_t getQueueFamilyIndex() const noexcept { return queueFamilyIndex; }
        State getState() const noexcept { return state; }
        VkCommandBufferUsageFlags getUsage() const noexcept { return usage; }
        bool allowsReset() const noexcept { return VK_TRUE == resetCommandBuffer; }
        bool insideRenderPass() const noexcept { return inRenderPass; }
        bool insideConditionalRendering() const noexcept { return inConditionalRendering; }
        bool insideTransformFeedback() const noexcept { return inTransformFeedback; }
        void enableOcclusionQuery(bool enable,
            VkQueryControlFlags queryFlags) noexcept;
        void enableConditionalRendering(bool enable) noexcept;
        void queryPipelineStatistics(VkQueryPipelineStatisticFlags pipelineStatistics) noexcept;
        uint32_t inUseObjectCount() const noexcept;
        void releaseObjectsInUse() noexcept;

        // Debug stuff
        bool begin(const char *blockName,
            uint32_t blockColor,
            VkCommandBufferUsageFlags flags = 0) noexcept;
        bool beginInherited(const std::shared_ptr<RenderPass>& renderPass,
            uint32_t subpass,
            const std::shared_ptr<Framebuffer>& framebuffer,
            const char *blockName,
            uint32_t blockColor,
            VkCommandBufferUsageFlags flags = 0) noexcept;
        void beginRenderPass(const std::shared_ptr<RenderPass>& renderPass,
            const std::shared_ptr<Framebuffer>& framebuffer,
            const std::vector<ClearValue>& clearValues,
            const char *renderPassName,
            uint32_t renderPassColor,
            const VkRect2D& renderArea = {0, 0, 0, 0},
            VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE) noexcept;
    #ifdef VK_KHR_imageless_framebuffer
        void beginRenderPass(const std::shared_ptr<RenderPass>& renderPass,
            const std::shared_ptr<ImagelessFramebuffer>& framebuffer,
            const std::vector<std::shared_ptr<ImageView>>& attachments,
            const std::vector<ClearValue>& clearValues,
            const char *renderPassName,
            uint32_t renderPassColor,
            const VkRect2D& renderArea = {0, 0, 0, 0},
            VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE) noexcept;
    #endif // VK_KHR_imageless_framebuffer

    #ifdef VK_KHR_device_group
        bool beginDeviceGroup(uint32_t deviceMask,
            const char *blockName,
            uint32_t blockColor,
            VkCommandBufferUsageFlags flags = 0) noexcept;
        void beginDeviceGroupRenderPass(uint32_t deviceMask,
            const std::shared_ptr<RenderPass>& renderPass,
            const std::shared_ptr<Framebuffer>& framebuffer,
            const std::vector<VkRect2D>& deviceRenderAreas,
            const std::vector<ClearValue>& clearValues,
            const char *renderPassName,
            uint32_t renderPassColor,
            VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE) noexcept;
    #ifdef VK_KHR_imageless_framebuffer
        void beginDeviceGroupRenderPass(uint32_t deviceMask,
            const std::shared_ptr<RenderPass>& renderPass,
            const std::shared_ptr<ImagelessFramebuffer>& framebuffer,
            const std::vector<std::shared_ptr<ImageView>>& attachments,
            const std::vector<VkRect2D>& deviceRenderAreas,
            const std::vector<ClearValue>& clearValues,
            const char *renderPassName,
            uint32_t renderPassColor,
            VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE) noexcept;
    #endif // VK_KHR_imageless_framebuffer
    #endif // VK_KHR_device_group

    protected:
        CommandBuffer(VkCommandBufferLevel level,
            VkCommandBuffer handle,
            const CommandPool *cmdPool);
        CommandBuffer(VkCommandBufferLevel level,
            const CommandPool *cmdPool);
        void queueSubmissionFinished() noexcept;
        void executionFinished() noexcept;
        friend CommandPool;
        friend Queue;

    private:
        struct RenderPassState
        {
            std::shared_ptr<RenderPass> renderPass;
            std::shared_ptr<Framebuffer> framebuffer;
            std::vector<std::shared_ptr<ImageView>> attachments;
        };

        struct PipelineBarrierBatch
        {
            VkPipelineStageFlags srcStageMask;
            VkPipelineStageFlags dstStageMask;
            VkDependencyFlags dependencyFlags;
            std::vector<MemoryBarrier> memoryBarriers;
            std::vector<BufferMemoryBarrier> bufferMemoryBarriers;
            std::vector<ImageMemoryBarrier> imageMemoryBarriers;
        };

        VkDevice getNativeDevice() const noexcept;
        void changeImageMipLayouts(const ImageMemoryBarrier& barrier) const noexcept;
        PipelineBarrierBatch *lookupBarrierBatch(VkPipelineStageFlags srcStageMask,
            VkPipelineStageFlags dstStageMask,
            VkDependencyFlags dependencyFlags) noexcept;
    #ifdef VK_KHR_acceleration_structure
        void rebuildAccelerationStructure(VkBuildAccelerationStructureModeKHR mode,
            const std::shared_ptr<BottomLevelAccelerationStructure>& accelerationStructure,
            const std::list<AccelerationStructureGeometry>& geometries,
            const std::vector<VkAccelerationStructureBuildRangeInfoKHR>& buildRanges,
            const std::shared_ptr<Buffer>& scratchBuffer);
        void rebuildAccelerationStructure(VkBuildAccelerationStructureModeKHR mode,
            const std::shared_ptr<TopLevelAccelerationStructure>& accelerationStructure,
            const AccelerationStructureGeometryInstances& instances,
            const std::shared_ptr<Buffer>& scratchBuffer);
    #endif // VK_KHR_acceleration_structure

        const VkCommandBufferLevel level;
        const VkCommandPool cmdPool;
        const uint32_t queueFamilyIndex;
        std::shared_ptr<Device> device;
        std::unique_ptr<Fence> fence;
        VkCommandBufferUsageFlags usage;
        std::atomic<State> state;
        const VkBool32 resetCommandBuffer : 1;
        VkBool32 debugMarkerEnabled : 1;
        VkBool32 debugUtilsEnabled : 1;
        VkBool32 occlusionQueryEnable : 1;
        VkBool32 conditionalRenderingEnable : 1;
        VkBool32 negativeViewportHeightEnabled : 1;
        VkBool32 inRenderPass : 1;
        VkBool32 inConditionalRendering : 1;
        VkBool32 inTransformFeedback : 1;
        VkBool32 labeledRecording : 1;
        VkBool32 labeledRenderPass : 1;
        VkQueryControlFlags queryFlags;
        VkQueryPipelineStatisticFlags pipelineStatistics;
        RenderPassState renderPassState;
        std::shared_ptr<Buffer> markerBuffer;
        std::atomic<VkDeviceSize> markerOffset;
        mutable std::unordered_map<hash_t, PipelineBarrierBatch> pipelineBarriers;
        // User may optionally compile library with MAGMA_RETAIN_OBJECTS_IN_USE
        // define to allow deferred release of resources bound to command buffer.
    #ifdef MAGMA_RETAIN_OBJECTS_IN_USE
        mutable std::unordered_set<std::shared_ptr<const DeviceChild>> inUse;
    #endif
    };

    /* See 6.1. Command Buffer Lifecycle
       Each command buffer is always in one of the following states: */

    enum class CommandBuffer::State : uint8_t
    {
        Initial, Recording, Executable, Pending, Invalid
    };
} // namespace magma

#include "commandBuffer.inl"
