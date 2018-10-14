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
    /* The stencil test is controlled with the front and back stencil state. */

    struct StencilOpState : VkStencilOpState
    {
        StencilOpState(VkStencilOp failOp,
            VkStencilOp passOp,
            VkStencilOp depthFailOp,
            VkCompareOp compareOp,
            uint32_t compareMask = 0x0,
            uint32_t writeMask = 0x0,
            uint32_t reference = 0) noexcept;
        size_t hash() const noexcept;
        bool operator==(const StencilOpState&) const noexcept;
    };

    namespace states
    {
        extern const StencilOpState stencilAlwaysDontWrite;

        extern const StencilOpState stencilZeroDepthPass;
        extern const StencilOpState stencilZeroDepthFail;
        extern const StencilOpState stencilOneDepthPass;
        extern const StencilOpState stencilOneDepthFail;
        extern const StencilOpState stencilInvertDepthPass;
        extern const StencilOpState stencilInvertDepthFail;

        extern const StencilOpState stencilIncrClampDepthPass;
        extern const StencilOpState stencilIncrWrapDepthPass;
        extern const StencilOpState stencilDecrClampDepthPass;
        extern const StencilOpState stencilDecrWrapDepthPass;

        extern const StencilOpState stencilIncrClampDepthFail;
        extern const StencilOpState stencilIncrWrapDepthFail;
        extern const StencilOpState stencilDecrClampDepthFail;
        extern const StencilOpState stencilDecrWrapDepthFail;
    }

    /* The stencil test conditionally disables coverage of a sample
       based on the outcome of a comparison between the stencil value
       in the depth/stencil attachment at fragment location and a reference value.
       The stencil test also updates the value in the stencil attachment,
       depending on the test state, the stencil value and the stencil write masks. */

    struct DepthStencilState : VkPipelineDepthStencilStateCreateInfo
    {
        DepthStencilState(VkCompareOp depthCompareOp,
            bool depthWriteEnable,
            const StencilOpState& front = states::stencilAlwaysDontWrite,
            const StencilOpState& back = states::stencilAlwaysDontWrite) noexcept;
        DepthStencilState(const DepthStencilState& state,
            const StencilOpState& front,
            const StencilOpState& back) noexcept;
        size_t hash() const noexcept;
        bool operator==(const DepthStencilState&) const noexcept;
    };

    /* The depth bounds test conditionally disables coverage of a sample
       based on the outcome of a comparison between the value Z in the depth attachment
       at location (xf,yf) (for the appropriate sample) and a range of values. */

    struct DepthBoundsState : DepthStencilState
    {
        DepthBoundsState(const DepthStencilState& state,
            float minDepthBounds,
            float maxDepthBounds) noexcept;
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
