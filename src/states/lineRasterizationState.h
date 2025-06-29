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

#ifdef VK_EXT_line_rasterization
namespace magma
{
    /* Exposes some line rasterization features that are
       commonly used in CAD applications. */

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

        const VkPipelineRasterizationLineStateCreateInfoEXT lineRasterizationInfo;
    };
} // namespace magma

#include "lineRasterizationState.inl"

#define MAGMA_LINE_RASTERIZATION_STATE_STIPPLE_FACTORS(Mode, Pattern, lineRasterizationMode, stipplePattern)\
    constexpr LineRasterizationState stippledLine##Mode##Pattern(fillCullNoneCcw, lineRasterizationMode, 1, stipplePattern);\
    constexpr LineRasterizationState stippledLine##Mode##Pattern##x2(fillCullNoneCcw, lineRasterizationMode, 2, stipplePattern);\
    constexpr LineRasterizationState stippledLine##Mode##Pattern##x4(fillCullNoneCcw, lineRasterizationMode, 4, stipplePattern);\
    constexpr LineRasterizationState stippledLine##Mode##Pattern##x6(fillCullNoneCcw, lineRasterizationMode, 6, stipplePattern);\
    constexpr LineRasterizationState stippledLine##Mode##Pattern##x8(fillCullNoneCcw, lineRasterizationMode, 8, stipplePattern);

#define MAGMA_LINE_RASTERIZATION_STATE_STIPPLE_PATTERNS(Mode, lineRasterizationMode)\
    MAGMA_LINE_RASTERIZATION_STATE_STIPPLE_FACTORS(Mode, PatternA, lineRasterizationMode, 0b0101010101010101); /*Í Í Í Í Í Í Í Í */\
    MAGMA_LINE_RASTERIZATION_STATE_STIPPLE_FACTORS(Mode, PatternB, lineRasterizationMode, 0b0111011101110111); /*ÍÍÍ ÍÍÍ ÍÍÍ ÍÍÍ */\
    MAGMA_LINE_RASTERIZATION_STATE_STIPPLE_FACTORS(Mode, PatternC, lineRasterizationMode, 0b0001111100011111); /*ÍÍÍÍÍ   ÍÍÍÍÍ   */\
    MAGMA_LINE_RASTERIZATION_STATE_STIPPLE_FACTORS(Mode, PatternD, lineRasterizationMode, 0b0000110000111111); /*ÍÍÍÍÍÍ    ÍÍ    */\
    MAGMA_LINE_RASTERIZATION_STATE_STIPPLE_FACTORS(Mode, PatternE, lineRasterizationMode, 0b0000100001111111); /*ÍÍÍÍÍÍÍ    Í    */\
    MAGMA_LINE_RASTERIZATION_STATE_STIPPLE_FACTORS(Mode, PatternF, lineRasterizationMode, 0b0001000100010001); /*Í   Í   Í   Í   */

#define MAGMA_LINE_RASTERIZATION_STATE_PERMUTATIONS(Mode, lineRasterizationMode)\
    constexpr LineRasterizationState solidLine##Mode(fillCullNoneCcw, lineRasterizationMode);\
    MAGMA_LINE_RASTERIZATION_STATE_STIPPLE_PATTERNS(Mode, lineRasterizationMode)

namespace magma:: renderstate
{
    MAGMA_LINE_RASTERIZATION_STATE_PERMUTATIONS(Default, VK_LINE_RASTERIZATION_MODE_DEFAULT_EXT)
    MAGMA_LINE_RASTERIZATION_STATE_PERMUTATIONS(Rectangular, VK_LINE_RASTERIZATION_MODE_RECTANGULAR_EXT)
    MAGMA_LINE_RASTERIZATION_STATE_PERMUTATIONS(Bresenham, VK_LINE_RASTERIZATION_MODE_BRESENHAM_EXT)
    MAGMA_LINE_RASTERIZATION_STATE_PERMUTATIONS(RectangularSmooth, VK_LINE_RASTERIZATION_MODE_RECTANGULAR_SMOOTH_EXT)
} // namespace magma:: renderstate
#endif // VK_EXT_line_rasterization
