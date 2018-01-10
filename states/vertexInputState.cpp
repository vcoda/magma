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

namespace magma
{
VertexInputState::VertexInputState(const VertexInputBinding& binding, const VertexInputAttribute& attribute):
    VertexInputState(std::vector<VertexInputBinding>{binding}, std::vector<VertexInputAttribute>{attribute})
{}

VertexInputState::VertexInputState(const VertexInputBinding& binding, const std::vector<VertexInputAttribute>& attributes):
    VertexInputState(std::vector<VertexInputBinding>{binding}, attributes)
{}

VertexInputState::VertexInputState(const std::vector<VertexInputBinding>& bindings, const std::vector<VertexInputAttribute>& attributes)
{
    MAGMA_ASSERT(!bindings.empty());
    MAGMA_ASSERT(!attributes.empty());
    state.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    state.pNext = nullptr;
    state.flags = 0;
    state.vertexBindingDescriptionCount = MAGMA_COUNT(bindings);
    state.pVertexBindingDescriptions = new VkVertexInputBindingDescription[state.vertexBindingDescriptionCount];
    MAGMA_COPY_VECTOR(state.pVertexBindingDescriptions, bindings);
    state.vertexAttributeDescriptionCount = MAGMA_COUNT(attributes);
    state.pVertexAttributeDescriptions = new VkVertexInputAttributeDescription[state.vertexAttributeDescriptionCount];
    MAGMA_COPY_VECTOR(state.pVertexAttributeDescriptions, attributes);
}

VertexInputState::~VertexInputState()
{
    delete[] state.pVertexBindingDescriptions;
    delete[] state.pVertexAttributeDescriptions;
}

namespace states
{
const VertexInputState vertexHalf(VertexInputBinding(0, sizeof(short)), VertexInputAttribute(0, 0, VK_FORMAT_R16_SFLOAT, 0));
const VertexInputState vertex2Half(VertexInputBinding(0, sizeof(short) * 2), VertexInputAttribute(0, 0, VK_FORMAT_R16G16_SFLOAT, 0));
const VertexInputState vertex3Half(VertexInputBinding(0, sizeof(short) * 3), VertexInputAttribute(0, 0, VK_FORMAT_R16G16B16_SFLOAT, 0));
const VertexInputState vertex4Half(VertexInputBinding(0, sizeof(short) * 4), VertexInputAttribute(0, 0, VK_FORMAT_R16G16B16A16_SFLOAT, 0));

const VertexInputState vertexFloat(VertexInputBinding(0, sizeof(float)), VertexInputAttribute(0, 0, VK_FORMAT_R32_SFLOAT, 0));
const VertexInputState vertex2Float(VertexInputBinding(0, sizeof(float) * 2), VertexInputAttribute(0, 0, VK_FORMAT_R32G32_SFLOAT, 0));
const VertexInputState vertex3Float(VertexInputBinding(0, sizeof(float) * 3), VertexInputAttribute(0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0));
const VertexInputState vertex4Float(VertexInputBinding(0, sizeof(float) * 4), VertexInputAttribute(0, 0, VK_FORMAT_R32G32B32A32_SFLOAT, 0));
} // namespace states
} // namespace magma
