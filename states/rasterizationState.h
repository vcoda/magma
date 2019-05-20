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
#include "../api/vulkan.h"

namespace magma
{
    /* Rasterization state of graphics pipeline. */

    struct RasterizationState : VkPipelineRasterizationStateCreateInfo
    {
        constexpr RasterizationState(VkPolygonMode polygonMode,
            VkCullModeFlags cullMode,
            VkFrontFace frontFace,
            bool depthClampEnable = false,
            bool rasterizerDiscardEnable = false) noexcept;
        constexpr size_t hash() const noexcept;
        constexpr bool operator==(const RasterizationState&) const noexcept;
    };

    /* Controls whether to bias fragment depth values:
       depthBiasConstantFactor is a scalar factor controlling the constant depth value added to each fragment.
       depthBiasClamp is the maximum (or minimum) depth bias of a fragment.
       depthBiasSlopeFactor is a scalar factor applied to a fragment's slope in depth bias calculations. */

    struct DepthBiasRasterizationState : RasterizationState
    {
        constexpr DepthBiasRasterizationState(const RasterizationState& state,
            float depthBiasConstantFactor,
            float depthBiasClamp,
            float depthBiasSlopeFactor) noexcept;
    };

    /* When overestimation is enabled, if any part of the primitive, including its edges,
       covers any part of the rectangular pixel area, then a fragment is generated with all
       coverage samples turned on. When underestimation is enabled, fragments are only generated
       if the rectangular pixel area is fully covered by the generating primitive. */

    struct ConservativeRasterizationState : RasterizationState
    {
        constexpr ConservativeRasterizationState(const RasterizationState& state,
            VkConservativeRasterizationModeEXT conservativeRasterizationMode,
            float extraPrimitiveOverestimationSize = 0.f) noexcept;
        constexpr size_t hash() const noexcept;
        constexpr bool operator==(const ConservativeRasterizationState&) const noexcept;

        VkPipelineRasterizationConservativeStateCreateInfoEXT conservative;
    };

    /* This extension enables applications to opt into a relaxed, implementation
       defined primitive rasterization order that may allow better parallel processing
       of primitives and thus enabling higher primitive throughput. */

    struct RasterizationOrderState : RasterizationState
    {
        constexpr RasterizationOrderState(const RasterizationState& state,
            VkRasterizationOrderAMD rasterizationOrder) noexcept;
        size_t hash() const noexcept;
        constexpr bool operator==(const RasterizationOrderState&) const noexcept;

        VkPipelineRasterizationStateRasterizationOrderAMD order;
    };
}

#include "rasterizationState.inl"

namespace magma
{
    namespace renderstates
    {
        constexpr RasterizationState fillCullNoneCCW(VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FRONT_FACE_COUNTER_CLOCKWISE);
        constexpr RasterizationState fillCullBackCCW(VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE);
        constexpr RasterizationState fillCullFrontCCW(VK_POLYGON_MODE_FILL, VK_CULL_MODE_FRONT_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE);
        constexpr RasterizationState fillCullFrontAndBackCCW(VK_POLYGON_MODE_FILL, VK_CULL_MODE_FRONT_AND_BACK, VK_FRONT_FACE_COUNTER_CLOCKWISE);

        constexpr RasterizationState lineCullNoneCCW(VK_POLYGON_MODE_LINE, VK_CULL_MODE_NONE, VK_FRONT_FACE_COUNTER_CLOCKWISE);
        constexpr RasterizationState lineCullBackCCW(VK_POLYGON_MODE_LINE, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE);
        constexpr RasterizationState lineCullFrontCCW(VK_POLYGON_MODE_LINE, VK_CULL_MODE_FRONT_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE);
        constexpr RasterizationState lineCullFrontAndBackCCW(VK_POLYGON_MODE_LINE, VK_CULL_MODE_FRONT_AND_BACK, VK_FRONT_FACE_COUNTER_CLOCKWISE);

        constexpr RasterizationState pointCullNoneCCW(VK_POLYGON_MODE_POINT, VK_CULL_MODE_NONE, VK_FRONT_FACE_COUNTER_CLOCKWISE);
        constexpr RasterizationState pointCullBackCCW(VK_POLYGON_MODE_POINT, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE);
        constexpr RasterizationState pointCullFrontCCW(VK_POLYGON_MODE_POINT, VK_CULL_MODE_FRONT_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE);
        constexpr RasterizationState pointCullFrontAndBackCCW(VK_POLYGON_MODE_POINT, VK_CULL_MODE_FRONT_AND_BACK, VK_FRONT_FACE_COUNTER_CLOCKWISE);

