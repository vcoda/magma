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
    class PipelineCache;

    /* With Vulkan it is possible to create multiple compute
       pipelines in a single API call. As there are may be
       hundreds of such pipelines in the sophisticated computing
       program, it may be more efficient for graphics driver
       to create all of them at once. */

    class ComputePipelineBatch : public TPipelineBatch<ComputePipeline>
    {
    public:
        explicit ComputePipelineBatch(uint32_t capacity = 32);
        uint32_t batchPipeline(const PipelineShaderStage& shaderStage,
            std::shared_ptr<PipelineLayout> layout,
            std::shared_ptr<ComputePipeline> basePipeline = nullptr,
            VkPipelineCreateFlags flags = 0);
        void buildPipelines(std::shared_ptr<Device> device,
            std::shared_ptr<PipelineCache> pipelineCache = nullptr,
        #ifdef VK_KHR_pipeline_library
            std::shared_ptr<PipelineLibrary> pipelineLibrary = nullptr,
        #endif
            std::shared_ptr<IAllocator> allocator = nullptr) override;

    private:
        std::vector<VkComputePipelineCreateInfo> pipelineInfos;
    };
} // namespace magma
