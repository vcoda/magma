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
#include "samplerState.h"

namespace magma
{
    /* Depth sampler state with comparison function for Percentage Closer Filtering etc. */

    struct DepthComparisonSamplerState : SamplerState
    {
        constexpr DepthComparisonSamplerState(VkFilter magFilter,
            VkFilter minFilter,
            VkCompareOp compareOp) noexcept;
    };
}

#include "depthComparisonSamplerState.inl"

namespace magma
{
    namespace sampler
    {
        constexpr DepthComparisonSamplerState magMinNearestNever(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_COMPARE_OP_NEVER);
        constexpr DepthComparisonSamplerState magMinNearestLess(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_COMPARE_OP_LESS);
        constexpr DepthComparisonSamplerState magMinNearestEqual(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_COMPARE_OP_EQUAL);
        constexpr DepthComparisonSamplerState magMinNearestLessOrEqual(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_COMPARE_OP_LESS_OR_EQUAL);
        constexpr DepthComparisonSamplerState magMinNearestGreater(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_COMPARE_OP_GREATER);
        constexpr DepthComparisonSamplerState magMinNearestNotEqual(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_COMPARE_OP_NOT_EQUAL);
        constexpr DepthComparisonSamplerState magMinNearestGreaterOrEqual(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_COMPARE_OP_GREATER_OR_EQUAL);
        constexpr DepthComparisonSamplerState magMinNearestAlways(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_COMPARE_OP_ALWAYS);

        constexpr DepthComparisonSamplerState magMinLinearNever(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_COMPARE_OP_NEVER);
        constexpr DepthComparisonSamplerState magMinLinearLess(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_COMPARE_OP_LESS);
        constexpr DepthComparisonSamplerState magMinLinearEqual(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_COMPARE_OP_EQUAL);
        constexpr DepthComparisonSamplerState magMinLinearLessOrEqual(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_COMPARE_OP_LESS_OR_EQUAL);
        constexpr DepthComparisonSamplerState magMinLinearGreater(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_COMPARE_OP_GREATER);
        constexpr DepthComparisonSamplerState magMinLinearNotEqual(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_COMPARE_OP_NOT_EQUAL);
        constexpr DepthComparisonSamplerState magMinLinearGreaterOrEqual(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_COMPARE_OP_GREATER_OR_EQUAL);
        constexpr DepthComparisonSamplerState magMinLinearAlways(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_COMPARE_OP_ALWAYS);
    } // namespace sampler
} // namespace magma
