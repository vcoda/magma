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
#include "pipelineShaderStage.h"

namespace magma
{
#define MAGMA_DEFINE_PIPELINE_SHADER_STAGE(ShaderStage, stage)\
    class ShaderStage : public PipelineShaderStage\
    {\
    public:\
        explicit ShaderStage(std::shared_ptr<ShaderModule> module,\
            const char *entrypoint,\
            std::shared_ptr<Specialization> specialization = nullptr,\
            VkPipelineShaderStageCreateFlags flags = 0) noexcept:\
            PipelineShaderStage(stage, std::move(module), entrypoint, std::move(specialization), flags) {}\
    };

    MAGMA_DEFINE_PIPELINE_SHADER_STAGE(VertexShaderStage, VK_SHADER_STAGE_VERTEX_BIT)
    MAGMA_DEFINE_PIPELINE_SHADER_STAGE(TesselationControlShaderStage, VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT)
    MAGMA_DEFINE_PIPELINE_SHADER_STAGE(TesselationEvaluationShaderStage, VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT)
    MAGMA_DEFINE_PIPELINE_SHADER_STAGE(GeometryShaderStage, VK_SHADER_STAGE_GEOMETRY_BIT)
    MAGMA_DEFINE_PIPELINE_SHADER_STAGE(FragmentShaderStage, VK_SHADER_STAGE_FRAGMENT_BIT)
    MAGMA_DEFINE_PIPELINE_SHADER_STAGE(ComputeShaderStage, VK_SHADER_STAGE_COMPUTE_BIT)
#ifdef VK_KHR_ray_tracing_pipeline
    MAGMA_DEFINE_PIPELINE_SHADER_STAGE(RaygenShaderStage, VK_SHADER_STAGE_RAYGEN_BIT_KHR)
    MAGMA_DEFINE_PIPELINE_SHADER_STAGE(AnyHitShaderStage, VK_SHADER_STAGE_ANY_HIT_BIT_KHR)
    MAGMA_DEFINE_PIPELINE_SHADER_STAGE(ClosestHitShaderStage, VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR)
    MAGMA_DEFINE_PIPELINE_SHADER_STAGE(MissShaderStage, VK_SHADER_STAGE_MISS_BIT_KHR)
    MAGMA_DEFINE_PIPELINE_SHADER_STAGE(IntersectionShaderStage, VK_SHADER_STAGE_INTERSECTION_BIT_KHR)
    MAGMA_DEFINE_PIPELINE_SHADER_STAGE(CallableShaderStage, VK_SHADER_STAGE_CALLABLE_BIT_KHR)
#endif // VK_KHR_ray_tracing_pipeline
#ifdef VK_EXT_mesh_shader
    MAGMA_DEFINE_PIPELINE_SHADER_STAGE(TaskShaderStage, VK_SHADER_STAGE_TASK_BIT_EXT)
    MAGMA_DEFINE_PIPELINE_SHADER_STAGE(MeshShaderStage, VK_SHADER_STAGE_MESH_BIT_EXT)
#endif // VK_EXT_mesh_shader
} // namespace magma
