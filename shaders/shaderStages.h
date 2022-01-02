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
#include "pipelineShaderStage.h"

namespace magma
{
    class VertexShaderStage : public PipelineShaderStage
    {
    public:
        explicit VertexShaderStage(std::shared_ptr<ShaderModule> module, const char *const entrypoint,
            std::shared_ptr<Specialization> specialization = nullptr, VkPipelineShaderStageCreateFlags flags = 0) noexcept:
            PipelineShaderStage(VK_SHADER_STAGE_VERTEX_BIT, std::move(module), entrypoint, std::move(specialization), flags) {}
    };

    class TesselationControlShaderStage : public PipelineShaderStage
    {
    public:
        explicit TesselationControlShaderStage(std::shared_ptr<ShaderModule> module, const char *const entrypoint,
            std::shared_ptr<Specialization> specialization = nullptr, VkPipelineShaderStageCreateFlags flags = 0) noexcept:
            PipelineShaderStage(VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT, std::move(module), entrypoint, std::move(specialization), flags) {}
    };

    class TesselationEvaluationShaderStage : public PipelineShaderStage
    {
    public:
        explicit TesselationEvaluationShaderStage(std::shared_ptr<ShaderModule> module, const char *const entrypoint,
            std::shared_ptr<Specialization> specialization = nullptr, VkPipelineShaderStageCreateFlags flags = 0) noexcept:
            PipelineShaderStage(VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, std::move(module), entrypoint, std::move(specialization), flags) {}
    };

    class GeometryShaderStage : public PipelineShaderStage
    {
    public:
        explicit GeometryShaderStage(std::shared_ptr<ShaderModule> module, const char *const entrypoint,
            std::shared_ptr<Specialization> specialization = nullptr, VkPipelineShaderStageCreateFlags flags = 0) noexcept:
            PipelineShaderStage(VK_SHADER_STAGE_GEOMETRY_BIT, std::move(module), entrypoint, std::move(specialization), flags) {}
    };

    class FragmentShaderStage : public PipelineShaderStage
    {
    public:
        explicit FragmentShaderStage(std::shared_ptr<ShaderModule> module, const char *const entrypoint,
            std::shared_ptr<Specialization> specialization = nullptr, VkPipelineShaderStageCreateFlags flags = 0) noexcept:
            PipelineShaderStage(VK_SHADER_STAGE_FRAGMENT_BIT, std::move(module), entrypoint, std::move(specialization), flags) {}
    };

    class ComputeShaderStage : public PipelineShaderStage
    {
    public:
        explicit ComputeShaderStage(std::shared_ptr<ShaderModule> module, const char *const entrypoint,
            std::shared_ptr<Specialization> specialization = nullptr, VkPipelineShaderStageCreateFlags flags = 0) noexcept:
            PipelineShaderStage(VK_SHADER_STAGE_COMPUTE_BIT, std::move(module), entrypoint, std::move(specialization), flags) {}
    };

#ifdef VK_NV_ray_tracing
    class RaygenShaderStage : public PipelineShaderStage
    {
    public:
        explicit RaygenShaderStage(std::shared_ptr<ShaderModule> module, const char *const entrypoint,
            std::shared_ptr<Specialization> specialization = nullptr, VkPipelineShaderStageCreateFlags flags = 0) noexcept:
            PipelineShaderStage(VK_SHADER_STAGE_RAYGEN_BIT_NV, std::move(module), entrypoint, std::move(specialization), flags) {}
    };

    class AnyHitShaderStage : public PipelineShaderStage
    {
    public:
        explicit AnyHitShaderStage(std::shared_ptr<ShaderModule> module, const char *const entrypoint,
            std::shared_ptr<Specialization> specialization = nullptr, VkPipelineShaderStageCreateFlags flags = 0) noexcept:
            PipelineShaderStage(VK_SHADER_STAGE_ANY_HIT_BIT_NV, std::move(module), entrypoint, std::move(specialization), flags) {}
    };

    class ClosestHitShaderStage : public PipelineShaderStage
    {
    public:
        explicit ClosestHitShaderStage(std::shared_ptr<ShaderModule> module, const char *const entrypoint,
            std::shared_ptr<Specialization> specialization = nullptr, VkPipelineShaderStageCreateFlags flags = 0) noexcept:
            PipelineShaderStage(VK_SHADER_STAGE_CLOSEST_HIT_BIT_NV, std::move(module), entrypoint, std::move(specialization), flags) {}
    };

    class MissShaderStage : public PipelineShaderStage
    {
    public:
        explicit MissShaderStage(std::shared_ptr<ShaderModule> module, const char *const entrypoint,
            std::shared_ptr<Specialization> specialization = nullptr, VkPipelineShaderStageCreateFlags flags = 0) noexcept:
            PipelineShaderStage(VK_SHADER_STAGE_MISS_BIT_NV, std::move(module), entrypoint, std::move(specialization), flags) {}
    };

    class IntersectionShaderStage : public PipelineShaderStage
    {
    public:
        explicit IntersectionShaderStage(std::shared_ptr<ShaderModule> module, const char *const entrypoint,
            std::shared_ptr<Specialization> specialization = nullptr, VkPipelineShaderStageCreateFlags flags = 0) noexcept:
            PipelineShaderStage(VK_SHADER_STAGE_INTERSECTION_BIT_NV, std::move(module), entrypoint, std::move(specialization), flags) {}
    };

    class CallableShaderStage : public PipelineShaderStage
    {
    public:
        explicit CallableShaderStage(std::shared_ptr<ShaderModule> module, const char *const entrypoint,
            std::shared_ptr<Specialization> specialization = nullptr, VkPipelineShaderStageCreateFlags flags = 0) noexcept:
            PipelineShaderStage(VK_SHADER_STAGE_CALLABLE_BIT_NV, std::move(module), entrypoint, std::move(specialization), flags) {}
    };
#endif // VK_NV_ray_tracing

#ifdef VK_NV_mesh_shader
    class TaskShaderStage : public PipelineShaderStage
    {
    public:
        explicit TaskShaderStage(std::shared_ptr<ShaderModule> module, const char *const entrypoint,
            std::shared_ptr<Specialization> specialization = nullptr, VkPipelineShaderStageCreateFlags flags = 0) noexcept:
            PipelineShaderStage(VK_SHADER_STAGE_TASK_BIT_NV, std::move(module), entrypoint, std::move(specialization), flags) {}
    };

    class MeshShaderStage : public PipelineShaderStage
    {
    public:
        explicit MeshShaderStage(std::shared_ptr<ShaderModule> module, const char *const entrypoint,
            std::shared_ptr<Specialization> specialization = nullptr, VkPipelineShaderStageCreateFlags flags = 0) noexcept:
            PipelineShaderStage(VK_SHADER_STAGE_MESH_BIT_NV, std::move(module), entrypoint, std::move(specialization), flags) {}
    };
#endif // VK_NV_mesh_shader
} // namespace magma
