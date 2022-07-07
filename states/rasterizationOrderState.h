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
        constexpr hash_t hash() const noexcept;
        constexpr bool operator==(const RasterizationOrderState&) const noexcept;

        const VkPipelineRasterizationStateRasterizationOrderAMD order;
    };
} // namespace magma

#include "rasterizationOrderState.inl"

namespace magma
{
#define MAGMA_DEFINE_RASTERIZATION_ORDER_STATE_PERMUTATIONS(name, Order, rasterizationOrder)\
    constexpr RasterizationOrderState name##Order##CullNoneCCW(name##CullNoneCCW, rasterizationOrder);\
    constexpr RasterizationOrderState name##Order##CullBackCCW(name##CullFrontCCW, rasterizationOrder);\
    constexpr RasterizationOrderState name##Order##CullFrontCCW(name##CullBackCCW, rasterizationOrder);\
    constexpr RasterizationOrderState name##Order##CullFrontAndBackCCW(name##CullFrontAndBackCCW, rasterizationOrder);\
    constexpr RasterizationOrderState name##Order##CullNoneCW(name##CullNoneCW, rasterizationOrder);\
    constexpr RasterizationOrderState name##Order##CullBackCW(name##CullFrontCW, rasterizationOrder);\
    constexpr RasterizationOrderState name##Order##CullFrontCW(name##CullBackCW, rasterizationOrder);\
    constexpr RasterizationOrderState name##Order##CullFrontAndBackCW(name##CullFrontAndBackCW, rasterizationOrder);

    namespace renderstate
    {
        MAGMA_DEFINE_RASTERIZATION_ORDER_STATE_PERMUTATIONS(fill, Strict, VK_RASTERIZATION_ORDER_STRICT_AMD)
        MAGMA_DEFINE_RASTERIZATION_ORDER_STATE_PERMUTATIONS(line, Strict, VK_RASTERIZATION_ORDER_STRICT_AMD)
        MAGMA_DEFINE_RASTERIZATION_ORDER_STATE_PERMUTATIONS(point, Strict, VK_RASTERIZATION_ORDER_STRICT_AMD)
        MAGMA_DEFINE_RASTERIZATION_ORDER_STATE_PERMUTATIONS(fill, Relaxed, VK_RASTERIZATION_ORDER_RELAXED_AMD)
        MAGMA_DEFINE_RASTERIZATION_ORDER_STATE_PERMUTATIONS(line, Relaxed, VK_RASTERIZATION_ORDER_RELAXED_AMD)
        MAGMA_DEFINE_RASTERIZATION_ORDER_STATE_PERMUTATIONS(point, Relaxed, VK_RASTERIZATION_ORDER_RELAXED_AMD)
    } // namespace renderstate
} // namespace magma
#endif // VK_AMD_rasterization_order
