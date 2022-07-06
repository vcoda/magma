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
#pragma once

namespace magma
{
    /* The viewport transformation is determined by the selected viewport's
       width and height in pixels and its center, as well as its depth range
       min and max determining a depth range scale value. If VK_EXT_depth_clip_control
       is supported, fragment's depth may be in [0,1] or [-1,1] range. */

    class ViewportState : public VkPipelineViewportStateCreateInfo
    {
    public:
        explicit ViewportState() noexcept;
        explicit ViewportState(float x, float y, float width, float height,
            float minDepth = 0.f, float maxDepth = 1.f,
            bool negativeOneToOne = false) noexcept;
        explicit ViewportState(int32_t x, int32_t y, uint32_t width, int32_t height,
            float minDepth = 0.f, float maxDepth = 1.f,
            bool negativeOneToOne = false) noexcept;
        explicit ViewportState(const VkExtent2D& extent,
            int32_t x = 0, int32_t y = 0,
            float minDepth = 0.f, float maxDepth = 1.f,
            bool negativeOneToOne = false) noexcept;
        explicit ViewportState(const VkViewport& viewport,
            bool negativeOneToOne = false) noexcept;
        explicit ViewportState(const VkViewport& viewport,
            const VkRect2D& scissor,
            bool negativeOneToOne = false) noexcept;
        ViewportState(const ViewportState&) noexcept;
        ViewportState& operator=(const ViewportState&) noexcept;
        virtual hash_t hash() const noexcept;
        bool operator==(const ViewportState&) const noexcept;

    protected:
        ViewportState(bool negativeOneToOne) noexcept;

        VkViewport viewport;
        VkRect2D scissor;
#ifdef VK_EXT_depth_clip_control
        VkPipelineViewportDepthClipControlCreateInfoEXT depthClipControl;
#endif
    };
} // namespace magma
