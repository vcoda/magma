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
        constexpr RasterizationState(const RasterizationState&) noexcept;
        constexpr bool chained() const noexcept { return pNext != nullptr; }
        constexpr hash_t hash() const noexcept;
        hash_t chainedHash() const noexcept;
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

    /* Allows to construct rasterization state with enabled rasterizer discard flag from any existing state.
       Primitives are discarded immediately before the rasterization stage. */

    struct DiscardRasterizationState : RasterizationState
    {
        constexpr DiscardRasterizationState(const RasterizationState& state) noexcept;
    };
} // namespace magma

#include "rasterizationState.inl"

namespace magma
{
#define MAGMA_RASTERIZATION_STATE_PERMUTATIONS(name, polygonMode)\
    constexpr RasterizationState name##CullNoneCCw(polygonMode, VK_CULL_MODE_NONE, VK_FRONT_FACE_COUNTER_CLOCKWISE);\
    constexpr RasterizationState name##CullFrontCCw(polygonMode, VK_CULL_MODE_FRONT_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE);\
    constexpr RasterizationState name##CullBackCCw(polygonMode, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE);\
    constexpr RasterizationState name##CullFrontAndBackCCw(polygonMode, VK_CULL_MODE_FRONT_AND_BACK, VK_FRONT_FACE_COUNTER_CLOCKWISE);\
    constexpr RasterizationState name##CullNoneCw(polygonMode, VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE);\
    constexpr RasterizationState name##CullFrontCw(polygonMode, VK_CULL_MODE_FRONT_BIT, VK_FRONT_FACE_CLOCKWISE);\
    constexpr RasterizationState name##CullBackCw(polygonMode, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_CLOCKWISE);\
    constexpr RasterizationState name##CullFrontAndBackCw(polygonMode, VK_CULL_MODE_FRONT_AND_BACK, VK_FRONT_FACE_CLOCKWISE);

    namespace renderstate
    {
        MAGMA_RASTERIZATION_STATE_PERMUTATIONS(fill, VK_POLYGON_MODE_FILL)
        MAGMA_RASTERIZATION_STATE_PERMUTATIONS(line, VK_POLYGON_MODE_LINE)
        MAGMA_RASTERIZATION_STATE_PERMUTATIONS(point, VK_POLYGON_MODE_POINT)
    #ifdef VK_NV_fill_rectangle
        MAGMA_RASTERIZATION_STATE_PERMUTATIONS(fillRectangle, VK_POLYGON_MODE_FILL_RECTANGLE_NV)
    #endif
    } // namespace renderstate
} // namespace magma
