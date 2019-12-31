/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2019 Victor Coda.

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
#include "sampler.h"
#include "../allocator/allocator.h"
#include "../misc/exception.h"
#include "../helpers/stackArray.h"
#include "../detail/copy.h"

namespace magma
{
DescriptorSetLayout::Binding::Binding(uint32_t binding, const Descriptor& descriptor, VkShaderStageFlags stageFlags,
    const ImmutableSamplerList& immutableSamplers /* {} */) noexcept:
    VkDescriptorSetLayoutBinding{}
{
    this->binding = binding;
    descriptorType = descriptor.type;
    descriptorCount = descriptor.descriptorCount;
    this->stageFlags = stageFlags;
    if (0 == immutableSamplers.size())
        pImmutableSamplers = nullptr;
    else
    {
        MAGMA_STACK_ARRAY(VkSampler, dereferencedImmutableSamplers, immutableSamplers.size());
        for (const auto& sampler : immutableSamplers)
            dereferencedImmutableSamplers.put(*sampler);
        pImmutableSamplers = detail::copyArray(static_cast<const VkSampler *>(dereferencedImmutableSamplers),
            dereferencedImmutableSamplers.size());
    }
}

DescriptorSetLayout::Binding::~Binding()
{
    delete[] pImmutableSamplers;
}

std::size_t DescriptorSetLayout::Binding::hash() const noexcept
{
    std::size_t hash = detail::hashArgs(
        binding,
        descriptorType,
        descriptorCount,
        stageFlags);
    if (pImmutableSamplers)
    {   // pImmutableSamplers must be a valid pointer to an array of descriptorCount valid VkSampler handles.
        detail::hashCombine(hash, detail::hashArray(pImmutableSamplers, descriptorCount));
    }
    return hash;
}

DescriptorSetLayout::DescriptorSetLayout(std::shared_ptr<Device> device, const Binding& binding,
    VkDescriptorSetLayoutCreateFlags flags /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    NonDispatchable(VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT, std::move(device), std::move(allocator))
{
    VkDescriptorSetLayoutCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = flags;
    info.bindingCount = 1;
    info.pBindings = &binding;
    const VkResult create = vkCreateDescriptorSetLayout(MAGMA_HANDLE(device), &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create descriptor set layout");
    bindings.push_back(binding);
    hash = detail::hashArgs(
        info.sType,
        info.flags,
        info.bindingCount);
}

DescriptorSetLayout::DescriptorSetLayout(std::shared_ptr<Device> device, const std::initializer_list<Binding>& bindings,
    VkDescriptorSetLayoutCreateFlags flags /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    NonDispatchable(VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT, std::move(device), std::move(allocator)),
    bindings(bindings)
{
    VkDescriptorSetLayoutCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = flags;
    info.bindingCount = MAGMA_COUNT(bindings);
    info.pBindings = bindings.begin();
    const VkResult create = vkCreateDescriptorSetLayout(MAGMA_HANDLE(device), &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create descriptor set layout");
    hash = detail::hashArgs(
        info.sType,
        info.flags,
        info.bindingCount);
}

DescriptorSetLayout::~DescriptorSetLayout()
{
    vkDestroyDescriptorSetLayout(MAGMA_HANDLE(device), handle, nullptr);
}

std::size_t DescriptorSetLayout::getHash() const noexcept
{
    std::size_t hash = this->hash;
    for (const Binding& binding : bindings) detail::hashCombine(hash, binding.hash());
    return hash;
}
} // namespace magma
