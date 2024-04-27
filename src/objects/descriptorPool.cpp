/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

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
DescriptorPool::DescriptorPool(std::shared_ptr<Device> device, uint32_t maxSets, const descriptor::DescriptorPool& descriptorPool,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    bool freeDescriptorSet /* false */,
    bool updateAfterBind /* false */,
    uint32_t maxInlineUniformBlockBindings /* 0 */,
    const StructureChain& extendedInfo /* default */):
    DescriptorPool(std::move(device), maxSets, std::vector<descriptor::DescriptorPool>{descriptorPool}, std::move(allocator),
        freeDescriptorSet, updateAfterBind, maxInlineUniformBlockBindings, extendedInfo)
{}

DescriptorPool::DescriptorPool(std::shared_ptr<Device> device_, uint32_t maxSets, const std::vector<descriptor::DescriptorPool>& descriptorPools,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    bool freeDescriptorSet /* false */,
    bool updateAfterBind /* false */,
    uint32_t maxInlineUniformBlockBindings /* 0 */,
    const StructureChain& extendedInfo /* default */):
    NonDispatchable(VK_OBJECT_TYPE_DESCRIPTOR_POOL, std::move(device_), std::move(allocator)),
    freeDescriptorSet(freeDescriptorSet)
{
    MAGMA_UNUSED(updateAfterBind);
    MAGMA_UNUSED(maxInlineUniformBlockBindings);
    VkDescriptorPoolCreateInfo descriptorPoolInfo;
    descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPoolInfo.pNext = extendedInfo.headNode();
    descriptorPoolInfo.flags = 0;
    if (freeDescriptorSet)
    {   // Mobile implementations may use a simpler allocator if that flag is not set,
        // relying on the fact that pool memory will only be recycled in block.
        descriptorPoolInfo.flags |= VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    }
#ifdef VK_EXT_descriptor_indexing
    if (updateAfterBind)
    {   // Specifies that descriptor sets allocated from this pool can include bindings
        // with the VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT_EXT bit set.
        descriptorPoolInfo.flags |= VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT_EXT;
    }
#endif // VK_EXT_descriptor_indexing
    descriptorPoolInfo.maxSets = maxSets;
    descriptorPoolInfo.poolSizeCount = MAGMA_COUNT(descriptorPools);
    descriptorPoolInfo.pPoolSizes = descriptorPools.data();
#ifdef VK_EXT_inline_uniform_block
    VkDescriptorPoolInlineUniformBlockCreateInfoEXT descriptorPoolInlineUniformBlockInfo;
    if ((maxInlineUniformBlockBindings > 0) && device->extensionEnabled(VK_EXT_INLINE_UNIFORM_BLOCK_EXTENSION_NAME))
    {
        descriptorPoolInfo.pNext = &descriptorPoolInlineUniformBlockInfo;
        descriptorPoolInlineUniformBlockInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_INLINE_UNIFORM_BLOCK_CREATE_INFO_EXT;
        descriptorPoolInlineUniformBlockInfo.pNext = extendedInfo.headNode();
        descriptorPoolInlineUniformBlockInfo.maxInlineUniformBlockBindings = maxInlineUniformBlockBindings;
    }
#endif // VK_EXT_inline_uniform_block
    const VkResult result = vkCreateDescriptorPool(MAGMA_HANDLE(device), &descriptorPoolInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create descriptor pool");
}

DescriptorPool::~DescriptorPool()
{
    vkDestroyDescriptorPool(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}

void DescriptorPool::reset(VkDescriptorPoolResetFlags flags /* 0 */)
{   // Set the pointer back to the start of pool memory
    const VkResult result = vkResetDescriptorPool(MAGMA_HANDLE(device), handle, flags);
    MAGMA_HANDLE_RESULT(result, "failed to reset descriptor pool");
}
} // namespace magma
