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
#include "descriptorPool.h"
#include "descriptorSet.h"
#include "descriptorSetLayout.h"
#include "device.h"
#include "../allocator/allocator.h"
#include "../helpers/stackArray.h"
#include "../misc/exception.h"

namespace magma
{
DescriptorPool::DescriptorPool(std::shared_ptr<const Device> device,
    uint32_t maxDescriptorSets,
    const std::vector<Descriptor>& descriptors,
    bool freeDescriptorSet /* false */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    NonDispatchable(VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_POOL_EXT, std::move(device), std::move(allocator))
{
    VkDescriptorPoolCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    if (freeDescriptorSet)
        info.flags |= VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    info.maxSets = maxDescriptorSets;
    info.poolSizeCount = MAGMA_COUNT(descriptors);
    info.pPoolSizes = descriptors.data();
    const VkResult create = vkCreateDescriptorPool(MAGMA_HANDLE(device), &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create descriptor pool");
}

DescriptorPool::~DescriptorPool()
{
    vkDestroyDescriptorPool(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(allocator));
}

void DescriptorPool::reset()
{
    const VkResult reset = vkResetDescriptorPool(MAGMA_HANDLE(device), handle, 0);
    MAGMA_THROW_FAILURE(reset, "failed to reset descriptor pool");
}

std::shared_ptr<DescriptorSet> DescriptorPool::allocateDescriptorSet(std::shared_ptr<DescriptorSetLayout> setLayout)
{
    VkDescriptorSetAllocateInfo info;
    info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    info.pNext = nullptr;
    info.descriptorPool = handle;
    info.descriptorSetCount = 1;
    const VkDescriptorSetLayout dereferencedLayouts[1] = {*setLayout};
    info.pSetLayouts = dereferencedLayouts;
    VkDescriptorSet descriptorSet;
    const VkResult alloc = vkAllocateDescriptorSets(MAGMA_HANDLE(device), &info, &descriptorSet);
    MAGMA_THROW_FAILURE(alloc, "failed to allocate descriptor set");
    return std::shared_ptr<DescriptorSet>(new DescriptorSet(descriptorSet,  device, shared_from_this(), setLayout));
}

void DescriptorPool::freeDescriptorSet(std::shared_ptr<DescriptorSet>& descriptorSet)
{
    const VkDescriptorSet dereferencedLayouts[1] = {*descriptorSet};
    vkFreeDescriptorSets(MAGMA_HANDLE(device), handle, 1, dereferencedLayouts);
    descriptorSet.reset();
}

std::vector<std::shared_ptr<DescriptorSet>> DescriptorPool::allocateDescriptorSets(const std::vector<std::shared_ptr<DescriptorSetLayout>>& setLayouts)
{
    VkDescriptorSetAllocateInfo info;
    info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    info.pNext = nullptr;
    info.descriptorPool = handle;
    info.descriptorSetCount = MAGMA_COUNT(setLayouts);
    MAGMA_STACK_ARRAY(VkDescriptorSetLayout, dereferencedLayouts, setLayouts.size());
    for (const auto& layout : setLayouts)
        dereferencedLayouts.put(*layout);
    info.pSetLayouts = dereferencedLayouts;
    MAGMA_STACK_ARRAY(VkDescriptorSet, nativeDescriptorSets, info.descriptorSetCount);
    const VkResult alloc = vkAllocateDescriptorSets(MAGMA_HANDLE(device), &info, nativeDescriptorSets);
    MAGMA_THROW_FAILURE(alloc, "failed to allocate descriptor sets");
    std::vector<std::shared_ptr<DescriptorSet>> descriptorSets;
    int i = 0;
    for (const VkDescriptorSet descriptorSet : nativeDescriptorSets)
        descriptorSets.emplace_back(new DescriptorSet(descriptorSet, device, shared_from_this(), setLayouts[i++]));
    return std::move(descriptorSets);
}

void DescriptorPool::freeDescriptorSets(std::vector<std::shared_ptr<DescriptorSet>>& descriptorSets)
{
    MAGMA_STACK_ARRAY(VkDescriptorSet, dereferencedDescriptorSets, descriptorSets.size());
    for (const auto& descriptorSet : descriptorSets)
        dereferencedDescriptorSets.put(*descriptorSet);
    vkFreeDescriptorSets(MAGMA_HANDLE(device), handle, dereferencedDescriptorSets.size(), dereferencedDescriptorSets);
    descriptorSets.clear();
}
} // namespace magma
