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
#pragma once
#include <limits>
#include <vector>
#include "handle.h"
#include "../shared.h"

namespace magma
{
    class Buffer;
    class ComputePipeline;
    class DescriptorSet;
    class Device;
    class Event;
    class Framebuffer;
    class GraphicsPipeline;
    class IndexBuffer;
    class Image;
    class PipelineLayout;
    class QueryPool;
    class RenderPass;
    class VertexBuffer;

    struct BufferMemoryBarrier;
    struct Viewport;
    struct Scissor;
    struct ClearValue;
    struct ColorClear;
    struct DepthStencilClear;
    struct ClearAttachment;
    struct ImageMemoryBarrier;

    // Methods order follows Vulkan API order
    class CommandBuffer : public Dispatchable<VkCommandBuffer>
    {
        CommandBuffer(VkCommandBuffer handle, std::shared_ptr<const Device> device);

    public:
        bool begin(VkCommandBufferUsageFlags flags = 0) noexcept;
        bool beginInherited(
            const std::shared_ptr<RenderPass>& renderPass, 
            uint32_t subpass, 
            const std::shared_ptr<Framebuffer>& framebuffer,
            VkCommandBufferUsageFlags flags = 0) noexcept;
        void end();
        bool reset(bool releaseResources) noexcept;

        void bindPipeline(const std::shared_ptr<GraphicsPipeline>& pipeline) noexcept;
        void bindPipeline(const std::shared_ptr<ComputePipeline>& pipeline) noexcept;

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
            const std::shared_ptr<PipelineLayout>& pipelineLayout,
            const std::shared_ptr<DescriptorSet>& descriptorSet,
            VkPipelineBindPoint pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS) noexcept;
        void bindDescriptorSet(
            const std::shared_ptr<PipelineLayout>& pipelineLayout,
            const std::shared_ptr<DescriptorSet>& descriptorSet,
            uint32_t dynamic0ffset,
            VkPipelineBindPoint pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS) noexcept;
        template <uint32_t descriptorSetCount> void bindDescriptorSets(
            const std::shared_ptr<PipelineLayout>& pipelineLayout,
            const std::shared_ptr<magma::DescriptorSet>(&descriptorSets)[descriptorSetCount],
            const std::initializer_list<uint32_t>& offsets = {},
            VkPipelineBindPoint pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS) noexcept;
        void bindDescriptorSets(
            const std::shared_ptr<PipelineLayout>& pipelineLayout,
            const std::initializer_list<std::shared_ptr<DescriptorSet>>& descriptorSets,
            const std::initializer_list<uint32_t>& offsets = {},
            VkPipelineBindPoint pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS) noexcept;

        void bindIndexBuffer(
            const std::shared_ptr<IndexBuffer>& indexBuffer,
            VkDeviceSize offset = 0) noexcept;
        void bindVertexBuffer(
            uint32_t firstBinding,
            const std::shared_ptr<VertexBuffer>& vertexBuffer,
            VkDeviceSize offset = 0) noexcept;
        void bindVertexBuffers(
            uint32_t firstBinding,
            const std::vector<std::shared_ptr<VertexBuffer>>& vertexBuffers,
            const std::vector<VkDeviceSize>& offsets) noexcept;

