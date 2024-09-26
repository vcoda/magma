/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

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
        /* Auxiliary multi-sample color framebuffer with resolve attachment. */

        class ColorMultisampleFramebuffer : public Framebuffer
        {
        public:
            explicit ColorMultisampleFramebuffer(std::shared_ptr<Device> device,
                const VkFormat colorFormat,
                const VkExtent2D& extent,
                uint32_t sampleCount,
                std::shared_ptr<Allocator> allocator = nullptr,
                bool colorClearOp = true,
                bool msaaColorSampled = false,
                bool msaaDepthSampled = false,
                bool msaaStencilSampled = false,
                const VkComponentMapping& swizzle = {
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                    VK_COMPONENT_SWIZZLE_IDENTITY});
            explicit ColorMultisampleFramebuffer(std::shared_ptr<Device> device,
                const VkFormat colorFormat,
                const VkFormat depthStencilFormat,
                const VkExtent2D& extent,
                uint32_t sampleCount,
                std::shared_ptr<Allocator> allocator = nullptr,
                bool colorClearOp = true,
                bool msaaColorSampled = false,
                bool msaaDepthSampled = false,
                bool msaaStencilSampled = false,
                const VkComponentMapping& swizzle = {
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                    VK_COMPONENT_SWIZZLE_IDENTITY});
            const std::shared_ptr<ImageView>& getColorView() const noexcept { return resolveView; }
            const std::shared_ptr<ImageView>& getMultisampleColorView() const noexcept { return msaaColorView; }
            const std::shared_ptr<ImageView>& getMultisampleDepthStencilView() const noexcept { return msaaDepthStencilView; }
            bool hasColorClear() const noexcept { return colorClearOp; }

        private:
            const bool colorClearOp;
            std::shared_ptr<ImageView> msaaColorView;
            std::shared_ptr<ImageView> msaaDepthStencilView;
            std::shared_ptr<ImageView> resolveView;
        };
    } // namespace aux
} // namespace magma
