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
#include "pch.h"
#pragma hdrstop
#include <cmath>
#include "multiViewportState.h"

namespace magma
{
MultiViewportState::MultiViewportState(const std::vector<VkViewport>& viewports_):
    ViewportState(),
    viewports(viewports_)
{
    viewportCount = MAGMA_COUNT(viewports);
    pViewports = viewports.data();
    for (const VkViewport& viewport : viewports)
    {
        VkRect2D scissor;
        scissor.offset.x = static_cast<int32_t>(viewport.x);
        scissor.offset.y = static_cast<int32_t>(viewport.y);
        scissor.extent.width = static_cast<uint32_t>(viewport.width);
        scissor.extent.height = static_cast<uint32_t>(std::fabs(viewport.height));
        scissors.push_back(scissor);
    }
    scissorCount = MAGMA_COUNT(scissors);
    pScissors = scissors.data();
}

MultiViewportState::MultiViewportState(const std::vector<VkViewport>& viewports_, const std::vector<VkRect2D>& scissors_):
    ViewportState(),
    viewports(viewports_),
    scissors(scissors_)
{
    viewportCount = MAGMA_COUNT(viewports);
    pViewports = viewports.data();
    scissorCount = MAGMA_COUNT(scissors);
    pScissors = scissors.data();
}

MultiViewportState::MultiViewportState(const MultiViewportState& other):
    viewports(other.viewports),
    scissors(other.scissors)
{
    sType = other.sType;
    pNext = nullptr;
    flags = other.flags;
    viewportCount = other.viewportCount;
    pViewports = viewports.data();
    scissorCount = other.scissorCount;
    pScissors = scissors.data();
}

MultiViewportState& MultiViewportState::operator=(const MultiViewportState& other)
{
    if (this != &other)
    {
        viewports = other.viewports,
        scissors = other.scissors;
        flags = other.flags;
        viewportCount = other.viewportCount;
        pViewports = viewports.data();
        scissorCount = other.scissorCount;
        pScissors = scissors.data();
    }
    return *this;
}

hash_t MultiViewportState::hash() const noexcept
{
    hash_t hash = core::hashArgs(
        sType,
        flags,
        viewportCount,
        scissorCount);
    for (const VkViewport& viewport : viewports)
    {
        hash = core::hashCombine(hash, core::hashArgs(
            viewport.x,
            viewport.y,
            viewport.width,
            viewport.height,
            viewport.minDepth,
            viewport.maxDepth));
    }
    for (const VkRect2D& scissor : scissors)
    {
        hash = core::hashCombine(hash, core::hashArgs(
            scissor.offset.x,
            scissor.offset.y,
            scissor.extent.width,
            scissor.extent.height));
    }
    return hash;
}

bool MultiViewportState::operator==(const MultiViewportState& other) const noexcept
{
    return (flags == other.flags) &&
        (viewportCount == other.viewportCount) &&
        (scissorCount == other.scissorCount) &&
        core::compareArrays(pViewports, other.pViewports, viewportCount) &&
        core::compareArrays(pScissors, other.pScissors, scissorCount);
}
} // namespace magma
