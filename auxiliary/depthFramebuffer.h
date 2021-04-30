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
        /* Auxiliary depth-only framebuffer. */

        class DepthFramebuffer : public Framebuffer
        {
        public:
            explicit DepthFramebuffer(std::shared_ptr<Device> device,
                const VkFormat depthFormat,
                const VkExtent2D& extent,
                std::shared_ptr<Allocator> allocator = nullptr);
            std::shared_ptr<ImageView> getDepthView() noexcept { return depthView; }
            std::shared_ptr<const ImageView> getDepthView() const noexcept { return depthView; }

        private:
            std::shared_ptr<Image2D> depth;
            std::shared_ptr<ImageView> depthView;
        };
    } // namespace aux
} // namespace magma
