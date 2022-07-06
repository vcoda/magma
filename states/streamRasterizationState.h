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

#ifdef VK_EXT_transform_feedback
namespace magma
{
    /* The vertex stream used for rasterization is specified by this state.
       If the implementation is capable, a vertex stream other than zero can be rasterized.
       If this structure is not present, rasterizationStream is assumed to be zero. */

    struct StreamRasterizationState : RasterizationState
    {
        constexpr StreamRasterizationState(const RasterizationState& state,
            uint32_t rasterizationStream,
            VkPipelineRasterizationStateStreamCreateFlagsEXT flags = 0) noexcept;
        constexpr hash_t hash() const noexcept;
        constexpr bool operator==(const StreamRasterizationState&) const noexcept;

        const VkPipelineRasterizationStateStreamCreateInfoEXT stream;
    };

    #include "streamRasterizationState.inl"
} // namespace magma
#endif // VK_EXT_transform_feedback
