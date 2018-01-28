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

namespace magma
{
    class Device;
    class DescriptorSetLayout;

    class PipelineLayout : public NonDispatchable<VkPipelineLayout>
    {
    public:
        PipelineLayout(std::shared_ptr<const Device> device);
        PipelineLayout(std::shared_ptr<const Device> device,
            std::shared_ptr<const DescriptorSetLayout> setLayout,
            const std::vector<VkPushConstantRange>& pushConstantRanges = {});
        template<uint32_t setLayoutCount> PipelineLayout(std::shared_ptr<const Device> device,
            const std::shared_ptr<DescriptorSetLayout>(&setLayouts)[setLayoutCount]);
        template<uint32_t setLayoutCount, uint32_t pushConstantRangeCount> PipelineLayout(std::shared_ptr<const Device> device,
            const std::shared_ptr<DescriptorSetLayout>(&setLayouts)[setLayoutCount],
            const std::shared_ptr<VkPushConstantRange>(&pushConstantRanges)[pushConstantRangeCount]);
        PipelineLayout(std::shared_ptr<const Device> device,
            const std::vector<std::shared_ptr<const DescriptorSetLayout>>& setLayouts,
            const std::vector<VkPushConstantRange>& pushConstantRanges = {});
        ~PipelineLayout();
    };

    template<uint32_t setLayoutCount>
    inline PipelineLayout::PipelineLayout(std::shared_ptr<const Device> device,
        const std::shared_ptr<DescriptorSetLayout>(&setLayouts)[setLayoutCount]):
        PipelineLayout(device, std::vector<std::shared_ptr<const DescriptorSetLayout>>(setLayouts, setLayouts + setLayoutCount))
    {}

    template<uint32_t setLayoutCount, uint32_t pushConstantRangeCount>
    inline PipelineLayout::PipelineLayout(std::shared_ptr<const Device> device,
        const std::shared_ptr<DescriptorSetLayout>(&setLayouts)[setLayoutCount],
        const std::shared_ptr<VkPushConstantRange>(&pushConstantRanges)[pushConstantRangeCount]):
        PipelineLayout(device, 
            std::vector<std::shared_ptr<const DescriptorSetLayout>>(setLayouts, setLayouts + setLayoutCount),
            std::vector<std::shared_ptr<const DescriptorSetLayout>>(pushConstantRanges, pushConstantRanges + pushConstantRangeCount))
    {}
} // namespace magma
