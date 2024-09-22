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
#include "../allocator/allocator.h"
#include "../exceptions/errorResult.h"

namespace magma
{
DescriptorPool::DescriptorPool(std::shared_ptr<Device> device, uint32_t maxSets, const descriptor::DescriptorPool& descriptorPool,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkDescriptorPoolCreateFlags flags /* 0 */,
    const StructureChain& extendedInfo /* default */):
    DescriptorPool(std::move(device), maxSets, std::vector<descriptor::DescriptorPool>{descriptorPool},
        std::move(allocator), flags, extendedInfo)
{}

DescriptorPool::DescriptorPool(std::shared_ptr<Device> device, uint32_t maxSets, const std::vector<descriptor::DescriptorPool>& descriptorPools,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkDescriptorPoolCreateFlags flags /* 0 */,
    const StructureChain& extendedInfo /* default */):
    NonDispatchable(VK_OBJECT_TYPE_DESCRIPTOR_POOL, std::move(device), std::move(allocator)),
    flags(flags)
{
    VkDescriptorPoolCreateInfo descriptorPoolInfo;
    descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPoolInfo.pNext = extendedInfo.headNode();
    descriptorPoolInfo.flags = flags;
    descriptorPoolInfo.maxSets = maxSets;
    descriptorPoolInfo.poolSizeCount = core::countof(descriptorPools);
    descriptorPoolInfo.pPoolSizes = descriptorPools.data();
    const VkResult result = vkCreateDescriptorPool(getNativeDevice(), &descriptorPoolInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create descriptor pool");
}

DescriptorPool::~DescriptorPool()
{
    vkDestroyDescriptorPool(getNativeDevice(), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}

void DescriptorPool::reset(VkDescriptorPoolResetFlags flags /* 0 */)
{   // Set the pointer back to the start of pool memory
    const VkResult result = vkResetDescriptorPool(getNativeDevice(), handle, flags);
    MAGMA_HANDLE_RESULT(result, "failed to reset descriptor pool");
}
} // namespace magma
