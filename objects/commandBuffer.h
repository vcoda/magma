/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
#include "dispatchable.h"
#include "descriptorSet.h"
#include "vertexBuffer.h"
#include "indexBuffer.h"
#include "pipeline.h"
#include "pipelineLayout.h"
#include "../misc/clearValue.h"
#include "../misc/clearAttachment.h"
#include "../misc/viewport.h"
#include "../misc/scissor.h"
#include "../helpers/stackArray.h"

namespace magma
{
    class CommandPool;
    class Framebuffer;
    class RenderPass;
    class Image;
    class Event;
    class Fence;
    class QueryPool;
    class Geometry;
    class AccelerationStructure;
    class AccelerationStructureCompactedSizeQuery;

    struct MemoryBarrier;
    struct BufferMemoryBarrier;
    class ImageMemoryBarrier;

    namespace memory
    {
        class LinearPlacementPool;
    }

    /* Command buffers are objects used to record commands which can be subsequently
       submitted to a device queue for execution. */

    class CommandBuffer : public Dispatchable<VkCommandBuffer>
    {
    protected:
        friend CommandPool;
        explicit CommandBuffer(
            VkCommandBufferLevel level,
            std::shared_ptr<CommandPool> pool);
        explicit CommandBuffer(
            VkCommandBufferLevel level,
            VkCommandBuffer handle,
            std::shared_ptr<CommandPool> pool);

    public:
        ~CommandBuffer();
        bool begin(VkCommandBufferUsageFlags flags = 0) noexcept;
        bool beginInherited(
            const std::shared_ptr<RenderPass>& renderPass,
            uint32_t subpass,
            const std::shared_ptr<Framebuffer>& framebuffer,
            VkCommandBufferUsageFlags flags = 0) noexcept;
        void end();
        bool reset(bool releaseResources) noexcept;
        void bindPipeline(const std::shared_ptr<Pipeline>& pipeline) noexcept;
        void setViewport(
            float x, float y,
            float width, float height,
            float minDepth = 0.f,
            float maxDepth = 1.f) noexcept;
        void setViewport(
            uint32_t x, uint32_t y,
            uint32_t width,
            int32_t height, // Viewport height can be negative (if supported)
            float minDepth = 0.f,
            float maxDepth = 1.f) noexcept;
        void setViewport(const Viewport& viewport) noexcept;
        void setViewports(const std::initializer_list<Viewport>& viewports) noexcept;
        void setScissor(
            int32_t x, int32_t y,
            uint32_t width, uint32_t height) noexcept;
        void setScissor(const Scissor& scissor) noexcept;
        void setScissors(const std::initializer_list<Scissor>& scissors) noexcept;

        void setLineWidth(float lineWidth) noexcept;
        void setDepthBias(
            float depthBiasConstantFactor,
            float depthBiasClamp,
            float depthBiasSlopeFactor) noexcept;
        void setBlendConstants(const float blendConstants[4]) noexcept;
        void setDepthBounds(
            float minDepthBounds,
            float maxDepthBounds) noexcept;
        void setStencilCompareMask(
            bool frontFace,
            bool backFace,
            uint32_t compareMask) noexcept;
        void setStencilWriteMask(
            bool frontFace,
            bool backFace,
            uint32_t writeMask) noexcept;
        void setStencilReference(
            bool frontFace,
            bool backFace,
            uint32_t reference) noexcept;

        void bindDescriptorSet(
            const std::shared_ptr<Pipeline>& pipeline,
            const std::shared_ptr<DescriptorSet>& descriptorSet) noexcept;
        void bindDescriptorSet(
            const std::shared_ptr<Pipeline>& pipeline,
            const std::shared_ptr<DescriptorSet>& descriptorSet,
            uint32_t dynamicOffset) noexcept;
        void bindDescriptorSets(
            const std::shared_ptr<Pipeline>& pipeline,
            const std::initializer_list<std::shared_ptr<DescriptorSet>>& descriptorSets,
            const std::initializer_list<uint32_t>& dynamicOffsets = {}) noexcept;
        template<uint32_t descriptorSetCount>
        void bindDescriptorSets(
            const std::shared_ptr<Pipeline>& pipeline,
            const std::shared_ptr<magma::DescriptorSet>(&descriptorSets)[descriptorSetCount],
            const std::initializer_list<uint32_t>& dynamicOffsets = {}) noexcept;

