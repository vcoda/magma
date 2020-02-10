/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2019 Victor Coda.

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
#include "../shaders/rayTracingShaderGroup.h"

namespace magma
{
    class PipelineCache;

    /* Raytracing pipelines consist of multiple shader stages,
       fixed-function traversal stages, and a pipeline layout. */

    class RayTracingPipeline : public Pipeline
    {
    public:
        explicit RayTracingPipeline(std::shared_ptr<Device> device,
            const std::vector<PipelineShaderStage>& stages,
            const std::vector<RayTracingShaderGroup>& groups,
            uint32_t maxRecursionDepth,
            std::shared_ptr<PipelineLayout> layout,
            std::shared_ptr<RayTracingPipeline> basePipeline = nullptr,
            std::shared_ptr<PipelineCache> pipelineCache = nullptr,
            VkPipelineCreateFlags flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr);
        std::vector<VkShaderModule> getShaderGroupHandles(uint32_t firstGroup, uint32_t groupCount) const;
        void compileDeferred(uint32_t shaderIndex);
    };
} // namespace magma
