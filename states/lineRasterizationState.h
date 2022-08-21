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
#include "rasterizationState.h"

#ifdef VK_EXT_line_rasterization
namespace magma
{
    /* Exposes some line rasterization features that are commonly used in CAD applications. */

    struct LineRasterizationState : RasterizationState
    {
        constexpr LineRasterizationState(const RasterizationState& state,
            VkLineRasterizationModeEXT lineRasterizationMode) noexcept;
        constexpr LineRasterizationState(const RasterizationState& state,
            VkLineRasterizationModeEXT lineRasterizationMode,
            uint32_t stippleFactor,
            uint16_t stipplePattern) noexcept;
        constexpr hash_t hash() const noexcept;
        constexpr bool operator==(const LineRasterizationState&) const noexcept;

        const VkPipelineRasterizationLineStateCreateInfoEXT line;
    };
} // namespace magma

#include "lineRasterizationState.inl"

namespace magma
{
#define MAGMA_LINE_RASTERIZATION_STATE_STIPPLE_PATTERNS(Mode, Factor, lineRasterizationMode, stippleFactor)\
    constexpr LineRasterizationState stippledLine##Mode##PatternA##Factor(fillCullNoneCCw, lineRasterizationMode, stippleFactor, 0b0101010101010101);\
    constexpr LineRasterizationState stippledLine##Mode##PatternB##Factor(fillCullNoneCCw, lineRasterizationMode, stippleFactor, 0b0111011101110111);\
    constexpr LineRasterizationState stippledLine##Mode##PatternC##Factor(fillCullNoneCCw, lineRasterizationMode, stippleFactor, 0b0001111100011111);\
    constexpr LineRasterizationState stippledLine##Mode##PatternD##Factor(fillCullNoneCCw, lineRasterizationMode, stippleFactor, 0b0001000100010001);

#define MAGMA_LINE_RASTERIZATION_STATE_STIPPLE_FACTORS(Mode, lineRasterizationMode)\
    MAGMA_LINE_RASTERIZATION_STATE_STIPPLE_PATTERNS(Mode, x1, lineRasterizationMode, 1);\
    MAGMA_LINE_RASTERIZATION_STATE_STIPPLE_PATTERNS(Mode, x2, lineRasterizationMode, 2);\
    MAGMA_LINE_RASTERIZATION_STATE_STIPPLE_PATTERNS(Mode, x4, lineRasterizationMode, 4);\
    MAGMA_LINE_RASTERIZATION_STATE_STIPPLE_PATTERNS(Mode, x6, lineRasterizationMode, 6);\
    MAGMA_LINE_RASTERIZATION_STATE_STIPPLE_PATTERNS(Mode, x8, lineRasterizationMode, 8);

#define MAGMA_LINE_RASTERIZATION_STATE_PERMUTATIONS(Mode, lineRasterizationMode)\
    constexpr LineRasterizationState solidLine##Mode(fillCullNoneCCw, lineRasterizationMode);\
    MAGMA_LINE_RASTERIZATION_STATE_STIPPLE_FACTORS(Mode, lineRasterizationMode)

    namespace renderstate
    {
        MAGMA_LINE_RASTERIZATION_STATE_PERMUTATIONS(Default, VK_LINE_RASTERIZATION_MODE_DEFAULT_EXT)
        MAGMA_LINE_RASTERIZATION_STATE_PERMUTATIONS(Rectangular, VK_LINE_RASTERIZATION_MODE_RECTANGULAR_EXT)
        MAGMA_LINE_RASTERIZATION_STATE_PERMUTATIONS(Bresenham, VK_LINE_RASTERIZATION_MODE_BRESENHAM_EXT)
        MAGMA_LINE_RASTERIZATION_STATE_PERMUTATIONS(SmoothRectangular, VK_LINE_RASTERIZATION_MODE_RECTANGULAR_SMOOTH_EXT)
    } // namespace renderstate
} // namespace magma
#endif // VK_EXT_line_rasterization
