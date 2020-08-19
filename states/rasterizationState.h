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
            bool rasterizerDiscardEnable = false) noexcept;
        std::size_t hash() const noexcept;
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

	/* In order to use this state, wideLines member of the VkPhysicalDeviceFeatures structure should be set to VK_TRUE.
	   When this feature is supported, the range and granularity of supported line widths are indicated by the
	   lineWidthRange and lineWidthGranularity members of the VkPhysicalDeviceLimits structure, respectively. */

	struct WideLineRasterizationState : RasterizationState
	{
		constexpr WideLineRasterizationState(const RasterizationState& state,
			float lineWidth) noexcept;
	};

    /* When overestimation is enabled, if any part of the primitive, including its edges,
       covers any part of the rectangular pixel area, then a fragment is generated with all
       coverage samples turned on. When underestimation is enabled, fragments are only generated
       if the rectangular pixel area is fully covered by the generating primitive. */

#ifdef VK_EXT_conservative_rasterization
    struct ConservativeRasterizationState : RasterizationState
    {
        constexpr ConservativeRasterizationState(const RasterizationState& state,
            VkConservativeRasterizationModeEXT conservativeRasterizationMode,
            float extraPrimitiveOverestimationSize = 0.f) noexcept;
        std::size_t hash() const noexcept;
        constexpr bool operator==(const ConservativeRasterizationState&) const noexcept;

        VkPipelineRasterizationConservativeStateCreateInfoEXT conservative;
    };
#endif // VK_EXT_conservative_rasterization

    /* This extension enables applications to opt into a relaxed, implementation
       defined primitive rasterization order that may allow better parallel processing
       of primitives and thus enabling higher primitive throughput. */

#ifdef VK_AMD_rasterization_order
    struct RasterizationOrderState : RasterizationState
    {
        constexpr RasterizationOrderState(const RasterizationState& state,
            VkRasterizationOrderAMD rasterizationOrder) noexcept;
        std::size_t hash() const noexcept;
        constexpr bool operator==(const RasterizationOrderState&) const noexcept;

        VkPipelineRasterizationStateRasterizationOrderAMD order;
    };