        void bindIndexBuffer(
            const std::shared_ptr<BaseIndexBuffer>& indexBuffer,
            VkDeviceSize offset = 0) noexcept;
        void bindVertexBuffer(
            uint32_t firstBinding,
            const std::shared_ptr<BaseVertexBuffer>& vertexBuffer,
            VkDeviceSize offset = 0) noexcept;
        void bindVertexBuffers(
            uint32_t firstBinding,
            const std::vector<std::shared_ptr<BaseVertexBuffer>>& vertexBuffers,
            std::vector<VkDeviceSize> offsets = {}) noexcept;

        void draw(
            uint32_t vertexCount,
            uint32_t firstVertex = 0) const noexcept;
        void drawInstanced(
            uint32_t vertexCount,
            uint32_t instanceCount,
            uint32_t firstVertex = 0,
            uint32_t firstInstance = 0) const noexcept;
        void drawIndexed(
            uint32_t indexCount,
            uint32_t firstIndex = 0,
            int32_t vertexOffset = 0) const noexcept;
        void drawIndexedInstanced(
            uint32_t indexCount,
            uint32_t instanceCount,
            uint32_t firstIndex,
            int32_t vertexOffset,
            uint32_t firstInstance) const noexcept;
        void drawIndirect(
            const std::shared_ptr<Buffer>& buffer,
            VkDeviceSize offset,
            uint32_t drawCount,
            uint32_t stride) const noexcept;
        void drawIndexedIndirect(
            const std::shared_ptr<Buffer>& buffer,
            VkDeviceSize offset,
            uint32_t drawCount,
            uint32_t stride) const noexcept;
        void dispatch(
            uint32_t groupCountX,
            uint32_t groupCountY,
            uint32_t groupCountZ) const noexcept;
        void dispatchIndirect(
            const std::shared_ptr<Buffer>& buffer,
            VkDeviceSize offset) const noexcept;

        void copyBuffer(
            const std::shared_ptr<Buffer>& srcBuffer,
            const std::shared_ptr<Buffer>& dstBuffer,
            VkDeviceSize srcOffset = 0,
            VkDeviceSize dstOffset = 0,
            VkDeviceSize size = VK_WHOLE_SIZE) const noexcept;
        void copyBuffer(
            const std::shared_ptr<Buffer>& srcBuffer,
            const std::shared_ptr<Buffer>& dstBuffer,
            const VkBufferCopy& region) const noexcept;

        void copyImage(
            const std::shared_ptr<Image>& srcImage,
            const std::shared_ptr<Image>& dstImage,
            const VkImageCopy& region) const noexcept;
        void blitImage(
            const std::shared_ptr<Image>& srcImage,
            const std::shared_ptr<Image>& dstImage,
            const VkImageBlit& region,
            VkFilter filter) const noexcept;

        void copyBufferToImage(
            const std::shared_ptr<Buffer>& srcBuffer,
            const std::shared_ptr<Image>& dstImage,
            const VkBufferImageCopy& region) const noexcept;
        void copyBufferToImage(
            const std::shared_ptr<Buffer>& srcBuffer,
            const std::shared_ptr<Image>& dstImage,
            const std::vector<VkBufferImageCopy>& regions) const noexcept;
        void copyImageToBuffer(
            const std::shared_ptr<Image>& srcImage,
            const std::shared_ptr<Buffer>& dstBuffer,
            const VkBufferImageCopy& region) const noexcept;
        void copyImageToBuffer(
            const std::shared_ptr<Image>& srcImage,
            const std::shared_ptr<Buffer>& dstBuffer,
            const std::vector<VkBufferImageCopy>& regions) const noexcept;

