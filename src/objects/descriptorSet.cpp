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
#include <sstream>
#include "descriptorSet.h"
#include "descriptorSetLayout.h"
#include "descriptorPool.h"
#include "device.h"
#include "../shaders/shaderReflection.h"
#include "../allocator/allocator.h"
#include "../helpers/enumerationCast.h"
#include "../helpers/streamInsertOperators.h"

namespace magma
{
DescriptorSet::DescriptorSet(std::shared_ptr<DescriptorPool> descriptorPool, std::shared_ptr<IAllocator> allocator) noexcept:
    NonDispatchable(VK_OBJECT_TYPE_DESCRIPTOR_SET, descriptorPool->getDevice(), std::move(allocator)),
    descriptorPool(std::move(descriptorPool))
{}

void DescriptorSet::allocate(VkDescriptorSetLayoutCreateFlags flags, const StructureChain& extendedInfo)
{
    std::vector<VkDescriptorSetLayoutBinding> bindings;
    for (auto descriptor: descriptors)
        bindings.push_back(*descriptor);
    // Create descriptor set layout
    setLayout = std::make_unique<DescriptorSetLayout>(device, bindings, hostAllocator, flags);
    // Allocate descriptor set
    VkDescriptorSetAllocateInfo descriptorSetAllocateInfo;
    descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    descriptorSetAllocateInfo.pNext = extendedInfo.headNode();
    descriptorSetAllocateInfo.descriptorPool = *descriptorPool;
    descriptorSetAllocateInfo.descriptorSetCount = 1;
    descriptorSetAllocateInfo.pSetLayouts = setLayout->getHandleAddress();
    const VkResult result = vkAllocateDescriptorSets(getNativeDevice(), &descriptorSetAllocateInfo, &handle);
    MAGMA_HANDLE_RESULT(result, "failed to allocate descriptor set");
}

DescriptorSet::~DescriptorSet()
{
    bool canFreeDescriptorSet = descriptorPool->getFlags() & VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    if (canFreeDescriptorSet)
        vkFreeDescriptorSets(getNativeDevice(), *descriptorPool, 1, &handle);
}

bool DescriptorSet::dirty() const
{
    return std::any_of(descriptors.begin(), descriptors.end(),
        [](auto descriptor) {
            return descriptor->modified();
        });
}

void DescriptorSet::update()
{
    auto descriptorWrites = stackalloc(VkWriteDescriptorSet, descriptors.size());
    const uint32_t descriptorWriteCount = writeDescriptors(descriptorWrites);
    if (descriptorWriteCount)
        device->updateDescriptorSets(descriptorWriteCount, descriptorWrites, 0, nullptr);
}

uint32_t DescriptorSet::writeDescriptors(VkWriteDescriptorSet *descriptorWrites) const
{
    MAGMA_ASSERT(dirty());
    uint32_t descriptorWriteCount = 0;
    for (auto descriptor: descriptors)
    {   // Write dirty descriptors
        if (descriptor->modified())
            descriptor->write(handle, descriptorWrites[descriptorWriteCount++]);
    }
    return descriptorWriteCount;
}

void DescriptorSet::validateReflection(const std::unique_ptr<const ShaderReflection>& shaderReflection, uint32_t setIndex) const
{
    std::vector<const SpvReflectDescriptorSet *> descriptorSets = shaderReflection->enumerateDescriptorSets();
    if (setIndex >= descriptorSets.size())
        MAGMA_ERROR("set index exceeds number of reflected descriptor sets");
    const SpvReflectDescriptorSet *descriptorSet = descriptorSets[setIndex];
    for (auto descriptor: descriptors)
    {
        if (!descriptor->resourceBinded())
            std::cout << "warning: binding #" << descriptor->binding << " has no binded resource" << std::endl;
        const SpvReflectDescriptorBinding *reflectedBinding = nullptr;
        for (uint32_t i = 0; i < descriptorSet->binding_count; ++i)
        {
            if (descriptor->binding == descriptorSet->bindings[i]->binding)
            {
                reflectedBinding = descriptorSet->bindings[i];
                break;
            }
        }
        if (!reflectedBinding)
        {
            std::cout << "warning: binding #" << descriptor->binding << " not found in the descriptor set # " << setIndex << std::endl;
            continue;
        }
        std::ostringstream out;
        const VkDescriptorType reflectedDescriptorType = helpers::spirvToDescriptorType(reflectedBinding->descriptor_type);
        if (descriptor->descriptorType != reflectedDescriptorType)
        {
            if ((VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC == descriptor->descriptorType) &&
                (SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER != reflectedBinding->descriptor_type))
            {   // Descriptor type is different
                out << "descriptor type mismatch:" << std::endl
                    << "binding #" << descriptor->binding << std::endl
                    << "expected: " << reflectedDescriptorType << std::endl
                    << "defined: " << descriptor->descriptorType;
            }
        }
        if (descriptor->descriptorCount != reflectedBinding->count)
        {   // Array size (or number of bytes) is different
            out << "descriptor count mismatch:" << std::endl
                << "binding #" << descriptor->binding << std::endl
                << "expected: " << reflectedBinding->count << ", defined: " << descriptor->descriptorCount;
        }
        switch (descriptor->descriptorType)
        {
        case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
        case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
        case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
        case VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT:
            {
                const SpvReflectImageTraits& imageTraits = reflectedBinding->image;
                const VkImageType imageType = helpers::spirvDimToImageType(imageTraits.dim);
                if (descriptor->getImageType() != VK_IMAGE_TYPE_MAX_ENUM &&
                    descriptor->getImageType() != imageType)
                {   // Type of assigned image is different
                    out << "descriptor image type mismatch:" << std::endl
                        << "binding #" << descriptor->binding << std::endl
                        << "expected: " << imageType << std::endl
                        << "assigned: " << descriptor->getImageType();
                }
            }
            break;
        default:
            break;
        }
        if (out.tellp() > 0)
        {   // Make sure error string will survive stack unwinding
            static std::string error;
            error = out.str();
            MAGMA_ERROR(error.c_str());
        }
    }
}
} // namespace magma
