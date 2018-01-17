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
#include <vector>
#include "handle.h"
#include "shaderModule.h"

namespace magma
{
    class Device;
    class PipelineLayout;
    class PipelineCache;

    class Pipeline : public NonDispatchable<VkPipeline>
    {
    public:
        ~Pipeline();

    protected:
        Pipeline(std::shared_ptr<const Device> device);

    protected:
        std::unique_ptr<PipelineLayout> defaultLayout;
    };

    class VertexInputState;
    class InputAssemblyState;
    class TesselationState;
    class ViewportState;
    class RasterizationState;
    class MultisampleState;
    class DepthStencilState;
    class ColorBlendState;
    class RenderPass;

    class GraphicsPipeline : public Pipeline
    {
    public:
        GraphicsPipeline(std::shared_ptr<const Device> device, std::shared_ptr<const PipelineCache> pipelineCache,
            const std::vector<ShaderStage>& stages,
            const VertexInputState& vertexInputState,
            const InputAssemblyState& inputAssemblyState,
            const RasterizationState& rasterizationState,
            const MultisampleState& multisampleState,
            const DepthStencilState& depthStencilState,
            const ColorBlendState& colorBlendState,
            const std::initializer_list<VkDynamicState>& dynamicStates,
            std::shared_ptr<const PipelineLayout> layout,
            std::shared_ptr<const RenderPass> renderPass,
            uint32_t subpass = 0,
            VkPipelineCreateFlags flags = 0);
        GraphicsPipeline(std::shared_ptr<const Device> device, std::shared_ptr<const PipelineCache> pipelineCache,
            const std::vector<ShaderStage>& stages,
            const VertexInputState& vertexInputState,
            const InputAssemblyState& inputAssemblyState,
            const TesselationState& tesselationState,
            const ViewportState& viewportState,
            const RasterizationState& rasterizationState,
            const MultisampleState& multisampleState,
            const DepthStencilState& depthStencilState,
            const ColorBlendState& colorBlendState,
            const std::initializer_list<VkDynamicState>& dynamicStates,
            std::shared_ptr<const PipelineLayout> layout,
            std::shared_ptr<const RenderPass> renderPass,
            uint32_t subpass = 0,
            VkPipelineCreateFlags flags = 0);
    };

    class ComputePipeline : public Pipeline
    {
    public:
        ComputePipeline(std::shared_ptr<const Device> device, std::shared_ptr<const PipelineCache> pipelineCache,
            const ShaderStage& stage,
            std::shared_ptr<const PipelineLayout> layout = nullptr,
            VkPipelineCreateFlags flags = 0);
    };
} // namespace magma
