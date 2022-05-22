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
#include "../objects/image.h"

namespace magma
{
    namespace aux
    {
        /* Sprite is a two-dimensional bitmap that is integrated into a background image.
           Drawing is performed via image blitting with format conversion, scaling and filtering.
           Color key and alpha blending aren't supported by image blit API. */

        class Sprite : public Image
        {
        public:
            explicit Sprite(std::shared_ptr<CommandBuffer> cmdBuffer,
                VkFormat format,
                const VkExtent2D& extent,
                std::shared_ptr<const SrcTransferBuffer> buffer,
                VkDeviceSize offset,
                std::shared_ptr<Allocator> allocator = nullptr,
                const Sharing& sharing = Sharing());
            explicit Sprite(std::shared_ptr<CommandBuffer> cmdBuffer,
                VkFormat format,
                const VkExtent2D& extent,
                VkDeviceSize size,
                const void *data,
                std::shared_ptr<Allocator> allocator = nullptr,
                const Sharing& sharing = Sharing(),
                CopyMemoryFunction copyFn = nullptr);
            void setWidth(uint32_t width_) noexcept { width = width_; }
            uint32_t getWidth() const noexcept { return width; }
            void setHeight(uint32_t height_) noexcept { height = height_; }
            uint32_t getHeight() const noexcept { return height; }
            bool isScaled() const noexcept { return (width != extent.width) || (height != extent.height); }
            void setPosition(int32_t x, int32_t y) noexcept { position = {x, y}; }
            void setPosition(const VkOffset2D& position_) noexcept { position = position_; }
            const VkOffset2D& getPosition() const noexcept { return position; }
            void flipHorizontal() noexcept { std::swap(topLeft.x, bottomRight.x); }
            bool isFlippedHorizontally() const noexcept { return topLeft.x > bottomRight.x; }
            void flipVertical() noexcept { std::swap(topLeft.y, bottomRight.y); }
            bool isFlippedVertically() const noexcept { return topLeft.y > bottomRight.y; }
            void blit(std::shared_ptr<CommandBuffer> cmdBuffer,
                std::shared_ptr<Image> dstImage,
                VkFilter filter = VK_FILTER_NEAREST) const noexcept;

        private:
            uint32_t width;
            uint32_t height;
            VkOffset2D position;
            VkOffset2D topLeft;
            VkOffset2D bottomRight;
        };
    } // namespace aux
} // namespace magma