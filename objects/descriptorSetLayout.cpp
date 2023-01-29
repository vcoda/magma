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
#include "pch.h"
#pragma hdrstop
#include "descriptorSetLayout.h"
#include "device.h"
#include "../misc/extProcAddress.h"
#include "../allocator/allocator.h"
#include "../exceptions/errorResult.h"

namespace magma
{
DescriptorSetLayout::DescriptorSetLayout(std::shared_ptr<Device> device, const std::vector<VkDescriptorSetLayoutBinding>& bindings,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkDescriptorSetLayoutCreateFlags flags /* 0 */,
    const StructureChain& extendedInfo /* default */):
    NonDispatchable(VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT, std::move(device), std::move(allocator))
{
    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo;
    descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorSetLayoutInfo.pNext = extendedInfo.getChainedNodes();
    descriptorSetLayoutInfo.flags = flags;
    descriptorSetLayoutInfo.bindingCount = MAGMA_COUNT(bindings);
    descriptorSetLayoutInfo.pBindings = bindings.data();
    const VkResult result = vkCreateDescriptorSetLayout(MAGMA_HANDLE(device), &descriptorSetLayoutInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to create descriptor set layout");
    hash = core::hashArgs(
        descriptorSetLayoutInfo.sType,
        descriptorSetLayoutInfo.flags,
        descriptorSetLayoutInfo.bindingCount,
        extendedInfo.getHash());
    for (const auto& binding : bindings)
    {
        hash = core::hashCombine(hash, core::hashArgs(
            binding.binding,
            binding.descriptorType,
            binding.descriptorCount,
            binding.stageFlags));
    }
}

DescriptorSetLayout::~DescriptorSetLayout()
{
    vkDestroyDescriptorSetLayout(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}
} // namespace magma
