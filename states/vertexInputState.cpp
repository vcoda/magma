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
#include "vertexInputState.h"
#include "../misc/format.h"
#include "../core/copy.h"
#include "../core/compare.h"

namespace magma
{
VertexInputState::VertexInputState() noexcept
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    vertexBindingDescriptionCount = 0;
    pVertexBindingDescriptions = nullptr;
    vertexAttributeDescriptionCount = 0;
    pVertexAttributeDescriptions = nullptr;
#ifdef VK_EXT_vertex_attribute_divisor
    divisor.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_DIVISOR_STATE_CREATE_INFO_EXT;
    divisor.pNext = nullptr;
    divisor.vertexBindingDivisorCount = 0;
    divisor.pVertexBindingDivisors = nullptr;
#endif // VK_EXT_vertex_attribute_divisor
}

VertexInputState::VertexInputState(const VertexInputBinding& binding,
    const std::initializer_list<VertexInputAttribute>& attributes) noexcept
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    vertexBindingDescriptionCount = 1;
    pVertexBindingDescriptions = core::copyArray<VkVertexInputBindingDescription>(&binding, 1);
    vertexAttributeDescriptionCount = MAGMA_COUNT(attributes);
    pVertexAttributeDescriptions = core::copyInitializerList<VkVertexInputAttributeDescription>(attributes);
#ifdef VK_EXT_vertex_attribute_divisor
    divisor.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_DIVISOR_STATE_CREATE_INFO_EXT;
    divisor.pNext = nullptr;
    divisor.vertexBindingDivisorCount = 0;
    divisor.pVertexBindingDivisors = nullptr;
#endif // VK_EXT_vertex_attribute_divisor
}

VertexInputState::VertexInputState(const std::initializer_list<VertexInputBinding>& bindings,
    const std::initializer_list<VertexInputAttribute>& attributes) noexcept
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    vertexBindingDescriptionCount = MAGMA_COUNT(bindings);
    pVertexBindingDescriptions = core::copyInitializerList<VkVertexInputBindingDescription>(bindings);
    vertexAttributeDescriptionCount = MAGMA_COUNT(attributes);
    pVertexAttributeDescriptions = core::copyInitializerList<VkVertexInputAttributeDescription>(attributes);
#ifdef VK_EXT_vertex_attribute_divisor
    divisor.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_DIVISOR_STATE_CREATE_INFO_EXT;
    divisor.pNext = nullptr;
    divisor.vertexBindingDivisorCount = 0;
    divisor.pVertexBindingDivisors = nullptr;
#endif // VK_EXT_vertex_attribute_divisor
}

#ifdef VK_EXT_vertex_attribute_divisor
VertexInputState::VertexInputState(const VertexInputBindingDivisor& binding,
    const std::initializer_list<VertexInputAttribute>& attributes) noexcept
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    pNext = &divisor;
    flags = 0;
    vertexBindingDescriptionCount = 1;
    pVertexBindingDescriptions = core::copyArray<VkVertexInputBindingDescription>(&binding, 1);
    vertexAttributeDescriptionCount = MAGMA_COUNT(attributes);
    pVertexAttributeDescriptions = core::copyInitializerList<VkVertexInputAttributeDescription>(attributes);
    divisor.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_DIVISOR_STATE_CREATE_INFO_EXT;
    divisor.pNext = nullptr;
    divisor.vertexBindingDivisorCount = 1;
    VkVertexInputBindingDivisorDescriptionEXT *vertexBindingDivisor = new(std::nothrow) VkVertexInputBindingDivisorDescriptionEXT[1];
    vertexBindingDivisor->binding = binding.binding;
    vertexBindingDivisor->divisor = binding.divisor;
    divisor.pVertexBindingDivisors = vertexBindingDivisor;
}

VertexInputState::VertexInputState(const std::initializer_list<VertexInputBindingDivisor>& bindings,
    const std::initializer_list<VertexInputAttribute>& attributes) noexcept
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    pNext = &divisor;
    flags = 0;
    vertexBindingDescriptionCount = 0;
    VkVertexInputBindingDescription *vertexBindingDescriptions = new(std::nothrow) VkVertexInputBindingDescription[bindings.size()];
    for (const auto& binding : bindings)
    {
        vertexBindingDescriptions[vertexBindingDescriptionCount].binding = binding.binding;
        vertexBindingDescriptions[vertexBindingDescriptionCount].stride = binding.stride;
        vertexBindingDescriptions[vertexBindingDescriptionCount++].inputRate = binding.inputRate;
    }
    pVertexBindingDescriptions = vertexBindingDescriptions;
    vertexAttributeDescriptionCount = MAGMA_COUNT(attributes);
    pVertexAttributeDescriptions = core::copyInitializerList<VkVertexInputAttributeDescription>(attributes);
    divisor.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_DIVISOR_STATE_CREATE_INFO_EXT;
    divisor.pNext = nullptr;
    divisor.vertexBindingDivisorCount = 0;
    VkVertexInputBindingDivisorDescriptionEXT *vertexBindingDivisors = new(std::nothrow) VkVertexInputBindingDivisorDescriptionEXT[vertexBindingDescriptionCount];
    for (const auto& binding : bindings)
    {
        vertexBindingDivisors[divisor.vertexBindingDivisorCount].binding = binding.binding;
        vertexBindingDivisors[divisor.vertexBindingDivisorCount++].divisor = binding.divisor;
    }
    divisor.pVertexBindingDivisors = vertexBindingDivisors;
}
#endif // VK_EXT_vertex_attribute_divisor

