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
#include "tesselationState.h"
#include "../utilities/hash.h"

namespace magma
{
TesselationState::TesselationState(uint32_t patchControlPoints /* 0 */) noexcept
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    this->patchControlPoints = patchControlPoints;
}

size_t TesselationState::hash() const noexcept
{
    return utilities::hashVariadic(
        flags,
        patchControlPoints);
}

bool TesselationState::operator==(const TesselationState& other) const noexcept
{
    return (flags == other.flags) && (patchControlPoints == other.patchControlPoints);
}
} // namespace magma
