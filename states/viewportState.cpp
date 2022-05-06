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
    if (VkViewport *viewport = new(std::nothrow) VkViewport)
    {
        viewport->x = x;
        viewport->y = y;
        viewport->width = width;
        viewport->height = height;
        viewport->minDepth = minDepth;
        viewport->maxDepth = maxDepth;
        pViewports = viewport;
    }
    if (VkRect2D *scissor = new(std::nothrow) VkRect2D)
    {
        scissor->offset.x = static_cast<int32_t>(x);
        scissor->offset.y = static_cast<int32_t>(y);
        scissor->extent.width = static_cast<uint32_t>(width);
        scissor->extent.height = static_cast<uint32_t>(std::abs(height));
        pScissors = scissor;
    }
}

ViewportState::ViewportState(int32_t x, int32_t y, uint32_t width, int32_t height,
    float minDepth /* 0 */, float maxDepth /* 1 */) noexcept:
    ViewportState()
{
    if (VkViewport *viewport = new(std::nothrow) VkViewport)
    {
        viewport->x = static_cast<float>(x);
        viewport->y = static_cast<float>(y);
        viewport->width = static_cast<float>(width);
        viewport->height = static_cast<float>(height);
        viewport->minDepth = minDepth;
        viewport->maxDepth = maxDepth;
        pViewports = viewport;
    }
    if (VkRect2D *scissor = new(std::nothrow) VkRect2D)
    {
        scissor->offset.x = x;
        scissor->offset.y = y;
        scissor->extent.width = width;
        scissor->extent.height = static_cast<uint32_t>(std::abs(height));
        pScissors = scissor;
    }
}

ViewportState::ViewportState(const VkExtent2D& extent,
    int32_t x /* 0 */, int32_t y /* 0 */,
    float minDepth /* 0 */, float maxDepth /* 1 */) noexcept:
    ViewportState()
{
    if (VkViewport *viewport = new(std::nothrow) VkViewport)
    {
        viewport->x = static_cast<float>(x);
        viewport->y = static_cast<float>(y);
        viewport->width = static_cast<float>(extent.width);
        viewport->height = static_cast<float>(extent.height);
        viewport->minDepth = minDepth;
        viewport->maxDepth = maxDepth;
        pViewports = viewport;
    }
    if (VkRect2D *scissor = new(std::nothrow) VkRect2D)
    {
        scissor->offset.x = x;
        scissor->offset.y = y;
        scissor->extent = extent;
        pScissors = scissor;
    }
}

ViewportState::ViewportState(const VkViewport& viewport) noexcept:
    ViewportState()
{
    pViewports = core::copy(&viewport);
    if (VkRect2D *scissor = new(std::nothrow) VkRect2D)
    {
        scissor->offset.x = static_cast<int32_t>(viewport.x);
        scissor->offset.y = static_cast<int32_t>(viewport.y);
        scissor->extent.width = static_cast<uint32_t>(viewport.width);
        scissor->extent.height = static_cast<uint32_t>(std::abs(viewport.height));
        pScissors = scissor;
    }
}

ViewportState::ViewportState(const VkViewport& viewport, const VkRect2D& scissor) noexcept:
    ViewportState()
{
    pViewports = core::copy(&viewport);
    pScissors = core::copy(&scissor);
}

ViewportState::ViewportState(const std::vector<VkViewport>& viewports):
    ViewportState()
{
    if (!viewports.empty())
    {
        std::vector<VkRect2D> scissors;
        for (const VkViewport& viewport : viewports)
        {
            VkRect2D scissor;
            scissor.offset.x = static_cast<int32_t>(viewport.x);
            scissor.offset.y = static_cast<int32_t>(viewport.y);
            scissor.extent.width = static_cast<uint32_t>(viewport.width);
            scissor.extent.height = static_cast<uint32_t>(std::abs(viewport.height));
            scissors.push_back(scissor);
        }
        viewportCount = MAGMA_COUNT(viewports);
        if (1 == viewportCount)
            pViewports = core::copy(&viewports.front());
        else
            pViewports = core::copyVector(viewports);
        scissorCount = MAGMA_COUNT(scissors);
        if (1 == scissorCount)
            pScissors = core::copy(&scissors.front());
        else
            pScissors = core::copyVector(scissors);
    }
}

ViewportState::ViewportState(const std::vector<VkViewport>& viewports, const std::vector<VkRect2D>& scissors) noexcept:
    ViewportState()
{
    viewportCount = MAGMA_COUNT(viewports);
    if (1 == viewportCount)
        pViewports = core::copy(&viewports.front());
    else
        pViewports = core::copyVector(viewports);
    scissorCount = MAGMA_COUNT(scissors);
    if (1 == scissorCount)
        pScissors = core::copy(&scissors.front());
    else
        pScissors = core::copyVector(scissors);
}

ViewportState::ViewportState(const ViewportState& other) noexcept
{
    sType = other.sType;
    pNext = nullptr;
    flags = other.flags;
    viewportCount = other.viewportCount;
    if (1 == viewportCount)
        pViewports = core::copy(other.pViewports);
    else
        pViewports = core::copyArray(other.pViewports, other.viewportCount);
    scissorCount = other.scissorCount;
    if (1 == scissorCount)
        pScissors = core::copy(other.pScissors);
    else
        pScissors = core::copyArray(other.pScissors, other.scissorCount);
}

ViewportState& ViewportState::operator=(const ViewportState& other) noexcept
{
    if (this != &other)
    {
        this->~ViewportState();
        flags = other.flags;
        viewportCount = other.viewportCount;
        if (1 == viewportCount)
            pViewports = core::copy(other.pViewports);
        else
            pViewports = core::copyArray(other.pViewports, other.viewportCount);
        scissorCount = other.scissorCount;
        if (1 == scissorCount)
            pScissors = core::copy(other.pScissors);
        else
            pScissors = core::copyArray(other.pScissors, other.scissorCount);
    }
    return *this;
}

ViewportState::~ViewportState()
{
    if (1 == viewportCount)
        delete pViewports;
    else
        delete[] pViewports;
    if (1 == scissorCount)
        delete pScissors;
    else
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