        constexpr RasterizationState fillCullNoneCW(VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE);
        constexpr RasterizationState fillCullBackCW(VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_CLOCKWISE);
        constexpr RasterizationState fillCullFrontCW(VK_POLYGON_MODE_FILL, VK_CULL_MODE_FRONT_BIT, VK_FRONT_FACE_CLOCKWISE);
        constexpr RasterizationState fillCullFrontAndBackCW(VK_POLYGON_MODE_FILL, VK_CULL_MODE_FRONT_AND_BACK, VK_FRONT_FACE_CLOCKWISE);

        constexpr RasterizationState lineCullNoneCW(VK_POLYGON_MODE_LINE, VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE);
        constexpr RasterizationState lineCullBackCW(VK_POLYGON_MODE_LINE, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_CLOCKWISE);
        constexpr RasterizationState lineCullFrontCW(VK_POLYGON_MODE_LINE, VK_CULL_MODE_FRONT_BIT, VK_FRONT_FACE_CLOCKWISE);
        constexpr RasterizationState lineCullFrontAndBackCW(VK_POLYGON_MODE_LINE, VK_CULL_MODE_FRONT_AND_BACK, VK_FRONT_FACE_CLOCKWISE);

        constexpr RasterizationState pointCullNoneCW(VK_POLYGON_MODE_POINT, VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE);
        constexpr RasterizationState pointCullBackCW(VK_POLYGON_MODE_POINT, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_CLOCKWISE);
        constexpr RasterizationState pointCullFrontCW(VK_POLYGON_MODE_POINT, VK_CULL_MODE_FRONT_BIT, VK_FRONT_FACE_CLOCKWISE);
        constexpr RasterizationState pointCullFrontAndBackCW(VK_POLYGON_MODE_POINT, VK_CULL_MODE_FRONT_AND_BACK, VK_FRONT_FACE_CLOCKWISE);

