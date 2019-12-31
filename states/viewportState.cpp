/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2019 Victor Coda.

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
#include "../detail/copy.h"
#include "../detail/compare.h"

namespace magma
{
ViewportState::ViewportState() noexcept
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
    float minDepth /* 0 */, float maxDepth /* 1 */) noexcept
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
    initialize(viewport, scissor);
}

ViewportState::ViewportState(float x, float y, const VkExtent2D& extent,
    float minDepth /* 0 */, float maxDepth /* 1 */) noexcept
{
    VkViewport viewport;
    viewport.x = x;
    viewport.y = y;
    viewport.width = static_cast<float>(extent.width);
    viewport.height = static_cast<float>(extent.height);
    viewport.minDepth = minDepth;
    viewport.maxDepth = maxDepth;
    VkRect2D scissor;
    scissor.offset.x = static_cast<int32_t>(x);
    scissor.offset.y = static_cast<int32_t>(y);
    scissor.extent = extent;
    initialize(viewport, scissor);
}

ViewportState::ViewportState(uint32_t x, uint32_t y, uint32_t width, int32_t height,
    float minDepth /* 0 */, float maxDepth /* 1 */) noexcept
{
    VkViewport viewport;
    viewport.x = static_cast<float>(x);
    viewport.y = static_cast<float>(y);
    viewport.width = static_cast<float>(width);
    viewport.height = static_cast<float>(height);
    viewport.minDepth = minDepth;
    viewport.maxDepth = maxDepth;
    VkRect2D scissor;
    scissor.offset.x = static_cast<int32_t>(x);
    scissor.offset.y = static_cast<int32_t>(y);
    scissor.extent.width = static_cast<uint32_t>(width);
    scissor.extent.height = static_cast<uint32_t>(height >= 0 ? height : -height);
    initialize(viewport, scissor);
}

ViewportState::ViewportState(const VkRect2D& vp, const VkRect2D& scissor,
    float minDepth /* 0 */, float maxDepth /* 1 */) noexcept
{
    VkViewport viewport;
    viewport.x = static_cast<float>(vp.offset.x);
    viewport.y = static_cast<float>(vp.offset.y);
    viewport.width = static_cast<float>(vp.extent.width);
    viewport.height = static_cast<float>(vp.extent.height);
    viewport.minDepth = minDepth;
    viewport.maxDepth = maxDepth;
    initialize(viewport, scissor);
}

ViewportState::ViewportState(const VkViewport& viewport) noexcept
{
    VkRect2D scissor;
    scissor.offset.x = static_cast<int32_t>(viewport.x);
    scissor.offset.y = static_cast<int32_t>(viewport.y);
    scissor.extent.width = static_cast<uint32_t>(viewport.width);
    scissor.extent.height = static_cast<uint32_t>(viewport.height);
    initialize(viewport, scissor);
}

ViewportState::ViewportState(const VkViewport& viewport, const VkRect2D& scissor) noexcept
{
    initialize(viewport, scissor);
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
    initialize(viewports, scissors);
}

ViewportState::ViewportState(const std::vector<VkViewport>& viewports, const std::vector<VkRect2D>& scissors) noexcept
{
    initialize(viewports, scissors);
}

ViewportState::ViewportState(const ViewportState& other) noexcept
{
    detail::copy(this, &other);
    pViewports = detail::copyArray(other.pViewports, viewportCount);
    pScissors = detail::copyArray(other.pScissors, scissorCount);
}

ViewportState& ViewportState::operator=(const ViewportState& other) noexcept
{
    if (this != &other)
    {
        delete[] pViewports;
        delete[] pScissors;
        detail::copy(this, &other);
        pViewports = detail::copyArray(other.pViewports, viewportCount);
        pScissors = detail::copyArray(other.pScissors, scissorCount);
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
    std::size_t hash = detail::hashArgs(
        sType,
        flags,
        viewportCount,
        scissorCount);
    if (pViewports) for (uint32_t i = 0; i < viewportCount; ++i)
    {
        detail::hashCombine(hash, detail::hashArgs(
            pViewports[i].x,
            pViewports[i].y,
            pViewports[i].width,
            pViewports[i].height,
            pViewports[i].minDepth,
            pViewports[i].maxDepth));
    }
    if (pScissors) for (uint32_t i = 0; i < scissorCount; ++i)
    {
        detail::hashCombine(hash, detail::hashArgs(
            pScissors[i].offset.x,
            pScissors[i].offset.y,
            pScissors[i].extent.width,
            pScissors[i].extent.height));
    }
    return hash;
}

bool ViewportState::operator==(const ViewportState& other) const noexcept
{
    return (flags == other.flags) &&
        (viewportCount == other.viewportCount) &&
        (scissorCount == other.scissorCount) &&
        detail::compareArrays(pViewports, other.pViewports, viewportCount) &&
        detail::compareArrays(pScissors, other.pScissors, scissorCount);
}

void ViewportState::initialize(const VkViewport& viewport, const VkRect2D& scissor) noexcept
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    viewportCount = 1;
    pViewports = detail::copyArray(&viewport, 1);
    scissorCount = 1;
    pScissors = detail::copyArray(&scissor, 1);
}

void ViewportState::initialize(const std::vector<VkViewport>& viewports, const std::vector<VkRect2D>& scissors) noexcept
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    viewportCount = MAGMA_COUNT(viewports);
    pViewports = detail::copyVector(viewports);
    scissorCount = MAGMA_COUNT(scissors);
    pScissors = detail::copyVector(scissors);
}
} // namespace magma
