/*
Magma - C++1x interface over Khronos Vulkan API.
Copyright (C) 2018 Victor Coda.

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
#include "../internal/copy.h"
#include "../internal/compare.h"

namespace magma
{
VertexInputState::VertexInputState()
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    vertexBindingDescriptionCount = 0;
    pVertexBindingDescriptions = nullptr;
    vertexAttributeDescriptionCount = 0;
    pVertexAttributeDescriptions = nullptr;
}

VertexInputState::VertexInputState(const VertexInputBinding& binding,
    const std::initializer_list<VertexInputAttribute>& attributes)
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    vertexBindingDescriptionCount = 1;
    pVertexBindingDescriptions = internal::copyArray<VkVertexInputBindingDescription>(&binding, 1);
    vertexAttributeDescriptionCount = MAGMA_COUNT(attributes);
    try {
        pVertexAttributeDescriptions = internal::copyInitializerList<VkVertexInputAttributeDescription>(attributes);
    } catch (const std::bad_alloc& err) {
        delete[] pVertexBindingDescriptions;
        throw err;
    }
}

VertexInputState::VertexInputState(const std::initializer_list<VertexInputBinding>& bindings,
    const std::initializer_list<VertexInputAttribute>& attributes)
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    vertexBindingDescriptionCount = MAGMA_COUNT(bindings);
    pVertexBindingDescriptions = internal::copyInitializerList<VkVertexInputBindingDescription>(bindings);
    vertexAttributeDescriptionCount = MAGMA_COUNT(attributes);
    try {
        pVertexAttributeDescriptions = internal::copyInitializerList<VkVertexInputAttributeDescription>(attributes);
    } catch (const std::bad_alloc& err) {
        delete[] pVertexBindingDescriptions;
        throw err;
    }
}

VertexInputState::~VertexInputState()
{
    delete[] pVertexBindingDescriptions;
    delete[] pVertexAttributeDescriptions;
}

uint32_t VertexInputState::stride(uint32_t binding) const noexcept
{
    size_t stride = 0;
    for (uint32_t i = 0; i < vertexAttributeDescriptionCount; ++i)
    {
        const VkVertexInputAttributeDescription& attrib = pVertexAttributeDescriptions[i];
        if (attrib.binding == binding)
        {   // TODO: alignment?
            const size_t attribSize = Format(attrib.format).size();
            stride += attribSize;
        }
    }
    return static_cast<uint32_t>(stride);
}

std::size_t VertexInputState::hash() const
{
    size_t hash = internal::hashArgs(
        sType,
        flags,
        vertexBindingDescriptionCount,
        vertexAttributeDescriptionCount);
    for (uint32_t i = 0; i < vertexBindingDescriptionCount; ++i)
    {
        internal::hashCombine(hash, internal::hashArgs(
            pVertexBindingDescriptions[i].binding,
            pVertexBindingDescriptions[i].stride,
            pVertexBindingDescriptions[i].inputRate));
    }
    for (uint32_t i = 0; i < vertexAttributeDescriptionCount; ++i)
    {
        internal::hashCombine(hash, internal::hashArgs(
            pVertexAttributeDescriptions[i].location,
            pVertexAttributeDescriptions[i].binding,
            pVertexAttributeDescriptions[i].format,
            pVertexAttributeDescriptions[i].offset));
    }
    return hash;
}

bool VertexInputState::operator==(const VertexInputState& other) const noexcept
{
   return (flags == other.flags) &&
        (vertexBindingDescriptionCount == other.vertexBindingDescriptionCount) &&
        (vertexAttributeDescriptionCount == other.vertexAttributeDescriptionCount) &&
        internal::compareArrays(pVertexBindingDescriptions, other.pVertexBindingDescriptions, vertexBindingDescriptionCount) &&
        internal::compareArrays(pVertexAttributeDescriptions, other.pVertexAttributeDescriptions, vertexAttributeDescriptionCount);
}
} // namespace magma
