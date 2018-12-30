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
#include "shaderStages.h"

namespace magma
{
    class RaygenShaderStage : public PipelineShaderStage
    {
    public:
        explicit RaygenShaderStage(std::shared_ptr<const ShaderModule> module,
            const char *const entrypoint,
            std::shared_ptr<const Specialization> specialization = nullptr,
            VkPipelineShaderStageCreateFlags flags = 0) noexcept:
            PipelineShaderStage(VK_SHADER_STAGE_RAYGEN_BIT_NV, std::move(module), entrypoint, std::move(specialization), flags)
        {}
    };

    class AnyHitShaderStage : public PipelineShaderStage
    {
    public:
        explicit AnyHitShaderStage(std::shared_ptr<const ShaderModule> module,
            const char *const entrypoint,
            std::shared_ptr<const Specialization> specialization = nullptr,
            VkPipelineShaderStageCreateFlags flags = 0) noexcept:
            PipelineShaderStage(VK_SHADER_STAGE_ANY_HIT_BIT_NV, std::move(module), entrypoint, std::move(specialization), flags)
        {}
    };

    class ClosestHitShaderStage : public PipelineShaderStage
    {
    public:
        explicit ClosestHitShaderStage(std::shared_ptr<const ShaderModule> module,
            const char *const entrypoint,
            std::shared_ptr<const Specialization> specialization = nullptr,
            VkPipelineShaderStageCreateFlags flags = 0) noexcept:
            PipelineShaderStage(VK_SHADER_STAGE_CLOSEST_HIT_BIT_NV, std::move(module), entrypoint, std::move(specialization), flags)
        {}
    };

    class MissShaderStage : public PipelineShaderStage
    {
    public:
        explicit MissShaderStage(std::shared_ptr<const ShaderModule> module,
            const char *const entrypoint,
            std::shared_ptr<const Specialization> specialization = nullptr,
            VkPipelineShaderStageCreateFlags flags = 0) noexcept:
            PipelineShaderStage(VK_SHADER_STAGE_MISS_BIT_NV, std::move(module), entrypoint, std::move(specialization), flags)
        {}
    };

    class IntersectionShaderStage : public PipelineShaderStage
    {
    public:
        explicit IntersectionShaderStage(std::shared_ptr<const ShaderModule> module,
            const char *const entrypoint,
            std::shared_ptr<const Specialization> specialization = nullptr,
            VkPipelineShaderStageCreateFlags flags = 0) noexcept:
            PipelineShaderStage(VK_SHADER_STAGE_INTERSECTION_BIT_NV, std::move(module), entrypoint, std::move(specialization), flags)
        {}
    };

    class CallableShaderStage : public PipelineShaderStage
    {
    public:
        explicit CallableShaderStage(std::shared_ptr<const ShaderModule> module,
            const char *const entrypoint,
            std::shared_ptr<const Specialization> specialization = nullptr,
            VkPipelineShaderStageCreateFlags flags = 0) noexcept:
            PipelineShaderStage(VK_SHADER_STAGE_CALLABLE_BIT_NV, std::move(module), entrypoint, std::move(specialization), flags)
        {}
    };

    class RaytracingShaderGroup : public VkRayTracingShaderGroupCreateInfoNV
    {
    protected:
        explicit RaytracingShaderGroup(VkRayTracingShaderGroupTypeNV type,
            uint32_t generalShader,
            uint32_t closestHitShader,
            uint32_t anyHitShader,
            uint32_t intersectionShader);
    };

    // TODO: reconsider

    class RaytracingGeneralShaderGroup : public RaytracingShaderGroup
    {
    public:
        explicit RaytracingGeneralShaderGroup(uint32_t generalShader):
        RaytracingShaderGroup(VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_NV, 
            generalShader, VK_SHADER_UNUSED_NV, VK_SHADER_UNUSED_NV, VK_SHADER_UNUSED_NV) {}   
    };

    class RaytracingTrianglesHitShaderGroup : public RaytracingShaderGroup
    {
    public:
        explicit RaytracingTrianglesHitShaderGroup(uint32_t closestHitShader, uint32_t anyHitShader):
        RaytracingShaderGroup(VK_RAY_TRACING_SHADER_GROUP_TYPE_TRIANGLES_HIT_GROUP_NV,
            VK_SHADER_UNUSED_NV, closestHitShader, anyHitShader, VK_SHADER_UNUSED_NV) {}
    };

    class RaytracingProceduralHitShaderGroup : public RaytracingShaderGroup
    {
    public:
        explicit RaytracingProceduralHitShaderGroup(uint32_t intersectionShader):
        RaytracingShaderGroup(VK_RAY_TRACING_SHADER_GROUP_TYPE_PROCEDURAL_HIT_GROUP_NV,
            VK_SHADER_UNUSED_NV, VK_SHADER_UNUSED_NV, VK_SHADER_UNUSED_NV, intersectionShader) {}
    };
} // namespace magma
