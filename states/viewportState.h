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
#pragma once
#include <vector>
#include "../vulkan.h"

namespace magma
{
    /* The viewport transformation is determined by the selected viewport's
       width and height in pixels and its center, as well as its depth range
       min and max determining a depth range scale value. */

    class ViewportState final : public VkPipelineViewportStateCreateInfo
    {
    public:
        ViewportState() noexcept;
        ViewportState(float x, float y, float width, float height,
            float minDepth = 0.f, float maxDepth = 1.f);
        ViewportState(float x, float y, const VkExtent2D& extent,
            float minDepth = 0.f, float maxDepth = 1.f);
        ViewportState(uint32_t x, uint32_t y, uint32_t width, int32_t height,
            float minDepth = 0.f, float maxDepth = 1.f);
        ViewportState(const VkRect2D& viewport, const VkRect2D& scissor,
            float minDepth = 0.f, float maxDepth = 1.f);
        ViewportState(const VkViewport& viewport);
        ViewportState(const VkViewport& viewport, const VkRect2D& scissor);
        ViewportState(const std::vector<VkViewport>& viewports);
        ViewportState(const std::vector<VkViewport>& viewports,
            const std::vector<VkRect2D>& scissors);
        ViewportState(const ViewportState&);
        ViewportState& operator=(const ViewportState&);
        ~ViewportState();
        size_t hash() const noexcept;
        bool operator==(const ViewportState&) const noexcept;

    private:
        void initialize(const VkViewport& viewport, const VkRect2D& scissor);
        void initialize(const std::vector<VkViewport>& viewports,
            const std::vector<VkRect2D>& scissors);
    };
} // namespace magma