#endif // VK_AMD_rasterization_order
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

        constexpr WideLineRasterizationState wideLine1_25CullNoneCCW(lineCullNoneCCW, 1.25f);
        constexpr WideLineRasterizationState wideLine1_25CullBackCCW(lineCullBackCCW, 1.25f);
        constexpr WideLineRasterizationState wideLine1_25CullFrontCCW(lineCullFrontCCW, 1.25f);
        constexpr WideLineRasterizationState wideLine1_25CullFrontAndBackCCW(lineCullFrontAndBackCCW, 1.25f);

        constexpr WideLineRasterizationState wideLine1_5CullNoneCCW(lineCullNoneCCW, 1.5f);
        constexpr WideLineRasterizationState wideLine1_5CullBackCCW(lineCullBackCCW, 1.5f);
        constexpr WideLineRasterizationState wideLine1_5CullFrontCCW(lineCullFrontCCW, 1.5f);
        constexpr WideLineRasterizationState wideLine1_5CullFrontAndBackCCW(lineCullFrontAndBackCCW, 1.5f);

        constexpr WideLineRasterizationState wideLine1_75CullNoneCCW(lineCullNoneCCW, 1.75f);
        constexpr WideLineRasterizationState wideLine1_75CullBackCCW(lineCullBackCCW, 1.75f);
        constexpr WideLineRasterizationState wideLine1_75CullFrontCCW(lineCullFrontCCW, 1.75f);
        constexpr WideLineRasterizationState wideLine1_75CullFrontAndBackCCW(lineCullFrontAndBackCCW, 1.75f);

        constexpr WideLineRasterizationState wideLine2_CullNoneCCW(lineCullNoneCCW, 2.f);
        constexpr WideLineRasterizationState wideLine2_CullBackCCW(lineCullBackCCW, 2.f);
        constexpr WideLineRasterizationState wideLine2_CullFrontCCW(lineCullFrontCCW, 2.f);
        constexpr WideLineRasterizationState wideLine2_CullFrontAndBackCCW(lineCullFrontAndBackCCW, 2.f);

        constexpr WideLineRasterizationState wideLine2_25CullNoneCCW(lineCullNoneCCW, 2.25f);
        constexpr WideLineRasterizationState wideLine2_25CullBackCCW(lineCullBackCCW, 2.25f);
        constexpr WideLineRasterizationState wideLine2_25CullFrontCCW(lineCullFrontCCW, 2.25f);
        constexpr WideLineRasterizationState wideLine2_25CullFrontAndBackCCW(lineCullFrontAndBackCCW, 2.25f);

        constexpr WideLineRasterizationState wideLine2_5CullNoneCCW(lineCullNoneCCW, 2.5f);
        constexpr WideLineRasterizationState wideLine2_5CullBackCCW(lineCullBackCCW, 2.5f);
        constexpr WideLineRasterizationState wideLine2_5CullFrontCCW(lineCullFrontCCW, 2.5f);
        constexpr WideLineRasterizationState wideLine2_5CullFrontAndBackCCW(lineCullFrontAndBackCCW, 2.5f);

        constexpr WideLineRasterizationState wideLine2_75CullNoneCCW(lineCullNoneCCW, 2.75f);
        constexpr WideLineRasterizationState wideLine2_75CullBackCCW(lineCullBackCCW, 2.75f);
        constexpr WideLineRasterizationState wideLine2_75CullFrontCCW(lineCullFrontCCW, 2.75f);
        constexpr WideLineRasterizationState wideLine2_75CullFrontAndBackCCW(lineCullFrontAndBackCCW, 2.75f);

        constexpr WideLineRasterizationState wideLine3_CullNoneCCW(lineCullNoneCCW, 3.f);
        constexpr WideLineRasterizationState wideLine3_CullBackCCW(lineCullBackCCW, 3.f);
        constexpr WideLineRasterizationState wideLine3_CullFrontCCW(lineCullFrontCCW, 3.f);
        constexpr WideLineRasterizationState wideLine3_CullFrontAndBackCCW(lineCullFrontAndBackCCW, 3.f);

        constexpr WideLineRasterizationState wideLine3_25CullNoneCCW(lineCullNoneCCW, 3.25f);
        constexpr WideLineRasterizationState wideLine3_25CullBackCCW(lineCullBackCCW, 3.25f);
        constexpr WideLineRasterizationState wideLine3_25CullFrontCCW(lineCullFrontCCW, 3.25f);
        constexpr WideLineRasterizationState wideLine3_25CullFrontAndBackCCW(lineCullFrontAndBackCCW, 3.25f);

        constexpr WideLineRasterizationState wideLine3_5CullNoneCCW(lineCullNoneCCW, 3.5f);
        constexpr WideLineRasterizationState wideLine3_5CullBackCCW(lineCullBackCCW, 3.5f);
        constexpr WideLineRasterizationState wideLine3_5CullFrontCCW(lineCullFrontCCW, 3.5f);
        constexpr WideLineRasterizationState wideLine3_5CullFrontAndBackCCW(lineCullFrontAndBackCCW, 3.5f);

        constexpr WideLineRasterizationState wideLine3_75CullNoneCCW(lineCullNoneCCW, 3.75f);
        constexpr WideLineRasterizationState wideLine3_75CullBackCCW(lineCullBackCCW, 3.75f);
        constexpr WideLineRasterizationState wideLine3_75CullFrontCCW(lineCullFrontCCW, 3.75f);
        constexpr WideLineRasterizationState wideLine3_75CullFrontAndBackCCW(lineCullFrontAndBackCCW, 3.75f);

        constexpr WideLineRasterizationState wideLine4_CullNoneCCW(lineCullNoneCCW, 4.f);
        constexpr WideLineRasterizationState wideLine4_CullBackCCW(lineCullBackCCW, 4.f);
        constexpr WideLineRasterizationState wideLine4_CullFrontCCW(lineCullFrontCCW, 4.f);
        constexpr WideLineRasterizationState wideLine4_CullFrontAndBackCCW(lineCullFrontAndBackCCW, 4.f);

        constexpr WideLineRasterizationState wideLine1_25CullNoneCW(lineCullNoneCW, 1.25f);
        constexpr WideLineRasterizationState wideLine1_25CullBackCW(lineCullBackCW, 1.25f);
        constexpr WideLineRasterizationState wideLine1_25CullFrontCW(lineCullFrontCW, 1.25f);
        constexpr WideLineRasterizationState wideLine1_25CullFrontAndBackCW(lineCullFrontAndBackCW, 1.25f);

        constexpr WideLineRasterizationState wideLine1_5CullNoneCW(lineCullNoneCW, 1.5f);
        constexpr WideLineRasterizationState wideLine1_5CullBackCW(lineCullBackCW, 1.5f);
        constexpr WideLineRasterizationState wideLine1_5CullFrontCW(lineCullFrontCW, 1.5f);
        constexpr WideLineRasterizationState wideLine1_5CullFrontAndBackCW(lineCullFrontAndBackCW, 1.5f);

        constexpr WideLineRasterizationState wideLine1_75CullNoneCW(lineCullNoneCW, 1.75f);
        constexpr WideLineRasterizationState wideLine1_75CullBackCW(lineCullBackCW, 1.75f);
        constexpr WideLineRasterizationState wideLine1_75CullFrontCW(lineCullFrontCW, 1.75f);
        constexpr WideLineRasterizationState wideLine1_75CullFrontAndBackCW(lineCullFrontAndBackCW, 1.75f);

        constexpr WideLineRasterizationState wideLine2_CullNoneCW(lineCullNoneCW, 2.f);
        constexpr WideLineRasterizationState wideLine2_CullBackCW(lineCullBackCW, 2.f);
        constexpr WideLineRasterizationState wideLine2_CullFrontCW(lineCullFrontCW, 2.f);
        constexpr WideLineRasterizationState wideLine2_CullFrontAndBackCW(lineCullFrontAndBackCW, 2.f);

        constexpr WideLineRasterizationState wideLine2_25CullNoneCW(lineCullNoneCW, 2.25f);
        constexpr WideLineRasterizationState wideLine2_25CullBackCW(lineCullBackCW, 2.25f);
        constexpr WideLineRasterizationState wideLine2_25CullFrontCW(lineCullFrontCW, 2.25f);
        constexpr WideLineRasterizationState wideLine2_25CullFrontAndBackCW(lineCullFrontAndBackCW, 2.25f);

        constexpr WideLineRasterizationState wideLine2_5CullNoneCW(lineCullNoneCW, 2.5f);
        constexpr WideLineRasterizationState wideLine2_5CullBackCW(lineCullBackCW, 2.5f);
        constexpr WideLineRasterizationState wideLine2_5CullFrontCW(lineCullFrontCW, 2.5f);
        constexpr WideLineRasterizationState wideLine2_5CullFrontAndBackCW(lineCullFrontAndBackCW, 2.5f);

        constexpr WideLineRasterizationState wideLine2_75CullNoneCW(lineCullNoneCW, 2.75f);
        constexpr WideLineRasterizationState wideLine2_75CullBackCW(lineCullBackCW, 2.75f);
        constexpr WideLineRasterizationState wideLine2_75CullFrontCW(lineCullFrontCW, 2.75f);
        constexpr WideLineRasterizationState wideLine2_75CullFrontAndBackCW(lineCullFrontAndBackCW, 2.75f);

        constexpr WideLineRasterizationState wideLine3_CullNoneCW(lineCullNoneCW, 3.f);
        constexpr WideLineRasterizationState wideLine3_CullBackCW(lineCullBackCW, 3.f);
        constexpr WideLineRasterizationState wideLine3_CullFrontCW(lineCullFrontCW, 3.f);
        constexpr WideLineRasterizationState wideLine3_CullFrontAndBackCW(lineCullFrontAndBackCW, 3.f);

        constexpr WideLineRasterizationState wideLine3_25CullNoneCW(lineCullNoneCW, 3.25f);
        constexpr WideLineRasterizationState wideLine3_25CullBackCW(lineCullBackCW, 3.25f);
        constexpr WideLineRasterizationState wideLine3_25CullFrontCW(lineCullFrontCW, 3.25f);
        constexpr WideLineRasterizationState wideLine3_25CullFrontAndBackCW(lineCullFrontAndBackCW, 3.25f);

        constexpr WideLineRasterizationState wideLine3_5CullNoneCW(lineCullNoneCW, 3.5f);
        constexpr WideLineRasterizationState wideLine3_5CullBackCW(lineCullBackCW, 3.5f);
        constexpr WideLineRasterizationState wideLine3_5CullFrontCW(lineCullFrontCW, 3.5f);
        constexpr WideLineRasterizationState wideLine3_5CullFrontAndBackCW(lineCullFrontAndBackCW, 3.5f);

        constexpr WideLineRasterizationState wideLine3_75CullNoneCW(lineCullNoneCW, 3.75f);
        constexpr WideLineRasterizationState wideLine3_75CullBackCW(lineCullBackCW, 3.75f);
        constexpr WideLineRasterizationState wideLine3_75CullFrontCW(lineCullFrontCW, 3.75f);
        constexpr WideLineRasterizationState wideLine3_75CullFrontAndBackCW(lineCullFrontAndBackCW, 3.75f);

        constexpr WideLineRasterizationState wideLine4_CullNoneCW(lineCullNoneCW, 4.f);
        constexpr WideLineRasterizationState wideLine4_CullBackCW(lineCullBackCW, 4.f);
        constexpr WideLineRasterizationState wideLine4_CullFrontCW(lineCullFrontCW, 4.f);
        constexpr WideLineRasterizationState wideLine4_CullFrontAndBackCW(lineCullFrontAndBackCW, 4.f);

