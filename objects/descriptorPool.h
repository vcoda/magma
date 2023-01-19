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
#include "../descriptors/descriptorPool.h"

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
            const descriptor::DescriptorPool& descriptorPool,
            std::shared_ptr<IAllocator> allocator = nullptr,
            bool freeDescriptorSet = false,
            bool updateAfterBind = false,
            uint32_t maxInlineUniformBlockBindings = 0);
        explicit DescriptorPool(std::shared_ptr<Device> device,
            uint32_t maxSets,
            const std::vector<descriptor::DescriptorPool>& descriptorPools,
            std::shared_ptr<IAllocator> allocator = nullptr,
            bool freeDescriptorSet = false,
            bool updateAfterBind = false,
            uint32_t maxInlineUniformBlockBindings = 0);
        ~DescriptorPool();
        void reset();
        bool canFreeDescriptorSet() const noexcept { return freeDescriptorSet; }

    private:
        const bool freeDescriptorSet;
    };
} // namespace magma
