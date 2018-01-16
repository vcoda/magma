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
#include <vector>
#include "handle.h"
#include "../descriptors/descriptors.h"

namespace magma
{
    class Device;

    class DescriptorSetLayout : public NonDispatchable<VkDescriptorSetLayout>
    {
    public:
        struct Binding : VkDescriptorSetLayoutBinding
        {
            Binding(uint32_t binding,
                const Descriptor& descriptor,
                VkShaderStageFlags stageFlags);
        };

        struct VertexStageBinding : Binding
        {
            VertexStageBinding(uint32_t binding, const Descriptor& descriptor): Binding(binding, descriptor, VK_SHADER_STAGE_VERTEX_BIT) {}
        };

        struct GeometryStageBinding : Binding
        {
            GeometryStageBinding(uint32_t binding, const Descriptor& descriptor): Binding(binding, descriptor, VK_SHADER_STAGE_GEOMETRY_BIT) {}
        };

        struct FragmentStageBinding : Binding
        {
            FragmentStageBinding(uint32_t binding, const Descriptor& descriptor): Binding(binding, descriptor, VK_SHADER_STAGE_FRAGMENT_BIT) {}
        };

        struct VertexFragmentStageBinding : Binding
        {
            VertexFragmentStageBinding(uint32_t binding, const Descriptor& descriptor): Binding(binding, descriptor, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT) {}
        };

        struct ComputeStageBinding : Binding
        {
            ComputeStageBinding(uint32_t binding, const Descriptor& descriptor): Binding(binding, descriptor, VK_SHADER_STAGE_COMPUTE_BIT) {}
        };

        struct AllGraphicsStageBinding : Binding
        {
            AllGraphicsStageBinding(uint32_t binding, const Descriptor& descriptor): Binding(binding, descriptor, VK_SHADER_STAGE_ALL_GRAPHICS) {}
        };

        struct AllStageBinding : Binding
        {
            AllStageBinding(uint32_t binding, const Descriptor& descriptor): Binding(binding, descriptor, VK_SHADER_STAGE_ALL) {}
        };

    public:
        DescriptorSetLayout(std::shared_ptr<const Device> device,
            const Binding& binding);
        DescriptorSetLayout(std::shared_ptr<const Device> device,
            const std::initializer_list<Binding>& bindings);
        ~DescriptorSetLayout();
    };
} // namespace magma