VertexInputState::VertexInputState(const VertexInputState& other) noexcept
{
    sType = other.sType;
    pNext = nullptr;
    flags = other.flags;
    vertexBindingDescriptionCount = other.vertexBindingDescriptionCount;
    pVertexBindingDescriptions = core::copyArray(other.pVertexBindingDescriptions, vertexBindingDescriptionCount);
    vertexAttributeDescriptionCount = other.vertexAttributeDescriptionCount;
    pVertexAttributeDescriptions = core::copyArray(other.pVertexAttributeDescriptions, vertexAttributeDescriptionCount);
#ifdef VK_EXT_vertex_attribute_divisor
    divisor.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_DIVISOR_STATE_CREATE_INFO_EXT;
    divisor.pNext = nullptr;
    divisor.vertexBindingDivisorCount = other.divisor.vertexBindingDivisorCount;
    divisor.pVertexBindingDivisors = core::copyArray(other.divisor.pVertexBindingDivisors, other.divisor.vertexBindingDivisorCount);
    if (other.pNext)
        pNext = &divisor;
#endif // VK_EXT_vertex_attribute_divisor
}

VertexInputState& VertexInputState::operator=(const VertexInputState& other) noexcept
{
    if (this != &other)
    {
        sType = other.sType;
        pNext = nullptr;
        flags = other.flags;
        vertexBindingDescriptionCount = other.vertexBindingDescriptionCount;
        delete[] pVertexBindingDescriptions;
        pVertexBindingDescriptions = core::copyArray(other.pVertexBindingDescriptions, vertexBindingDescriptionCount);
        vertexAttributeDescriptionCount = other.vertexAttributeDescriptionCount;
        delete[] pVertexAttributeDescriptions;
        pVertexAttributeDescriptions = core::copyArray(other.pVertexAttributeDescriptions, vertexAttributeDescriptionCount);
#ifdef VK_EXT_vertex_attribute_divisor
        divisor.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_DIVISOR_STATE_CREATE_INFO_EXT;
        divisor.pNext = nullptr;
        divisor.vertexBindingDivisorCount = other.divisor.vertexBindingDivisorCount;
        delete[] divisor.pVertexBindingDivisors;
        divisor.pVertexBindingDivisors = core::copyArray(other.divisor.pVertexBindingDivisors, other.divisor.vertexBindingDivisorCount);
        if (other.pNext)
            pNext = &divisor;
#endif // VK_EXT_vertex_attribute_divisor
    }
    return *this;
}

VertexInputState::~VertexInputState()
{
    delete[] pVertexBindingDescriptions;
    delete[] pVertexAttributeDescriptions;
#ifdef VK_EXT_vertex_attribute_divisor
    delete[] divisor.pVertexBindingDivisors;
#endif
}

uint32_t VertexInputState::stride(uint32_t binding) const noexcept
{
    std::size_t stride = 0;
    for (uint32_t i = 0; i < vertexAttributeDescriptionCount; ++i)
    {
        const VkVertexInputAttributeDescription& attrib = pVertexAttributeDescriptions[i];
        if (attrib.binding == binding)
        {   // TODO: alignment?
            const std::size_t attribSize = Format(attrib.format).size();
            stride += attribSize;
        }
    }
    return static_cast<uint32_t>(stride);
}

hash_t VertexInputState::hash() const noexcept
{
    hash_t hash = core::hashArgs(
        sType,
        flags,
        vertexBindingDescriptionCount,
        vertexAttributeDescriptionCount);
    for (uint32_t i = 0; i < vertexBindingDescriptionCount; ++i)
    {
        hash = core::hashCombine(hash, core::hashArgs(
            pVertexBindingDescriptions[i].binding,
            pVertexBindingDescriptions[i].stride,
            pVertexBindingDescriptions[i].inputRate));
    }
    for (uint32_t i = 0; i < vertexAttributeDescriptionCount; ++i)
    {
        hash = core::hashCombine(hash, core::hashArgs(
            pVertexAttributeDescriptions[i].location,
            pVertexAttributeDescriptions[i].binding,
            pVertexAttributeDescriptions[i].format,
            pVertexAttributeDescriptions[i].offset));
    }
#ifdef VK_EXT_vertex_attribute_divisor
    hash = core::hashCombine(hash, core::hashArgs(
        divisor.sType,
        divisor.vertexBindingDivisorCount));
    for (uint32_t i = 0; i < divisor.vertexBindingDivisorCount; ++i)
    {
        hash = core::hashCombine(hash, core::hashArgs(
            divisor.pVertexBindingDivisors[i].binding,
            divisor.pVertexBindingDivisors[i].divisor));
    }
#endif // VK_EXT_vertex_attribute_divisor
    return hash;
}

bool VertexInputState::operator==(const VertexInputState& other) const noexcept
{
   return (flags == other.flags) &&
        (vertexBindingDescriptionCount == other.vertexBindingDescriptionCount) &&
        (vertexAttributeDescriptionCount == other.vertexAttributeDescriptionCount) &&
        core::compareArrays(pVertexBindingDescriptions, other.pVertexBindingDescriptions, vertexBindingDescriptionCount) &&
        core::compareArrays(pVertexAttributeDescriptions, other.pVertexAttributeDescriptions, vertexAttributeDescriptionCount) &&
    #ifdef VK_EXT_vertex_attribute_divisor
        (divisor.vertexBindingDivisorCount == other.divisor.vertexBindingDivisorCount) &&
        core::compareArrays(divisor.pVertexBindingDivisors, other.divisor.pVertexBindingDivisors, divisor.vertexBindingDivisorCount);
    #else
        true;
    #endif
}
} // namespace magma
