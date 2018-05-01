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
    class IAllocator;

    class PipelineLayout : public NonDispatchable<VkPipelineLayout>
    {
    public:
        PipelineLayout(std::shared_ptr<const Device> device,
            const std::initializer_list<VkPushConstantRange>& pushConstantRanges = {},
            std::shared_ptr<IAllocator> allocator = nullptr);
        PipelineLayout(std::shared_ptr<const DescriptorSetLayout> setLayout,
            const std::initializer_list<VkPushConstantRange>& pushConstantRanges = {},
            std::shared_ptr<IAllocator> allocator = nullptr);
        PipelineLayout(const std::vector<std::shared_ptr<const DescriptorSetLayout>>& setLayouts,
            const std::initializer_list<VkPushConstantRange>& pushConstantRanges = {},
            std::shared_ptr<IAllocator> allocator = nullptr);
        template<uint32_t setLayoutCount> 
        PipelineLayout(const std::shared_ptr<DescriptorSetLayout>(&setLayouts)[setLayoutCount],
            const std::initializer_list<VkPushConstantRange>& pushConstantRanges = {},
            std::shared_ptr<IAllocator> allocator = nullptr):
            PipelineLayout(std::vector<std::shared_ptr<const DescriptorSetLayout>>(setLayouts, setLayouts + setLayoutCount), pushConstantRanges, allocator) {}
        ~PipelineLayout();
    };
} // namespace magma
