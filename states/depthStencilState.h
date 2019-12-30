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
#include "../detail/compare.h"

namespace magma
{
    /* The stencil test is controlled with the front and back stencil state. */

    struct StencilOpState : VkStencilOpState
    {
        constexpr StencilOpState(VkStencilOp failOp,
            VkStencilOp passOp,
            VkStencilOp depthFailOp,
            VkCompareOp compareOp,
            uint32_t compareMask = 0x0,
            uint32_t writeMask = 0x0,
            uint32_t reference = 0):
            VkStencilOpState{failOp, passOp, depthFailOp, compareOp, compareMask, writeMask, reference}
        {}
        constexpr std::size_t hash() const;
        constexpr bool operator==(const StencilOpState&) const;
    };

    namespace renderstates
    {
        constexpr StencilOpState stencilAlwaysDontWrite(VK_STENCIL_OP_KEEP, VK_STENCIL_OP_KEEP, VK_STENCIL_OP_KEEP, VK_COMPARE_OP_ALWAYS, 0x0, 0x0, 0);
    }

    /* The stencil test conditionally disables coverage of a sample
       based on the outcome of a comparison between the stencil value
       in the depth/stencil attachment at fragment location and a reference value.
       The stencil test also updates the value in the stencil attachment,
       depending on the test state, the stencil value and the stencil write masks. */

    struct DepthStencilState : VkPipelineDepthStencilStateCreateInfo
    {
        constexpr DepthStencilState(VkCompareOp depthCompareOp,
            bool depthWriteEnable,
            const StencilOpState& front = renderstates::stencilAlwaysDontWrite,
            const StencilOpState& back = renderstates::stencilAlwaysDontWrite);
        constexpr DepthStencilState(const DepthStencilState& state,
            const StencilOpState& front,
            const StencilOpState& back);
        constexpr std::size_t hash() const;
        constexpr bool operator==(const DepthStencilState&) const;
    };

    /* The depth bounds test conditionally disables coverage of a sample
       based on the outcome of a comparison between the value Z in the depth attachment
       at location (xf,yf) (for the appropriate sample) and a range of values. */

    struct DepthBoundsState : DepthStencilState
    {
        constexpr DepthBoundsState(const DepthStencilState& state,
            float minDepthBounds,
            float maxDepthBounds);
    };
}

#include "depthStencilState.inl"

namespace magma
{
    namespace renderstates
    {
        constexpr StencilOpState stencilZeroDepthPass(VK_STENCIL_OP_KEEP, VK_STENCIL_OP_ZERO, VK_STENCIL_OP_KEEP, VK_COMPARE_OP_LESS, 0xFF, 0xFF, 0);
        constexpr StencilOpState stencilZeroDepthFail(VK_STENCIL_OP_KEEP, VK_STENCIL_OP_KEEP, VK_STENCIL_OP_ZERO, VK_COMPARE_OP_LESS, 0xFF, 0xFF, 0);
        constexpr StencilOpState stencilOneDepthPass(VK_STENCIL_OP_KEEP, VK_STENCIL_OP_REPLACE, VK_STENCIL_OP_KEEP, VK_COMPARE_OP_LESS, 0xFF, 0xFF, 1);
        constexpr StencilOpState stencilOneDepthFail(VK_STENCIL_OP_KEEP, VK_STENCIL_OP_KEEP, VK_STENCIL_OP_REPLACE, VK_COMPARE_OP_LESS, 0xFF, 0xFF, 1);
        constexpr StencilOpState stencilInvertDepthPass(VK_STENCIL_OP_KEEP, VK_STENCIL_OP_INVERT, VK_STENCIL_OP_KEEP, VK_COMPARE_OP_LESS, 0xFF, 0xFF, 0);
        constexpr StencilOpState stencilInvertDepthFail(VK_STENCIL_OP_KEEP, VK_STENCIL_OP_KEEP, VK_STENCIL_OP_INVERT, VK_COMPARE_OP_LESS, 0xFF, 0xFF, 0);

