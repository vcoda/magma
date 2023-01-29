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

#ifdef VK_KHR_maintenance3
DescriptorSetLayout::DescriptorSetLayout(std::shared_ptr<Device> device_, const std::vector<VkDescriptorSetLayoutBinding>& bindings,
    DescriptorSetLayout::Support& supportResult,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkDescriptorSetLayoutCreateFlags flags /* 0 */,
    const StructureChain& extendedInfo /* default */):
    NonDispatchable(VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT, std::move(device_), std::move(allocator)),
    hash(0)
{
    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo;
    VkDescriptorSetLayoutSupportKHR descriptorSetLayoutSupport;
    descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorSetLayoutInfo.pNext = extendedInfo.getChainedNodes();
    descriptorSetLayoutInfo.flags = flags;
    descriptorSetLayoutInfo.bindingCount = MAGMA_COUNT(bindings);
    descriptorSetLayoutInfo.pBindings = bindings.data();
    descriptorSetLayoutSupport.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_SUPPORT_KHR;
    descriptorSetLayoutSupport.pNext = nullptr;
    descriptorSetLayoutSupport.supported = VK_FALSE;
#ifdef VK_EXT_descriptor_indexing
    VkDescriptorSetVariableDescriptorCountLayoutSupportEXT descriptorSetVariableDescriptorCountLayoutSupport = {};
    if (device->extensionEnabled(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME))
    {
        descriptorSetLayoutSupport.pNext = &descriptorSetVariableDescriptorCountLayoutSupport;
        descriptorSetVariableDescriptorCountLayoutSupport.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_LAYOUT_SUPPORT_EXT;
        descriptorSetVariableDescriptorCountLayoutSupport.pNext = nullptr;
        descriptorSetVariableDescriptorCountLayoutSupport.maxVariableDescriptorCount = 0;
    }
#endif // VK_EXT_descriptor_indexing
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkGetDescriptorSetLayoutSupportKHR, VK_KHR_MAINTENANCE_3_EXTENSION_NAME);
    vkGetDescriptorSetLayoutSupportKHR(MAGMA_HANDLE(device), &descriptorSetLayoutInfo, &descriptorSetLayoutSupport);
    supportResult.supported = static_cast<bool>(descriptorSetLayoutSupport.supported);
#ifdef VK_EXT_descriptor_indexing
    // maxVariableDescriptorCount indicates the maximum number of descriptors supported
    // in the highest numbered binding of the layout, if that binding is variable-sized.
    // If the highest numbered binding of the layout has a descriptor type of VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK
    // then maxVariableDescriptorCount indicates the maximum byte size supported for the binding,
    // if that binding is variable-sized.
    supportResult.maxVariableDescriptorCount = descriptorSetVariableDescriptorCountLayoutSupport.maxVariableDescriptorCount;
#endif // VK_EXT_descriptor_indexing
    if (supportResult.supported)
    {
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
}
#endif // VK_KHR_maintenance3

DescriptorSetLayout::~DescriptorSetLayout()
{
    vkDestroyDescriptorSetLayout(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}
} // namespace magma
