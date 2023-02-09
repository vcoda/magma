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
#include "pipelineLayout.h"
#include "descriptorSetLayout.h"
#include "device.h"
#include "../allocator/allocator.h"
#include "../helpers/stackArray.h"
#include "../exceptions/errorResult.h"

namespace magma
{
PipelineLayout::PipelineLayout(std::shared_ptr<Device> device,
    const std::initializer_list<PushConstantRange>& pushConstantRanges,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkPipelineLayoutCreateFlags flags /* 0 */):
    NonDispatchable(VK_OBJECT_TYPE_PIPELINE_LAYOUT, std::move(device), std::move(allocator))
{
    VkPipelineLayoutCreateInfo pipelineLayoutInfo;
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.pNext = nullptr;
    pipelineLayoutInfo.flags = flags;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = MAGMA_COUNT(pushConstantRanges);
    pipelineLayoutInfo.pPushConstantRanges = pushConstantRanges.begin();
    const VkResult result = vkCreatePipelineLayout(MAGMA_HANDLE(device), &pipelineLayoutInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to create pipeline layout");
    hash = core::hashArgs(
        pipelineLayoutInfo.sType,
        pipelineLayoutInfo.flags,
        pipelineLayoutInfo.setLayoutCount,
        pipelineLayoutInfo.pushConstantRangeCount);
    for (const PushConstantRange& pushConstantRange: pushConstantRanges)
        hash = core::hashCombine(hash, pushConstantRange.hash());
}

PipelineLayout::PipelineLayout(std::shared_ptr<const DescriptorSetLayout> setLayout,
    const std::initializer_list<PushConstantRange>& pushConstantRanges,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkPipelineLayoutCreateFlags flags /* 0 */):
    NonDispatchable(VK_OBJECT_TYPE_PIPELINE_LAYOUT, setLayout->getDevice(), std::move(allocator))
{
    VkPipelineLayoutCreateInfo pipelineLayoutInfo;
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.pNext = nullptr;
    pipelineLayoutInfo.flags = flags;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = setLayout->getHandleAddress();
    pipelineLayoutInfo.pushConstantRangeCount = MAGMA_COUNT(pushConstantRanges);
    pipelineLayoutInfo.pPushConstantRanges = pushConstantRanges.begin();
    const VkResult result = vkCreatePipelineLayout(MAGMA_HANDLE(device), &pipelineLayoutInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to create pipeline layout");
    hash = core::hashArgs(
        pipelineLayoutInfo.sType,
        pipelineLayoutInfo.flags,
        pipelineLayoutInfo.setLayoutCount,
        pipelineLayoutInfo.pushConstantRangeCount);
    hash = core::hashCombine(hash, setLayout->getHash());
    for (const PushConstantRange& pushConstantRange: pushConstantRanges)
        hash = core::hashCombine(hash, pushConstantRange.hash());
    descriptorSetLayouts.emplace(*setLayout, setLayout->getHash());
}

PipelineLayout::PipelineLayout(const std::initializer_list<std::shared_ptr<const DescriptorSetLayout>>& setLayouts,
    const std::initializer_list<PushConstantRange>& pushConstantRanges,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkPipelineLayoutCreateFlags flags /* 0 */):
    NonDispatchable(VK_OBJECT_TYPE_PIPELINE_LAYOUT, (*setLayouts.begin())->getDevice(), std::move(allocator))
{
    MAGMA_STACK_ARRAY(VkDescriptorSetLayout, dereferencedSetLayouts, setLayouts.size());
    for (const auto& setLayout: setLayouts)
    {
        dereferencedSetLayouts.put(*setLayout);
        descriptorSetLayouts.emplace(*setLayout, setLayout->getHash());
    }
    VkPipelineLayoutCreateInfo pipelineLayoutInfo;
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.pNext = nullptr;
    pipelineLayoutInfo.flags = flags;
    pipelineLayoutInfo.setLayoutCount = MAGMA_COUNT(dereferencedSetLayouts);
    pipelineLayoutInfo.pSetLayouts = dereferencedSetLayouts;
    pipelineLayoutInfo.pushConstantRangeCount = MAGMA_COUNT(pushConstantRanges);
    pipelineLayoutInfo.pPushConstantRanges = pushConstantRanges.begin();
    const VkResult result = vkCreatePipelineLayout(MAGMA_HANDLE(device), &pipelineLayoutInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to create pipeline layout");
    hash = core::hashArgs(
        pipelineLayoutInfo.sType,
        pipelineLayoutInfo.flags,
        pipelineLayoutInfo.setLayoutCount,
        pipelineLayoutInfo.pushConstantRangeCount);
    for (const auto& layout: descriptorSetLayouts)
        hash = core::hashCombine(hash, layout.second);
    for (const PushConstantRange& pushConstantRange: pushConstantRanges)
        hash = core::hashCombine(hash, pushConstantRange.hash());
}

PipelineLayout::~PipelineLayout()
{
    vkDestroyPipelineLayout(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}

bool PipelineLayout::hasLayout(std::shared_ptr<const DescriptorSetLayout> setLayout) const noexcept
{
    for (const auto it: descriptorSetLayouts)
    {
        if ((it.first == *setLayout) || (it.second == setLayout->getHash()))
            return true;
    }
    return false;
}

hash_t PipelineLayout::getHash() const noexcept
{
    hash_t hash = this->hash;
    for (auto& layout: descriptorSetLayouts)
        hash = core::hashCombine(hash, layout.second);
    return hash;
}
} // namespace magma
