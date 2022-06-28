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

namespace magma
{
ViewportState::ViewportState() noexcept:
    viewport{},
    scissor{}
#ifdef VK_EXT_depth_clip_control
   ,depthClipControl{}
#endif
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    viewportCount = 1;
    pViewports = nullptr;
    scissorCount = 1;
    pScissors = nullptr;
}

ViewportState::ViewportState(bool negativeOneToOne) noexcept:
    ViewportState()
{
    pViewports = &viewport;
    pScissors = &scissor;
    if (negativeOneToOne)
    {
#ifdef VK_EXT_depth_clip_control
        depthClipControl.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_DEPTH_CLIP_CONTROL_CREATE_INFO_EXT;
        depthClipControl.pNext = nullptr;
        depthClipControl.negativeOneToOne = VK_TRUE;
        pNext = &depthClipControl;
#endif // VK_EXT_depth_clip_control
    }
}

ViewportState::ViewportState(float x, float y, float width, float height,
    float minDepth /* 0 */, float maxDepth /* 1 */,
    bool negativeOneToOne /* false */) noexcept:
    ViewportState(negativeOneToOne)
{
    viewport.x = x;
    viewport.y = y;
    viewport.width = width;
    viewport.height = height;
    viewport.minDepth = minDepth;
    viewport.maxDepth = maxDepth;
    scissor.offset.x = static_cast<int32_t>(x);
    scissor.offset.y = static_cast<int32_t>(y);
    scissor.extent.width = static_cast<uint32_t>(width);
    scissor.extent.height = static_cast<uint32_t>(std::abs(height));
}

ViewportState::ViewportState(int32_t x, int32_t y, uint32_t width, int32_t height,
    float minDepth /* 0 */, float maxDepth /* 1 */,
    bool negativeOneToOne /* false */) noexcept:
    ViewportState(negativeOneToOne)
{
    viewport.x = static_cast<float>(x);
    viewport.y = static_cast<float>(y);
    viewport.width = static_cast<float>(width);
    viewport.height = static_cast<float>(height);
    viewport.minDepth = minDepth;
    viewport.maxDepth = maxDepth;
    scissor.offset.x = x;
    scissor.offset.y = y;
    scissor.extent.width = width;
    scissor.extent.height = static_cast<uint32_t>(std::abs(height));
}

ViewportState::ViewportState(const VkExtent2D& extent,
    int32_t x /* 0 */, int32_t y /* 0 */,
    float minDepth /* 0 */, float maxDepth /* 1 */,
    bool negativeOneToOne /* false */) noexcept:
    ViewportState(negativeOneToOne)
{
    viewport.x = static_cast<float>(x);
    viewport.y = static_cast<float>(y);
    viewport.width = static_cast<float>(extent.width);
    viewport.height = static_cast<float>(extent.height);
    viewport.minDepth = minDepth;
    viewport.maxDepth = maxDepth;
    scissor.offset.x = x;
    scissor.offset.y = y;
    scissor.extent = extent;
}

ViewportState::ViewportState(const VkViewport& viewport_,
    bool negativeOneToOne /* false */) noexcept:
    ViewportState(negativeOneToOne)
{
    viewport = viewport_;
    scissor.offset.x = static_cast<int32_t>(viewport.x);
    scissor.offset.y = static_cast<int32_t>(viewport.y);
    scissor.extent.width = static_cast<uint32_t>(viewport.width);
    scissor.extent.height = static_cast<uint32_t>(std::abs(viewport.height));
}

ViewportState::ViewportState(const VkViewport& viewport_, const VkRect2D& scissor_,
    bool negativeOneToOne /* false */) noexcept:
    ViewportState(negativeOneToOne)
{
    viewport = viewport_;
    scissor = scissor_;
}

ViewportState::ViewportState(const ViewportState& other) noexcept:
    viewport(other.viewport),
    scissor(other.scissor)
#ifdef VK_EXT_depth_clip_control
   ,depthClipControl(other.depthClipControl)
#endif
{
    sType = other.sType;
    if (!other.pNext)
        pNext = nullptr;
#ifdef VK_EXT_depth_clip_control
    else if (depthClipControl.negativeOneToOne)
        pNext = &depthClipControl;
    else
        pNext = nullptr;
#endif // VK_EXT_depth_clip_control
    flags = other.flags;
    viewportCount = other.viewportCount;
    pViewports = &viewport;
    scissorCount = other.scissorCount;
    pScissors = &scissor;
}

ViewportState& ViewportState::operator=(const ViewportState& other) noexcept
{
    if (this != &other)
    {
        viewport = other.viewport;
        scissor = other.scissor;
#ifdef VK_EXT_depth_clip_control
        depthClipControl = other.depthClipControl;
#endif
        sType = other.sType;
        if (!other.pNext)
            pNext = nullptr;
#ifdef VK_EXT_depth_clip_control
        else if (depthClipControl.negativeOneToOne)
            pNext = &depthClipControl;
        else
            pNext = nullptr;
#endif // VK_EXT_depth_clip_control
        flags = other.flags;
        viewportCount = other.viewportCount;
        scissorCount = other.scissorCount;
    }
    return *this;
}

std::size_t ViewportState::hash() const noexcept
{
    return core::hashArgs(
        sType,
        flags,
        viewportCount,
        scissorCount,
        viewport.x,
        viewport.y,
        viewport.width,
        viewport.height,
        viewport.minDepth,
        viewport.maxDepth,
        scissor.offset.x,
        scissor.offset.y,
        scissor.extent.width,
        scissor.extent.height,
#ifdef VK_EXT_depth_clip_control
        depthClipControl.sType,
        depthClipControl.negativeOneToOne,
#endif // VK_EXT_depth_clip_control
        0);
}

bool ViewportState::operator==(const ViewportState& other) const noexcept
{
    return (flags == other.flags) &&
        (viewportCount == other.viewportCount) &&
        (scissorCount == other.scissorCount) &&
        (viewport.x == other.viewport.x) &&
        (viewport.y == other.viewport.y) &&
        (viewport.width == other.viewport.width) &&
        (viewport.height == other.viewport.height) &&
        (viewport.minDepth == other.viewport.minDepth) &&
        (viewport.maxDepth == other.viewport.maxDepth) &&
        (scissor.offset.x == other.scissor.offset.x) &&
        (scissor.offset.y == other.scissor.offset.y) &&
        (scissor.extent.width == other.scissor.extent.width) &&
        (scissor.extent.height == other.scissor.extent.height &&
#ifdef VK_EXT_depth_clip_control
        (depthClipControl.negativeOneToOne == other.depthClipControl.negativeOneToOne) &&
#endif
        true);
}
} // namespace magma
