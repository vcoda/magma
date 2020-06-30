/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2019 Victor Coda.

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
#include "../core/noncopyable.h"

namespace magma
{
    class Device;
    class CommandBuffer;
    class Queue;
    class SwapchainColorAttachment;
    class LinearTiledImage2D;
    class IAllocator;

    namespace aux
    {
        /* Auxiliary frame grabber to take screenshots. */

        class FrameGrabber : public core::NonCopyable
        {
        public:
            explicit FrameGrabber(std::shared_ptr<Device> device,
                std::shared_ptr<IAllocator> allocator = nullptr);
            void captureFrame(std::shared_ptr<SwapchainColorAttachment> srcImage,
                std::shared_ptr<CommandBuffer> cmdBuffer);
            void readPixels(std::function<void(uint32_t col,
                uint32_t row,
                uint32_t rgba)> forEachPixel) const;
            void readPixels(std::function<void(uint32_t row,
                const std::vector<uint32_t>& rowPixels)> forEachRow) const;
            VkExtent2D getImageExtent() const;

        private:
            bool checkBlitSupport(VkFormat srcFormat,
                VkFormat dstFormat) const noexcept;

            std::shared_ptr<Device> device;
            std::shared_ptr<Queue> queue;
            std::shared_ptr<IAllocator> allocator;
            std::shared_ptr<LinearTiledImage2D> dstImage;
            bool swizzleBgra = false;
        };
    } // namespace aux
} // namespace magma
