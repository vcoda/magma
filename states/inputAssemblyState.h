/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2019 Victor Coda.

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
        std::size_t hash() const;
        constexpr bool operator==(const InputAssemblyState&) const;
    };
} // namespace magma

#include "inputAssemblyState.inl"

namespace magma
{
    namespace renderstates
    {
        constexpr InputAssemblyState pointList(VK_PRIMITIVE_TOPOLOGY_POINT_LIST);
        constexpr InputAssemblyState lineList(VK_PRIMITIVE_TOPOLOGY_LINE_LIST);
        constexpr InputAssemblyState lineStrip(VK_PRIMITIVE_TOPOLOGY_LINE_STRIP);
        constexpr InputAssemblyState triangleList(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
        constexpr InputAssemblyState triangleStrip(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP);
        constexpr InputAssemblyState triangleFan(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN);
        constexpr InputAssemblyState lineListWithAdjacency(VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY);
        constexpr InputAssemblyState lineStripWithAdjacency(VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY);
        constexpr InputAssemblyState triangleListWithAdjacency(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY);
        constexpr InputAssemblyState triangleStripWithAdjacency(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY);
        constexpr InputAssemblyState patchList(VK_PRIMITIVE_TOPOLOGY_PATCH_LIST);

        constexpr InputAssemblyState pointListRestart(VK_PRIMITIVE_TOPOLOGY_POINT_LIST, true);
        constexpr InputAssemblyState lineListRestart(VK_PRIMITIVE_TOPOLOGY_LINE_LIST, true);
        constexpr InputAssemblyState lineStripRestart(VK_PRIMITIVE_TOPOLOGY_LINE_STRIP, true);
        constexpr InputAssemblyState triangleListRestart(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, true);
        constexpr InputAssemblyState triangleStripRestart(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP, true);
        constexpr InputAssemblyState triangleFanRestart(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN, true);
        constexpr InputAssemblyState lineListWithAdjacencyRestart(VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY, true);
        constexpr InputAssemblyState lineStripWithAdjacencyRestart(VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY, true);
        constexpr InputAssemblyState triangleListWithAdjacencyRestart(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY, true);
        constexpr InputAssemblyState triangleStripWithAdjacencyRestart(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY, true);
        constexpr InputAssemblyState patchListRestart(VK_PRIMITIVE_TOPOLOGY_PATCH_LIST, true);
    } // namespace renderstates
} // namespace magma
