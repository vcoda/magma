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
    class PipelineCache;
    class PipelineLayout;

    /* Some Vulkan commands specify geometric objects to be drawn or computational work to be performed,
       while others specify state controlling how objects are handled by the various pipeline stages,
       or control data transfer between memory organized as images and buffers. Commands are effectively sent
       through a processing pipeline, either a graphics pipeline or a compute pipeline. */

    class Pipeline : public NonDispatchable<VkPipeline>
    {
    public:
        ~Pipeline();
        void getShaderStatistics(VkShaderStageFlagBits stage, VkShaderStatisticsInfoAMD& info) const;
        std::vector<uint8_t> getShaderBinary(VkShaderStageFlagBits stage) const;
        std::string getShaderDisassembly(VkShaderStageFlagBits stage) const;

    protected:
        Pipeline(std::shared_ptr<const Device> device,
            std::shared_ptr<IAllocator> allocator);

    protected:
        std::unique_ptr<PipelineLayout> defaultLayout;
    };

    struct InputAssemblyState;
    struct TesselationState;
    struct RasterizationState;
    struct MultisampleState;
    struct DepthStencilState;

    class VertexInputState;
    class ViewportState;
    class ColorBlendState;
    class RenderPass;

    /* Graphics pipelines consist of multiple shader stages,
       multiple fixed-function pipeline stages, and a pipeline layout. */

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
            VkPipelineCreateFlags flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr);
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
            VkPipelineCreateFlags flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr);
    };

    /* The compute pipeline is a separate pipeline from the graphics pipeline,
       which operates on one-, two-, or three-dimensional workgroups
       which can read from and write to buffer and image memory. */

    class ComputePipeline : public Pipeline
    {
    public:
        ComputePipeline(std::shared_ptr<const Device> device, std::shared_ptr<const PipelineCache> pipelineCache,
            const ShaderStage& stage,
            std::shared_ptr<const PipelineLayout> layout = nullptr,
            VkPipelineCreateFlags flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr);
    };
} // namespace magma
