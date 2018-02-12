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
#include "../vulkan.h"

namespace magma
{
    struct StencilOpState : VkStencilOpState
    {
        StencilOpState(VkStencilOp failOp,
            VkStencilOp passOp,
            VkStencilOp depthFailOp,
            VkCompareOp compareOp,
            uint32_t compareMask = 0x0, 
            uint32_t writeMask = 0x0, 
            uint32_t reference = 0x0);
    };

    namespace states
    {
        extern const StencilOpState stencilAlwaysDontWrite;
    }

    struct DepthStencilState : VkPipelineDepthStencilStateCreateInfo
    {
        DepthStencilState(VkCompareOp depthCompareOp,
            bool depthWriteEnable,
            const StencilOpState& front = states::stencilAlwaysDontWrite,
            const StencilOpState& back = states::stencilAlwaysDontWrite);
    };

    namespace states
    {
        extern const DepthStencilState depthLess;
        extern const DepthStencilState depthEqual;
        extern const DepthStencilState depthLessOrEqual;
        extern const DepthStencilState depthGreater;
        extern const DepthStencilState depthNoEqual;
        extern const DepthStencilState depthGreaterOrEqual;
        extern const DepthStencilState depthAlways;
        extern const DepthStencilState depthNever;

        extern const DepthStencilState depthLessDontWrite;
        extern const DepthStencilState depthEqualDontWrite;
        extern const DepthStencilState depthLessOrEqualDontWrite;
        extern const DepthStencilState depthGreaterDontWrite;
        extern const DepthStencilState depthNoEqualDontWrite;
        extern const DepthStencilState depthGreaterOrEqualDontWrite;
        extern const DepthStencilState depthAlwaysDontWrite;
    } // namespace states
} // namespace magma