        void draw(
            uint32_t vertexCount,
            uint32_t firstVertex) const noexcept;
        void drawInstanced(
            uint32_t vertexCount,
            uint32_t instanceCount,
            uint32_t firstVertex,
            uint32_t firstInstance) const noexcept;
        void drawIndexed(
            uint32_t indexCount,
            uint32_t firstIndex,
            int32_t vertexOffset) const noexcept;
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
            uint32_t x,
            uint32_t y,
            uint32_t z) const noexcept;
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
            VkImageLayout dstImageLayout,
            const VkBufferImageCopy& region) const noexcept;
        void copyImageToBuffer(
            const std::shared_ptr<Image>& srcImage,
            const std::shared_ptr<Buffer>& dstBuffer, 
            const VkBufferImageCopy& region) const noexcept;

        template<typename Type>
        void updateBuffer(
            const std::shared_ptr<Buffer>& buffer, 
            const std::vector<Type>& data,
            VkDeviceSize offset = 0) const noexcept;
        template<typename Type>
        void fillBuffer(const std::shared_ptr<Buffer>& dstBuffer,
            const std::vector<Type>& data,
            VkDeviceSize offset = 0) const noexcept;  

        void clearColorImage(
            const std::shared_ptr<Image>& image, 
            const ColorClear& color, 
            const VkImageSubresourceRange& range) const noexcept;
        void clearDepthStencilImage(
            const std::shared_ptr<Image>& image, 
            const DepthStencilClear& depthStencil, 
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
            std::shared_ptr<Event>&event); 
        void waitEvents(
            std::vector<std::shared_ptr<Event>>& events);

        void pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::shared_ptr<Buffer>& buffer, const BufferMemoryBarrier& barrier) noexcept;
        void pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const std::vector<std::shared_ptr<Buffer>>& buffers, const BufferMemoryBarrier& barrier) noexcept;
        void pipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, const ImageMemoryBarrier& barrier) noexcept;
        
        void beginQuery(
            const std::shared_ptr<QueryPool>& queryPool,
            uint32_t queryIndex,
            bool precise) noexcept;
        void endQuery(
            const std::shared_ptr<QueryPool>& queryPool,
            uint32_t queryIndex) noexcept;
        void resetQueryPool(const std::shared_ptr<QueryPool>& queryPool) noexcept;
        void copyQueryResults(
            const std::shared_ptr<QueryPool>& queryPool,
            const std::shared_ptr<Buffer>& buffer,
            bool wait,
            uint32_t firstQuery = 0,
            uint32_t queryCount = std::numeric_limits<uint32_t>::max(),
            VkDeviceSize dstOffset = 0,
            bool write64Bit = true) noexcept;
        void writeTimestamp() noexcept; // TODO: implement

        template<typename Type, uint32_t pushConstantCount> void pushConstants(
            const std::shared_ptr<PipelineLayout>& layout,
            VkShaderStageFlags stageFlags, 
            const Type(&values)[pushConstantCount],
            uint32_t offset = 0) noexcept;
        template<typename Type> void pushConstants(
            const std::shared_ptr<PipelineLayout>& layout,
            VkShaderStageFlags stageFlags, 
            const std::vector<Type>& values,
            uint32_t offset = 0) noexcept;
        template<typename Type> void pushConstantBlock(
            const std::shared_ptr<PipelineLayout>& layout,
            VkShaderStageFlags stageFlags,
            const Type& block,
            uint32_t offset = 0) noexcept;

        void beginRenderPass(
            const std::shared_ptr<RenderPass>& renderPass,
            const std::shared_ptr<Framebuffer>& framebuffer,
            const ClearValue& clearValue,
            VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE) noexcept;
        void beginRenderPass(
            const std::shared_ptr<RenderPass>& renderPass,
            const std::shared_ptr<Framebuffer>& framebuffer,
            const std::initializer_list<ClearValue>& clearValues,
            VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE) noexcept;
        void nextSubpass() noexcept; // TODO: implement
        void endRenderPass() noexcept;

        void executeCommands(const std::vector<std::shared_ptr<CommandBuffer>>& commandBuffers) noexcept;

        // EXT_debug_marker
        void beginDebugMarker(
            const char *name, 
            const float color[4]) noexcept;
        void endDebugMarker() noexcept;
        void insertDebugMarker(const char *name) noexcept;
        
        // Non-API utility methods
        void setRenderArea(const VkRect2D& rc) noexcept;
        void setRenderArea(
            int32_t x, int32_t y, 
            const VkExtent2D& extent) noexcept;
        void setRenderArea(
            int32_t x, int32_t y, 
            uint32_t width, uint32_t height) noexcept;

        void enableOcclusionQuery(
            bool enable, 
            VkQueryControlFlags queryFlags) noexcept;
        void queryPipelineStatistics(VkQueryPipelineStatisticFlags pipelineStatistics) noexcept;

    private:
		VkRect2D renderArea = {{0, 0}, {0, 0}};
        bool occlusionQueryEnable = false;
        VkQueryControlFlags queryFlags = 0;
        VkQueryPipelineStatisticFlags pipelineStatistics = 0;
        friend class CommandPool;
    };
} // namespace magma

#include "commandBuffer.inl"
