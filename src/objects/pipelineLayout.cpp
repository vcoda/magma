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
#include "pipelineLayout.h"
#include "descriptorSetLayout.h"
#include "device.h"
#include "../allocator/allocator.h"
#include "../exceptions/errorResult.h"
#include "../helpers/streamInsertOperators.h"
#include "../helpers/stringifyFlags.h"

namespace magma
{
PipelineLayout::PipelineLayout(std::shared_ptr<Device> device,
    const std::initializer_list<PushConstantRange>& pushConstantRanges_,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkPipelineLayoutCreateFlags flags /* 0 */):
    NonDispatchable(VK_OBJECT_TYPE_PIPELINE_LAYOUT, std::move(device), std::move(allocator)),
    pushConstantRanges(pushConstantRanges_),
    flags(flags)
{
    VkPipelineLayoutCreateInfo pipelineLayoutInfo;
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.pNext = nullptr;
    pipelineLayoutInfo.flags = flags;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = core::countof(pushConstantRanges);
    pipelineLayoutInfo.pPushConstantRanges = pushConstantRanges.data();
    const VkResult result = vkCreatePipelineLayout(getNativeDevice(), &pipelineLayoutInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create pipeline layout");
    hash = core::hashArgs(
        pipelineLayoutInfo.sType,
        pipelineLayoutInfo.flags,
        pipelineLayoutInfo.setLayoutCount,
        pipelineLayoutInfo.pushConstantRangeCount);
    for (auto const& pushConstantRange: pushConstantRanges)
        hash = core::hashCombine(hash, pushConstantRange.hash());
}

PipelineLayout::PipelineLayout(std::shared_ptr<const DescriptorSetLayout> setLayout,
    const std::initializer_list<PushConstantRange>& pushConstantRanges_,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkPipelineLayoutCreateFlags flags /* 0 */):
    NonDispatchable(VK_OBJECT_TYPE_PIPELINE_LAYOUT, setLayout->getDevice(), std::move(allocator)),
    setLayouts({setLayout}),
    pushConstantRanges(pushConstantRanges_),
    flags(flags)
{
    VkPipelineLayoutCreateInfo pipelineLayoutInfo;
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.pNext = nullptr;
    pipelineLayoutInfo.flags = flags;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = setLayout->getHandleAddress();
    pipelineLayoutInfo.pushConstantRangeCount = core::countof(pushConstantRanges);
    pipelineLayoutInfo.pPushConstantRanges = pushConstantRanges.data();
    const VkResult result = vkCreatePipelineLayout(getNativeDevice(), &pipelineLayoutInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create pipeline layout");
    hash = core::hashArgs(
        pipelineLayoutInfo.sType,
        pipelineLayoutInfo.flags,
        pipelineLayoutInfo.setLayoutCount,
        pipelineLayoutInfo.pushConstantRangeCount);
    hash = core::hashCombine(hash, setLayout->getHash());
    for (auto const& pushConstantRange: pushConstantRanges)
        hash = core::hashCombine(hash, pushConstantRange.hash());
}

PipelineLayout::PipelineLayout(const std::initializer_list<std::shared_ptr<const DescriptorSetLayout>>& setLayouts_,
    const std::initializer_list<PushConstantRange>& pushConstantRanges_,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkPipelineLayoutCreateFlags flags /* 0 */):
    NonDispatchable(VK_OBJECT_TYPE_PIPELINE_LAYOUT, (*setLayouts_.begin())->getDevice(), std::move(allocator)),
    pushConstantRanges(pushConstantRanges_),
    flags(flags)
{
    MAGMA_STACK_ARRAY(VkDescriptorSetLayout, dereferencedSetLayouts, setLayouts_.size());
    for (auto const& layout: setLayouts_)
        dereferencedSetLayouts.put(*layout);
    VkPipelineLayoutCreateInfo pipelineLayoutInfo;
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.pNext = nullptr;
    pipelineLayoutInfo.flags = flags;
    pipelineLayoutInfo.setLayoutCount = dereferencedSetLayouts.count();
    pipelineLayoutInfo.pSetLayouts = dereferencedSetLayouts;
    pipelineLayoutInfo.pushConstantRangeCount = core::countof(pushConstantRanges);
    pipelineLayoutInfo.pPushConstantRanges = pushConstantRanges.data();
    const VkResult result = vkCreatePipelineLayout(getNativeDevice(), &pipelineLayoutInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create pipeline layout");
    hash = core::hashArgs(
        pipelineLayoutInfo.sType,
        pipelineLayoutInfo.flags,
        pipelineLayoutInfo.setLayoutCount,
        pipelineLayoutInfo.pushConstantRangeCount);
    for (auto const& layout: setLayouts_)
        hash = core::hashCombine(hash, layout->getHash());
    for (auto const& pushConstantRange: pushConstantRanges)
        hash = core::hashCombine(hash, pushConstantRange.hash());
    for (auto const& layout: setLayouts_)
        setLayouts.push_back(layout);
}

PipelineLayout::~PipelineLayout()
{
    vkDestroyPipelineLayout(getNativeDevice(), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}

bool PipelineLayout::hasLayout(std::shared_ptr<const DescriptorSetLayout> setLayout) const noexcept
{
    return std::any_of(setLayouts.begin(), setLayouts.end(),
        [&setLayout](auto const& weak)
        {
            if (auto layout = weak.lock())
                return layout->getHash() == setLayout->getHash();
            return false;
        });
}

hash_t PipelineLayout::getHash() const noexcept
{
    hash_t hash = this->hash;
    for (auto const& layout: setLayouts)
    {
        auto setLayout = layout.lock();
        if (setLayout)
            hash = core::hashCombine(hash, setLayout->getHash());
    }
    return hash;
}

std::ostream& operator<<(std::ostream& out, const PipelineLayout& layout)
{
    out << "VkPipelineLayoutCreateInfo [" << std::endl
        << "\tsetLayoutCount: " << layout.setLayouts.size() << std::endl
        << "\tpSetLayouts: ";
    if (layout.setLayouts.empty())
        out << "NULL" << std::endl;
    else
    {
        out << std::endl;
        for (auto const& it: layout.setLayouts)
        {
            auto setLayout = it.lock();
            if (setLayout)
            {
                for (auto const& binding: setLayout->getBindings())
                {
                    out << "\t[" << std::endl
                        << "\t\tbinding: " << binding.binding << std::endl
                        << "\t\tdescriptorType: " << binding.descriptorType << std::endl
                        << "\t\tdescriptorCount: " << binding.descriptorCount << std::endl
                        << "\t\tstageFlags: " << helpers::stringifyShaderStageFlags(binding.stageFlags) << std::endl
                        << "\t]" << std::endl;
                }
            }
        }
    }
    out << "\tpushConstantRangeCount: " << layout.pushConstantRanges.size() << std::endl
        << "\tpPushConstantRanges: ";
    if (layout.pushConstantRanges.empty())
        out << "NULL" << std::endl;
    else
    {
        out << std::endl;
        for (auto const& range: layout.pushConstantRanges)
        {
            out << "\t[" << std::endl
                << "\t\tstageFlags: " << helpers::stringifyShaderStageFlags(range.stageFlags) << std::endl
                << "\t\toffset: " << range.offset << std::endl
                << "\t\tsize: " << range.size << std::endl
                << "\t]" << std::endl;
        }
    }
    out << "]";
    return out;
}
} // namespace magma
