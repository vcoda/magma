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

    protected:
        explicit RayTracingPipeline(VkPipeline pipeline,
            uint32_t shaderGroupCount,
            uint32_t maxRecursionDepth,
            std::size_t hash,
            std::shared_ptr<Device> device,
            std::shared_ptr<PipelineLayout> layout,
            std::shared_ptr<IAllocator> allocator);
        friend class RayTracingPipelines;

    private:
        const uint32_t shaderGroupCount;
        const uint32_t maxRecursionDepth;
    };

    /* Exposes the Vulkan ability to create multiple ray tracing pipeline objects in a single call.
       As there may be thousands of ray tracing pipelines in the complicated rendering engine,
       it may be more efficient for the driver to create them at once. */

    class RayTracingPipelines : public core::NonCopyable
    {
    public:
        explicit RayTracingPipelines(std::size_t capacity = 32);
        uint32_t newPipeline(const std::vector<PipelineShaderStage>& shaderStages,
            const std::vector<RayTracingShaderGroup>& shaderGroups,
            uint32_t maxRecursionDepth,
            std::shared_ptr<PipelineLayout> layout,
            std::shared_ptr<RayTracingPipeline> basePipeline = nullptr,
            VkPipelineCreateFlags flags = 0);
        void buildPipelines(std::shared_ptr<Device> device,
            std::shared_ptr<PipelineCache> pipelineCache,
            std::shared_ptr<IAllocator> allocator = nullptr);
        uint32_t getPipelineCount() const noexcept { return MAGMA_COUNT(rayTracingPipelines); }
        std::shared_ptr<RayTracingPipeline> getPipeline(uint32_t index) const noexcept { return rayTracingPipelines[index]; }

    private:
        std::shared_ptr<Device> device;
        std::list<std::vector<PipelineShaderStage>> stages;
        std::list<std::vector<RayTracingShaderGroup>> groups;
        std::list<std::shared_ptr<PipelineLayout>> layouts;
        std::list<std::shared_ptr<RayTracingPipeline>> basePipelines;
        std::list<std::size_t> hashes;
        std::vector<VkRayTracingPipelineCreateInfoNV> pipelineInfos;
        std::vector<std::shared_ptr<RayTracingPipeline>> rayTracingPipelines;
    };
#endif // VK_NV_ray_tracing
} // namespace magma
