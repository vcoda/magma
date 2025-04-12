/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2025 Victor Coda.

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
#include "variableCountDescriptorSet.h"
#include "descriptorSetLayout.h"
#include "descriptorPool.h"

namespace magma
{
#ifdef VK_EXT_descriptor_indexing
void VariableCountDescriptorSet::allocate(VkDescriptorSetLayoutCreateFlags flags, const StructureChain& extendedInfo)
{   // Sort descriptors by binding order
    std::map<uint32_t, const DescriptorSetLayoutBinding *> sortedDescriptors;
    for (auto descriptor: descriptors)
        sortedDescriptors[descriptor->binding] = descriptor;
    std::vector<VkDescriptorSetLayoutBinding> bindings;
    std::vector<VkDescriptorBindingFlagsEXT> bindingFlags;
    for (auto const& [index, descriptor]: sortedDescriptors)
    {
        MAGMA_UNUSED(index);
        bindings.push_back(*descriptor);
        if (descriptor->getBindingFlags() & VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT_EXT)
        {
            bool canUpdateAfterBind = descriptorPool->getFlags() & VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT_EXT;
            if (!canUpdateAfterBind)
                MAGMA_ERROR("descriptor with update-after-bind flag can't be allocated from this pool");
        }
        bindingFlags.push_back(descriptor->getBindingFlags());
    }
    // If an element of pBindingFlags includes VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT,
    // then it must be the element with the highest binding number.
    bindingFlags.back() |= VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT_EXT;
    // Create descriptor set layout
    VkDescriptorSetLayoutBindingFlagsCreateInfoEXT descriptorSetLayoutBindingFlagsInfo;
    descriptorSetLayoutBindingFlagsInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO_EXT;
    descriptorSetLayoutBindingFlagsInfo.pNext = nullptr;
    descriptorSetLayoutBindingFlagsInfo.bindingCount = core::countof(bindingFlags);
    descriptorSetLayoutBindingFlagsInfo.pBindingFlags = bindingFlags.data();
    setLayout = std::make_unique<DescriptorSetLayout>(device, bindings, hostAllocator, flags,
        StructureChain(descriptorSetLayoutBindingFlagsInfo));
    // Allocate descriptor set
    VkDescriptorSetAllocateInfo descriptorSetAllocateInfo;
    VkDescriptorSetVariableDescriptorCountAllocateInfoEXT descriptorSetVariableDescriptorCountAllocateInfo;
    descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    descriptorSetAllocateInfo.pNext = &descriptorSetVariableDescriptorCountAllocateInfo;
    descriptorSetAllocateInfo.descriptorPool = *descriptorPool;
    descriptorSetAllocateInfo.descriptorSetCount = 1;
    descriptorSetAllocateInfo.pSetLayouts = setLayout->getHandleAddress();
    descriptorSetVariableDescriptorCountAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO_EXT;
    descriptorSetVariableDescriptorCountAllocateInfo.pNext = extendedInfo.headNode();
    descriptorSetVariableDescriptorCountAllocateInfo.descriptorSetCount = descriptorSetAllocateInfo.descriptorSetCount;
    descriptorSetVariableDescriptorCountAllocateInfo.pDescriptorCounts = &bindings.back().descriptorCount;
    const VkResult result = vkAllocateDescriptorSets(getNativeDevice(), &descriptorSetAllocateInfo, &handle);
    MAGMA_HANDLE_RESULT(result, "failed to allocate variable count descriptor set");
}
#endif // VK_EXT_descriptor_indexing
} // namespace magma
