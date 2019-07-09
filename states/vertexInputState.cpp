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
#include "vertexInputState.h"
#include "../internal/copy.h"
#include "../internal/compare.h"

namespace magma
{
ManagedVertexInputState::ManagedVertexInputState(const VertexInputBinding& binding, const VertexInputAttribute& attribute):
    ManagedVertexInputState({binding}, {attribute})
{}

ManagedVertexInputState::ManagedVertexInputState(const VertexInputBinding& binding, const std::initializer_list<VertexInputAttribute>& attributes):
    ManagedVertexInputState({binding}, attributes)
{}

ManagedVertexInputState::ManagedVertexInputState(const std::initializer_list<VertexInputBinding>& bindings,
    const std::initializer_list<VertexInputAttribute>& attributes)
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    vertexBindingDescriptionCount = MAGMA_COUNT(bindings);
    pVertexBindingDescriptions = internal::copyArray<VkVertexInputBindingDescription>(bindings.begin(), bindings.size());
    vertexAttributeDescriptionCount = MAGMA_COUNT(attributes);
    try {
        pVertexAttributeDescriptions = internal::copyArray<VkVertexInputAttributeDescription>(attributes.begin(), attributes.size());
    } catch (const std::bad_alloc& exc)
    {
        delete[] pVertexBindingDescriptions;
        throw exc;
    }
}

ManagedVertexInputState::ManagedVertexInputState(const ManagedVertexInputState& other)
{
    internal::copy(this, &other);
    pVertexBindingDescriptions = internal::copyArray(other.pVertexBindingDescriptions, vertexBindingDescriptionCount);
    try {
        pVertexAttributeDescriptions = internal::copyArray(other.pVertexAttributeDescriptions, vertexAttributeDescriptionCount);
    } catch (const std::bad_alloc& exc)
    {
        delete[] pVertexBindingDescriptions;
        throw exc;
    }
}

ManagedVertexInputState& ManagedVertexInputState::operator=(const ManagedVertexInputState& other)
{
    if (this != &other)
    {
        internal::copy(this, &other);
        pVertexBindingDescriptions = internal::copyArray(other.pVertexBindingDescriptions, vertexBindingDescriptionCount);
        try {
            pVertexAttributeDescriptions = internal::copyArray(other.pVertexAttributeDescriptions, vertexAttributeDescriptionCount);
        } catch (const std::bad_alloc& exc)
        {
            delete[] pVertexBindingDescriptions;
            throw exc;
        }
    }
    return *this;
}

ManagedVertexInputState::~ManagedVertexInputState()
{
    delete[] pVertexBindingDescriptions;
    delete[] pVertexAttributeDescriptions;
}

size_t ManagedVertexInputState::hash() const noexcept
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

bool ManagedVertexInputState::operator==(const ManagedVertexInputState& other) const noexcept
{
    return (flags == other.flags) &&
        (vertexBindingDescriptionCount == other.vertexBindingDescriptionCount) &&
        (vertexAttributeDescriptionCount == other.vertexAttributeDescriptionCount) &&
        internal::compareArrays(pVertexBindingDescriptions, other.pVertexBindingDescriptions, vertexBindingDescriptionCount) &&
        internal::compareArrays(pVertexAttributeDescriptions, other.pVertexAttributeDescriptions, vertexAttributeDescriptionCount);
}
} // namespace magma
