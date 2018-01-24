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
#include "image.h"

namespace magma
{
    class Image2D : public Image
    {
    protected:
        Image2D(std::shared_ptr<const Device> device,
            VkFormat format,
            const VkExtent2D& extent,
            uint32_t mipLevels,
            uint32_t samples,
            VkImageUsageFlags usage);
        Image2D(std::shared_ptr<const Device> device,
            VkImage image,
            VkFormat format);

    public:
        Image2D(std::shared_ptr<const Device> device,
            VkFormat format,
            const std::vector<VkExtent2D>& mipExtents,
            const std::vector<const void *>& mipData,
            const std::vector<VkDeviceSize>& mipSizes,
            std::shared_ptr<CommandBuffer> cmdBuffer);
    };

    class ColorAttachment2D : public Image2D
    {
    public:
        ColorAttachment2D(std::shared_ptr<const Device> device,
            VkFormat colorFormat,
            const VkExtent2D& extent,
            uint32_t mipLevels,
            uint32_t samples,
            bool sampled = true);
    };

    class DepthStencilAttachment2D : public Image2D
    {
    public:
        DepthStencilAttachment2D(std::shared_ptr<const Device> device,
            VkFormat depthStencilFormat,
            const VkExtent2D& extent,
            uint32_t mipLevels,
            uint32_t samples,
            bool sampled = false);
    };

    class SwapchainColorAttachment2D : public Image2D
    {
        SwapchainColorAttachment2D(std::shared_ptr<const Device> device,
            VkImage image,
            VkFormat format);
        friend class Swapchain;

    public:
        ~SwapchainColorAttachment2D();
    };
} // namespace magma