        void updateBuffer(
            const std::shared_ptr<Buffer>& buffer,
            VkDeviceSize dataSize,
            const void *data,
            VkDeviceSize offset = 0) const noexcept;
        template<typename Type>
        void updateBuffer(
            const std::shared_ptr<Buffer>& buffer,
            const std::vector<Type>& data,
            VkDeviceSize offset = 0) const noexcept;
        void fillBuffer(
            const std::shared_ptr<Buffer>& buffer,
            uint32_t value,
            VkDeviceSize size = 0,
            VkDeviceSize offset = 0) const noexcept;

        void clearColorImage(
            const std::shared_ptr<Image>& image,
            const ClearColor& color,
            const VkImageSubresourceRange& range) const noexcept;
        void clearDepthStencilImage(
            const std::shared_ptr<Image>& image,
            const ClearDepthStencil& depthStencil,
            const VkImageSubresourceRange& range) const noexcept;
        void clearAttachments(
            const std::initializer_list<ClearAttachment>& attachments,
            const VkClearRect& clearRect) const noexcept;
        void resolveImage(
            const std::shared_ptr<Image>& srcImage,
            const std::shared_ptr<Image>& dstImage,
            const VkImageResolve& region) const noexcept;

        void setEvent(
            const std::shared_ptr<Event>& event,
            VkPipelineStageFlags stageMask) noexcept;
        void resetEvent(
            const std::shared_ptr<Event>& event,
            VkPipelineStageFlags stageMask) noexcept;
        void waitEvent(
            const std::shared_ptr<Event>& event,
            VkPipelineStageFlags srcStageMask,
            VkPipelineStageFlags dstStageMask,
            const std::vector<MemoryBarrier>& memoryBarriers = {},
            const std::vector<BufferMemoryBarrier>& bufferMemoryBarriers = {},
            const std::vector<ImageMemoryBarrier>& imageMemoryBarriers = {}) noexcept;
        void waitEvents(
            const std::vector<std::shared_ptr<Event>>& events,
            VkPipelineStageFlags srcStageMask,
            VkPipelineStageFlags dstStageMask,
            const std::vector<MemoryBarrier>& memoryBarriers = {},
            const std::vector<BufferMemoryBarrier>& bufferMemoryBarriers = {},
            const std::vector<ImageMemoryBarrier>& imageMemoryBarriers = {}) noexcept;

        void pipelineBarrier(
            VkPipelineStageFlags srcStageMask,
            VkPipelineStageFlags dstStageMask,
            const MemoryBarrier& barrier,
            VkDependencyFlags dependencyFlags = 0) noexcept;
        void pipelineBarrier(
            VkPipelineStageFlags srcStageMask,
            VkPipelineStageFlags dstStageMask,
            const BufferMemoryBarrier& barrier,
            VkDependencyFlags dependencyFlags = 0) noexcept;
        void pipelineBarrier(
            VkPipelineStageFlags srcStageMask,
            VkPipelineStageFlags dstStageMask,
            const ImageMemoryBarrier& imageMemoryBarrier,
            VkDependencyFlags dependencyFlags = 0) noexcept;
        void pipelineBarrier(
            VkPipelineStageFlags srcStageMask,
            VkPipelineStageFlags dstStageMask,
            const std::vector<MemoryBarrier>& memoryBarriers = {},
            const std::vector<BufferMemoryBarrier>& bufferMemoryBarriers = {},
            const std::vector<ImageMemoryBarrier>& imageMemoryBarriers = {},
            VkDependencyFlags dependencyFlags = 0) noexcept;

        void beginQuery(
            const std::shared_ptr<QueryPool>& queryPool,
            uint32_t queryIndex,
            bool precise) noexcept;
        void endQuery(
            const std::shared_ptr<QueryPool>& queryPool,
            uint32_t queryIndex) noexcept;
        void resetQueryPool(const std::shared_ptr<QueryPool>& queryPool) noexcept;
        void writeTimestamp(
            VkPipelineStageFlagBits pipelineStage,
            const std::shared_ptr<QueryPool>& queryPool,
            uint32_t queryIndex) noexcept;
        void copyQueryResults(
            const std::shared_ptr<QueryPool>& queryPool,
            const std::shared_ptr<Buffer>& buffer,
            bool wait,
            uint32_t firstQuery = 0,
            uint32_t queryCount = std::numeric_limits<uint32_t>::max(),
            VkDeviceSize dstOffset = 0,
            bool write64Bit = true) noexcept;

