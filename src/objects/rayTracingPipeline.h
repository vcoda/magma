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
#include "pipeline.h"
#include "../shaders/pipelineShaderStage.h"
#include "../shaders/rayTracingShaderGroup.h"

namespace magma
{
    class PipelineCache;
#ifdef VK_KHR_pipeline_library
    class PipelineLibrary;
#endif
#ifdef VK_KHR_deferred_host_operations
    class DeferredOperation;
#endif

    /* Rasterization has been the dominant method to produce interactive
       graphics, but increasing performance of graphics hardware has made
       ray tracing a viable option for interactive rendering. Being able
       to integrate ray tracing with traditional rasterization makes it
       easier for applications to incrementally add ray traced effects
       to existing applications or to do hybrid approaches with rasterization
       for primary visibility and ray tracing for secondary queries. */

#ifdef VK_KHR_ray_tracing_pipeline
    class RayTracingPipeline : public Pipeline
    {
    public:
        explicit RayTracingPipeline(std::shared_ptr<Device> device,
            const std::vector<PipelineShaderStage>& shaderStages,
            const std::vector<RayTracingShaderGroup>& shaderGroups,
            uint32_t maxRecursionDepth,
            core::variant_ptr<PipelineLayout> layout,
            std::shared_ptr<IAllocator> allocator = nullptr,
            const std::unique_ptr<PipelineCache>& pipelineCache = nullptr,
        #ifdef VK_KHR_pipeline_library
            const std::unique_ptr<PipelineLibrary>& pipelineLibrary = nullptr,
        #endif
            std::shared_ptr<RayTracingPipeline> basePipeline = nullptr,
            std::shared_ptr<DeferredOperation> deferredOperation = nullptr,
            const std::vector<VkDynamicState>& dynamicStates = {},
            VkPipelineCreateFlags flags = 0,
            const StructureChain& extendedInfo = StructureChain());
        const std::vector<PipelineShaderStage>& getShaderStages() const noexcept { return shaderStages; }
        const std::vector<RayTracingShaderGroup>& getShaderGroups() const noexcept { return shaderGroups; }
        uint32_t getMaxRecursionDepth() const noexcept { return maxRecursionDepth; }
        VkDeviceSize getGeneralShaderStackSize(uint32_t group) const noexcept;
        VkDeviceSize getClosestHitShaderStackSize(uint32_t group) const noexcept;
        VkDeviceSize getAnyHitShaderStackSize(uint32_t group) const noexcept;
        VkDeviceSize getIntersectionShaderStackSize(uint32_t group) const noexcept;
        std::vector<uint8_t> getShaderGroupHandles() const;
        std::vector<uint8_t> getCaptureReplayShaderGroupHandles() const;

    private:
        friend class RayTracingPipelineBatch;
        MAGMA_MAKE_SHARED(RayTracingPipeline)
        RayTracingPipeline(VkPipeline handle,
            std::shared_ptr<Device> device,
            core::variant_ptr<PipelineLayout> layout,
            std::shared_ptr<Pipeline> basePipeline,
            std::shared_ptr<IAllocator> allocator,
            const std::vector<PipelineShaderStage>& shaderStages,
            const std::vector<RayTracingShaderGroup>& shaderGroups,
            uint32_t maxRecursionDepth,
        #ifdef VK_EXT_pipeline_creation_feedback
            VkPipelineCreationFeedbackEXT creationFeedback,
            const std::vector<VkPipelineCreationFeedbackEXT>& stageCreationFeedbacks,
        #endif // VK_EXT_pipeline_creation_feedback
            hash_t hash);
        VkDeviceSize getShaderGroupStackSize(uint32_t group,
            VkShaderGroupShaderKHR groupShader) const noexcept;

        const std::vector<PipelineShaderStage> shaderStages;
        const std::vector<RayTracingShaderGroup> shaderGroups;
        const uint32_t maxRecursionDepth;
    };

    /* Calculates hash of ray tracing pipeline. This function
       is shared between classes to make sure that hash computation
       is consitent across different parts of the library. */

    hash_t psoHash(VkPipelineCreateFlags flags,
        const std::vector<PipelineShaderStage>& shaderStages,
        const std::vector<RayTracingShaderGroup>& shaderGroups,
        const std::vector<VkDynamicState>& dynamicStates,
        const PipelineLayout *layout,
        uint32_t maxRecursionDepth,
        const StructureChain& extendedInfo = StructureChain()) noexcept;
#endif // VK_KHR_ray_tracing_pipeline
} // namespace magma
