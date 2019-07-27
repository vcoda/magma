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
#include "nondispatchable.h"
#include "shaderStages.h"
#include "shaderStagesRaytrace.h"

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
        VkShaderStatisticsInfoAMD getShaderStatistics(VkShaderStageFlagBits stage) const;
        std::vector<uint8_t> getShaderBinary(VkShaderStageFlagBits stage) const;
        std::string getShaderDisassembly(VkShaderStageFlagBits stage) const;

    protected:
        explicit Pipeline(std::shared_ptr<Device> device,
            std::shared_ptr<IAllocator> allocator);

    protected:
        std::unique_ptr<PipelineLayout> defaultLayout;
    };

    struct InputAssemblyState;
    struct TesselationState;
    struct RasterizationState;
    struct MultisampleState;
    struct DepthStencilState;
    struct ColorBlendState;
    struct VertexInputState;

    class ViewportState;
    class RenderPass;

    /* Graphics pipelines consist of multiple shader stages,
       multiple fixed-function pipeline stages, and a pipeline layout. */

    class GraphicsPipeline : public Pipeline
    {
    public:
        explicit GraphicsPipeline(std::shared_ptr<Device> device, std::shared_ptr<const PipelineCache> pipelineCache,
            const std::vector<PipelineShaderStage>& stages,
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
            std::shared_ptr<const GraphicsPipeline> basePipeline = nullptr,
            VkPipelineCreateFlags flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr);
        explicit GraphicsPipeline(std::shared_ptr<Device> device, std::shared_ptr<const PipelineCache> pipelineCache,
            const std::vector<PipelineShaderStage>& stages,
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
            std::shared_ptr<const GraphicsPipeline> basePipeline = nullptr,
            VkPipelineCreateFlags flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr);
    };

    /* The compute pipeline is a separate pipeline from the graphics pipeline,
       which operates on one-, two-, or three-dimensional workgroups
       which can read from and write to buffer and image memory. */

    class ComputePipeline : public Pipeline
    {
    public:
        explicit ComputePipeline(std::shared_ptr<Device> device, std::shared_ptr<const PipelineCache> pipelineCache,
            const PipelineShaderStage& stage,
            std::shared_ptr<const PipelineLayout> layout = nullptr,
            std::shared_ptr<const ComputePipeline> basePipeline = nullptr,
            VkPipelineCreateFlags flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr);
    };

    /* Raytracing pipelines consist of multiple shader stages,
       fixed-function traversal stages, and a pipeline layout. */

    class RaytracingPipeline : public Pipeline
    {
    public:
        explicit RaytracingPipeline(std::shared_ptr<Device> device, std::shared_ptr<const PipelineCache> pipelineCache,
            const std::vector<PipelineShaderStage>& stages,
            const std::vector<RaytracingShaderGroup>& groups,
            uint32_t maxRecursionDepth,
            std::shared_ptr<const PipelineLayout> layout,
            std::shared_ptr<const RaytracingPipeline> basePipeline = nullptr,
            VkPipelineCreateFlags flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr);
        std::vector<VkShaderModule> getShaderGroupHandles(uint32_t firstGroup, uint32_t groupCount) const;
        void compileDeferred(uint32_t shaderIndex);
    };
} // namespace magma
