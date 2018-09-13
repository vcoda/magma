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
#include "inputAssemblyState.h"
#include "../helpers/hash.h"
#include "../shared.h"

namespace magma
{
InputAssemblyState::InputAssemblyState(VkPrimitiveTopology topology,
    bool primitiveRestartEnable /* false */) noexcept
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    this->topology = topology;
    this->primitiveRestartEnable = MAGMA_BOOLEAN(primitiveRestartEnable);
}

size_t InputAssemblyState::hash() const noexcept
{
    return helpers::hashVariadic(
        flags,
        topology,
        primitiveRestartEnable);
}

bool InputAssemblyState::operator==(const InputAssemblyState& other) const noexcept
{
    return (flags == other.flags) &&
        (topology == other.topology) &&
        (primitiveRestartEnable == other.primitiveRestartEnable);
}

namespace states
{
const InputAssemblyState pointList(VK_PRIMITIVE_TOPOLOGY_POINT_LIST);
const InputAssemblyState lineList(VK_PRIMITIVE_TOPOLOGY_LINE_LIST);
const InputAssemblyState lineStrip(VK_PRIMITIVE_TOPOLOGY_LINE_STRIP);
const InputAssemblyState triangleList(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
const InputAssemblyState triangleStrip(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP);
const InputAssemblyState triangleFan(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN);
const InputAssemblyState lineListWithAdjacency(VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY);
const InputAssemblyState lineStripWithAdjacency(VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY);
const InputAssemblyState triangleListWithAdjacency(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY);
const InputAssemblyState triangleStripWithAdjacency(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY);
const InputAssemblyState patchList(VK_PRIMITIVE_TOPOLOGY_PATCH_LIST);

const InputAssemblyState pointListRestart(VK_PRIMITIVE_TOPOLOGY_POINT_LIST, true);
const InputAssemblyState lineListRestart(VK_PRIMITIVE_TOPOLOGY_LINE_LIST, true);
const InputAssemblyState lineStripRestart(VK_PRIMITIVE_TOPOLOGY_LINE_STRIP, true);
const InputAssemblyState triangleListRestart(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, true);
const InputAssemblyState triangleStripRestart(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP, true);
const InputAssemblyState triangleFanRestart(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN, true);
const InputAssemblyState lineListWithAdjacencyRestart(VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY, true);
const InputAssemblyState lineStripWithAdjacencyRestart(VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY, true);
const InputAssemblyState triangleListWithAdjacencyRestart(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY, true);
const InputAssemblyState triangleStripWithAdjacencyRestart(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY, true);
const InputAssemblyState patchListRestart(VK_PRIMITIVE_TOPOLOGY_PATCH_LIST, true);
} // namespace states
} // namespace magma
