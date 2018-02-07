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
        struct VertexStageBinding : DescriptorSetLayout::Binding
        {
            VertexStageBinding(uint32_t binding, const Descriptor& descriptor): DescriptorSetLayout::Binding(binding, descriptor, VK_SHADER_STAGE_VERTEX_BIT) {}
        };

        struct GeometryStageBinding : DescriptorSetLayout::Binding
        {
            GeometryStageBinding(uint32_t binding, const Descriptor& descriptor): DescriptorSetLayout::Binding(binding, descriptor, VK_SHADER_STAGE_GEOMETRY_BIT) {}
        };

        struct FragmentStageBinding : DescriptorSetLayout::Binding
        {
            FragmentStageBinding(uint32_t binding, const Descriptor& descriptor): DescriptorSetLayout::Binding(binding, descriptor, VK_SHADER_STAGE_FRAGMENT_BIT) {}
        };

        struct VertexGeometryStageBinding : DescriptorSetLayout::Binding
        {
            VertexGeometryStageBinding(uint32_t binding, const Descriptor& descriptor):
                DescriptorSetLayout::Binding(binding, descriptor, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_GEOMETRY_BIT) {}
        };

        struct VertexFragmentStageBinding : DescriptorSetLayout::Binding
        {
            VertexFragmentStageBinding(uint32_t binding, const Descriptor& descriptor): 
                DescriptorSetLayout::Binding(binding, descriptor, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT) {}
        };

        struct ComputeStageBinding : DescriptorSetLayout::Binding
        {
            ComputeStageBinding(uint32_t binding, const Descriptor& descriptor): DescriptorSetLayout::Binding(binding, descriptor, VK_SHADER_STAGE_COMPUTE_BIT) {}
        };

        struct AllGraphicsStageBinding : DescriptorSetLayout::Binding
        {
            AllGraphicsStageBinding(uint32_t binding, const Descriptor& descriptor): DescriptorSetLayout::Binding(binding, descriptor, VK_SHADER_STAGE_ALL_GRAPHICS) {}
        };

        struct AllStageBinding : DescriptorSetLayout::Binding
        {
            AllStageBinding(uint32_t binding, const Descriptor& descriptor): DescriptorSetLayout::Binding(binding, descriptor, VK_SHADER_STAGE_ALL) {}
        };
    } // namespace bindings
} // namespace magma
