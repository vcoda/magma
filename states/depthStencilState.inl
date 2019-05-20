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
constexpr size_t StencilOpState::hash() const noexcept
{
    return internal::hashArgs(
        failOp,
        passOp,
        depthFailOp,
        compareOp,
        compareMask,
        writeMask,
        reference);
}

constexpr bool StencilOpState::operator==(const StencilOpState& other) const noexcept
{
    return (failOp == other.failOp) &&
        (passOp == other.passOp) &&
        (depthFailOp == other.depthFailOp) &&
        (compareOp == other.compareOp) &&
        (compareMask == other.compareMask) &&
        (writeMask == other.writeMask) &&
        (reference == other.reference);
}

constexpr DepthStencilState::DepthStencilState(VkCompareOp depthCompareOp, bool depthWriteEnable,
    const StencilOpState& front, const StencilOpState& back) noexcept
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    depthTestEnable = (depthCompareOp != VK_COMPARE_OP_ALWAYS);
    this->depthWriteEnable = MAGMA_BOOLEAN(depthWriteEnable);
    this->depthCompareOp = depthCompareOp;
    this->front = front;
    this->back = back;
    stencilTestEnable = (front.compareOp != VK_COMPARE_OP_ALWAYS) || (back.compareOp != VK_COMPARE_OP_ALWAYS);
    depthBoundsTestEnable = VK_FALSE;
    minDepthBounds = 0.f;
    maxDepthBounds = 1.f;
}

constexpr DepthStencilState::DepthStencilState(const DepthStencilState& state,
    const StencilOpState& front, const StencilOpState& back) noexcept:
    DepthStencilState(state.depthCompareOp, state.depthCompareOp, front, back)
{}

constexpr size_t DepthStencilState::hash() const noexcept
{
    return internal::hashArgs(
        flags,
        depthWriteEnable,
        depthCompareOp,
        front.failOp,
        front.passOp,
        front.depthFailOp,
        front.compareOp,
        front.compareMask,
        front.writeMask,
        front.reference,
        back.failOp,
        back.passOp,
        back.depthFailOp,
        back.compareOp,
        back.compareMask,
        back.writeMask,
        back.reference,
        stencilTestEnable,
        depthBoundsTestEnable,
        minDepthBounds,
        maxDepthBounds);
}

constexpr bool DepthStencilState::operator==(const DepthStencilState& other) const noexcept
{
    return (flags == other.flags) &&
        (depthTestEnable == other.depthTestEnable) &&
        (depthWriteEnable == other.depthWriteEnable) &&
        (depthCompareOp == other.depthCompareOp) &&
        (memcmp(&front, &other.front, sizeof(VkStencilOpState)) == 0) &&
        (memcmp(&back, &other.back, sizeof(VkStencilOpState)) == 0) &&
        (stencilTestEnable == other.stencilTestEnable) &&
        (depthBoundsTestEnable == other.depthBoundsTestEnable) &&
        (minDepthBounds == other.minDepthBounds) &&
        (maxDepthBounds == other.maxDepthBounds);
}

constexpr DepthBoundsState::DepthBoundsState(const DepthStencilState& state,
    float minDepthBounds, float maxDepthBounds) noexcept:
    DepthStencilState(state.depthCompareOp, state.depthWriteEnable)
{
    front = state.front;
    back = state.back;
    depthBoundsTestEnable = VK_TRUE;
    this->minDepthBounds = minDepthBounds;
    this->maxDepthBounds = maxDepthBounds;
}
} // namespace magma