#ifdef VK_NV_fill_rectangle
        constexpr RasterizationState fillRectangleCullNoneCCW(VK_POLYGON_MODE_FILL_RECTANGLE_NV, VK_CULL_MODE_NONE, VK_FRONT_FACE_COUNTER_CLOCKWISE);
        constexpr RasterizationState fillRectangleCullBackCCW(VK_POLYGON_MODE_FILL_RECTANGLE_NV, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE);
        constexpr RasterizationState fillRectangleCullFrontCCW(VK_POLYGON_MODE_FILL_RECTANGLE_NV, VK_CULL_MODE_FRONT_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE);
        constexpr RasterizationState fillRectangleCullFrontAndBackCCW(VK_POLYGON_MODE_FILL_RECTANGLE_NV, VK_CULL_MODE_FRONT_AND_BACK, VK_FRONT_FACE_COUNTER_CLOCKWISE);

        constexpr RasterizationState fillRectangleCullNoneCW(VK_POLYGON_MODE_FILL_RECTANGLE_NV, VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE);
        constexpr RasterizationState fillRectangleCullBackCW(VK_POLYGON_MODE_FILL_RECTANGLE_NV, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_CLOCKWISE);
        constexpr RasterizationState fillRectangleCullFrontCW(VK_POLYGON_MODE_FILL_RECTANGLE_NV, VK_CULL_MODE_FRONT_BIT, VK_FRONT_FACE_CLOCKWISE);
        constexpr RasterizationState fillRectangleCullFrontAndBackCW(VK_POLYGON_MODE_FILL_RECTANGLE_NV, VK_CULL_MODE_FRONT_AND_BACK, VK_FRONT_FACE_CLOCKWISE);
#endif // VK_NV_fill_rectangle

#ifdef VK_EXT_conservative_rasterization
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
#endif // VK_EXT_conservative_rasterization

#ifdef VK_AMD_rasterization_order
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
#endif // VK_AMD_rasterization_order
    } // namespace renderstates
 } // namespace magma
