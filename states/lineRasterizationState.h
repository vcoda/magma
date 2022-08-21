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

#ifdef VK_EXT_line_rasterization
namespace magma
{
    /* Exposes some line rasterization features that are commonly used in CAD applications. */

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

        const VkPipelineRasterizationLineStateCreateInfoEXT line;
    };
} // namespace magma

#include "lineRasterizationState.inl"

#endif // VK_EXT_line_rasterization
