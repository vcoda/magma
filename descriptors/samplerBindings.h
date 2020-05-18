/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2019 Victor Coda.

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

        struct VertexStageSamplerBinding : DescriptorSetLayout::SamplerBinding
        {
            VertexStageSamplerBinding(const uint32_t binding, const Descriptor& descriptor, const ImmutableSamplerList& immutableSamplers) noexcept:
                DescriptorSetLayout::SamplerBinding(binding, descriptor, VK_SHADER_STAGE_VERTEX_BIT, immutableSamplers) {}
        };

        struct TesselationControlStageSamplerBinding : DescriptorSetLayout::SamplerBinding
        {
            TesselationControlStageSamplerBinding(const uint32_t binding, const Descriptor& descriptor, const ImmutableSamplerList& immutableSamplers) noexcept:
                DescriptorSetLayout::SamplerBinding(binding, descriptor, VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT, immutableSamplers) {}
        };

        struct TesselationEvaluationStageSamplerBinding : DescriptorSetLayout::SamplerBinding
        {
            TesselationEvaluationStageSamplerBinding(const uint32_t binding, const Descriptor& descriptor, const ImmutableSamplerList& immutableSamplers) noexcept:
                DescriptorSetLayout::SamplerBinding(binding, descriptor, VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, immutableSamplers) {}
        };

        struct GeometryStageSamplerBinding : DescriptorSetLayout::SamplerBinding
        {
            GeometryStageSamplerBinding(const uint32_t binding, const Descriptor& descriptor, const ImmutableSamplerList& immutableSamplers) noexcept:
                DescriptorSetLayout::SamplerBinding(binding, descriptor, VK_SHADER_STAGE_GEOMETRY_BIT, immutableSamplers) {}
        };

        struct FragmentStageSamplerBinding : DescriptorSetLayout::SamplerBinding
        {
            FragmentStageSamplerBinding(const uint32_t binding, const Descriptor& descriptor, const ImmutableSamplerList& immutableSamplers) noexcept:
                DescriptorSetLayout::SamplerBinding(binding, descriptor, VK_SHADER_STAGE_FRAGMENT_BIT, immutableSamplers) {}
        };

        struct ComputeStageSamplerBinding : DescriptorSetLayout::SamplerBinding
        {
            ComputeStageSamplerBinding(const uint32_t binding, const Descriptor& descriptor, const ImmutableSamplerList& immutableSamplers) noexcept:
                DescriptorSetLayout::SamplerBinding(binding, descriptor, VK_SHADER_STAGE_COMPUTE_BIT, immutableSamplers) {}
        };

        struct GraphicsStageSamplerBinding : DescriptorSetLayout::SamplerBinding
        {
            GraphicsStageSamplerBinding(const uint32_t binding, const Descriptor& descriptor, const ImmutableSamplerList& immutableSamplers) noexcept:
                DescriptorSetLayout::SamplerBinding(binding, descriptor, VK_SHADER_STAGE_ALL_GRAPHICS, immutableSamplers) {}
        };

        struct AllStagesSamplerBinding : DescriptorSetLayout::SamplerBinding
        {
            AllStagesSamplerBinding(const uint32_t binding, const Descriptor& descriptor, const ImmutableSamplerList& immutableSamplers) noexcept:
                DescriptorSetLayout::SamplerBinding(binding, descriptor, VK_SHADER_STAGE_ALL, immutableSamplers) {}
        };

        struct VertexGeometryStageSamplerBinding : DescriptorSetLayout::SamplerBinding
        {
            VertexGeometryStageSamplerBinding(const uint32_t binding, const Descriptor& descriptor, const ImmutableSamplerList& immutableSamplers) noexcept:
                DescriptorSetLayout::SamplerBinding(binding, descriptor, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_GEOMETRY_BIT, immutableSamplers) {}
        };

        struct VertexFragmentStageSamplerBinding : DescriptorSetLayout::SamplerBinding
        {
            VertexFragmentStageSamplerBinding(const uint32_t binding, const Descriptor& descriptor, const ImmutableSamplerList& immutableSamplers) noexcept:
                DescriptorSetLayout::SamplerBinding(binding, descriptor, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, immutableSamplers) {}
        };
    } // namespace bindings
} // namespace magma
