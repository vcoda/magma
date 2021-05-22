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
#include "descriptorPool.h"
#include "descriptorSet.h"
#include "descriptorSetLayout.h"
#include "device.h"
#include "../allocator/allocator.h"
#include "../helpers/stackArray.h"
#include "../exceptions/errorResult.h"

namespace magma
{
DescriptorPool::DescriptorPool(std::shared_ptr<Device> device, uint32_t maxSets, const Descriptor& descriptor,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    bool freeDescriptorSet /* false */,
    uint32_t maxInlineUniformBlockBindings /* 0 */):
    DescriptorPool(std::move(device), maxSets, std::vector<Descriptor>{descriptor}, std::move(allocator), freeDescriptorSet, maxInlineUniformBlockBindings)
{}

DescriptorPool::DescriptorPool(std::shared_ptr<Device> device, uint32_t maxSets, const std::vector<Descriptor>& descriptors,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    bool freeDescriptorSet /* false */,
    uint32_t maxInlineUniformBlockBindings /* 0 */):
    NonDispatchable(VK_OBJECT_TYPE_DESCRIPTOR_POOL, std::move(device), std::move(allocator))
{
    MAGMA_UNUSED(maxInlineUniformBlockBindings);
    VkDescriptorPoolCreateInfo poolInfo;
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.pNext = nullptr;
    poolInfo.flags = 0;
    if (freeDescriptorSet)
        poolInfo.flags |= VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    poolInfo.maxSets = maxSets;
    poolInfo.poolSizeCount = MAGMA_COUNT(descriptors);
    poolInfo.pPoolSizes = descriptors.data();
#ifdef VK_EXT_inline_uniform_block
    VkDescriptorPoolInlineUniformBlockCreateInfoEXT inlineUniformBlockInfo;
    if (maxInlineUniformBlockBindings > 0)
    {
		inlineUniformBlockInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_INLINE_UNIFORM_BLOCK_CREATE_INFO_EXT;
        inlineUniformBlockInfo.pNext = nullptr;
		inlineUniformBlockInfo.maxInlineUniformBlockBindings = maxInlineUniformBlockBindings;
		poolInfo.pNext = &inlineUniformBlockInfo;
    }
#endif // VK_EXT_inline_uniform_block
    const VkResult create = vkCreateDescriptorPool(MAGMA_HANDLE(device), &poolInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create descriptor pool");
}

DescriptorPool::~DescriptorPool()
{
    vkDestroyDescriptorPool(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}

void DescriptorPool::reset()
{
    const VkResult reset = vkResetDescriptorPool(MAGMA_HANDLE(device), handle, 0);
    MAGMA_THROW_FAILURE(reset, "failed to reset descriptor pool");
}

std::shared_ptr<DescriptorSet> DescriptorPool::allocateDescriptorSet(std::shared_ptr<DescriptorSetLayout> setLayout,
    uint32_t maxDescriptorWrites /* 16 */)
{
    return std::make_shared<DescriptorSet>(shared_from_this(), 0, std::move(setLayout), maxDescriptorWrites);
}

void DescriptorPool::freeDescriptorSet(std::shared_ptr<DescriptorSet>& descriptorSet) noexcept
{
    descriptorSet.reset();
}
} // namespace magma
