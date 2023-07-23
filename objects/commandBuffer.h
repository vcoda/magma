/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2022 Victor Coda.

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
#include "device.h"
#include "dispatchable.h"
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
#include "../misc/extProcAddress.h"
#include "../misc/clearValue.h"
#include "../misc/clearAttachment.h"
#include "../misc/viewport.h"
#include "../misc/scissor.h"
#include "../helpers/stackArray.h"

namespace magma
{
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
    class Geometry;
    class AccelerationStructure;
    class AccelerationStructureCompactedSizeQuery;

    struct MemoryBarrier;
    struct BufferMemoryBarrier;
    class ImageMemoryBarrier;

    /* Command buffers are objects used to record commands which can be subsequently
       submitted to a device queue for execution. */

    class CommandBuffer : public Dispatchable<VkCommandBuffer>
    {
        friend CommandPool;
        friend Queue;

    public:
        enum class State : uint32_t
        {
            Initial, Recording, Executable, Pending, Invalid
        };

        ~CommandBuffer();
        bool begin(VkCommandBufferUsageFlags flags = 0) noexcept;
        bool beginInherited(const std::shared_ptr<RenderPass>& renderPass,
            uint32_t subpass,
            const std::shared_ptr<Framebuffer>& framebuffer,
            VkCommandBufferUsageFlags flags = 0) noexcept;
        void end();
        bool reset(bool releaseResources) noexcept;
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
    #endif
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
            const std::shared_ptr<PipelineLayout>& layout,
            uint32_t setIndex,
            const std::shared_ptr<DescriptorSet>& descriptorSet,
            uint32_t dynamicOffset = std::numeric_limits<uint32_t>::max()) noexcept;
        void bindDescriptorSet(VkPipelineBindPoint bindPoint,
            const std::shared_ptr<PipelineLayout>& layout,
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
            const VkOffset3D& srcOffset = VkOffset3D{0, 0, 0},
            const VkOffset3D& dstOffset = VkOffset3D{0, 0, 0}) const noexcept;

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
        template<typename Type>
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
        void resolveImage(const std::shared_ptr<Image>& srcImage,
            const std::shared_ptr<Image>& dstImage,
            const VkImageResolve& region) const noexcept;

        void setEvent(const std::shared_ptr<Event>& event,
            VkPipelineStageFlags stageMask) noexcept;
        void resetEvent(const std::shared_ptr<Event>& event,
            VkPipelineStageFlags stageMask) noexcept;
        void waitEvent(const std::shared_ptr<Event>& event,
            VkPipelineStageFlags srcStageMask,
            VkPipelineStageFlags dstStageMask,
            const std::vector<MemoryBarrier>& memoryBarriers = {},
            const std::vector<BufferMemoryBarrier>& bufferMemoryBarriers = {},
            const std::vector<ImageMemoryBarrier>& imageMemoryBarriers = {}) noexcept;
        void waitEvents(const std::vector<std::shared_ptr<Event>>& events,
            VkPipelineStageFlags srcStageMask,
            VkPipelineStageFlags dstStageMask,
            const std::vector<MemoryBarrier>& memoryBarriers = {},
            const std::vector<BufferMemoryBarrier>& bufferMemoryBarriers = {},
            const std::vector<ImageMemoryBarrier>& imageMemoryBarriers = {}) noexcept;

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
            const std::vector<MemoryBarrier>& memoryBarriers = {},
            const std::vector<BufferMemoryBarrier>& bufferMemoryBarriers = {},
            const std::vector<ImageMemoryBarrier>& imageMemoryBarriers = {},
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
        template<typename Type>
        void copyQueryResults(const std::shared_ptr<QueryPool>& queryPool,
            const std::shared_ptr<Buffer>& dstBuffer,
            bool wait,
            uint32_t firstQuery = 0,
            uint32_t queryCount = std::numeric_limits<uint32_t>::max(),
            VkDeviceSize dstOffset = 0) noexcept;
        template<typename Type>
        void copyQueryResultsWithAvailability(const std::shared_ptr<QueryPool>& queryPool,
            const std::shared_ptr<Buffer>& dstBuffer,
            uint32_t firstQuery = 0,
            uint32_t queryCount = std::numeric_limits<uint32_t>::max(),
            VkDeviceSize dstOffset = 0) noexcept;

