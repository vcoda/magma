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
        RasterizationState(VkPolygonMode polygonMode,
            VkCullModeFlags cullMode,
            VkFrontFace frontFace,
            bool depthClampEnable = false,
            bool rasterizerDiscardEnable = false) noexcept;
        size_t hash() const noexcept;
        bool operator==(const RasterizationState&) const noexcept;
    };

    /* Controls whether to bias fragment depth values:
       depthBiasConstantFactor is a scalar factor controlling the constant depth value added to each fragment.
       depthBiasClamp is the maximum (or minimum) depth bias of a fragment.
       depthBiasSlopeFactor is a scalar factor applied to a fragment's slope in depth bias calculations. */

    struct DepthBiasRasterizationState : RasterizationState
    {
        DepthBiasRasterizationState(const RasterizationState& state,
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
        ConservativeRasterizationState(const RasterizationState& state,
            VkConservativeRasterizationModeEXT conservativeRasterizationMode,
            float extraPrimitiveOverestimationSize = 0.f) noexcept;
        size_t hash() const noexcept;
        bool operator==(const ConservativeRasterizationState&) const noexcept;

        VkPipelineRasterizationConservativeStateCreateInfoEXT conservative;
    };

    /* This extension enables applications to opt into a relaxed, implementation
       defined primitive rasterization order that may allow better parallel processing
       of primitives and thus enabling higher primitive throughput. */

    struct RasterizationOrderState : RasterizationState
    {
        RasterizationOrderState(const RasterizationState& state,
            VkRasterizationOrderAMD rasterizationOrder) noexcept;
        size_t hash() const noexcept;
        bool operator==(const RasterizationOrderState&) const noexcept;

        VkPipelineRasterizationStateRasterizationOrderAMD order;
    };

    namespace renderstates
    {
        extern const RasterizationState fillCullNoneCCW;
        extern const RasterizationState fillCullBackCCW;
        extern const RasterizationState fillCullFrontCCW;
        extern const RasterizationState fillCullFrontAndBackCCW;

        extern const RasterizationState lineCullNoneCCW;
        extern const RasterizationState lineCullBackCCW;
        extern const RasterizationState lineCullFrontCCW;
        extern const RasterizationState lineCullFrontAndBackCCW;

        extern const RasterizationState pointCullNoneCCW;
        extern const RasterizationState pointCullBackCCW;
        extern const RasterizationState pointCullFrontCCW;
        extern const RasterizationState pointCullFrontAndBackCCW;

        extern const RasterizationState fillCullNoneCW;
        extern const RasterizationState fillCullBackCW;
        extern const RasterizationState fillCullFrontCW;
        extern const RasterizationState fillCullFrontAndBackCW;

        extern const RasterizationState lineCullNoneCW;
        extern const RasterizationState lineCullBackCW;
        extern const RasterizationState lineCullFrontCW;
        extern const RasterizationState lineCullFrontAndBackCW;

        extern const RasterizationState pointCullNoneCW;
        extern const RasterizationState pointCullBackCW;
        extern const RasterizationState pointCullFrontCW;
        extern const RasterizationState pointCullFrontAndBackCW;

        extern const ConservativeRasterizationState fillCullNoneCCWOverestimate;
        extern const ConservativeRasterizationState fillCullBackCCWOverestimate;
        extern const ConservativeRasterizationState fillCullFrontCCWOverestimate;
        extern const ConservativeRasterizationState fillCullFrontAndBackCCWOverestimate;

        extern const ConservativeRasterizationState lineCullNoneCCWOverestimate;
        extern const ConservativeRasterizationState lineCullBackCCWOverestimate;
        extern const ConservativeRasterizationState lineCullFrontCCWOverestimate;
        extern const ConservativeRasterizationState lineCullFrontAndBackCCWOverestimate;

        extern const ConservativeRasterizationState pointCullNoneCCWOverestimate;
        extern const ConservativeRasterizationState pointCullBackCCWOverestimate;
        extern const ConservativeRasterizationState pointCullFrontCCWOverestimate;
        extern const ConservativeRasterizationState pointCullFrontAndBackCCWOverestimate;

        extern const ConservativeRasterizationState fillCullNoneCWOverestimate;
        extern const ConservativeRasterizationState fillCullBackCWOverestimate;
        extern const ConservativeRasterizationState fillCullFrontCWOverestimate;
        extern const ConservativeRasterizationState fillCullFrontAndBackCWOverestimate;

        extern const ConservativeRasterizationState lineCullNoneCWOverestimate;
        extern const ConservativeRasterizationState lineCullBackCWOverestimate;
        extern const ConservativeRasterizationState lineCullFrontCWOverestimate;
        extern const ConservativeRasterizationState lineCullFrontAndBackCWOverestimate;

        extern const ConservativeRasterizationState pointCullNoneCWOverestimate;
        extern const ConservativeRasterizationState pointCullBackCWOverestimate;
        extern const ConservativeRasterizationState pointCullFrontCWOverestimate;
        extern const ConservativeRasterizationState pointCullFrontAndBackCWOverestimate;

        extern const ConservativeRasterizationState fillCullNoneCCWUnderestimate;
        extern const ConservativeRasterizationState fillCullBackCCWUnderestimate;
        extern const ConservativeRasterizationState fillCullFrontCCWUnderestimate;
        extern const ConservativeRasterizationState fillCullFrontAndBackCCWUnderestimate;

        extern const ConservativeRasterizationState lineCullNoneCCWUnderestimate;
        extern const ConservativeRasterizationState lineCullBackCCWUnderestimate;
        extern const ConservativeRasterizationState lineCullFrontCCWUnderestimate;
        extern const ConservativeRasterizationState lineCullFrontAndBackCCWUnderestimate;

        extern const ConservativeRasterizationState pointCullNoneCCWUnderestimate;
        extern const ConservativeRasterizationState pointCullBackCCWUnderestimate;
        extern const ConservativeRasterizationState pointCullFrontCCWUnderestimate;
        extern const ConservativeRasterizationState pointCullFrontAndBackCCWUnderestimate;

        extern const ConservativeRasterizationState fillCullNoneCWUnderestimate;
        extern const ConservativeRasterizationState fillCullBackCWUnderestimate;
        extern const ConservativeRasterizationState fillCullFrontCWUnderestimate;
        extern const ConservativeRasterizationState fillCullFrontAndBackCWUnderestimate;

        extern const ConservativeRasterizationState lineCullNoneCWUnderestimate;
        extern const ConservativeRasterizationState lineCullBackCWUnderestimate;
        extern const ConservativeRasterizationState lineCullFrontCWUnderestimate;
        extern const ConservativeRasterizationState lineCullFrontAndBackCWUnderestimate;

        extern const ConservativeRasterizationState pointCullNoneCWUnderestimate;
        extern const ConservativeRasterizationState pointCullBackCWUnderestimate;
        extern const ConservativeRasterizationState pointCullFrontCWUnderestimate;
        extern const ConservativeRasterizationState pointCullFrontAndBackCWUnderestimate;

        extern const RasterizationOrderState fillCullNoneCCWRelaxed;
        extern const RasterizationOrderState fillCullBackCCWRelaxed;
        extern const RasterizationOrderState fillCullFrontCCWRelaxed;
        extern const RasterizationOrderState fillCullFrontAndBackCCWRelaxed;

        extern const RasterizationOrderState lineCullNoneCCWRelaxed;
        extern const RasterizationOrderState lineCullBackCCWRelaxed;
        extern const RasterizationOrderState lineCullFrontCCWRelaxed;
        extern const RasterizationOrderState lineCullFrontAndBackCCWRelaxed;

        extern const RasterizationOrderState pointCullNoneCCWRelaxed;
        extern const RasterizationOrderState pointCullBackCCWRelaxed;
        extern const RasterizationOrderState pointCullFrontCCWRelaxed;
        extern const RasterizationOrderState pointCullFrontAndBackCCWRelaxed;

        extern const RasterizationOrderState fillCullNoneCWRelaxed;
        extern const RasterizationOrderState fillCullBackCWRelaxed;
        extern const RasterizationOrderState fillCullFrontCWRelaxed;
        extern const RasterizationOrderState fillCullFrontAndBackCWRelaxed;

        extern const RasterizationOrderState lineCullNoneCWRelaxed;
        extern const RasterizationOrderState lineCullBackCWRelaxed;
        extern const RasterizationOrderState lineCullFrontCWRelaxed;
        extern const RasterizationOrderState lineCullFrontAndBackCWRelaxed;

        extern const RasterizationOrderState pointCullNoneCWRelaxed;
        extern const RasterizationOrderState pointCullBackCWRelaxed;
        extern const RasterizationOrderState pointCullFrontCWRelaxed;
        extern const RasterizationOrderState pointCullFrontAndBackCWRelaxed;
    } // namespace renderstates
 } // namespace magma