        constexpr StencilOpState stencilIncrementClampDepthPass(VK_STENCIL_OP_KEEP, VK_STENCIL_OP_INCREMENT_AND_CLAMP, VK_STENCIL_OP_KEEP, VK_COMPARE_OP_LESS, 0xFF, 0xFF, 0);
        constexpr StencilOpState stencilIncrementWrapDepthPass(VK_STENCIL_OP_KEEP, VK_STENCIL_OP_INCREMENT_AND_WRAP, VK_STENCIL_OP_KEEP, VK_COMPARE_OP_LESS, 0xFF, 0xFF, 0);
        constexpr StencilOpState stencilDecrementClampDepthPass(VK_STENCIL_OP_KEEP, VK_STENCIL_OP_DECREMENT_AND_CLAMP, VK_STENCIL_OP_KEEP, VK_COMPARE_OP_LESS, 0xFF, 0xFF, 0);
        constexpr StencilOpState stencilDecrementWrapDepthPass(VK_STENCIL_OP_KEEP, VK_STENCIL_OP_DECREMENT_AND_WRAP, VK_STENCIL_OP_KEEP, VK_COMPARE_OP_LESS, 0xFF, 0xFF, 0);

        constexpr StencilOpState stencilIncrementClampDepthFail(VK_STENCIL_OP_KEEP, VK_STENCIL_OP_KEEP, VK_STENCIL_OP_INCREMENT_AND_CLAMP, VK_COMPARE_OP_LESS, 0xFF, 0xFF, 0);
        constexpr StencilOpState stencilIncrementWrapDepthFail(VK_STENCIL_OP_KEEP, VK_STENCIL_OP_KEEP, VK_STENCIL_OP_INCREMENT_AND_WRAP, VK_COMPARE_OP_LESS, 0xFF, 0xFF, 0);
        constexpr StencilOpState stencilDecrementClampDepthFail(VK_STENCIL_OP_KEEP, VK_STENCIL_OP_KEEP, VK_STENCIL_OP_DECREMENT_AND_CLAMP, VK_COMPARE_OP_LESS, 0xFF, 0xFF, 0);
        constexpr StencilOpState stencilDecrementWrapDepthFail(VK_STENCIL_OP_KEEP, VK_STENCIL_OP_KEEP, VK_STENCIL_OP_DECREMENT_AND_WRAP, VK_COMPARE_OP_LESS, 0xFF, 0xFF, 0);

        constexpr DepthStencilState depthLess(VK_COMPARE_OP_LESS, true);
        constexpr DepthStencilState depthEqual(VK_COMPARE_OP_EQUAL, true);
        constexpr DepthStencilState depthLessOrEqual(VK_COMPARE_OP_LESS_OR_EQUAL, true);
        constexpr DepthStencilState depthGreater(VK_COMPARE_OP_GREATER, true);
        constexpr DepthStencilState depthNoEqual(VK_COMPARE_OP_NOT_EQUAL, true);
        constexpr DepthStencilState depthGreaterOrEqual(VK_COMPARE_OP_GREATER_OR_EQUAL, true);
        constexpr DepthStencilState depthAlways(VK_COMPARE_OP_ALWAYS, true);

        constexpr DepthStencilState depthNeverDontWrite(VK_COMPARE_OP_NEVER, false);
        constexpr DepthStencilState depthLessDontWrite(VK_COMPARE_OP_LESS, false);
        constexpr DepthStencilState depthEqualDontWrite(VK_COMPARE_OP_EQUAL, false);
        constexpr DepthStencilState depthLessOrEqualDontWrite(VK_COMPARE_OP_LESS_OR_EQUAL, false);
        constexpr DepthStencilState depthGreaterDontWrite(VK_COMPARE_OP_GREATER, false);
        constexpr DepthStencilState depthNoEqualDontWrite(VK_COMPARE_OP_NOT_EQUAL, false);
        constexpr DepthStencilState depthGreaterOrEqualDontWrite(VK_COMPARE_OP_GREATER_OR_EQUAL, false);
        constexpr DepthStencilState depthAlwaysDontWrite(VK_COMPARE_OP_ALWAYS, false);
    } // namespace renderstates
} // namespace magma
