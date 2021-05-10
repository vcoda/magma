/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
#include "baseFramebuffer.h"

namespace magma
{
    class Device;
    class Image2D;
    class ImageView;
    class SwapchainColorAttachment;
    class Allocator;

    namespace aux
    {
        /* Auxiliary swapchain frame buffer. */

        class SwapchainFramebuffer : public Framebuffer
        {
        public:
            explicit SwapchainFramebuffer(std::shared_ptr<SwapchainColorAttachment> color,
                std::shared_ptr<Allocator> allocator = nullptr,
                VkFormat depthStencilFormat = VK_FORMAT_UNDEFINED,
                const VkComponentMapping& swizzle = {
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                    VK_COMPONENT_SWIZZLE_IDENTITY});

        private:
            std::shared_ptr<Image2D> depthStencil;
            std::shared_ptr<ImageView> colorView;
            std::shared_ptr<ImageView> depthStencilView;
        };
    } // namespace aux
} // namespace magma
