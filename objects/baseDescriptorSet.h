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
#include "../objects/nondispatchable.h"

namespace magma
{
    class DescriptorPool;
    class DescriptorSetLayout;
    class IAllocator;

    /*  A descriptor set object is an opaque object that contains storage for a set of descriptors,
        where the types and number of descriptors is defined by a descriptor set layout. */

    class BaseDescriptorSet : public NonDispatchable<VkDescriptorSet>
    {
    public:
        explicit BaseDescriptorSet(std::shared_ptr<DescriptorPool> descriptorPool,
            uint32_t setIndex,
            std::shared_ptr<IAllocator> allocator = nullptr);
        uint32_t getIndex() const noexcept { return setIndex; }
        std::shared_ptr<DescriptorPool> getPool() noexcept { return descriptorPool; }
        std::shared_ptr<const DescriptorPool> getPool() const noexcept { return descriptorPool; }
        std::shared_ptr<DescriptorSetLayout> getLayout() noexcept { return setLayout; }
        std::shared_ptr<const DescriptorSetLayout> getLayout() const noexcept { return setLayout; }
        virtual bool dirty() const noexcept = 0;
        virtual void update() = 0;

    protected:
        uint32_t setIndex;
        std::shared_ptr<DescriptorPool> descriptorPool;
        std::shared_ptr<DescriptorSetLayout> setLayout;
    };
} // namespace magma