        template<typename Type, uint32_t pushConstantCount>
        void pushConstants(
            const std::shared_ptr<PipelineLayout>& layout,
            VkShaderStageFlags stageFlags,
            const Type(&values)[pushConstantCount],
            uint32_t offset = 0) noexcept;
        template<typename Type>
        void pushConstants(
            const std::shared_ptr<PipelineLayout>& layout,
            VkShaderStageFlags stageFlags,
            const std::vector<Type>& values,
            uint32_t offset = 0) noexcept;
        template<typename BlockStructure>
        void pushConstantBlock(
            const std::shared_ptr<PipelineLayout>& layout,
            VkShaderStageFlags stageFlags,
            const BlockStructure& block,
            uint32_t offset = 0) noexcept;

        void beginRenderPass(
            const std::shared_ptr<RenderPass>& renderPass,
            const std::shared_ptr<Framebuffer>& framebuffer,
            const std::vector<ClearValue>& clearValues = {},
            const VkRect2D& renderArea = {0, 0, 0, 0},
            VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE) noexcept;
        void nextSubpass(VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE) noexcept;
        void endRenderPass() noexcept;

        void executeCommands(const std::vector<std::shared_ptr<CommandBuffer>>& commandBuffers) noexcept;

#ifdef VK_KHR_device_group
        bool beginDeviceGroup(
            uint32_t deviceMask,
            VkCommandBufferUsageFlags flags = 0) noexcept;
        void beginDeviceGroupRenderPass(
            uint32_t deviceMask,
            const std::shared_ptr<RenderPass>& renderPass,
            const std::shared_ptr<Framebuffer>& framebuffer,
            const std::vector<VkRect2D>& deviceRenderAreas = {},
            const std::vector<ClearValue>& clearValues = {},
            VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE) noexcept;
        void setDeviceMask(
            uint32_t deviceMask) noexcept;
        void dispatchBase(
            uint32_t baseGroupX,
            uint32_t baseGroupY,
            uint32_t baseGroupZ,
            uint32_t groupCountX,
            uint32_t groupCountY,
            uint32_t groupCountZ) const noexcept;
#endif // VK_KHR_device_group

#ifdef VK_EXT_conditional_rendering
        void beginConditionalRendering(
            const std::shared_ptr<Buffer>& buffer,
            VkDeviceSize offset = 0,
            bool inverted = false) noexcept;
        void endConditionalRendering() noexcept;
#endif // VK_EXT_conditional_rendering

#ifdef VK_NV_ray_tracing
        void buildAccelerationStructure(const std::shared_ptr<Buffer>& instanceData,
            VkDeviceSize instanceOffset,
            bool update,
            const std::shared_ptr<AccelerationStructure>& dst,
            const std::shared_ptr<AccelerationStructure>& src,
            const std::shared_ptr<Buffer>& scratch,
            VkDeviceSize scratchOffset = 0) noexcept;
        void copyAccelerationStructure(
            const std::shared_ptr<AccelerationStructure>& dst,
            const std::shared_ptr<AccelerationStructure>& src,
            bool clone) const noexcept;
        void writeAccelerationStructureProperties(
            const std::shared_ptr<AccelerationStructure>& accelerationStructure,
            const std::shared_ptr<AccelerationStructureCompactedSizeQuery>& queryPool,
            uint32_t firstQuery) noexcept;
        void traceRays(
            const std::shared_ptr<Buffer>& raygenShaderBindingTableBuffer,
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
        void beginDebugMarker(
            const char *name,
            uint32_t color) noexcept;
        void endDebugMarker() noexcept;
        void insertDebugMarker(
            const char *name,
            uint32_t color) noexcept;
#endif // VK_EXT_debug_marker

#ifdef VK_EXT_debug_utils
        void beginDebugLabel(
            const char *name,
            uint32_t color) noexcept;
        void endDebugLabel() noexcept;
        void insertDebugLabel(
            const char *name,
            uint32_t color) noexcept;
#endif // VK_EXT_debug_utils

        // Non-API utility methods
        bool primary() const noexcept { return VK_COMMAND_BUFFER_LEVEL_PRIMARY == level; }
        std::shared_ptr<CommandPool> getPool() const noexcept { return pool; }
        std::shared_ptr<Fence> getFence() const noexcept;
        uint32_t getDeviceMask() const noexcept { return deviceMask; }

        void enableOcclusionQuery(
            bool enable,
            VkQueryControlFlags queryFlags) noexcept;
        void enableConditionalRendering(bool enable) noexcept;
        void queryPipelineStatistics(VkQueryPipelineStatisticFlags pipelineStatistics) noexcept;

        // Debug stuff
        bool begin(const char *blockName,
            uint32_t blockColor,
            VkCommandBufferUsageFlags flags = 0) noexcept;
        bool beginInherited(
            const std::shared_ptr<RenderPass>& renderPass,
            uint32_t subpass,
            const std::shared_ptr<Framebuffer>& framebuffer,
            const char *blockName,
            uint32_t blockColor,
            VkCommandBufferUsageFlags flags = 0) noexcept;
        void beginRenderPass(
            const std::shared_ptr<RenderPass>& renderPass,
            const std::shared_ptr<Framebuffer>& framebuffer,
            const std::vector<ClearValue>& clearValues,
            const char *renderPassName,
            uint32_t renderPassColor,
            const VkRect2D& renderArea = {0, 0, 0, 0},
            VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE) noexcept;

#ifdef VK_KHR_device_group
        bool beginDeviceGroup(
            uint32_t deviceMask,
            const char *blockName,
            uint32_t blockColor,
            VkCommandBufferUsageFlags flags = 0) noexcept;
        void beginDeviceGroupRenderPass(
            uint32_t deviceMask,
            const std::shared_ptr<RenderPass>& renderPass,
            const std::shared_ptr<Framebuffer>& framebuffer,
            const std::vector<VkRect2D>& deviceRenderAreas,
            const std::vector<ClearValue>& clearValues,
            const char *renderPassName,
            uint32_t renderPassColor,
            VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE) noexcept;
#endif // VK_KHR_device_group

    protected:
        VkCommandBufferLevel level;
        std::shared_ptr<CommandPool> pool;
        std::shared_ptr<Fence> fence;
        uint32_t deviceMask = 0xFFFFFFFF;
        VkBool32 occlusionQueryEnable = VK_FALSE;
        VkBool32 conditionalRenderingEnable = VK_FALSE;
        VkQueryControlFlags queryFlags = 0;
        VkQueryPipelineStatisticFlags pipelineStatistics = 0;
#ifdef MAGMA_DEBUG_LABEL
        VkBool32 beginMarked = VK_FALSE;
        VkBool32 beginRenderPassMarked = VK_FALSE;
#endif
    };

