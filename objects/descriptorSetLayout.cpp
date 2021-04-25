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
#include "sampler.h"
#include "../allocator/allocator.h"
#include "../exceptions/errorResult.h"
#include "../helpers/stackArray.h"
#include "../core/copy.h"

namespace magma
{
DescriptorSetLayout::SamplerBinding::SamplerBinding(uint32_t binding, const Descriptor& descriptor,
    VkShaderStageFlags stageFlags, const ImmutableSamplerList& immutableSamplers) noexcept:
    Binding(binding, descriptor, stageFlags),
    immutableSamplers(immutableSamplers)
{
    copyImmutableSamplers();
}

DescriptorSetLayout::SamplerBinding::SamplerBinding(const SamplerBinding& binding) noexcept:
    Binding(binding),
    immutableSamplers(binding.immutableSamplers)
{
    copyImmutableSamplers();
}

DescriptorSetLayout::SamplerBinding& DescriptorSetLayout::SamplerBinding::operator=(const SamplerBinding& other) noexcept
{
    if (this != &other)
    {
        binding = other.binding;
        descriptorType = other.descriptorType;
        descriptorCount = other.descriptorCount;
        stageFlags = other.stageFlags;
        immutableSamplers = other.immutableSamplers;
        copyImmutableSamplers();
    }
    return *this;
}

DescriptorSetLayout::SamplerBinding::~SamplerBinding()
{
    delete[] pImmutableSamplers;
}

std::size_t DescriptorSetLayout::SamplerBinding::hash() const noexcept
{
    std::size_t hash = Binding::hash();
    if (pImmutableSamplers)
    {   // pImmutableSamplers must be a valid pointer to an array of descriptorCount valid VkSampler handles.
        core::hashCombine(hash, core::hashArray(pImmutableSamplers, descriptorCount));
    }
    return hash;
}

void DescriptorSetLayout::SamplerBinding::copyImmutableSamplers() noexcept
{
    if (pImmutableSamplers)
    {
        delete[] pImmutableSamplers;
        pImmutableSamplers = nullptr;
    }
    if (!immutableSamplers.empty())
    {
        MAGMA_ASSERT(descriptorCount <= immutableSamplers.size());
        pImmutableSamplers = new(std::nothrow) VkSampler[immutableSamplers.size()];
        if (pImmutableSamplers)
        {
            VkSampler *dereferencedSampler = (VkSampler *)pImmutableSamplers;
            for (const auto& sampler : immutableSamplers)
                *dereferencedSampler++ = *sampler;
        }
    }
}

DescriptorSetLayout::DescriptorSetLayout(std::shared_ptr<Device> device, const Binding& binding,
    VkDescriptorSetLayoutCreateFlags flags /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    DescriptorSetLayout(std::move(device), {binding}, {}, flags, std::move(allocator))
{}

DescriptorSetLayout::DescriptorSetLayout(std::shared_ptr<Device> device, const SamplerBinding& samplerBinding,
    VkDescriptorSetLayoutCreateFlags flags /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    DescriptorSetLayout(std::move(device), {}, {samplerBinding}, flags, std::move(allocator))
{}

DescriptorSetLayout::DescriptorSetLayout(std::shared_ptr<Device> device, const std::initializer_list<Binding>& bindings,
    const std::initializer_list<SamplerBinding>& samplerBindings /* {} */,
    VkDescriptorSetLayoutCreateFlags flags /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    NonDispatchable(VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT, std::move(device), std::move(allocator)),
    bindings(bindings),
    samplerBindings(samplerBindings)
{
    static_assert(sizeof(VkDescriptorSetLayoutBinding) == sizeof(Binding), "type size mismatch");
    MAGMA_STACK_ARRAY(VkDescriptorSetLayoutBinding, dereferencedBindings, this->bindings.size() + this->samplerBindings.size());
    for (const auto& binding : this->bindings)
        dereferencedBindings.put(binding);
    for (const auto& binding : this->samplerBindings)
        dereferencedBindings.put(binding);
    VkDescriptorSetLayoutCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = flags;
    info.bindingCount = MAGMA_COUNT(bindings);
    info.pBindings = dereferencedBindings;
    const VkResult create = vkCreateDescriptorSetLayout(MAGMA_HANDLE(device), &info, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create descriptor set layout");
    hash = core::hashArgs(
        info.sType,
        info.flags,
        info.bindingCount);
}

DescriptorSetLayout::~DescriptorSetLayout()
{
    vkDestroyDescriptorSetLayout(MAGMA_HANDLE(device), handle, nullptr);
}

const DescriptorSetLayout::Binding& DescriptorSetLayout::getBinding(uint32_t binding) const
{ 
    for (auto& it : bindings)
        if (it.binding == binding)
            return it;
    throw exception::Exception("binding not found");
}
        
const DescriptorSetLayout::SamplerBinding& DescriptorSetLayout::getSamplerBinding(uint32_t binding) const
{ 
    for (auto& it : samplerBindings)
        if (it.binding == binding)
            return it;
    throw exception::Exception("binding not found");
}

std::size_t DescriptorSetLayout::getHash() const noexcept
{   // Compute complex hash on demand
    std::size_t hash = this->hash;
    for (const auto& binding : bindings)
        core::hashCombine(hash, binding.hash());
    for (const auto& binding : samplerBindings)
        core::hashCombine(hash, binding.hash());
    return hash;
}
} // namespace magma
