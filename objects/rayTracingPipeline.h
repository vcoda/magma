/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2022 Victor Coda.

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
#include "../shaders/rayTracingShaderGroup.h"

namespace magma
{
    class PipelineCache;

    /* Raytracing pipelines consist of multiple shader stages,
       fixed-function traversal stages, and a pipeline layout. */

#ifdef VK_NV_ray_tracing
    class RayTracingPipeline : public Pipeline
    {
    public:
        explicit RayTracingPipeline(std::shared_ptr<Device> device,
            const std::vector<PipelineShaderStage>& shaderStages,
            const std::vector<RayTracingShaderGroup>& shaderGroups,
            uint32_t maxRecursionDepth,
            std::shared_ptr<PipelineLayout> layout,
            std::shared_ptr<IAllocator> allocator = nullptr,
            std::shared_ptr<PipelineCache> pipelineCache = nullptr,
            std::shared_ptr<RayTracingPipeline> basePipeline = nullptr,
            VkPipelineCreateFlags flags = 0);
        uint32_t getShaderGroupCount() const noexcept { return shaderGroupCount; }
        uint32_t getMaxRecursionDepth() const noexcept { return maxRecursionDepth; }
        std::vector<uint8_t> getShaderGroupHandles() const;
        void compileDeferred(uint32_t shaderIndex);

    private:
        explicit RayTracingPipeline(VkPipeline pipeline,
            std::shared_ptr<Device> device,
            std::shared_ptr<PipelineLayout> layout,
            std::shared_ptr<Pipeline> basePipeline,
            std::shared_ptr<IAllocator> allocator,
            uint32_t shaderGroupCount,
            uint32_t maxRecursionDepth,
        #ifdef VK_EXT_pipeline_creation_feedback
            VkPipelineCreationFeedbackEXT creationFeedback,
        #endif
            hash_t hash);
        friend class RayTracingPipelines;

        const uint32_t shaderGroupCount;
        const uint32_t maxRecursionDepth;
    };
#endif // VK_NV_ray_tracing
} // namespace magma
