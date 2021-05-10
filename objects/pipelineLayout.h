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
#include "nondispatchable.h"
#include "../misc/pushConstants.h"

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
        explicit PipelineLayout(std::shared_ptr<Device> device,
            const PushConstantRange& pushConstantRange,
            std::shared_ptr<IAllocator> allocator = nullptr);
        explicit PipelineLayout(std::shared_ptr<Device> device,
            std::shared_ptr<IAllocator> allocator = nullptr,
            const std::initializer_list<PushConstantRange>& pushConstantRanges = {});
        explicit PipelineLayout(std::shared_ptr<DescriptorSetLayout> setLayout,
            const PushConstantRange& pushConstantRange,
            std::shared_ptr<IAllocator> allocator = nullptr);
        explicit PipelineLayout(std::shared_ptr<DescriptorSetLayout> setLayout,
            std::shared_ptr<IAllocator> allocator = nullptr,
            const std::initializer_list<PushConstantRange>& pushConstantRanges = {});
        explicit PipelineLayout(const std::vector<std::shared_ptr<DescriptorSetLayout>>& setLayouts,
            std::shared_ptr<IAllocator> allocator = nullptr,
            const std::initializer_list<PushConstantRange>& pushConstantRanges = {});
        template<uint32_t setLayoutCount>
        explicit PipelineLayout(const std::shared_ptr<DescriptorSetLayout>(&setLayouts)[setLayoutCount],
            std::shared_ptr<IAllocator> allocator = nullptr,
            const std::initializer_list<PushConstantRange>& pushConstantRanges = {}):
            PipelineLayout(std::vector<std::shared_ptr<DescriptorSetLayout>>(setLayouts, setLayouts + setLayoutCount), std::move(allocator), pushConstantRanges) {}
        ~PipelineLayout();
        const std::vector<std::shared_ptr<DescriptorSetLayout>>& getSetLayouts() const noexcept { return setLayouts; }
        bool hasSetLayout(std::shared_ptr<DescriptorSetLayout> setLayout) const noexcept;
        std::size_t getHash() const noexcept;

    private:
        std::vector<std::shared_ptr<DescriptorSetLayout>> setLayouts;
        std::size_t hash;
    };
} // namespace magma
