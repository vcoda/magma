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
#include "../helpers/copy.h"
#include "../shared.h"

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
    pVertexBindingDescriptions = helpers::copy(new VkVertexInputBindingDescription[vertexBindingDescriptionCount], bindings);
    vertexAttributeDescriptionCount = MAGMA_COUNT(attributes);
    pVertexAttributeDescriptions = helpers::copy(new VkVertexInputAttributeDescription[vertexAttributeDescriptionCount], attributes);
}

VertexInputState::VertexInputState(const VertexInputState& other)
{
    helpers::copy(this, &other);
    pVertexBindingDescriptions = helpers::copy(new VkVertexInputBindingDescription[vertexBindingDescriptionCount],
        other.pVertexBindingDescriptions, vertexBindingDescriptionCount);
    pVertexAttributeDescriptions = helpers::copy(new VkVertexInputAttributeDescription[vertexAttributeDescriptionCount],
        other.pVertexAttributeDescriptions, vertexAttributeDescriptionCount);
}

VertexInputState& VertexInputState::operator=(const VertexInputState& other)
{
    if (this != &other)
    {
        helpers::copy(this, &other);
        pVertexBindingDescriptions = helpers::copy(new VkVertexInputBindingDescription[vertexBindingDescriptionCount],
            other.pVertexBindingDescriptions, vertexBindingDescriptionCount);
        pVertexAttributeDescriptions = helpers::copy(new VkVertexInputAttributeDescription[vertexAttributeDescriptionCount],
            other.pVertexAttributeDescriptions, vertexAttributeDescriptionCount);
    }
    return *this;
}

VertexInputState::~VertexInputState()
{
    delete[] pVertexBindingDescriptions;
    delete[] pVertexAttributeDescriptions;
}

namespace states
{
const VertexInputState nullVertexInput;

const VertexInputState pos2Float(VertexInputBinding(0, sizeof(float) * 2),
    VertexInputAttribute(0, 0, VK_FORMAT_R32G32_SFLOAT, 0));
const VertexInputState pos3Float(VertexInputBinding(0, sizeof(float) * 3),
    VertexInputAttribute(0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0));
const VertexInputState pos4Float(VertexInputBinding(0, sizeof(float) * 4),
    VertexInputAttribute(0, 0, VK_FORMAT_R32G32B32A32_SFLOAT, 0));

const VertexInputState pos2Float_Tex2Float(VertexInputBinding(0, sizeof(float) * 4), {
    VertexInputAttribute(0, 0, VK_FORMAT_R32G32_SFLOAT, 0),
    VertexInputAttribute(0, 1, VK_FORMAT_R32G32_SFLOAT, 8)
});
const VertexInputState pos2Float_Col3Float(VertexInputBinding(0, sizeof(float) * 5), {
    VertexInputAttribute(0, 0, VK_FORMAT_R32G32_SFLOAT, 0),
    VertexInputAttribute(0, 1, VK_FORMAT_R32G32B32_SFLOAT, 8)
});
const VertexInputState pos2Float_Col4Float(VertexInputBinding(0, sizeof(float) * 6), {
    VertexInputAttribute(0, 0, VK_FORMAT_R32G32_SFLOAT, 0),
    VertexInputAttribute(0, 1, VK_FORMAT_R32G32B32A32_SFLOAT, 8)
});
const VertexInputState pos2Float_Col4Unorm(VertexInputBinding(0, sizeof(float) * 2 + sizeof(char) * 4), {
    VertexInputAttribute(0, 0, VK_FORMAT_R32G32_SFLOAT, 0),
    VertexInputAttribute(0, 1, VK_FORMAT_R8G8B8A8_UNORM, 8)
});

const VertexInputState pos3Float_Tex2Float(VertexInputBinding(0, sizeof(float) * 5), {
    VertexInputAttribute(0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0),
    VertexInputAttribute(0, 1, VK_FORMAT_R32G32_SFLOAT, 8)
});
const VertexInputState pos3Float_Col3Float(VertexInputBinding(0, sizeof(float) * 6), {
    VertexInputAttribute(0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0),
    VertexInputAttribute(0, 1, VK_FORMAT_R32G32B32_SFLOAT, 12)
});
const VertexInputState pos3Float_Col4Float(VertexInputBinding(0, sizeof(float) * 7), {
    VertexInputAttribute(0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0),
    VertexInputAttribute(0, 1, VK_FORMAT_R32G32B32A32_SFLOAT, 12)
});
const VertexInputState pos3Float_Col4Unorm(VertexInputBinding(0, sizeof(float) * 3 + sizeof(char) * 4), {
    VertexInputAttribute(0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0),
    VertexInputAttribute(0, 1, VK_FORMAT_R8G8B8A8_UNORM, 12)
});

const VertexInputState pos3Float_Nrm3Float(pos3Float_Col3Float);

const VertexInputState pos3Float_Nrm3Float_Col3Float(VertexInputBinding(0, sizeof(float) * 9), {
    VertexInputAttribute(0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0),
    VertexInputAttribute(0, 1, VK_FORMAT_R32G32B32_SFLOAT, 12),
    VertexInputAttribute(0, 2, VK_FORMAT_R32G32B32_SFLOAT, 24)
});
const VertexInputState pos3Float_Nrm3Float_Col4Float(VertexInputBinding(0, sizeof(float) * 10), {
    VertexInputAttribute(0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0),
    VertexInputAttribute(0, 1, VK_FORMAT_R32G32B32_SFLOAT, 12),
    VertexInputAttribute(0, 2, VK_FORMAT_R32G32B32A32_SFLOAT, 24)
});
const VertexInputState pos3Float_Nrm3Float_Tex2Float(VertexInputBinding(0, sizeof(float) * 8), {
    VertexInputAttribute(0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0),
    VertexInputAttribute(0, 1, VK_FORMAT_R32G32B32_SFLOAT, 12),
    VertexInputAttribute(0, 2, VK_FORMAT_R32G32_SFLOAT, 24)
});
} // namespace states
} // namespace magma
