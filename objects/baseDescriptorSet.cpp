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
#include "pch.h"
#pragma hdrstop
#include "baseDescriptorSet.h"
#include "descriptorPool.h"

namespace magma
{
BaseDescriptorSet::BaseDescriptorSet(std::shared_ptr<DescriptorPool> descriptorPool, uint32_t setIndex,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    BaseDescriptorSet(std::move(descriptorPool), setIndex, nullptr, std::move(allocator))
{}

BaseDescriptorSet::BaseDescriptorSet(std::shared_ptr<DescriptorPool> descriptorPool, uint32_t setIndex, std::shared_ptr<DescriptorSetLayout> setLayout,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    NonDispatchable(VK_OBJECT_TYPE_DESCRIPTOR_SET, descriptorPool->getDevice(), std::move(allocator)),
    setIndex(setIndex),
    setLayout(std::move(setLayout)),
    descriptorPool(std::move(descriptorPool))
{}
} // namespace magma
