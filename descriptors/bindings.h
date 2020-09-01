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
#include "../objects/descriptorSetLayout.h"

namespace magma
{
    namespace bindings
    {
        /* Each individual descriptor binding is specified by a descriptor type, a count (array size)
           of the number of descriptors in the binding and a set of shader stages that can access the binding. */

        struct VertexStageBinding : DescriptorSetLayout::Binding
        {
            constexpr VertexStageBinding(const uint32_t binding, const Descriptor& descriptor) noexcept:
                DescriptorSetLayout::Binding(binding, descriptor, VK_SHADER_STAGE_VERTEX_BIT) {}
        };

        struct TesselationControlStageBinding : DescriptorSetLayout::Binding
        {
            constexpr TesselationControlStageBinding(const uint32_t binding, const Descriptor& descriptor) noexcept:
                DescriptorSetLayout::Binding(binding, descriptor, VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT) {}
        };

        struct TesselationEvaluationStageBinding : DescriptorSetLayout::Binding
        {
            constexpr TesselationEvaluationStageBinding(const uint32_t binding, const Descriptor& descriptor) noexcept:
                DescriptorSetLayout::Binding(binding, descriptor, VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT) {}
        };

        struct GeometryStageBinding : DescriptorSetLayout::Binding
        {
            constexpr GeometryStageBinding(const uint32_t binding, const Descriptor& descriptor) noexcept:
                DescriptorSetLayout::Binding(binding, descriptor, VK_SHADER_STAGE_GEOMETRY_BIT) {}
        };

        struct FragmentStageBinding : DescriptorSetLayout::Binding
        {
            constexpr FragmentStageBinding(const uint32_t binding, const Descriptor& descriptor) noexcept:
                DescriptorSetLayout::Binding(binding, descriptor, VK_SHADER_STAGE_FRAGMENT_BIT) {}
        };

        struct ComputeStageBinding : DescriptorSetLayout::Binding
        {
            constexpr ComputeStageBinding(const uint32_t binding, const Descriptor& descriptor) noexcept:
                DescriptorSetLayout::Binding(binding, descriptor, VK_SHADER_STAGE_COMPUTE_BIT) {}
        };

        struct GraphicsStageBinding : DescriptorSetLayout::Binding
        {
            constexpr GraphicsStageBinding(const uint32_t binding, const Descriptor& descriptor) noexcept:
                DescriptorSetLayout::Binding(binding, descriptor, VK_SHADER_STAGE_ALL_GRAPHICS) {}
        };

        struct AllStagesBinding : DescriptorSetLayout::Binding
        {
            constexpr AllStagesBinding(const uint32_t binding, const Descriptor& descriptor) noexcept:
                DescriptorSetLayout::Binding(binding, descriptor, VK_SHADER_STAGE_ALL) {}
        };

        struct VertexGeometryStageBinding : DescriptorSetLayout::Binding
        {
            constexpr VertexGeometryStageBinding(const uint32_t binding, const Descriptor& descriptor) noexcept:
                DescriptorSetLayout::Binding(binding, descriptor, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_GEOMETRY_BIT) {}
        };

        struct VertexFragmentStageBinding : DescriptorSetLayout::Binding
        {
            constexpr VertexFragmentStageBinding(const uint32_t binding, const Descriptor& descriptor) noexcept:
                DescriptorSetLayout::Binding(binding, descriptor, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT) {}
        };
    } // namespace bindings
} // namespace magma
