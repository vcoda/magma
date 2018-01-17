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
#include "viewportState.h"
#include "../shared.h"

namespace magma
{
ViewportState::ViewportState()
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
    float minDepth /* 0 */, float maxDepth /* 1 */)
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
    scissor.extent.height = static_cast<uint32_t>(height);
    initialize(viewport, scissor);
}

ViewportState::ViewportState(float x, float y, const VkExtent2D& extent, 
    float minDepth /* 0 */, float maxDepth /* 1 */)
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

ViewportState::ViewportState(const VkRect2D& vp, const VkRect2D& scissor,
    float minDepth /* 0 */, float maxDepth /* 1 */)
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

ViewportState::ViewportState(const VkViewport& viewport)
{
    VkRect2D scissor;
    scissor.offset.x = static_cast<int32_t>(viewport.x);
    scissor.offset.y = static_cast<int32_t>(viewport.y);
    scissor.extent.width = static_cast<uint32_t>(viewport.width);
    scissor.extent.height = static_cast<uint32_t>(viewport.height);
    initialize(viewport, scissor);
}

ViewportState::ViewportState(const VkViewport& viewport, const VkRect2D& scissor)
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

ViewportState::ViewportState(const std::vector<VkViewport>& viewports, const std::vector<VkRect2D>& scissors)
{
    initialize(viewports, scissors);
}

ViewportState::ViewportState(const ViewportState& other)
{
    helpers::copy(this, &other);
    pViewports = helpers::copy(new VkViewport[viewportCount], other.pViewports, viewportCount);
    pScissors = helpers::copy(new VkRect2D[scissorCount], other.pScissors, scissorCount);
}

ViewportState::~ViewportState()
{
    delete[] pViewports;
    delete[] pScissors;
}

void ViewportState::initialize(const VkViewport& viewport, const VkRect2D& scissor)
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    viewportCount = 1;
    pViewports = helpers::copy(new VkViewport[viewportCount], &viewport);
    scissorCount = 1;
    pScissors = helpers::copy(new VkRect2D[scissorCount], &scissor);
}

void ViewportState::initialize(const std::vector<VkViewport>& viewports, const std::vector<VkRect2D>& scissors)
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    viewportCount = MAGMA_COUNT(viewports);
    pViewports = helpers::copy(new VkViewport[viewportCount], viewports);
    scissorCount = MAGMA_COUNT(scissors);
    pScissors = helpers::copy(new VkRect2D[scissorCount], scissors);
}
} // namespace magma
