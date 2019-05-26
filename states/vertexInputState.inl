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
#pragma once
#include "../internal/hash.h"

namespace magma
{
constexpr VertexInputBinding::VertexInputBinding(uint32_t binding, uint32_t stride,
    VkVertexInputRate inputRate /* VK_VERTEX_INPUT_RATE_VERTEX */) noexcept
{
    this->binding = binding;
    this->stride = stride;
    this->inputRate = inputRate;
}

constexpr VertexInputAttribute::VertexInputAttribute(uint32_t location, uint32_t binding, VkFormat format, uint32_t offset) noexcept
{
    this->location = location;
    this->binding = binding;
    this->format = format;
    this->offset = offset;
}

constexpr VertexInputAttribute::VertexInputAttribute(uint32_t location, const VertexInputAttribute& attrib, uint32_t offset) noexcept
{
    this->location = location;
    binding = attrib.binding;
    format = attrib.format;
    this->offset = offset;
}

constexpr VertexInputState::VertexInputState() noexcept
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    vertexBindingDescriptionCount = 0;
    pVertexBindingDescriptions = nullptr;
    vertexAttributeDescriptionCount = 0;
    pVertexAttributeDescriptions = nullptr;
}

constexpr VertexInputState::VertexInputState(const VertexInputBinding& binding, const VertexInputAttribute& attribute)
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    vertexBindingDescriptionCount = 1;
    pVertexBindingDescriptions = &binding;
    vertexAttributeDescriptionCount = 1;
    pVertexAttributeDescriptions = &attribute;
}

constexpr VertexInputState::VertexInputState(const VertexInputBinding& binding, const std::initializer_list<VertexInputAttribute>& attributes)
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    vertexBindingDescriptionCount = 1;
    pVertexBindingDescriptions = &binding;
    vertexAttributeDescriptionCount = static_cast<uint32_t>(attributes.size());
    pVertexAttributeDescriptions = attributes.begin();
}

constexpr VertexInputState::VertexInputState(const std::initializer_list<VertexInputBinding>& bindings,
    const std::initializer_list<VertexInputAttribute>& attributes)
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    vertexBindingDescriptionCount = static_cast<uint32_t>(bindings.size());
    pVertexBindingDescriptions = bindings.begin();
    vertexAttributeDescriptionCount = static_cast<uint32_t>(attributes.size());
    pVertexAttributeDescriptions = attributes.begin();
}

constexpr size_t VertexInputState::hash() const noexcept
{
    size_t hash = internal::hashArgs(
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

inline bool VertexInputState::operator==(const VertexInputState& other) const noexcept
{
    bool equal = (flags == other.flags) &&
        (vertexBindingDescriptionCount == other.vertexBindingDescriptionCount) &&
        (vertexAttributeDescriptionCount == other.vertexAttributeDescriptionCount);
    if (equal)
    {
        for (uint32_t i = 0; i < vertexBindingDescriptionCount; ++i)
        {
            equal |= 
            (pVertexBindingDescriptions[i].binding == other.pVertexBindingDescriptions[i].binding) &&
            (pVertexBindingDescriptions[i].stride == other.pVertexBindingDescriptions[i].stride) &&
            (pVertexBindingDescriptions[i].inputRate == other.pVertexBindingDescriptions[i].inputRate);
        }
        for (uint32_t i = 0; i < vertexAttributeDescriptionCount; ++i)
        {
            equal |=
            (pVertexAttributeDescriptions[i].location == other.pVertexAttributeDescriptions[i].location) &&
            (pVertexAttributeDescriptions[i].binding == other.pVertexAttributeDescriptions[i].binding) &&
            (pVertexAttributeDescriptions[i].format == other.pVertexAttributeDescriptions[i].format) &&
            (pVertexAttributeDescriptions[i].offset == other.pVertexAttributeDescriptions[i].offset);
        }
    }
    return equal;
}
} // namespace magma
