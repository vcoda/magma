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
#include "nondispatchable.h"
#include "../misc/pushConstantRange.h"

namespace magma
{
    class DescriptorSetLayout;

    /* Access to descriptor sets from a pipeline is accomplished through a pipeline layout.
       Zero or more descriptor set layouts and zero or more push constant ranges are combined
       to form a pipeline layout object which describes the complete set of resources that can be accessed by a pipeline.
       The pipeline layout represents a sequence of descriptor sets with each having a specific layout.
       This sequence of layouts is used to determine the interface between shader stages and shader resources.
       Each pipeline is created using a pipeline layout. */

    class PipelineLayout : public NonDispatchable<VkPipelineLayout>
    {
    public:
        // Delegates
        explicit PipelineLayout(std::shared_ptr<Device> device,
            std::shared_ptr<IAllocator> allocator = nullptr);
        explicit PipelineLayout(std::shared_ptr<const DescriptorSetLayout> setLayout,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkPipelineLayoutCreateFlags flags = 0);
        explicit PipelineLayout(std::shared_ptr<Device> device,
            const PushConstantRange& pushConstantRange,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkPipelineLayoutCreateFlags flags = 0);
        explicit PipelineLayout(std::shared_ptr<const DescriptorSetLayout> setLayout,
            const PushConstantRange& pushConstantRange,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkPipelineLayoutCreateFlags flags = 0);
        explicit PipelineLayout(const std::initializer_list<std::shared_ptr<const DescriptorSetLayout>>& setLayouts,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkPipelineLayoutCreateFlags flags = 0);
        // Implementation
        explicit PipelineLayout(std::shared_ptr<Device> device,
            const std::initializer_list<PushConstantRange>& pushConstantRanges,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkPipelineLayoutCreateFlags flags = 0);
        explicit PipelineLayout(std::shared_ptr<const DescriptorSetLayout> setLayout,
            const std::initializer_list<PushConstantRange>& pushConstantRanges,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkPipelineLayoutCreateFlags flags = 0);
        explicit PipelineLayout(const std::initializer_list<std::shared_ptr<const DescriptorSetLayout>>& setLayouts,
            const std::initializer_list<PushConstantRange>& pushConstantRanges,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkPipelineLayoutCreateFlags flags = 0);
        ~PipelineLayout();
        bool hasLayout(std::shared_ptr<const DescriptorSetLayout> setLayout) const noexcept;
        hash_t getHash() const noexcept;

    private:
        std::unordered_map<VkDescriptorSetLayout, hash_t> descriptorSetLayouts;
        hash_t hash;
    };
} // namespace magma

#include "pipelineLayout.inl"
