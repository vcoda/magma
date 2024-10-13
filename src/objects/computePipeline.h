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

    /* The compute pipeline is a separate pipeline from the
       graphics pipeline, which operates on one-, two-, or
       three-dimensional workgroups which can read from and
       write to buffer and image memory. */

    class ComputePipeline : public Pipeline
    {
    public:
        explicit ComputePipeline(std::shared_ptr<Device> device,
            const PipelineShaderStage& shaderStage,
            core::variant_ptr<PipelineLayout> layout,
            std::shared_ptr<IAllocator> allocator = nullptr,
            std::shared_ptr<PipelineCache> pipelineCache = nullptr,
        #ifdef VK_KHR_pipeline_library
            std::shared_ptr<PipelineLibrary> pipelineLibrary = nullptr,
        #endif
            std::shared_ptr<ComputePipeline> basePipeline = nullptr,
            VkPipelineCreateFlags flags = 0,
            const StructureChain& extendedInfo = StructureChain());

    private:
        MAGMA_MAKE_SHARED(ComputePipeline)
        ComputePipeline(VkPipeline handle,
            std::shared_ptr<Device> device,
            core::variant_ptr<PipelineLayout> layout,
            std::shared_ptr<Pipeline> basePipeline,
            std::shared_ptr<IAllocator> allocator,
        #ifdef VK_EXT_pipeline_creation_feedback
            VkPipelineCreationFeedbackEXT creationFeedback,
            const std::vector<VkPipelineCreationFeedbackEXT>& stageCreationFeedbacks,
        #endif
            hash_t hash);
        friend class ComputePipelineBatch;
    };
} // namespace magma
