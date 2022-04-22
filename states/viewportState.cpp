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
#include "viewportState.h"
#include "../core/copy.h"
#include "../core/compare.h"

namespace magma
{
inline ViewportState::ViewportState() noexcept
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    viewportCount = 1;
    pViewports = nullptr;
    scissorCount = 1;
    pScissors = nullptr;
}

ViewportState::ViewportState(float x, float y, float width, float height,
    float minDepth /* 0 */, float maxDepth /* 1 */) noexcept:
    ViewportState()
{
    VkViewport viewport;
    viewport.x = x;
    viewport.y = y;
    viewport.width = width;
    viewport.height = height;
    viewport.minDepth = minDepth;
    viewport.maxDepth = maxDepth;
    VkRect2D scissor;
    scissor.offset.x = static_cast<int32_t>(x);
    scissor.offset.y = static_cast<int32_t>(y);
    scissor.extent.width = static_cast<uint32_t>(width);
    scissor.extent.height = static_cast<uint32_t>(height >= 0.f ? height: -height);
    pViewports = core::copyArray(&viewport, 1);
    pScissors = core::copyArray(&scissor, 1);
}

ViewportState::ViewportState(int32_t x, int32_t y, uint32_t width, int32_t height,
    float minDepth /* 0 */, float maxDepth /* 1 */) noexcept:
    ViewportState()
{
    VkViewport viewport;
    viewport.x = static_cast<float>(x);
    viewport.y = static_cast<float>(y);
    viewport.width = static_cast<float>(width);
    viewport.height = static_cast<float>(height);
    viewport.minDepth = minDepth;
    viewport.maxDepth = maxDepth;
    VkRect2D scissor;
    scissor.offset.x = x;
    scissor.offset.y = y;
    scissor.extent.width = width;
    scissor.extent.height = static_cast<uint32_t>(height >= 0 ? height : -height);
    pViewports = core::copyArray(&viewport, 1);
    pScissors = core::copyArray(&scissor, 1);
}

ViewportState::ViewportState(const VkExtent2D& extent,
    int32_t x /* 0 */, int32_t y /* 0 */,
    float minDepth /* 0 */, float maxDepth /* 1 */) noexcept:
    ViewportState()
{
    VkViewport viewport;
    viewport.x = static_cast<float>(x);
    viewport.y = static_cast<float>(y);
    viewport.width = static_cast<float>(extent.width);
    viewport.height = static_cast<float>(extent.height);
    viewport.minDepth = minDepth;
    viewport.maxDepth = maxDepth;
    VkRect2D scissor;
    scissor.offset.x = x;
    scissor.offset.y = y;
    scissor.extent = extent;
    pViewports = core::copyArray(&viewport, 1);
    pScissors = core::copyArray(&scissor, 1);
}

ViewportState::ViewportState(const VkViewport& viewport) noexcept:
    ViewportState()
{
    VkRect2D scissor;
    scissor.offset.x = static_cast<int32_t>(viewport.x);
    scissor.offset.y = static_cast<int32_t>(viewport.y);
    scissor.extent.width = static_cast<uint32_t>(viewport.width);
    scissor.extent.height = static_cast<uint32_t>(viewport.height);
    pViewports = core::copyArray(&viewport, 1);
    pScissors = core::copyArray(&scissor, 1);
}

ViewportState::ViewportState(const VkViewport& viewport, const VkRect2D& scissor) noexcept:
    ViewportState()
{
    pViewports = core::copyArray(&viewport, 1);
    pScissors = core::copyArray(&scissor, 1);
}

ViewportState::ViewportState(const std::vector<VkViewport>& viewports)
{
    std::vector<VkRect2D> scissors;
    for (const VkViewport& viewport : viewports)
    {
        VkRect2D scissor;
        scissor.offset.x = static_cast<int32_t>(viewport.x);
        scissor.offset.y = static_cast<int32_t>(viewport.y);
        scissor.extent.width = static_cast<uint32_t>(viewport.width);
        scissor.extent.height = static_cast<uint32_t>(viewport.height);
        scissors.push_back(scissor);
    }
    viewportCount = MAGMA_COUNT(viewports);
    pViewports = core::copyVector(viewports);
    scissorCount = MAGMA_COUNT(scissors);
    pScissors = core::copyVector(scissors);
}

ViewportState::ViewportState(const std::vector<VkViewport>& viewports, const std::vector<VkRect2D>& scissors) noexcept:
    ViewportState()
{
    viewportCount = MAGMA_COUNT(viewports);
    pViewports = core::copyVector(viewports);
    scissorCount = MAGMA_COUNT(scissors);
    pScissors = core::copyVector(scissors);
}

ViewportState::ViewportState(const ViewportState& other) noexcept
{
    sType = other.sType;
    pNext = nullptr;
    flags = other.flags;
    viewportCount = other.viewportCount;
    pViewports = core::copyArray(other.pViewports, viewportCount);
    scissorCount = other.scissorCount;
    pScissors = core::copyArray(other.pScissors, scissorCount);
}

ViewportState& ViewportState::operator=(const ViewportState& other) noexcept
{
    if (this != &other)
    {
        flags = other.flags;
        viewportCount = other.viewportCount;
        delete[] pViewports;
        pViewports = core::copyArray(other.pViewports, viewportCount);
        scissorCount = other.scissorCount;
        delete[] pScissors;
        pScissors = core::copyArray(other.pScissors, scissorCount);
    }
    return *this;
}

ViewportState::~ViewportState()
{
    delete[] pViewports;
    delete[] pScissors;
}

std::size_t ViewportState::hash() const noexcept
{
    std::size_t hash = core::hashArgs(
        sType,
        flags,
        viewportCount,
        scissorCount);
    if (auto viewport = pViewports)
    {
        for (uint32_t i = 0; i < viewportCount; ++i, ++viewport)
        {
            core::hashCombine(hash, core::hashArgs(
                viewport->x,
                viewport->y,
                viewport->width,
                viewport->height,
                viewport->minDepth,
                viewport->maxDepth));
        }
    }
    if (auto scissor = pScissors)
    {
        for (uint32_t i = 0; i < scissorCount; ++i, ++scissor)
        {
            core::hashCombine(hash, core::hashArgs(
                scissor->offset.x,
                scissor->offset.y,
                scissor->extent.width,
                scissor->extent.height));
        }
    }
    return hash;
}

bool ViewportState::operator==(const ViewportState& other) const noexcept
{
    return (flags == other.flags) &&
        (viewportCount == other.viewportCount) &&
        (scissorCount == other.scissorCount) &&
        core::compareArrays(pViewports, other.pViewports, viewportCount) &&
        core::compareArrays(pScissors, other.pScissors, scissorCount);
}
} // namespace magma
