/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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
    /* Depth sampler state with comparison function
       for Percentage Closer Filtering etc. */

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
#define MAGMA_DEPTH_COMPARISON_SAMPLER_STATE_PERMUTATIONS(Filter, magFilter, minFilter)\
    constexpr DepthComparisonSamplerState magMin##Filter##Never(magFilter, minFilter, VK_COMPARE_OP_NEVER);\
    constexpr DepthComparisonSamplerState magMin##Filter##Less(magFilter, minFilter, VK_COMPARE_OP_LESS);\
    constexpr DepthComparisonSamplerState magMin##Filter##Equal(magFilter, minFilter, VK_COMPARE_OP_EQUAL);\
    constexpr DepthComparisonSamplerState magMin##Filter##LessOrEqual(magFilter, minFilter, VK_COMPARE_OP_LESS_OR_EQUAL);\
    constexpr DepthComparisonSamplerState magMin##Filter##Greater(magFilter, minFilter, VK_COMPARE_OP_GREATER);\
    constexpr DepthComparisonSamplerState magMin##Filter##NotEqual(magFilter, minFilter, VK_COMPARE_OP_NOT_EQUAL);\
    constexpr DepthComparisonSamplerState magMin##Filter##GreaterOrEqual(magFilter, minFilter, VK_COMPARE_OP_GREATER_OR_EQUAL);\
    constexpr DepthComparisonSamplerState magMin##Filter##Always(magFilter, minFilter, VK_COMPARE_OP_ALWAYS);

    namespace sampler
    {
        MAGMA_DEPTH_COMPARISON_SAMPLER_STATE_PERMUTATIONS(Nearest, VK_FILTER_NEAREST, VK_FILTER_NEAREST)
        MAGMA_DEPTH_COMPARISON_SAMPLER_STATE_PERMUTATIONS(Linear, VK_FILTER_LINEAR, VK_FILTER_LINEAR)
    } // namespace sampler
} // namespace magma
