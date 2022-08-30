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
#include "image2D.h"

namespace magma
{
    /* Two-dimensional image that can be used as color attachment to framebuffer. */

    class ColorAttachment : public Image2D
    {
    public:
        explicit ColorAttachment(std::shared_ptr<Device> device,
            VkFormat colorFormat,
            const VkExtent2D& extent,
            uint32_t mipLevels,
            uint32_t samples,
            std::shared_ptr<Allocator> allocator = nullptr,
            const std::vector<VkFormat> viewFormats = {},
            bool sampled = true);
    };

    /* Two-dimensional image that can be used as depth/stencil attachment to framebuffer. */

    class DepthStencilAttachment : public Image2D
    {
    public:
        explicit DepthStencilAttachment(std::shared_ptr<Device> device,
            VkFormat depthStencilFormat,
            const VkExtent2D& extent,
            uint32_t mipLevels,
            uint32_t samples,
            std::shared_ptr<Allocator> allocator = nullptr,
            const std::vector<VkFormat> viewFormats = {},
            bool sampled = false);
    };

    /* Two-dimensional image that represents swapchain's color attachment to framebuffer.
       It is managed internally by swapchain and never created or destroyed by user. */

    class SwapchainColorAttachment : public Image2D
    {
        explicit SwapchainColorAttachment(std::shared_ptr<Device> device,
            VkImage handle,
            VkFormat format,
            const VkExtent2D& extent);
        friend class Swapchain;

    public:
        virtual ~SwapchainColorAttachment();
        VkExtent2D getExtent() const noexcept { return {extent.width, extent.height}; }
    };
} // namespace magma
