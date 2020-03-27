/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2019 Victor Coda.

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

namespace magma
{
    /* Rasterization state of graphics pipeline. */

    struct RasterizationState : VkPipelineRasterizationStateCreateInfo
    {
        constexpr RasterizationState(VkPolygonMode polygonMode,
            VkCullModeFlags cullMode,
            VkFrontFace frontFace,
            bool depthClampEnable = false,
            bool rasterizerDiscardEnable = false);
        constexpr std::size_t hash() const;
        constexpr bool operator==(const RasterizationState&) const;
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
            float depthBiasSlopeFactor);
    };

    /* When overestimation is enabled, if any part of the primitive, including its edges,
       covers any part of the rectangular pixel area, then a fragment is generated with all
       coverage samples turned on. When underestimation is enabled, fragments are only generated
       if the rectangular pixel area is fully covered by the generating primitive. */

    struct ConservativeRasterizationState : RasterizationState
    {
        constexpr ConservativeRasterizationState(const RasterizationState& state,
            VkConservativeRasterizationModeEXT conservativeRasterizationMode,
            float extraPrimitiveOverestimationSize = 0.f);
        constexpr std::size_t hash() const;
        constexpr bool operator==(const ConservativeRasterizationState&) const;

        VkPipelineRasterizationConservativeStateCreateInfoEXT conservative;
    };

    /* This extension enables applications to opt into a relaxed, implementation
       defined primitive rasterization order that may allow better parallel processing
       of primitives and thus enabling higher primitive throughput. */

    struct RasterizationOrderState : RasterizationState
    {
        constexpr RasterizationOrderState(const RasterizationState& state,
            VkRasterizationOrderAMD rasterizationOrder);
        constexpr std::size_t hash() const;
        constexpr bool operator==(const RasterizationOrderState&) const;

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

        constexpr RasterizationState fillRectangleCullNoneCCW(VK_POLYGON_MODE_FILL_RECTANGLE_NV, VK_CULL_MODE_NONE, VK_FRONT_FACE_COUNTER_CLOCKWISE);
        constexpr RasterizationState fillRectangleCullBackCCW(VK_POLYGON_MODE_FILL_RECTANGLE_NV, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE);
        constexpr RasterizationState fillRectangleCullFrontCCW(VK_POLYGON_MODE_FILL_RECTANGLE_NV, VK_CULL_MODE_FRONT_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE);
        constexpr RasterizationState fillRectangleCullFrontAndBackCCW(VK_POLYGON_MODE_FILL_RECTANGLE_NV, VK_CULL_MODE_FRONT_AND_BACK, VK_FRONT_FACE_COUNTER_CLOCKWISE);

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

        constexpr RasterizationState fillRectangleCullNoneCW(VK_POLYGON_MODE_FILL_RECTANGLE_NV, VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE);
        constexpr RasterizationState fillRectangleCullBackCW(VK_POLYGON_MODE_FILL_RECTANGLE_NV, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_CLOCKWISE);
        constexpr RasterizationState fillRectangleCullFrontCW(VK_POLYGON_MODE_FILL_RECTANGLE_NV, VK_CULL_MODE_FRONT_BIT, VK_FRONT_FACE_CLOCKWISE);
        constexpr RasterizationState fillRectangleCullFrontAndBackCW(VK_POLYGON_MODE_FILL_RECTANGLE_NV, VK_CULL_MODE_FRONT_AND_BACK, VK_FRONT_FACE_CLOCKWISE);

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
    } // namespace renderstates
 } // namespace magma
