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

#ifdef VK_AMD_rasterization_order
namespace magma
{
    /* This extension enables applications to opt into a relaxed, implementation
       defined primitive rasterization order that may allow better parallel processing
       of primitives and thus enabling higher primitive throughput. */

    struct RasterizationOrderState : RasterizationState
    {
        constexpr RasterizationOrderState(const RasterizationState& state,
            VkRasterizationOrderAMD rasterizationOrder) noexcept;
        std::size_t hash() const noexcept;
        constexpr bool operator==(const RasterizationOrderState&) const noexcept;

        const VkPipelineRasterizationStateRasterizationOrderAMD order;
    };

    #include "rasterizationOrderState.inl"

    namespace renderstate
    {
        constexpr RasterizationOrderState fillCullNoneStrictCCW(fillCullNoneCCW, VK_RASTERIZATION_ORDER_STRICT_AMD);
        constexpr RasterizationOrderState fillCullBackStrictCCW(fillCullBackCCW, VK_RASTERIZATION_ORDER_STRICT_AMD);
        constexpr RasterizationOrderState fillCullFrontStrictCCW(fillCullFrontCCW, VK_RASTERIZATION_ORDER_STRICT_AMD);
        constexpr RasterizationOrderState fillCullFrontAndBackStrictCCW(fillCullFrontAndBackCCW, VK_RASTERIZATION_ORDER_STRICT_AMD);

        constexpr RasterizationOrderState lineCullNoneStrictCCW(lineCullNoneCCW, VK_RASTERIZATION_ORDER_STRICT_AMD);
        constexpr RasterizationOrderState lineCullBackStrictCCW(lineCullBackCCW, VK_RASTERIZATION_ORDER_STRICT_AMD);
        constexpr RasterizationOrderState lineCullFrontStrictCCW(lineCullFrontCCW, VK_RASTERIZATION_ORDER_STRICT_AMD);
        constexpr RasterizationOrderState lineCullFrontAndBackStrictCCW(lineCullFrontAndBackCCW, VK_RASTERIZATION_ORDER_STRICT_AMD);

        constexpr RasterizationOrderState pointCullNoneStrictCCW(pointCullNoneCCW, VK_RASTERIZATION_ORDER_STRICT_AMD);
        constexpr RasterizationOrderState pointCullBackStrictCCW(pointCullBackCCW, VK_RASTERIZATION_ORDER_STRICT_AMD);
        constexpr RasterizationOrderState pointCullFrontStrictCCW(pointCullFrontCCW, VK_RASTERIZATION_ORDER_STRICT_AMD);
        constexpr RasterizationOrderState pointCullFrontAndBackStrictCCW(pointCullFrontAndBackCCW, VK_RASTERIZATION_ORDER_STRICT_AMD);

        constexpr RasterizationOrderState fillCullNoneStrictCW(fillCullNoneCW, VK_RASTERIZATION_ORDER_STRICT_AMD);
        constexpr RasterizationOrderState fillCullBackStrictCW(fillCullBackCW, VK_RASTERIZATION_ORDER_STRICT_AMD);
        constexpr RasterizationOrderState fillCullFrontStrictCW(fillCullFrontCW, VK_RASTERIZATION_ORDER_STRICT_AMD);
        constexpr RasterizationOrderState fillCullFrontAndBackStrictCW(fillCullFrontAndBackCW, VK_RASTERIZATION_ORDER_STRICT_AMD);

        constexpr RasterizationOrderState lineCullNoneStrictCW(lineCullNoneCW, VK_RASTERIZATION_ORDER_STRICT_AMD);
        constexpr RasterizationOrderState lineCullBackStrictCW(lineCullBackCW, VK_RASTERIZATION_ORDER_STRICT_AMD);
        constexpr RasterizationOrderState lineCullFrontStrictCW(lineCullFrontCW, VK_RASTERIZATION_ORDER_STRICT_AMD);
        constexpr RasterizationOrderState lineCullFrontAndBackStrictCW(lineCullFrontAndBackCW, VK_RASTERIZATION_ORDER_STRICT_AMD);

        constexpr RasterizationOrderState pointCullNoneStrictCW(pointCullNoneCW, VK_RASTERIZATION_ORDER_STRICT_AMD);
        constexpr RasterizationOrderState pointCullBackStrictCW(pointCullBackCW, VK_RASTERIZATION_ORDER_STRICT_AMD);
        constexpr RasterizationOrderState pointCullFrontStrictCW(pointCullFrontCW, VK_RASTERIZATION_ORDER_STRICT_AMD);
        constexpr RasterizationOrderState pointCullFrontAndBackStrictCW(pointCullFrontAndBackCW, VK_RASTERIZATION_ORDER_STRICT_AMD);

        constexpr RasterizationOrderState fillCullNoneRelaxedCCW(fillCullNoneCCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
        constexpr RasterizationOrderState fillCullBackRelaxedCCW(fillCullBackCCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
        constexpr RasterizationOrderState fillCullFrontRelaxedCCW(fillCullFrontCCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
        constexpr RasterizationOrderState fillCullFrontAndBackRelaxedCCW(fillCullFrontAndBackCCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);

        constexpr RasterizationOrderState lineCullNoneRelaxedCCW(lineCullNoneCCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
        constexpr RasterizationOrderState lineCullBackRelaxedCCW(lineCullBackCCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
        constexpr RasterizationOrderState lineCullFrontRelaxedCCW(lineCullFrontCCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
        constexpr RasterizationOrderState lineCullFrontAndBackRelaxedCCW(lineCullFrontAndBackCCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);

        constexpr RasterizationOrderState pointCullNoneRelaxedCCW(pointCullNoneCCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
        constexpr RasterizationOrderState pointCullBackRelaxedCCW(pointCullBackCCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
        constexpr RasterizationOrderState pointCullFrontRelaxedCCW(pointCullFrontCCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
        constexpr RasterizationOrderState pointCullFrontAndBackRelaxedCCW(pointCullFrontAndBackCCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);

        constexpr RasterizationOrderState fillCullNoneRelaxedCW(fillCullNoneCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
        constexpr RasterizationOrderState fillCullBackRelaxedCW(fillCullBackCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
        constexpr RasterizationOrderState fillCullFrontRelaxedCW(fillCullFrontCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
        constexpr RasterizationOrderState fillCullFrontAndBackRelaxedCW(fillCullFrontAndBackCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);

        constexpr RasterizationOrderState lineCullNoneRelaxedCW(lineCullNoneCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
        constexpr RasterizationOrderState lineCullBackRelaxedCW(lineCullBackCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
        constexpr RasterizationOrderState lineCullFrontRelaxedCW(lineCullFrontCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
        constexpr RasterizationOrderState lineCullFrontAndBackRelaxedCW(lineCullFrontAndBackCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);

        constexpr RasterizationOrderState pointCullNoneRelaxedCW(pointCullNoneCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
        constexpr RasterizationOrderState pointCullBackRelaxedCW(pointCullBackCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
        constexpr RasterizationOrderState pointCullFrontRelaxedCW(pointCullFrontCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
        constexpr RasterizationOrderState pointCullFrontAndBackRelaxedCW(pointCullFrontAndBackCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
    } // namespace renderstate
} // namespace magma
#endif // VK_AMD_rasterization_order
