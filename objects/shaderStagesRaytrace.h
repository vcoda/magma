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
            PipelineShaderStage(VK_SHADER_STAGE_RAYGEN_BIT_NVX, std::move(module), entrypoint, std::move(specialization), flags)
        {}
    };

    class AnyHitShaderStage : public PipelineShaderStage
    {
    public:
        explicit AnyHitShaderStage(std::shared_ptr<const ShaderModule> module,
            const char *const entrypoint,
            std::shared_ptr<const Specialization> specialization = nullptr,
            VkPipelineShaderStageCreateFlags flags = 0) noexcept:
            PipelineShaderStage(VK_SHADER_STAGE_ANY_HIT_BIT_NVX, std::move(module), entrypoint, std::move(specialization), flags)
        {}
    };

    class ClosestHitShaderStage : public PipelineShaderStage
    {
    public:
        explicit ClosestHitShaderStage(std::shared_ptr<const ShaderModule> module,
            const char *const entrypoint,
            std::shared_ptr<const Specialization> specialization = nullptr,
            VkPipelineShaderStageCreateFlags flags = 0) noexcept:
            PipelineShaderStage(VK_SHADER_STAGE_CLOSEST_HIT_BIT_NVX, std::move(module), entrypoint, std::move(specialization), flags)
        {}
    };

    class MissShaderStage : public PipelineShaderStage
    {
    public:
        explicit MissShaderStage(std::shared_ptr<const ShaderModule> module,
            const char *const entrypoint,
            std::shared_ptr<const Specialization> specialization = nullptr,
            VkPipelineShaderStageCreateFlags flags = 0) noexcept:
            PipelineShaderStage(VK_SHADER_STAGE_MISS_BIT_NVX, std::move(module), entrypoint, std::move(specialization), flags)
        {}
    };

    class IntersectionShaderStage : public PipelineShaderStage
    {
    public:
        explicit IntersectionShaderStage(std::shared_ptr<const ShaderModule> module,
            const char *const entrypoint,
            std::shared_ptr<const Specialization> specialization = nullptr,
            VkPipelineShaderStageCreateFlags flags = 0) noexcept:
            PipelineShaderStage(VK_SHADER_STAGE_INTERSECTION_BIT_NVX, std::move(module), entrypoint, std::move(specialization), flags)
        {}
    };

    class CallableShaderStage : public PipelineShaderStage
    {
    public:
        explicit CallableShaderStage(std::shared_ptr<const ShaderModule> module,
            const char *const entrypoint,
            std::shared_ptr<const Specialization> specialization = nullptr,
            VkPipelineShaderStageCreateFlags flags = 0) noexcept:
            PipelineShaderStage(VK_SHADER_STAGE_CALLABLE_BIT_NVX, std::move(module), entrypoint, std::move(specialization), flags)
        {}
    };
} // namespace magma
