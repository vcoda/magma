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
    class PipelineCache;

    class ComputePipeline : public Pipeline
    {
    public:
        explicit ComputePipeline(std::shared_ptr<Device> device,
            const PipelineShaderStage& shaderStage,
            std::shared_ptr<PipelineLayout> layout,
            std::shared_ptr<IAllocator> allocator = nullptr,
            std::shared_ptr<PipelineCache> pipelineCache = nullptr,
            std::shared_ptr<ComputePipeline> basePipeline = nullptr,
            VkPipelineCreateFlags flags = 0);

    private:
        explicit ComputePipeline(VkPipeline pipeline,
            std::shared_ptr<Device> device,
            std::shared_ptr<PipelineLayout> layout,
            std::shared_ptr<Pipeline> basePipeline,
            std::shared_ptr<IAllocator> allocator,
            std::size_t hash);
        friend class ComputePipelines;
    };

    /* Exposes Vulkan's ability to create multiple compute pipeline objects in a single call. */

    class ComputePipelines : public Pipelines
    {
    public:
        explicit ComputePipelines(std::size_t capacity = 32);
        uint32_t newPipeline(const PipelineShaderStage& shaderStage,
            std::shared_ptr<PipelineLayout> layout,
            std::shared_ptr<ComputePipeline> basePipeline = nullptr,
            VkPipelineCreateFlags flags = 0);
        void buildPipelines(std::shared_ptr<Device> device,
            std::shared_ptr<PipelineCache> pipelineCache,
            std::shared_ptr<IAllocator> allocator = nullptr);
        uint32_t getPipelineCount() const noexcept { return MAGMA_COUNT(computePipelines); }
        std::shared_ptr<ComputePipeline> getPipeline(uint32_t index) const noexcept { return computePipelines[index]; }

    private:
        std::vector<VkComputePipelineCreateInfo> pipelineInfos;
        std::vector<std::shared_ptr<ComputePipeline>> computePipelines;
    };
} // namespace magma
