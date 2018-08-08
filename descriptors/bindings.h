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
#include "../objects/descriptorSetLayout.h"

namespace magma
{
    namespace bindings
    {
        /* Each individual descriptor binding is specified by a descriptor type, a count (array size)
           of the number of descriptors in the binding, a set of shader stages that can access the binding,
           and (if using immutable samplers) an array of sampler descriptors. */

        struct VertexStageBinding : DescriptorSetLayout::Binding
        {
            VertexStageBinding(uint32_t binding, const Descriptor& descriptor) noexcept:
                DescriptorSetLayout::Binding(binding, descriptor, VK_SHADER_STAGE_VERTEX_BIT) {}
        };

        struct TesselationControlStageBinding : DescriptorSetLayout::Binding
        {
            TesselationControlStageBinding(uint32_t binding, const Descriptor& descriptor) noexcept:
                DescriptorSetLayout::Binding(binding, descriptor, VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT) {}
        };

        struct TesselationEvaluationStageBinding : DescriptorSetLayout::Binding
        {
            TesselationEvaluationStageBinding(uint32_t binding, const Descriptor& descriptor) noexcept:
                DescriptorSetLayout::Binding(binding, descriptor, VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT) {}
        };

        struct GeometryStageBinding : DescriptorSetLayout::Binding
        {
            GeometryStageBinding(uint32_t binding, const Descriptor& descriptor) noexcept:
                DescriptorSetLayout::Binding(binding, descriptor, VK_SHADER_STAGE_GEOMETRY_BIT) {}
        };

        struct FragmentStageBinding : DescriptorSetLayout::Binding
        {
            FragmentStageBinding(uint32_t binding, const Descriptor& descriptor) noexcept:
                DescriptorSetLayout::Binding(binding, descriptor, VK_SHADER_STAGE_FRAGMENT_BIT) {}
        };

        struct ComputeStageBinding : DescriptorSetLayout::Binding
        {
            ComputeStageBinding(uint32_t binding, const Descriptor& descriptor) noexcept:
                DescriptorSetLayout::Binding(binding, descriptor, VK_SHADER_STAGE_COMPUTE_BIT) {}
        };

        struct GraphicsStageBinding : DescriptorSetLayout::Binding
        {
            GraphicsStageBinding(uint32_t binding, const Descriptor& descriptor) noexcept:
                DescriptorSetLayout::Binding(binding, descriptor, VK_SHADER_STAGE_ALL_GRAPHICS) {}
        };

        struct AllStagesBinding : DescriptorSetLayout::Binding
        {
            AllStagesBinding(uint32_t binding, const Descriptor& descriptor) noexcept:
                DescriptorSetLayout::Binding(binding, descriptor, VK_SHADER_STAGE_ALL) {}
        };

        struct VertexGeometryStageBinding : DescriptorSetLayout::Binding
        {
            VertexGeometryStageBinding(uint32_t binding, const Descriptor& descriptor) noexcept:
                DescriptorSetLayout::Binding(binding, descriptor, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_GEOMETRY_BIT) {}
        };

        struct VertexFragmentStageBinding : DescriptorSetLayout::Binding
        {
            VertexFragmentStageBinding(uint32_t binding, const Descriptor& descriptor) noexcept:
                DescriptorSetLayout::Binding(binding, descriptor, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT) {}
        };
    } // namespace bindings
} // namespace magma
