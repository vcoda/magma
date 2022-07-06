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
    /* Each tessellation control shader invocation operates on an input patch.
       The input patch is sized according to tesselation state. */

    struct TesselationState : VkPipelineTessellationStateCreateInfo
    {
        constexpr TesselationState(uint32_t patchControlPoints = 0) noexcept;
        constexpr hash_t hash() const noexcept;
        constexpr bool operator==(const TesselationState&) const noexcept;
    };
} // namespace magma

#include "tesselationState.inl"
