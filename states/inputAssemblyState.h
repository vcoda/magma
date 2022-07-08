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
#pragma once

namespace magma
{
    /* Each draw is made up of zero or more vertices and zero or more instances,
       which are processed by the device and result in the assembly of primitives.
       Primitives are assembled according to the input assembly state. */

    struct InputAssemblyState : VkPipelineInputAssemblyStateCreateInfo
    {
        constexpr InputAssemblyState(VkPrimitiveTopology topology,
            bool primitiveRestartEnable = false) noexcept;
        constexpr hash_t hash() const noexcept;
        constexpr bool operator==(const InputAssemblyState&) const noexcept;
    };
} // namespace magma

#include "inputAssemblyState.inl"

namespace magma
{
#define MAGMA_DEFINE_INPUT_ASSEMBLY_STATE_PERMUTATIONS(name, topology)\
    constexpr InputAssemblyState name(topology, false);\
    constexpr InputAssemblyState name##Restart(topology, true);

    namespace renderstate
    {
        MAGMA_DEFINE_INPUT_ASSEMBLY_STATE_PERMUTATIONS(pointList, VK_PRIMITIVE_TOPOLOGY_POINT_LIST)
        MAGMA_DEFINE_INPUT_ASSEMBLY_STATE_PERMUTATIONS(lineList, VK_PRIMITIVE_TOPOLOGY_LINE_LIST)
        MAGMA_DEFINE_INPUT_ASSEMBLY_STATE_PERMUTATIONS(lineStrip, VK_PRIMITIVE_TOPOLOGY_LINE_STRIP)
        MAGMA_DEFINE_INPUT_ASSEMBLY_STATE_PERMUTATIONS(triangleList, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST)
        MAGMA_DEFINE_INPUT_ASSEMBLY_STATE_PERMUTATIONS(triangleStrip, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP)
        MAGMA_DEFINE_INPUT_ASSEMBLY_STATE_PERMUTATIONS(triangleFan, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN)
        MAGMA_DEFINE_INPUT_ASSEMBLY_STATE_PERMUTATIONS(lineListWithAdjacency, VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY)
        MAGMA_DEFINE_INPUT_ASSEMBLY_STATE_PERMUTATIONS(lineStripWithAdjacency, VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY)
        MAGMA_DEFINE_INPUT_ASSEMBLY_STATE_PERMUTATIONS(triangleListWithAdjacency, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY)
        MAGMA_DEFINE_INPUT_ASSEMBLY_STATE_PERMUTATIONS(triangleStripWithAdjacency, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY)
        MAGMA_DEFINE_INPUT_ASSEMBLY_STATE_PERMUTATIONS(patchList, VK_PRIMITIVE_TOPOLOGY_PATCH_LIST)
    } // namespace renderstate
} // namespace magma
