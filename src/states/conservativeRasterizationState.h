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
#include "rasterizationState.h"

#ifdef VK_EXT_conservative_rasterization
namespace magma
{
    /* When overestimation is enabled, if any part of the primitive,
       including its edges, covers any part of the rectangular pixel
       area, then a fragment is generated with all coverage samples
       turned on. When underestimation is enabled, fragments are only
       generated if the rectangular pixel area is fully covered by
       the generating primitive. */

    struct ConservativeRasterizationState : RasterizationState
    {
        constexpr ConservativeRasterizationState(const RasterizationState& state,
            VkConservativeRasterizationModeEXT conservativeRasterizationMode,
            float extraPrimitiveOverestimationSize = 0.f) noexcept;
        constexpr ConservativeRasterizationState(const ConservativeRasterizationState&) noexcept;
        constexpr hash_t hash() const noexcept;
        constexpr bool operator==(const ConservativeRasterizationState&) const noexcept;

        const VkPipelineRasterizationConservativeStateCreateInfoEXT conservativeRasterizationInfo;
    };
} // namespace magma

#include "conservativeRasterizationState.inl"

#define MAGMA_CONSERVATIVE_RASTERIZATION_STATE_PERMUTATIONS(polygonMode, ConservativeMode, conservativeRasterizationMode)\
    constexpr ConservativeRasterizationState polygonMode##CullNoneCcw##ConservativeMode(polygonMode##CullNoneCcw, conservativeRasterizationMode);\
    constexpr ConservativeRasterizationState polygonMode##CullFrontCcw##ConservativeMode(polygonMode##CullFrontCcw, conservativeRasterizationMode);\
    constexpr ConservativeRasterizationState polygonMode##CullBackCcw##ConservativeMode(polygonMode##CullBackCcw, conservativeRasterizationMode);\
    constexpr ConservativeRasterizationState polygonMode##CullFrontAndBackCcw##ConservativeMode(polygonMode##CullFrontAndBackCcw, conservativeRasterizationMode);\
    constexpr ConservativeRasterizationState polygonMode##CullNoneCw##ConservativeMode(polygonMode##CullNoneCw, conservativeRasterizationMode);\
    constexpr ConservativeRasterizationState polygonMode##CullFrontCw##ConservativeMode(polygonMode##CullFrontCw, conservativeRasterizationMode);\
    constexpr ConservativeRasterizationState polygonMode##CullBackCw##ConservativeMode(polygonMode##CullBackCw, conservativeRasterizationMode);\
    constexpr ConservativeRasterizationState polygonMode##CullFrontAndBackCw##ConservativeMode(polygonMode##CullFrontAndBackCw, conservativeRasterizationMode);

namespace magma::renderstate
{
    MAGMA_CONSERVATIVE_RASTERIZATION_STATE_PERMUTATIONS(fill, Overestimate, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT)
    MAGMA_CONSERVATIVE_RASTERIZATION_STATE_PERMUTATIONS(line, Overestimate, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT)
    MAGMA_CONSERVATIVE_RASTERIZATION_STATE_PERMUTATIONS(point, Overestimate, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT)
    MAGMA_CONSERVATIVE_RASTERIZATION_STATE_PERMUTATIONS(fill, Underestimate, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT)
    MAGMA_CONSERVATIVE_RASTERIZATION_STATE_PERMUTATIONS(line, Underestimate, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT)
    MAGMA_CONSERVATIVE_RASTERIZATION_STATE_PERMUTATIONS(point, Underestimate, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT)
#ifdef VK_NV_fill_rectangle
    MAGMA_CONSERVATIVE_RASTERIZATION_STATE_PERMUTATIONS(fillRectangle, Overestimate, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT)
    MAGMA_CONSERVATIVE_RASTERIZATION_STATE_PERMUTATIONS(fillRectangle, Underestimate, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT)
#endif // VK_NV_fill_rectangle
} // namespace magma::renderstate
#endif // VK_EXT_conservative_rasterization
