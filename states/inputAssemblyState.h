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
#include "../shared.h"

namespace magma
{
    class InputAssemblyState
    {
    public:
        InputAssemblyState(VkPrimitiveTopology topology, 
            bool primitiveRestartEnable = false);
        MAGMA_TYPE_CAST(VkPipelineInputAssemblyStateCreateInfo, state)

    private:
        VkPipelineInputAssemblyStateCreateInfo state;
    };

    namespace states
    {
        extern const InputAssemblyState pointList;
        extern const InputAssemblyState lineList;
        extern const InputAssemblyState lineStrip;
        extern const InputAssemblyState triangleList;
        extern const InputAssemblyState triangleStrip;
        extern const InputAssemblyState triangleFan;
        extern const InputAssemblyState lineListWithAdjacency;
        extern const InputAssemblyState lineStripWithAdjacency;
        extern const InputAssemblyState triangleListWithAdjacency;
        extern const InputAssemblyState triangleStripWithAdjacency;
        extern const InputAssemblyState patchList;

        extern const InputAssemblyState pointListRestart;
        extern const InputAssemblyState lineListRestart;
        extern const InputAssemblyState lineStripRestart;
        extern const InputAssemblyState triangleListRestart;
        extern const InputAssemblyState triangleStripRestart;
        extern const InputAssemblyState triangleFanRestart;
        extern const InputAssemblyState lineListWithAdjacencyRestart;
        extern const InputAssemblyState lineStripWithAdjacencyRestart;
        extern const InputAssemblyState triangleListWithAdjacencyRestart;
        extern const InputAssemblyState triangleStripWithAdjacencyRestart;
        extern const InputAssemblyState patchListRestart;
    } // namespace states
} // namespace magma
