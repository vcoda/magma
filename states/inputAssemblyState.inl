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
#include "../internal/hash.h"

namespace magma
{
constexpr InputAssemblyState::InputAssemblyState(VkPrimitiveTopology topology,
    bool primitiveRestartEnable /* false */) noexcept
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    this->topology = topology;
    this->primitiveRestartEnable = MAGMA_BOOLEAN(primitiveRestartEnable);
}

constexpr size_t InputAssemblyState::hash() const noexcept
{
    return internal::hashArgs(
        flags,
        topology,
        primitiveRestartEnable);
}

constexpr bool InputAssemblyState::operator==(const InputAssemblyState& other) const noexcept
{
    return (flags == other.flags) &&
        (topology == other.topology) &&
        (primitiveRestartEnable == other.primitiveRestartEnable);
}
} // namespace magma
