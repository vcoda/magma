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
    class Allocator;

    namespace aux
    {
        /* Auxiliary multi-sample color frame buffer that can be read
           as multisample texture in the shader. */

        class ColorMultisampleReadFramebuffer : public Framebuffer
        {
        public:
            explicit ColorMultisampleReadFramebuffer(std::shared_ptr<Device> device,
                const VkFormat colorFormat,
                const VkFormat depthStencilFormat,
                const VkExtent2D& extent,
                uint32_t sampleCount,
                bool depthSampled,
                bool stencilSampled,
                std::shared_ptr<Allocator> allocator = nullptr,
                const VkComponentMapping& swizzle = {
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                    VK_COMPONENT_SWIZZLE_IDENTITY});
            std::shared_ptr<ImageView> getMultisampleColorView() noexcept { return colorView; }
            std::shared_ptr<const ImageView> getMultisampleColorView() const noexcept { return colorView; }
            std::shared_ptr<ImageView> getMultisampleDepthStencilView() noexcept { return depthStencilView; }
            std::shared_ptr<const ImageView> getMultisampleDepthStencilView() const noexcept { return depthStencilView; }

        private:
            std::shared_ptr<Image2D> color;
            std::shared_ptr<Image2D> depthStencil;
            std::shared_ptr<Image2D> resolve;
            std::shared_ptr<ImageView> colorView;
            std::shared_ptr<ImageView> depthStencilView;
            std::shared_ptr<ImageView> resolveView;
        };
    } // namespace aux
} // namespace magma