    /* Primary command buffer, which can execute secondary command buffers,
       and which is submitted to the queue. */

    class PrimaryCommandBuffer : public CommandBuffer
    {
        friend CommandPool;
        friend memory::LinearPlacementPool;
        explicit PrimaryCommandBuffer(VkCommandBuffer handle,
            std::shared_ptr<CommandPool> pool):
            CommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, handle, std::move(pool)) {}

    public:
        explicit PrimaryCommandBuffer(std::shared_ptr<CommandPool> pool):
            CommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, std::move(pool)) {}
    };

    /* Secondary command buffer, which can be executed by primary command buffers,
       and which is not directly submitted to the queue. */

    class SecondaryCommandBuffer : public CommandBuffer
    {
        friend CommandPool;
        friend memory::LinearPlacementPool;
        explicit SecondaryCommandBuffer(VkCommandBuffer handle,
            std::shared_ptr<CommandPool> pool):
            CommandBuffer(VK_COMMAND_BUFFER_LEVEL_SECONDARY, handle, std::move(pool)) {}

    public:
        explicit SecondaryCommandBuffer(std::shared_ptr<CommandPool> pool):
            CommandBuffer(VK_COMMAND_BUFFER_LEVEL_SECONDARY, std::move(pool)) {}
    };
} // namespace magma

#include "commandBuffer.inl"
