/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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
#include "../shaders/rayTracingShaderGroup.h"

namespace magma
{
#ifdef VK_KHR_ray_tracing_pipeline
    class RayTracingPipeline;
#ifdef VK_KHR_pipeline_library
    class PipelineLibrary;
#endif
#ifdef VK_KHR_deferred_host_operations
    class DeferredOperation;
#endif

    /* As there are may be thousands of ray tracing pipelines in the
       sophisticated rendering program, grouping multiple pipeline
       compilations into a single command allows for better parallelization. */

    class RayTracingPipelineBatch : public BasePipelineBatch<RayTracingPipeline, VkRayTracingPipelineCreateInfoKHR>
    {
    public:
        explicit RayTracingPipelineBatch(std::shared_ptr<Device> device,
            std::shared_ptr<DeferredOperation> deferredOperation = nullptr) noexcept;
        uint32_t batchPipeline(const std::vector<PipelineShaderStage>& shaderStages,
            const std::vector<RayTracingShaderGroup>& shaderGroups,
            uint32_t maxRecursionDepth,
            std::unique_ptr<PipelineLayout> layout,
            std::shared_ptr<RayTracingPipeline> basePipeline = nullptr,
            const std::vector<VkDynamicState>& dynamicStates = {},
            VkPipelineCreateFlags flags = 0,
            const StructureChain& extendedInfo = StructureChain());
        void buildPipelines(std::shared_ptr<PipelineCache> pipelineCache = nullptr,
            std::shared_ptr<PipelineLibrary> pipelineLibrary = nullptr,
            std::shared_ptr<IAllocator> allocator = nullptr) override;

    private:
        std::shared_ptr<DeferredOperation> deferredOperation;
        std::forward_list<std::vector<VkShaderStageFlagBits>> shaderStageFlags;
        std::forward_list<std::vector<RayTracingShaderGroup>> groups;
        std::forward_list<uint32_t> maxRecursionDepths;
        std::forward_list<std::vector<VkDynamicState>> dynamicStates;
        std::forward_list<VkPipelineDynamicStateCreateInfo> dynamicStateInfos;
    };
#endif // VK_KHR_ray_tracing_pipeline
} // namespace magma
