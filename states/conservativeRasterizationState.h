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

#ifdef VK_EXT_conservative_rasterization
namespace magma
{
    /* When overestimation is enabled, if any part of the primitive, including its edges,
       covers any part of the rectangular pixel area, then a fragment is generated with all
       coverage samples turned on. When underestimation is enabled, fragments are only generated
       if the rectangular pixel area is fully covered by the generating primitive. */

    struct ConservativeRasterizationState : RasterizationState
    {
        constexpr ConservativeRasterizationState(const RasterizationState& state,
            VkConservativeRasterizationModeEXT conservativeRasterizationMode,
            float extraPrimitiveOverestimationSize = 0.f) noexcept;
        std::size_t hash() const noexcept;
        constexpr bool operator==(const ConservativeRasterizationState&) const noexcept;

        const VkPipelineRasterizationConservativeStateCreateInfoEXT conservative;
    };

    #include "conservativeRasterizationState.inl"

    namespace renderstate
    {
        constexpr ConservativeRasterizationState fillCullNoneOverestimateCCW(fillCullNoneCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
        constexpr ConservativeRasterizationState fillCullBackOverestimateCCW(fillCullBackCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
        constexpr ConservativeRasterizationState fillCullFrontOverestimateCCW(fillCullFrontCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
        constexpr ConservativeRasterizationState fillCullFrontAndBackOverestimateCCW(fillCullFrontAndBackCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);

        constexpr ConservativeRasterizationState lineCullNoneOverestimateCCW(lineCullNoneCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
        constexpr ConservativeRasterizationState lineCullBackOverestimateCCW(lineCullBackCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
        constexpr ConservativeRasterizationState lineCullFrontOverestimateCCW(lineCullFrontCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
        constexpr ConservativeRasterizationState lineCullFrontAndBackOverestimateCCW(lineCullFrontAndBackCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);

        constexpr ConservativeRasterizationState pointCullNoneOverestimateCCW(pointCullNoneCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
        constexpr ConservativeRasterizationState pointCullBackOverestimateCCW(pointCullBackCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
        constexpr ConservativeRasterizationState pointCullFrontOverestimateCCW(pointCullFrontCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
        constexpr ConservativeRasterizationState pointCullFrontAndBackOverestimateCCW(pointCullFrontAndBackCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);

        constexpr ConservativeRasterizationState fillCullNoneOverestimateCW(fillCullNoneCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
        constexpr ConservativeRasterizationState fillCullBackOverestimateCW(fillCullBackCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
        constexpr ConservativeRasterizationState fillCullFrontOverestimateCW(fillCullFrontCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
        constexpr ConservativeRasterizationState fillCullFrontAndBackOverestimateCW(fillCullFrontAndBackCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);

        constexpr ConservativeRasterizationState lineCullNoneOverestimateCW(lineCullNoneCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
        constexpr ConservativeRasterizationState lineCullBackOverestimateCW(lineCullBackCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
        constexpr ConservativeRasterizationState lineCullFrontOverestimateCW(lineCullFrontCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
        constexpr ConservativeRasterizationState lineCullFrontAndBackOverestimateCW(lineCullFrontAndBackCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);

        constexpr ConservativeRasterizationState pointCullNoneOverestimateCW(pointCullNoneCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
        constexpr ConservativeRasterizationState pointCullBackOverestimateCW(pointCullBackCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
        constexpr ConservativeRasterizationState pointCullFrontOverestimateCW(pointCullFrontCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
        constexpr ConservativeRasterizationState pointCullFrontAndBackOverestimateCW(pointCullFrontAndBackCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);

        constexpr ConservativeRasterizationState fillCullNoneUnderestimateCCW(fillCullNoneCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
        constexpr ConservativeRasterizationState fillCullBackUnderestimateCCW(fillCullBackCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
        constexpr ConservativeRasterizationState fillCullFrontUnderestimateCCW(fillCullFrontCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
        constexpr ConservativeRasterizationState fillCullFrontAndBackUnderestimateCCW(fillCullFrontAndBackCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);

        constexpr ConservativeRasterizationState lineCullNoneUnderestimateCCW(lineCullNoneCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
        constexpr ConservativeRasterizationState lineCullBackUnderestimateCCW(lineCullBackCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
        constexpr ConservativeRasterizationState lineCullFrontUnderestimateCCW(lineCullFrontCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
        constexpr ConservativeRasterizationState lineCullFrontAndBackUnderestimateCCW(lineCullFrontAndBackCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);

        constexpr ConservativeRasterizationState pointCullNoneUnderestimateCCW(pointCullNoneCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
        constexpr ConservativeRasterizationState pointCullBackUnderestimateCCW(pointCullBackCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
        constexpr ConservativeRasterizationState pointCullFrontUnderestimateCCW(pointCullFrontCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
        constexpr ConservativeRasterizationState pointCullFrontAndBackUnderestimateCCW(pointCullFrontAndBackCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);

        constexpr ConservativeRasterizationState fillCullNoneUnderestimateCW(fillCullNoneCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
        constexpr ConservativeRasterizationState fillCullBackUnderestimateCW(fillCullBackCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
        constexpr ConservativeRasterizationState fillCullFrontUnderestimateCW(fillCullFrontCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
        constexpr ConservativeRasterizationState fillCullFrontAndBackUnderestimateCW(fillCullFrontAndBackCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);

        constexpr ConservativeRasterizationState lineCullNoneUnderestimateCW(lineCullNoneCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
        constexpr ConservativeRasterizationState lineCullBackUnderestimateCW(lineCullBackCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
        constexpr ConservativeRasterizationState lineCullFrontUnderestimateCW(lineCullFrontCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
        constexpr ConservativeRasterizationState lineCullFrontAndBackUnderestimateCW(lineCullFrontAndBackCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);

        constexpr ConservativeRasterizationState pointCullNoneUnderestimateCW(pointCullNoneCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
        constexpr ConservativeRasterizationState pointCullBackUnderestimateCW(pointCullBackCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
        constexpr ConservativeRasterizationState pointCullFrontUnderestimateCW(pointCullFrontCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
        constexpr ConservativeRasterizationState pointCullFrontAndBackUnderestimateCW(pointCullFrontAndBackCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
    } // namespace renderstate
} // namespace magma
#endif // VK_EXT_conservative_rasterization