        constexpr ConservativeRasterizationState fillCullNoneCCWOverestimate(fillCullNoneCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
        constexpr ConservativeRasterizationState fillCullBackCCWOverestimate(fillCullBackCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
        constexpr ConservativeRasterizationState fillCullFrontCCWOverestimate(fillCullFrontCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
        constexpr ConservativeRasterizationState fillCullFrontAndBackCCWOverestimate(fillCullFrontAndBackCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);

        constexpr ConservativeRasterizationState lineCullNoneCCWOverestimate(lineCullNoneCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
        constexpr ConservativeRasterizationState lineCullBackCCWOverestimate(lineCullBackCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
        constexpr ConservativeRasterizationState lineCullFrontCCWOverestimate(lineCullFrontCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
        constexpr ConservativeRasterizationState lineCullFrontAndBackCCWOverestimate(lineCullFrontAndBackCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);

        constexpr ConservativeRasterizationState pointCullNoneCCWOverestimate(pointCullNoneCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
        constexpr ConservativeRasterizationState pointCullBackCCWOverestimate(pointCullBackCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
        constexpr ConservativeRasterizationState pointCullFrontCCWOverestimate(pointCullFrontCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
        constexpr ConservativeRasterizationState pointCullFrontAndBackCCWOverestimate(pointCullFrontAndBackCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);

        constexpr ConservativeRasterizationState fillCullNoneCWOverestimate(fillCullNoneCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
        constexpr ConservativeRasterizationState fillCullBackCWOverestimate(fillCullBackCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
        constexpr ConservativeRasterizationState fillCullFrontCWOverestimate(fillCullFrontCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
        constexpr ConservativeRasterizationState fillCullFrontAndBackCWOverestimate(fillCullFrontAndBackCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);

        constexpr ConservativeRasterizationState lineCullNoneCWOverestimate(lineCullNoneCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
        constexpr ConservativeRasterizationState lineCullBackCWOverestimate(lineCullBackCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
        constexpr ConservativeRasterizationState lineCullFrontCWOverestimate(lineCullFrontCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
        constexpr ConservativeRasterizationState lineCullFrontAndBackCWOverestimate(lineCullFrontAndBackCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);

        constexpr ConservativeRasterizationState pointCullNoneCWOverestimate(pointCullNoneCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
        constexpr ConservativeRasterizationState pointCullBackCWOverestimate(pointCullBackCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
        constexpr ConservativeRasterizationState pointCullFrontCWOverestimate(pointCullFrontCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
        constexpr ConservativeRasterizationState pointCullFrontAndBackCWOverestimate(pointCullFrontAndBackCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);

        constexpr ConservativeRasterizationState fillCullNoneCCWUnderestimate(fillCullNoneCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
        constexpr ConservativeRasterizationState fillCullBackCCWUnderestimate(fillCullBackCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
        constexpr ConservativeRasterizationState fillCullFrontCCWUnderestimate(fillCullFrontCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
        constexpr ConservativeRasterizationState fillCullFrontAndBackCCWUnderestimate(fillCullFrontAndBackCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);

        constexpr ConservativeRasterizationState lineCullNoneCCWUnderestimate(lineCullNoneCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
        constexpr ConservativeRasterizationState lineCullBackCCWUnderestimate(lineCullBackCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
        constexpr ConservativeRasterizationState lineCullFrontCCWUnderestimate(lineCullFrontCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
        constexpr ConservativeRasterizationState lineCullFrontAndBackCCWUnderestimate(lineCullFrontAndBackCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);

        constexpr ConservativeRasterizationState pointCullNoneCCWUnderestimate(pointCullNoneCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
        constexpr ConservativeRasterizationState pointCullBackCCWUnderestimate(pointCullBackCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
        constexpr ConservativeRasterizationState pointCullFrontCCWUnderestimate(pointCullFrontCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
        constexpr ConservativeRasterizationState pointCullFrontAndBackCCWUnderestimate(pointCullFrontAndBackCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);

        constexpr ConservativeRasterizationState fillCullNoneCWUnderestimate(fillCullNoneCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
        constexpr ConservativeRasterizationState fillCullBackCWUnderestimate(fillCullBackCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
        constexpr ConservativeRasterizationState fillCullFrontCWUnderestimate(fillCullFrontCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
        constexpr ConservativeRasterizationState fillCullFrontAndBackCWUnderestimate(fillCullFrontAndBackCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);

        constexpr ConservativeRasterizationState lineCullNoneCWUnderestimate(lineCullNoneCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
        constexpr ConservativeRasterizationState lineCullBackCWUnderestimate(lineCullBackCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
        constexpr ConservativeRasterizationState lineCullFrontCWUnderestimate(lineCullFrontCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
        constexpr ConservativeRasterizationState lineCullFrontAndBackCWUnderestimate(lineCullFrontAndBackCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);

        constexpr ConservativeRasterizationState pointCullNoneCWUnderestimate(pointCullNoneCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
        constexpr ConservativeRasterizationState pointCullBackCWUnderestimate(pointCullBackCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
        constexpr ConservativeRasterizationState pointCullFrontCWUnderestimate(pointCullFrontCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
        constexpr ConservativeRasterizationState pointCullFrontAndBackCWUnderestimate(pointCullFrontAndBackCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);

        constexpr RasterizationOrderState fillCullNoneCCWRelaxed(fillCullNoneCCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
        constexpr RasterizationOrderState fillCullBackCCWRelaxed(fillCullBackCCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
        constexpr RasterizationOrderState fillCullFrontCCWRelaxed(fillCullFrontCCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
        constexpr RasterizationOrderState fillCullFrontAndBackCCWRelaxed(fillCullFrontAndBackCCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);

        constexpr RasterizationOrderState lineCullNoneCCWRelaxed(lineCullNoneCCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
        constexpr RasterizationOrderState lineCullBackCCWRelaxed(lineCullBackCCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
        constexpr RasterizationOrderState lineCullFrontCCWRelaxed(lineCullFrontCCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
        constexpr RasterizationOrderState lineCullFrontAndBackCCWRelaxed(lineCullFrontAndBackCCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);

        constexpr RasterizationOrderState pointCullNoneCCWRelaxed(pointCullNoneCCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
        constexpr RasterizationOrderState pointCullBackCCWRelaxed(pointCullBackCCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
        constexpr RasterizationOrderState pointCullFrontCCWRelaxed(pointCullFrontCCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
        constexpr RasterizationOrderState pointCullFrontAndBackCCWRelaxed(pointCullFrontAndBackCCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);

        constexpr RasterizationOrderState fillCullNoneCWRelaxed(fillCullNoneCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
        constexpr RasterizationOrderState fillCullBackCWRelaxed(fillCullBackCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
        constexpr RasterizationOrderState fillCullFrontCWRelaxed(fillCullFrontCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
        constexpr RasterizationOrderState fillCullFrontAndBackCWRelaxed(fillCullFrontAndBackCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);

        constexpr RasterizationOrderState lineCullNoneCWRelaxed(lineCullNoneCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
        constexpr RasterizationOrderState lineCullBackCWRelaxed(lineCullBackCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
        constexpr RasterizationOrderState lineCullFrontCWRelaxed(lineCullFrontCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
        constexpr RasterizationOrderState lineCullFrontAndBackCWRelaxed(lineCullFrontAndBackCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);

        constexpr RasterizationOrderState pointCullNoneCWRelaxed(pointCullNoneCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
        constexpr RasterizationOrderState pointCullBackCWRelaxed(pointCullBackCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
        constexpr RasterizationOrderState pointCullFrontCWRelaxed(pointCullFrontCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
        constexpr RasterizationOrderState pointCullFrontAndBackCWRelaxed(pointCullFrontAndBackCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
    } // namespace renderstates
 } // namespace magma


