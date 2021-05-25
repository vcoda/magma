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
#include "descriptorSetLayout.h"
#include "device.h"
#include "../allocator/allocator.h"
#include "../exceptions/errorResult.h"

namespace magma
{
DescriptorSetLayout::DescriptorSetLayout(std::shared_ptr<Device> device, const std::vector<VkDescriptorSetLayoutBinding>& bindings,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkDescriptorSetLayoutCreateFlags flags /* 0 */):
    NonDispatchable(VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT, std::move(device), std::move(allocator))
{
    VkDescriptorSetLayoutCreateInfo layoutInfo;
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.pNext = nullptr;
    layoutInfo.flags = flags;
    layoutInfo.bindingCount = MAGMA_COUNT(bindings);
    layoutInfo.pBindings = bindings.data();
    const VkResult result = vkCreateDescriptorSetLayout(MAGMA_HANDLE(device), &layoutInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to create descriptor set layout");
    hash = core::hashArgs(
        layoutInfo.sType,
        layoutInfo.flags,
        layoutInfo.bindingCount);
    for (const auto& binding : bindings)
    {
        core::hashCombine(hash, core::hashArgs(
            binding.binding,
            binding.descriptorType,
            binding.descriptorCount,
            binding.stageFlags));
    }
}

DescriptorSetLayout::~DescriptorSetLayout()
{
    vkDestroyDescriptorSetLayout(MAGMA_HANDLE(device), handle, nullptr);
}
} // namespace magma
