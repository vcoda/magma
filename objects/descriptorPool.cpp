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
#include "../helpers/stackArray.h"

namespace magma
{
DescriptorPool::DescriptorPool(std::shared_ptr<const Device> device,
    uint32_t maxDescriptorSets,
    const std::vector<Descriptor>& descriptors,
    VkDescriptorPoolCreateFlags flags /* 0 */):
    NonDispatchable(VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_POOL_EXT, device)
{
    VkDescriptorPoolCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = flags;
    info.maxSets = maxDescriptorSets;
    info.poolSizeCount = MAGMA_COUNT(descriptors);
    info.pPoolSizes = descriptors.data();
    const VkResult create = vkCreateDescriptorPool(*device, &info, nullptr, &handle);
    MAGMA_THROW_FAILURE(create, "failed to create descriptor pool");
}

DescriptorPool::~DescriptorPool()
{
    vkDestroyDescriptorPool(*device, handle, nullptr);
}

void DescriptorPool::reset()
{
    const VkResult reset = vkResetDescriptorPool(*device, handle, 0);
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
    const VkResult alloc = vkAllocateDescriptorSets(*device, &info, &descriptorSet);
    MAGMA_THROW_FAILURE(alloc, "failed to allocate descriptor set");
    return std::shared_ptr<DescriptorSet>(new DescriptorSet(descriptorSet, device));
}

void DescriptorPool::freeDescriptorSet(std::shared_ptr<DescriptorSet> setLayout)
{
    const VkDescriptorSet dereferencedLayouts[1] = {*setLayout};
    vkFreeDescriptorSets(*device, handle, 1, dereferencedLayouts);
}

std::vector<std::shared_ptr<DescriptorSet>> DescriptorPool::allocateDescriptorSets(std::shared_ptr<DescriptorSetLayout> setLayout)
{
    std::vector<std::shared_ptr<DescriptorSet>> descriptorSets;
    return descriptorSets;
}

void DescriptorPool::freeDescriptorSets(std::vector<std::shared_ptr<DescriptorSet>>& descriptorSets)
{
    MAGMA_STACK_ARRAY(VkDescriptorSet, dereferencedDescriptorSets, descriptorSets.size());
    for (const auto& set : descriptorSets)
        dereferencedDescriptorSets.put(*set);
    vkFreeDescriptorSets(*device, handle, dereferencedDescriptorSets.size(), dereferencedDescriptorSets.data());
    descriptorSets.clear();
}
} // namespace magma
