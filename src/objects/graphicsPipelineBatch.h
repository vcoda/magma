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
#include "pipelineBatch.h"

namespace magma
{
    class GraphicsPipeline;
    class RenderPass;
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

    /* With Vulkan it is possible to create multiple graphics
       pipelines in a single API call. As there are may be
       thousands of such pipelines in the sophisticated rendering
       program, it may be more efficient for graphics driver
       to create all of them at once. */

    class GraphicsPipelineBatch : public TPipelineBatch<GraphicsPipeline>
    {
    public:
        explicit GraphicsPipelineBatch(std::shared_ptr<Device> device) noexcept;
        uint32_t batchPipeline(const std::vector<PipelineShaderStage>& shaderStages,
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
            std::shared_ptr<GraphicsPipeline> basePipeline = nullptr,
            VkPipelineCreateFlags flags = 0,
            const StructureChain& extendedInfo = StructureChain());
        void buildPipelines(std::shared_ptr<PipelineCache> pipelineCache = nullptr,
        #ifdef VK_KHR_pipeline_library
            std::shared_ptr<PipelineLibrary> pipelineLibrary = nullptr,
        #endif
            std::shared_ptr<IAllocator> allocator = nullptr) override;

    private:
        std::forward_list<VertexInputState> vertexInputStates;
        std::forward_list<InputAssemblyState> inputAssemblyStates;
        std::forward_list<TesselationState> tesselationStates;
        std::forward_list<ViewportState> viewportStates;
        std::forward_list<RasterizationState> rasterizationStates;
        std::forward_list<MultisampleState> multisampleStates;
        std::forward_list<DepthStencilState> depthStencilStates;
        std::forward_list<ColorBlendState> colorBlendStates;
        std::forward_list<std::vector<VkDynamicState>> dynamicStates;
        std::forward_list<VkPipelineDynamicStateCreateInfo> dynamicStateInfos;
        std::forward_list<std::shared_ptr<RenderPass>> renderPasses;
        std::forward_list<hash_t> rsHashes;
        std::vector<VkGraphicsPipelineCreateInfo> pipelineInfos;
    };
} // namespace magma
