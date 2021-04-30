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
#include "../descriptors/descriptors.h"

namespace magma
{
    class DescriptorSet;
    class DescriptorSetLayout;

    /* A descriptor pool maintains a pool of descriptors, from which descriptor sets are allocated.
       Descriptor pools are externally synchronized, meaning that the application must not allocate
       and/or free descriptor sets from the same pool in multiple threads simultaneously. */

    class DescriptorPool : public NonDispatchable<VkDescriptorPool>,
        public std::enable_shared_from_this<DescriptorPool>
    {
    public:
        explicit DescriptorPool(std::shared_ptr<Device> device,
            uint32_t maxSets,
            const Descriptor& descriptor,
            std::shared_ptr<IAllocator> allocator = nullptr,
            bool freeDescriptorSet = false);
        explicit DescriptorPool(std::shared_ptr<Device> device,
            uint32_t maxSets,
            const std::vector<Descriptor>& descriptors,
            std::shared_ptr<IAllocator> allocator = nullptr,
            bool freeDescriptorSet = false);
        ~DescriptorPool();
        void reset();
        std::shared_ptr<DescriptorSet> allocateDescriptorSet(std::shared_ptr<DescriptorSetLayout> setLayout,
            uint32_t maxDescriptorWrites = 16);
        void freeDescriptorSet(std::shared_ptr<DescriptorSet>& descriptorSet) noexcept;
        std::vector<std::shared_ptr<DescriptorSet>> allocateDescriptorSets(const std::vector<std::shared_ptr<DescriptorSetLayout>>& setLayouts,
            uint32_t maxDescriptorWrites = 16);
        void freeDescriptorSets(std::vector<std::shared_ptr<DescriptorSet>>& descriptorSets) noexcept;
    };
} // namespace magma
