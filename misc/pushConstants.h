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

namespace magma
{
    namespace pushconstants
    {
        /* Defines a set of push constant ranges for use in a single pipeline layout. */

        template<typename Type>
        struct PushConstantRange : VkPushConstantRange
        {
            constexpr PushConstantRange(VkShaderStageFlags flags, uint32_t rangeOffset = 0) noexcept
            {
                stageFlags = flags;
                offset = rangeOffset;
                size = sizeof(Type);
            }
        };

        template<typename Type>
        struct VertexConstantRange : PushConstantRange<Type>
        {
            constexpr VertexConstantRange(uint32_t offset = 0) noexcept:
                PushConstantRange<Type>(VK_SHADER_STAGE_VERTEX_BIT, offset) {}
        };

        template<typename Type>
        struct TesselationControlConstantRange : PushConstantRange<Type>
        {
            constexpr TesselationControlConstantRange(uint32_t offset = 0) noexcept:
                PushConstantRange<Type>(VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT, offset) {}
        };

        template<typename Type>
        struct TesselationEvaluationConstantRange : PushConstantRange<Type>
        {
            constexpr TesselationEvaluationConstantRange(uint32_t offset = 0) noexcept:
                PushConstantRange<Type>(VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, offset) {}
        };

        template<typename Type>
        struct GeometryConstantRange : PushConstantRange<Type>
        {
            constexpr GeometryConstantRange(uint32_t offset = 0) noexcept:
                PushConstantRange<Type>(VK_SHADER_STAGE_GEOMETRY_BIT, offset) {}
        };

        template<typename Type>
        struct FragmentConstantRange : PushConstantRange<Type>
        {
            constexpr FragmentConstantRange(uint32_t offset = 0) noexcept:
                PushConstantRange<Type>(VK_SHADER_STAGE_FRAGMENT_BIT, offset) {}
        };

        template<typename Type>
        struct ComputeConstantRange : PushConstantRange<Type>
        {
            constexpr ComputeConstantRange(uint32_t offset = 0) noexcept:
                PushConstantRange<Type>(VK_SHADER_STAGE_COMPUTE_BIT, offset) {}
        };

        template<typename Type>
        struct GraphicsConstantRange : PushConstantRange<Type>
        {
            constexpr GraphicsConstantRange(uint32_t offset = 0) noexcept:
                PushConstantRange<Type>(VK_SHADER_STAGE_ALL_GRAPHICS, offset) {}
        };

        template<typename Type>
        struct AllConstantRange : PushConstantRange<Type>
        {
            constexpr AllConstantRange(uint32_t offset = 0) noexcept:
                PushConstantRange<Type>(VK_SHADER_STAGE_ALL, offset) {}
        };

        template<typename Type>
        struct VertexGeometryConstantRange : PushConstantRange<Type>
        {
            constexpr VertexGeometryConstantRange(uint32_t offset = 0) noexcept:
                PushConstantRange<Type>(VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_GEOMETRY_BIT, offset) {}
        };

        template<typename Type>
        struct VertexFragmentConstantRange : PushConstantRange<Type>
        {
            constexpr VertexFragmentConstantRange(uint32_t offset = 0) noexcept:
                PushConstantRange<Type>(VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, offset) {}
        };
    } // namespace pushconstants
} // namespace magma
