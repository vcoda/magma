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
#include "base.h"
#include "../shaders/pipelineShaderStage.h"
#include "../misc/structureChain.h"

namespace magma
{
    class Device;
    class GraphicsPipeline;
    class PipelineLayout;
    class PipelineCache;
    class RenderPass;
    class IAllocator;

    class VertexInputState;
    struct InputAssemblyState;
    struct TesselationState;
    class ViewportState;
    struct RasterizationState;
    struct MultisampleState;
    struct DepthStencilState;
    struct ColorBlendState;

    namespace aux
    {
        typedef std::unordered_map<hash_t, std::shared_ptr<GraphicsPipeline>> PsoCache;

        /* Auxiliary cache used to speed up graphics pipeline
           construction in run-time. Utilizes VkPipelineCache
           object along with lookup of base pipeline and lookup
           of existing pipeline by computing its hash. */

        class GraphicsPipelineCache : public Base
        {
        public:
            explicit GraphicsPipelineCache(std::shared_ptr<Device> device,
                std::shared_ptr<PipelineCache> pipelineCache,
                bool useDerivativePipelines,
                bool disablePipelineOptimization,
                std::shared_ptr<IAllocator> allocator = nullptr);
            const std::shared_ptr<Device>& getDevice() const noexcept { return device; }
            const std::shared_ptr<PipelineCache>& getPipelineCache() const noexcept { return pipelineCache; }
            const std::shared_ptr<IAllocator>& getAllocator() const noexcept { return allocator; }
            uint32_t getCachedPipelineCount() const noexcept { return MAGMA_COUNT(pipelines); }
            std::shared_ptr<GraphicsPipeline> lookupPipeline(const std::vector<PipelineShaderStage>& shaderStages,
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
                uint32_t subpass = 0,
                VkPipelineCreateFlags flags = 0,
                const StructureChain& extendedInfo = StructureChain());

        private:
            std::shared_ptr<Device> device;
            std::shared_ptr<PipelineCache> pipelineCache;
            std::shared_ptr<IAllocator> allocator;
            VkPipelineCreateFlags psoFlags;
            PsoCache pipelines;
            PsoCache basePipelinesByRenderStates;
            PsoCache basePipelinesByShaderStages;
        };
    } // namespace aux
} // namespace magma
