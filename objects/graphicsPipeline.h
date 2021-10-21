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
#include "pipeline.h"
#include "../shaders/pipelineShaderStage.h"

namespace magma
{
    class RenderPass;
    class PipelineCache;

    class VertexInputState;
    struct InputAssemblyState;
    struct TesselationState;
    struct RasterizationState;
    struct MultisampleState;
    struct DepthStencilState;
    struct ColorBlendState;
    class ViewportState;

    /* Graphics pipelines consist of multiple shader stages,
       multiple fixed-function pipeline stages, and a pipeline layout. */

    class GraphicsPipeline : public Pipeline
    {
    public:
        explicit GraphicsPipeline(std::shared_ptr<Device> device,
            const std::vector<PipelineShaderStage>& shaderStages,
            const VertexInputState& vertexInputState,
            const InputAssemblyState& inputAssemblyState,
            const RasterizationState& rasterizationState,
            const MultisampleState& multisampleState,
            const DepthStencilState& depthStencilState,
            const ColorBlendState& colorBlendState,
            const std::initializer_list<VkDynamicState>& dynamicStates,
            std::shared_ptr<PipelineLayout> layout,
            std::shared_ptr<RenderPass> renderPass,
            uint32_t subpass,
            std::shared_ptr<IAllocator> allocator = nullptr,
            std::shared_ptr<PipelineCache> pipelineCache = nullptr,
            std::shared_ptr<GraphicsPipeline> basePipeline = nullptr,
            VkPipelineCreateFlags flags = 0);
        explicit GraphicsPipeline(std::shared_ptr<Device> device,
            const std::vector<PipelineShaderStage>& shaderStages,
            const VertexInputState& vertexInputState,
            const InputAssemblyState& inputAssemblyState,
            const TesselationState& tesselationState,
            const ViewportState& viewportState,
            const RasterizationState& rasterizationState,
            const MultisampleState& multisampleState,
            const DepthStencilState& depthStencilState,
            const ColorBlendState& colorBlendState,
            const std::initializer_list<VkDynamicState>& dynamicStates,
            std::shared_ptr<PipelineLayout> layout,
            std::shared_ptr<RenderPass> renderPass,
            uint32_t subpass,
            std::shared_ptr<IAllocator> allocator = nullptr,
            std::shared_ptr<PipelineCache> pipelineCache = nullptr,
            std::shared_ptr<GraphicsPipeline> basePipeline = nullptr,
            VkPipelineCreateFlags flags = 0);

    protected:
        explicit GraphicsPipeline(VkPipeline pipeline,
            std::size_t hash,
            std::shared_ptr<Device> device,
            std::shared_ptr<PipelineLayout> layout,
            std::shared_ptr<IAllocator> allocator);
        friend class GraphicsPipelines;
    };

    /* Exposes the Vulkan ability to create multiple graphics pipeline objects in a single call.
       As there may be thousands of graphics pipelines in the complicated rendering pipeline,
       it may be more efficient for the driver to create them at once. */

    class GraphicsPipelines : public core::NonCopyable
    {
        struct GraphicsPipelineCreateInfo;
        struct PipelineDynamicStateCreateInfo;

    public:
        uint32_t newPipeline(const std::vector<PipelineShaderStage>& shaderStages,
            const VertexInputState& vertexInputState,
            const InputAssemblyState& inputAssemblyState,
            const TesselationState& tesselationState,
            const ViewportState& viewportState,
            const RasterizationState& rasterizationState,
            const MultisampleState& multisampleState,
            const DepthStencilState& depthStencilState,
            const ColorBlendState& colorBlendState,
            const std::vector<VkDynamicState>& dynamicStates,
            std::shared_ptr<PipelineLayout> layout,
            std::shared_ptr<RenderPass> renderPass,
            uint32_t subpass,
            VkPipelineCreateFlags flags = 0);
        void buildPipelines(std::shared_ptr<Device> device,
            std::shared_ptr<PipelineCache> pipelineCache,
            std::shared_ptr<IAllocator> allocator = nullptr);
        uint32_t getPipelineCount() const noexcept { return MAGMA_COUNT(graphicsPipelines); }
        std::shared_ptr<GraphicsPipeline> getPipeline(uint32_t index) const noexcept { return graphicsPipelines[index]; }

    private:
        std::vector<GraphicsPipelineCreateInfo> pipelineInfos;
        std::vector<PipelineDynamicStateCreateInfo> dynamicStateInfos;
        std::vector<std::size_t> hashes;
        std::vector<std::shared_ptr<PipelineLayout>> pipelineLayouts;
        std::vector<std::shared_ptr<GraphicsPipeline>> graphicsPipelines;
    };

    struct GraphicsPipelines::GraphicsPipelineCreateInfo : VkGraphicsPipelineCreateInfo
    {
        ~GraphicsPipelineCreateInfo();
    };

    struct GraphicsPipelines::PipelineDynamicStateCreateInfo : VkPipelineDynamicStateCreateInfo
    {
        ~PipelineDynamicStateCreateInfo();
    };
} // namespace magma
