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
#include "pipeline.h"

namespace magma
{
    class RenderPass;
    class PipelineShaderStage;
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
       multiple fixed-function pipeline stages, and a pipeline layout.
       Any pipeline object state that is specified as dynamic is not
       applied to the current state when the pipeline object is bound,
       but is instead set by dynamic state setting commands. */

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
            const std::vector<VkDynamicState>& dynamicStates,
            std::shared_ptr<PipelineLayout> layout,
            std::shared_ptr<RenderPass> renderPass,
            uint32_t subpass,
            std::shared_ptr<IAllocator> allocator = nullptr,
            std::shared_ptr<PipelineCache> pipelineCache = nullptr,
            std::shared_ptr<GraphicsPipeline> basePipeline = nullptr,
            VkPipelineCreateFlags flags = 0,
            const StructureChain& extendedInfo = StructureChain());
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
            const std::vector<VkDynamicState>& dynamicStates,
            std::shared_ptr<PipelineLayout> layout,
            std::shared_ptr<RenderPass> renderPass,
            uint32_t subpass,
            std::shared_ptr<IAllocator> allocator = nullptr,
            std::shared_ptr<PipelineCache> pipelineCache = nullptr,
            std::shared_ptr<GraphicsPipeline> basePipeline = nullptr,
            VkPipelineCreateFlags flags = 0,
            const StructureChain& extendedInfo = StructureChain());

    private:
        GraphicsPipeline(VkPipeline handle,
            std::shared_ptr<Device> device,
            std::shared_ptr<PipelineLayout> layout,
            std::shared_ptr<Pipeline> basePipeline,
            std::shared_ptr<IAllocator> allocator,
            uint32_t stageCount,
        #ifdef VK_EXT_pipeline_creation_feedback
            VkPipelineCreationFeedbackEXT creationFeedback,
            const std::vector<VkPipelineCreationFeedbackEXT>& stageCreationFeedbacks,
        #endif // VK_EXT_pipeline_creation_feedback
            hash_t hash);
        friend class GraphicsPipelineBatch;
    };
} // namespace magma
