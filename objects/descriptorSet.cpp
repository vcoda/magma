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
#include <iostream>
#include "descriptorSet.h"
#include "descriptorSetLayout.h"
#include "descriptorPool.h"
#include "device.h"
#include "../shaders/shaderReflection.h"
#include "../shaders/shaderReflectionFactory.h"
#include "../helpers/spirvReflectionTypeCast.h"
#include "../helpers/stringize.h"
#include "../exceptions/errorResult.h"

namespace magma
{
DescriptorSet::DescriptorSet(std::shared_ptr<DescriptorPool> descriptorPool,
    uint32_t setIndex, const DescriptorSetReflection& setLayoutReflection, uint32_t stageFlags,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    std::shared_ptr<IShaderReflectionFactory> shaderReflectionFactory /* nullptr */,
    const std::string& shaderFileName /* default */):
    NonDispatchable(VK_OBJECT_TYPE_DESCRIPTOR_SET, descriptorPool->getDevice(), std::move(allocator)),
    setIndex(setIndex),
    setLayoutReflection(setLayoutReflection),
    setLayout(std::move(setLayout)),
    descriptorPool(std::move(descriptorPool))
{   // Validate descriptor bindings through shader reflection
    if (shaderReflectionFactory && !shaderFileName.empty())
        validateReflection(shaderReflectionFactory->getReflection(shaderFileName));
    // Prepare list of native bindings
    std::vector<VkDescriptorSetLayoutBinding> bindings;
    for (auto binding : setLayoutReflection.getBindings())
    {
        binding->stageFlags = stageFlags;
        bindings.push_back(*binding);
    }
    // Create descriptor set layout
    setLayout = std::make_shared<magma::DescriptorSetLayout>(device, bindings, hostAllocator, 0);
    // Allocate descriptor set
    const VkDescriptorSetLayout dereferencedSetLayouts[1] = {*setLayout};
    VkDescriptorSetAllocateInfo allocInfo;
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.pNext = nullptr;
    allocInfo.descriptorPool = *this->descriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = dereferencedSetLayouts;
    const VkResult result = vkAllocateDescriptorSets(MAGMA_HANDLE(device), &allocInfo, &handle);
    MAGMA_THROW_FAILURE(result, "failed to allocate descriptor set");
    // Assign handle
    for (auto binding : setLayoutReflection.getBindings())
        binding->descriptorWrite.dstSet = handle;
}

DescriptorSet::~DescriptorSet()
{
    vkFreeDescriptorSets(MAGMA_HANDLE(device), *descriptorPool, 1, &handle);
}

bool DescriptorSet::dirty() const noexcept
{
    return setLayoutReflection.dirty();
}

void DescriptorSet::update()
{
    std::vector<VkWriteDescriptorSet> descriptorWrites;
    descriptorWrites.reserve(setLayoutReflection.getBindings().size());
    for (auto binding : setLayoutReflection.getBindings())
    {
        if (binding->dirty())
        {
            descriptorWrites.push_back(binding->getDescriptorWrite());
            binding->written = false;
        }
    }
    if (!descriptorWrites.empty())
        device->updateDescriptorSets(descriptorWrites);
}

void DescriptorSet::validateReflection(std::shared_ptr<const ShaderReflection> shaderReflection) const
{
    std::vector<const SpvReflectDescriptorSet *> descriptorSets = shaderReflection->enumerateDescriptorSets();
    if (setIndex >= descriptorSets.size())
        MAGMA_THROW("set index exceeds number of reflected descriptor sets");
    const SpvReflectDescriptorSet *descriptorSet = descriptorSets[setIndex];
    for (const auto definedBinding : setLayoutReflection.getBindings())
    {
        const SpvReflectDescriptorBinding *reflectedBinding = nullptr;
        for (uint32_t i = 0; i < descriptorSet->binding_count; ++i)
        {
            if (definedBinding->binding == descriptorSet->bindings[i]->binding)
            {
                reflectedBinding = descriptorSet->bindings[i];
                break;
            }
        }
        if (!reflectedBinding)
        {
            std::cout << "warning: binding #" << definedBinding->binding << " not found in the reflection";
            continue;
        }
        const VkDescriptorType reflectedDescriptorType = helpers::castToDescriptorType(reflectedBinding->descriptor_type);
        if (definedBinding->descriptorType != reflectedDescriptorType)
        {
            if ((VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC == definedBinding->descriptorType) &&
                (SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER != reflectedBinding->descriptor_type))
            {
                std::ostringstream oss;
                oss << "descriptor type mismatch:" << std::endl
                    << "binding #" << definedBinding->binding << std::endl
                    << "expected: " << helpers::stringize(reflectedDescriptorType) << std::endl
                    << "defined: " << helpers::stringize(definedBinding->descriptorType);
                MAGMA_THROW(oss.str());
            }
        }
        if (definedBinding->descriptorCount != reflectedBinding->count)
        {
            std::ostringstream oss;
            oss << "descriptor count mismatch:" << std::endl
                << "binding #" << definedBinding->binding << std::endl
                << "expected: " << reflectedBinding->count << ", defined: " << definedBinding->descriptorCount;
            MAGMA_THROW(oss.str());
        }
    }
}
} // namespace magma
