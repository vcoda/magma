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
    class ComputePipeline;

    /* As there are may be hundreds of compute pipelines in the
       sophisticated compute program, grouping multiple pipeline
       compilations into a single command allows for better parallelization. */

    class ComputePipelineBatch : public BasePipelineBatch<ComputePipeline, VkComputePipelineCreateInfo>
    {
    public:
        explicit ComputePipelineBatch(std::shared_ptr<Device> device) noexcept;
        uint32_t batchPipeline(const PipelineShaderStage& shaderStage,
            core::variant_ptr<PipelineLayout> layout,
            std::shared_ptr<ComputePipeline> basePipeline = nullptr,
            VkPipelineCreateFlags flags = 0,
            const StructureChain& extendedInfo = StructureChain());
        void buildPipelines(lent_ptr<PipelineCache> pipelineCache = nullptr,
        #ifdef VK_KHR_pipeline_library
            lent_ptr<const PipelineLibrary> pipelineLibrary = nullptr,
        #endif
            std::shared_ptr<IAllocator> allocator = nullptr) override;
    };
} // namespace magma