        template<typename Type>
        void pushConstant(const std::shared_ptr<PipelineLayout>& layout,
            VkShaderStageFlags stageFlags,
            const Type& constant,
            uint32_t offset = 0) noexcept;
        template<typename Type, uint32_t pushConstantCount>
        void pushConstants(const std::shared_ptr<PipelineLayout>& layout,
            VkShaderStageFlags stageFlags,
            const Type(&constants)[pushConstantCount],
            uint32_t offset = 0) noexcept;
        template<typename Type>
        void pushConstants(const std::shared_ptr<PipelineLayout>& layout,
            VkShaderStageFlags stageFlags,
            const std::vector<Type>& constants,
            uint32_t offset = 0) noexcept;
        template<typename BlockType>
        void pushConstantBlock(const std::shared_ptr<PipelineLayout>& layout,
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
#   ifdef VK_KHR_imageless_framebuffer
        void beginDeviceGroupRenderPass(uint32_t deviceMask,
            const std::shared_ptr<RenderPass>& renderPass,
            const std::shared_ptr<ImagelessFramebuffer>& framebuffer,
            const std::vector<std::shared_ptr<ImageView>>& attachments,
            const std::vector<VkRect2D>& deviceRenderAreas = {},
            const std::vector<ClearValue>& clearValues = {},
            VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE) noexcept;
#   endif // VK_KHR_imageless_framebuffer
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

#ifdef VK_NV_ray_tracing
        void buildAccelerationStructure(const std::shared_ptr<Buffer>& instanceData,
            VkDeviceSize instanceOffset,
            bool update,
            const std::shared_ptr<AccelerationStructure>& dst,
            const std::shared_ptr<AccelerationStructure>& src,
            const std::shared_ptr<Buffer>& scratch,
            VkDeviceSize scratchOffset = 0) noexcept;
        void copyAccelerationStructure(const std::shared_ptr<AccelerationStructure>& dst,
            const std::shared_ptr<AccelerationStructure>& src,
            bool clone) const noexcept;
        void writeAccelerationStructureProperties(const std::shared_ptr<AccelerationStructure>& accelerationStructure,
            const std::shared_ptr<AccelerationStructureCompactedSizeQuery>& queryPool,
            uint32_t firstQuery) noexcept;
        void traceRays(const std::shared_ptr<Buffer>& raygenShaderBindingTableBuffer,
            VkDeviceSize raygenShaderBindingOffset,
            const std::shared_ptr<Buffer>& missShaderBindingTableBuffer,
            VkDeviceSize missShaderBindingOffset,
            VkDeviceSize missShaderBindingStride,
            const std::shared_ptr<Buffer>& hitShaderBindingTableBuffer,
            VkDeviceSize hitShaderBindingOffset,
            VkDeviceSize hitShaderBindingStride,
            const std::shared_ptr<Buffer>& callableShaderBindingTableBuffer,
            VkDeviceSize callableShaderBindingOffset,
            VkDeviceSize callableShaderBindingStride,
            uint32_t width,
            uint32_t height,
            uint32_t depth) noexcept;
#endif // VK_NV_ray_tracing

#ifdef VK_EXT_debug_marker
        void beginDebugMarker(const char *name,
            uint32_t color) noexcept;
        void endDebugMarker() noexcept;
        void insertDebugMarker(const char *name,
            uint32_t color) noexcept;
#endif // VK_EXT_debug_marker

#ifdef VK_EXT_debug_utils
        void beginDebugLabel(const char *name,
            uint32_t color) noexcept;
        void endDebugLabel() noexcept;
        void insertDebugLabel(
            const char *name,
            uint32_t color) noexcept;
#endif // VK_EXT_debug_utils

        std::shared_ptr<CommandPool> getCommandPool() const noexcept { return cmdPool; }
        std::shared_ptr<Fence> getFence() const noexcept { return fence; }
        bool primary() const noexcept { return VK_COMMAND_BUFFER_LEVEL_PRIMARY == level; }
        bool secondary() const noexcept { return VK_COMMAND_BUFFER_LEVEL_SECONDARY == level; }
        State getState() const noexcept { return state; }
        VkCommandBufferUsageFlags getUsageFlags() const noexcept { return usageFlags; }
        bool insideRenderPass() const noexcept { return withinRenderPass; }
        bool insideConditionalRendering() const noexcept { return withinConditionalRendering; }
        bool insideTransformFeedback() const noexcept { return withinTransformFeedback; }

        void enableOcclusionQuery(bool enable,
            VkQueryControlFlags queryFlags) noexcept;
        void enableConditionalRendering(bool enable) noexcept;
        void queryPipelineStatistics(VkQueryPipelineStatisticFlags pipelineStatistics) noexcept;

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
#   ifdef VK_KHR_imageless_framebuffer
        void beginDeviceGroupRenderPass(uint32_t deviceMask,
            const std::shared_ptr<RenderPass>& renderPass,
            const std::shared_ptr<ImagelessFramebuffer>& framebuffer,
            const std::vector<std::shared_ptr<ImageView>>& attachments,
            const std::vector<VkRect2D>& deviceRenderAreas,
            const std::vector<ClearValue>& clearValues,
            const char *renderPassName,
            uint32_t renderPassColor,
            VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE) noexcept;
#   endif // VK_KHR_imageless_framebuffer
#endif // VK_KHR_device_group

    protected:
        CommandBuffer(VkCommandBufferLevel level,
            std::shared_ptr<CommandPool> cmdPool);
        CommandBuffer(VkCommandBufferLevel level,
            VkCommandBuffer handle,
            std::shared_ptr<CommandPool> cmdPool);
        void onSubmit() noexcept;

        std::shared_ptr<CommandPool> cmdPool;
        std::shared_ptr<Fence> fence;
        const VkCommandBufferLevel level;
        VkCommandBufferUsageFlags usageFlags;
        std::atomic<State> state;
        VkBool32 occlusionQueryEnable : 1;
        VkBool32 conditionalRenderingEnable : 1;
        VkBool32 negativeViewportHeightEnabled : 1;
        VkBool32 withinRenderPass : 1;
        VkBool32 withinConditionalRendering : 1;
        VkBool32 withinTransformFeedback : 1;
        VkQueryControlFlags queryFlags;
        VkQueryPipelineStatisticFlags pipelineStatistics;
    };
} // namespace magma

#include "commandBuffer.inl"
