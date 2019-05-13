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
#include "../internal/hash.h"
#include "../internal/compareArrays.h"

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
}

VertexInputState::VertexInputState(const VertexInputBinding& binding, const VertexInputAttribute& attribute):
    VertexInputState({binding}, {attribute})
{}

VertexInputState::VertexInputState(const VertexInputBinding& binding, const std::initializer_list<VertexInputAttribute>& attributes):
    VertexInputState({binding}, attributes)
{}

VertexInputState::VertexInputState(const std::initializer_list<VertexInputBinding>& bindings,
    const std::initializer_list<VertexInputAttribute>& attributes)
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    vertexBindingDescriptionCount = MAGMA_COUNT(bindings);
    pVertexBindingDescriptions = internal::copyArray(static_cast<const VkVertexInputBindingDescription *>(bindings.begin()), bindings.size());
    vertexAttributeDescriptionCount = MAGMA_COUNT(attributes);
    try {
        pVertexAttributeDescriptions = internal::copyArray(static_cast<const VkVertexInputAttributeDescription *>(attributes.begin()), attributes.size());
    } catch (const std::bad_alloc& exc)
    {
        delete[] pVertexBindingDescriptions;
        throw exc;
    }
}

VertexInputState::VertexInputState(const VertexInputState& other)
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

VertexInputState& VertexInputState::operator=(const VertexInputState& other)
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

VertexInputState::~VertexInputState()
{
    delete[] pVertexBindingDescriptions;
    delete[] pVertexAttributeDescriptions;
}

size_t VertexInputState::hash() const noexcept
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

bool VertexInputState::operator==(const VertexInputState& other) const noexcept
{
    return (flags == other.flags) &&
        (vertexBindingDescriptionCount == other.vertexBindingDescriptionCount) &&
        (vertexAttributeDescriptionCount == other.vertexAttributeDescriptionCount) &&
        internal::compareArrays(pVertexBindingDescriptions, other.pVertexBindingDescriptions, vertexBindingDescriptionCount) &&
        internal::compareArrays(pVertexAttributeDescriptions, other.pVertexAttributeDescriptions, vertexAttributeDescriptionCount);
}

namespace renderstates
{
const VertexInputState nullVertexInput;

const VertexInputState pos2Float(VertexInputBinding(0, sizeof(float) * 2), attributes::xyFloat32);
const VertexInputState pos3Float(VertexInputBinding(0, sizeof(float) * 3), attributes::xyzFloat32);
const VertexInputState pos4Float(VertexInputBinding(0, sizeof(float) * 4), attributes::xyzwFloat32);

const VertexInputState pos2FloatTex2Float(VertexInputBinding(0, sizeof(float) * 4), {
    VertexInputAttribute(0, attributes::xyFloat32, 0),
    VertexInputAttribute(1, attributes::xyFloat32, 8)
});
const VertexInputState pos2FloatColor3Float(VertexInputBinding(0, sizeof(float) * 5), {
    VertexInputAttribute(0, attributes::xyFloat32, 0),
    VertexInputAttribute(1, attributes::xyzFloat32, 8)
});
const VertexInputState pos2FloatColor4Float(VertexInputBinding(0, sizeof(float) * 6), {
    VertexInputAttribute(0, attributes::xyFloat32, 0),
    VertexInputAttribute(1, attributes::xyzwFloat32, 8)
});
const VertexInputState pos2FloatColor4UNorm(VertexInputBinding(0, sizeof(float) * 2 + sizeof(char) * 4), {
    VertexInputAttribute(0, attributes::xyFloat32, 0),
    VertexInputAttribute(1, attributes::rgbaUNorm8, 8)
});

const VertexInputState pos3FloatTex2Float(VertexInputBinding(0, sizeof(float) * 5), {
    VertexInputAttribute(0, attributes::xyzFloat32, 0),
    VertexInputAttribute(1, attributes::xyFloat32, 12)
});
const VertexInputState pos3FloatColor3Float(VertexInputBinding(0, sizeof(float) * 6), {
    VertexInputAttribute(0, attributes::xyzFloat32, 0),
    VertexInputAttribute(1, attributes::xyzFloat32, 12)
});
const VertexInputState pos3FloatColor4Float(VertexInputBinding(0, sizeof(float) * 7), {
    VertexInputAttribute(0, attributes::xyzFloat32, 0),
    VertexInputAttribute(1, attributes::xyzwFloat32, 12)
});
const VertexInputState pos3FloatColor4UNorm(VertexInputBinding(0, sizeof(float) * 3 + sizeof(char) * 4), {
    VertexInputAttribute(0, attributes::xyzFloat32, 0),
    VertexInputAttribute(1, attributes::rgbaUNorm8, 12)
});

const VertexInputState pos3FloatNormal3Float(VertexInputBinding(0, sizeof(float) * 6), {
    VertexInputAttribute(0, attributes::xyzFloat32, 0),
    VertexInputAttribute(1, attributes::xyzFloat32, 12)
});
const VertexInputState pos3FloatNormal3FloatTex2Float(VertexInputBinding(0, sizeof(float) * 8), {
    VertexInputAttribute(0, attributes::xyzFloat32, 0),
    VertexInputAttribute(1, attributes::xyzFloat32, 12),
    VertexInputAttribute(2, attributes::xyFloat32, 24)
});
const VertexInputState pos3FloatNormal3FloatColor3Float(VertexInputBinding(0, sizeof(float) * 9), {
    VertexInputAttribute(0, attributes::xyzFloat32, 0),
    VertexInputAttribute(1, attributes::xyzFloat32, 12),
    VertexInputAttribute(2, attributes::xyzFloat32, 24)
});
const VertexInputState pos3FloatNormal3FloatColor4Float(VertexInputBinding(0, sizeof(float) * 10), {
    VertexInputAttribute(0, attributes::xyzFloat32, 0),
    VertexInputAttribute(1, attributes::xyzFloat32, 12),
    VertexInputAttribute(2, attributes::xyzwFloat32, 24)
});
const VertexInputState pos3FloatNormal3FloatColor4UNorm(VertexInputBinding(0, sizeof(float) * 6 + sizeof(char) * 4), {
    VertexInputAttribute(0, attributes::xyzFloat32, 0),
    VertexInputAttribute(1, attributes::xyzFloat32, 12),
    VertexInputAttribute(2, attributes::rgbaUNorm8, 24)
});
} // namespace renderstates
} // namespace magma
