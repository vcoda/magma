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

namespace magma
{
    /* Push constants represent a high speed path to modify
       constant data in pipelines that is expected to outperform
       memory-backed resource updates. */

    struct PushConstantRange : VkPushConstantRange
    {
        constexpr PushConstantRange(VkShaderStageFlags stageFlags,
            uint32_t offset,
            std::size_t size) noexcept;
        constexpr hash_t hash() const noexcept;
    };
} // namespace magma

#include "pushConstantRange.inl"

namespace magma
{
    namespace push
    {
        /* Defines stage flags describing the shader stages that
           will access a range of push constants. If a particular
           stage is not included in the range, then accessing members
           of that range of push constants from the corresponding
           shader stage will return undefined values. */

        template<class Type>
        struct PushConstantRange : magma::PushConstantRange
        {
            constexpr PushConstantRange(const VkShaderStageFlags flags, const uint32_t offset = 0) noexcept:
                magma::PushConstantRange(flags, offset, sizeof(Type)) {}
        };

        template<class Type>
        struct VertexConstantRange : PushConstantRange<Type>
        {
            constexpr VertexConstantRange(const uint32_t offset = 0) noexcept:
                PushConstantRange<Type>(VK_SHADER_STAGE_VERTEX_BIT, offset) {}
        };

        template<class Type>
        struct TesselationControlConstantRange : PushConstantRange<Type>
        {
            constexpr TesselationControlConstantRange(const uint32_t offset = 0) noexcept:
                PushConstantRange<Type>(VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT, offset) {}
        };

        template<class Type>
        struct TesselationEvaluationConstantRange : PushConstantRange<Type>
        {
            constexpr TesselationEvaluationConstantRange(const uint32_t offset = 0) noexcept:
                PushConstantRange<Type>(VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, offset) {}
        };

        template<class Type>
        struct GeometryConstantRange : PushConstantRange<Type>
        {
            constexpr GeometryConstantRange(const uint32_t offset = 0) noexcept:
                PushConstantRange<Type>(VK_SHADER_STAGE_GEOMETRY_BIT, offset) {}
        };

        template<class Type>
        struct FragmentConstantRange : PushConstantRange<Type>
        {
            constexpr FragmentConstantRange(const uint32_t offset = 0) noexcept:
                PushConstantRange<Type>(VK_SHADER_STAGE_FRAGMENT_BIT, offset) {}
        };

        template<class Type>
        struct ComputeConstantRange : PushConstantRange<Type>
        {
            constexpr ComputeConstantRange(const uint32_t offset = 0) noexcept:
                PushConstantRange<Type>(VK_SHADER_STAGE_COMPUTE_BIT, offset) {}
        };

        template<class Type>
        struct GraphicsConstantRange : PushConstantRange<Type>
        {
            constexpr GraphicsConstantRange(const uint32_t offset = 0) noexcept:
                PushConstantRange<Type>(VK_SHADER_STAGE_ALL_GRAPHICS, offset) {}
        };

        template<class Type>
        struct AllConstantRange : PushConstantRange<Type>
        {
            constexpr AllConstantRange(const uint32_t offset = 0) noexcept:
                PushConstantRange<Type>(VK_SHADER_STAGE_ALL, offset) {}
        };

        template<class Type>
        struct VertexGeometryConstantRange : PushConstantRange<Type>
        {
            constexpr VertexGeometryConstantRange(const uint32_t offset = 0) noexcept:
                PushConstantRange<Type>(VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_GEOMETRY_BIT, offset) {}
        };

        template<class Type>
        struct VertexFragmentConstantRange : PushConstantRange<Type>
        {
            constexpr VertexFragmentConstantRange(const uint32_t offset = 0) noexcept:
                PushConstantRange<Type>(VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, offset) {}
        };

    #ifdef VK_EXT_mesh_shader
        template<class Type>
        struct TaskConstantRange : PushConstantRange<Type>
        {
            constexpr TaskConstantRange(const uint32_t offset = 0) noexcept:
                PushConstantRange<Type>(VK_SHADER_STAGE_TASK_BIT_EXT, offset) {}
        };

        template<class Type>
        struct MeshConstantRange : PushConstantRange<Type>
        {
            constexpr MeshConstantRange(const uint32_t offset = 0) noexcept:
                PushConstantRange<Type>(VK_SHADER_STAGE_MESH_BIT_EXT, offset) {}
        };

        template<class Type>
        struct TaskMeshConstantRange : PushConstantRange<Type>
        {
            constexpr TaskMeshConstantRange(const uint32_t offset = 0) noexcept:
                PushConstantRange<Type>(VK_SHADER_STAGE_TASK_BIT_EXT | VK_SHADER_STAGE_MESH_BIT_EXT, offset) {}
        };

        template<class Type>
        struct TaskMeshFragmentConstantRange : PushConstantRange<Type>
        {
            constexpr TaskMeshFragmentConstantRange(const uint32_t offset = 0) noexcept:
                PushConstantRange<Type>(VK_SHADER_STAGE_TASK_BIT_EXT | VK_SHADER_STAGE_MESH_BIT_EXT | VK_SHADER_STAGE_FRAGMENT_BIT, offset) {}
        };
    #endif // VK_EXT_mesh_shader
    } // namespace push
} // namespace magma
