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
#include "pipeline.h"

namespace magma
{
    class PipelineShaderStage;
    class PipelineCache;
#ifdef VK_KHR_pipeline_library
    class PipelineLibrary;
#endif

    class VertexInputState;
    struct InputAssemblyState;
    struct TesselationState;
    struct RasterizationState;
    struct MultisampleState;
    struct DepthStencilState;
    struct ColorBlendState;
    class ViewportState;
    class RenderPass;

    /* Graphics pipelines consist of multiple shader stages,
       multiple fixed-function pipeline stages, and a pipeline
       layout. Any pipeline object state that is specified as
       dynamic is not applied to the current state when the
       pipeline object is bound, but is instead set by dynamic
       state setting commands. */

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
            core::variant_ptr<PipelineLayout> layout,
            lent_ptr<RenderPass> renderPass,
            uint32_t subpass,
            std::shared_ptr<IAllocator> allocator = nullptr,
            lent_ptr<PipelineCache> pipelineCache = nullptr,
        #ifdef VK_KHR_pipeline_library
            lent_ptr<PipelineLibrary> pipelineLibrary = nullptr,
        #endif
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
            core::variant_ptr<PipelineLayout> layout,
            lent_ptr<RenderPass> renderPass,
            uint32_t subpass,
            std::shared_ptr<IAllocator> allocator = nullptr,
            lent_ptr<PipelineCache> pipelineCache = nullptr,
        #ifdef VK_KHR_pipeline_library
            lent_ptr<PipelineLibrary> pipelineLibrary = nullptr,
        #endif
            std::shared_ptr<GraphicsPipeline> basePipeline = nullptr,
            VkPipelineCreateFlags flags = 0,
            const StructureChain& extendedInfo = StructureChain());
        hash_t getRenderStateHash() const noexcept { return rsHash; }

    private:
        MAGMA_MAKE_UNIQUE(GraphicsPipeline)
        GraphicsPipeline(VkPipeline handle,
            std::shared_ptr<Device> device,
            core::variant_ptr<PipelineLayout> layout,
            std::shared_ptr<Pipeline> basePipeline,
            std::shared_ptr<IAllocator> allocator,
            uint32_t stageCount,
        #ifdef VK_EXT_pipeline_creation_feedback
            VkPipelineCreationFeedbackEXT creationFeedback,
            const std::vector<VkPipelineCreationFeedbackEXT>& stageCreationFeedbacks,
        #endif // VK_EXT_pipeline_creation_feedback
            hash_t hash,
            hash_t rsHash);
        friend class GraphicsPipelineBatch;

        hash_t rsHash;
    };

    /* Calculates hash of render states and full hash of graphics
       pipeline state object. This function is shared between
       classes to make sure that hash computation is consitent
       across different parts of the library. */

    std::pair<hash_t, hash_t> psoHash(VkPipelineCreateFlags flags,
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
        const PipelineLayout *layout,
        const RenderPass *renderPass,
        uint32_t subpass,
        const StructureChain& extendedInfo = StructureChain()) noexcept;
} // namespace magma
