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

namespace magma
{
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
    scissor.extent.width = static_cast<uint32_t>(height);
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
    scissor.extent.width = static_cast<uint32_t>(viewport.height);
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
        scissor.extent.width = static_cast<uint32_t>(viewport.height);
        scissors.push_back(scissor);
    }
    initialize(viewports, scissors);
}

ViewportState::ViewportState(const std::vector<VkViewport>& viewports, const std::vector<VkRect2D>& scissors)
{
    initialize(viewports, scissors);
}

ViewportState::~ViewportState()
{
    delete[] state.pViewports;
    delete[] state.pScissors;
}

void ViewportState::initialize(const VkViewport& viewport, const VkRect2D& scissor)
{
    state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    state.pNext = nullptr;
    state.flags = 0;
    state.viewportCount = 1;
    state.pViewports = copy(new VkViewport[state.viewportCount], &viewport);
    state.scissorCount = 1;
    state.pScissors = copy(new VkRect2D[state.scissorCount], &scissor);
}

void ViewportState::initialize(const std::vector<VkViewport>& viewports, const std::vector<VkRect2D>& scissors)
{
    state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    state.pNext = nullptr;
    state.flags = 0;
    state.viewportCount = MAGMA_COUNT(viewports);
    state.pViewports = new VkViewport[state.viewportCount];
    MAGMA_COPY_VECTOR(state.pViewports, viewports);
    state.scissorCount = MAGMA_COUNT(scissors);
    state.pScissors = new VkRect2D[state.scissorCount];
    MAGMA_COPY_VECTOR(state.pScissors, scissors);
}
} // namespace magma
