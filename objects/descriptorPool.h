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
    class DescriptorSet;
    class DescriptorSetLayout;
    class IAllocator;

    class DescriptorPool : public NonDispatchable<VkDescriptorPool>
    {
    public:
        DescriptorPool(std::shared_ptr<const Device> device, 
            uint32_t maxDescriptorSets,
            const std::vector<Descriptor>& descriptors,
            bool freeDescriptorSet = false,
            std::shared_ptr<IAllocator> allocator = nullptr);
        ~DescriptorPool();
        void reset();
        std::shared_ptr<DescriptorSet> allocateDescriptorSet(std::shared_ptr<DescriptorSetLayout> setLayout);
        void freeDescriptorSet(std::shared_ptr<DescriptorSet>& descriptorSet);
        std::vector<std::shared_ptr<DescriptorSet>> allocateDescriptorSets(std::shared_ptr<DescriptorSetLayout> setLayout);
        void freeDescriptorSets(std::vector<std::shared_ptr<DescriptorSet>>& descriptorSets);
    };
} // namespace magma
