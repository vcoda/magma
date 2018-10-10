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
#include "../vulkan.h"

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

    struct DepthBiasRasterizationState : RasterizationState
    {
        DepthBiasRasterizationState(const RasterizationState& state,
            float depthBiasConstantFactor,
            float depthBiasClamp,
            float depthBiasSlopeFactor) noexcept;
    };

    /* If overestimation is enabled, fragments will be generated if the primitive area
       covers any portion of the pixel, including its edges or corners. */

    struct ConservativeRasterizationState : RasterizationState
    {
        ConservativeRasterizationState(const RasterizationState& state,
            VkConservativeRasterizationModeEXT conservativeRasterizationMode,
            float extraPrimitiveOverestimationSize) noexcept;
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

    namespace states
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
    } // namespace states
 } // namespace magma
