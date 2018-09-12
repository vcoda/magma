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
#include "depthStencilState.h"
#include "../shared.h"

namespace magma
{
StencilOpState::StencilOpState(VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp,
    uint32_t compareMask /* 0 */, uint32_t writeMask /* 0 */, uint32_t reference /* 0 */) noexcept
{
    this->failOp = failOp;
    this->passOp = passOp;
    this->depthFailOp = depthFailOp;
    this->compareOp = compareOp;
    this->compareMask = compareMask;
    this->writeMask = writeMask;
    this->reference = reference;
}

bool StencilOpState::operator==(const StencilOpState& other) const noexcept
{
    return (failOp == other.failOp) &&
        (passOp == other.passOp) &&
        (depthFailOp == other.depthFailOp) &&
        (compareOp == other.compareOp) &&
        (compareMask == other.compareMask) &&
        (writeMask == other.writeMask) &&
        (reference == other.reference);
}

DepthStencilState::DepthStencilState(VkCompareOp depthCompareOp, bool depthWriteEnable,
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

bool DepthStencilState::operator==(const DepthStencilState& other) const noexcept
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

namespace states
{
    const StencilOpState stencilAlwaysDontWrite(VK_STENCIL_OP_KEEP, VK_STENCIL_OP_KEEP, VK_STENCIL_OP_KEEP, VK_COMPARE_OP_ALWAYS, 0x0, 0x0, 0x0);

    const DepthStencilState depthLess(VK_COMPARE_OP_LESS, true);
    const DepthStencilState depthEqual(VK_COMPARE_OP_EQUAL, true);
    const DepthStencilState depthLessOrEqual(VK_COMPARE_OP_LESS_OR_EQUAL, true);
    const DepthStencilState depthGreater(VK_COMPARE_OP_GREATER, true);
    const DepthStencilState depthNoEqual(VK_COMPARE_OP_NOT_EQUAL, true);
    const DepthStencilState depthGreaterOrEqual(VK_COMPARE_OP_GREATER_OR_EQUAL, true);
    const DepthStencilState depthAlways(VK_COMPARE_OP_ALWAYS, true);

    const DepthStencilState depthNeverDontWrite(VK_COMPARE_OP_NEVER, false);
    const DepthStencilState depthLessDontWrite(VK_COMPARE_OP_LESS, false);
    const DepthStencilState depthEqualDontWrite(VK_COMPARE_OP_EQUAL, false);
    const DepthStencilState depthLessOrEqualDontWrite(VK_COMPARE_OP_LESS_OR_EQUAL, false);
    const DepthStencilState depthGreaterDontWrite(VK_COMPARE_OP_GREATER, false);
    const DepthStencilState depthNoEqualDontWrite(VK_COMPARE_OP_NOT_EQUAL, false);
    const DepthStencilState depthGreaterOrEqualDontWrite(VK_COMPARE_OP_GREATER_OR_EQUAL, false);
    const DepthStencilState depthAlwaysDontWrite(VK_COMPARE_OP_ALWAYS, false);
} // namespace states
} // namespace magma
