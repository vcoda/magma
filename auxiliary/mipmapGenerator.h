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
#include "../internal/noncopyable.h"

namespace magma
{
    class Device;
    class Image;
    class CommandBuffer;
    class Queue;

    namespace aux
    {
        /* Generates MIP levels from base texture level using blit operations. */

        class MipmapGenerator : public internal::NonCopyable
        {
        public:
            explicit MipmapGenerator(std::shared_ptr<Device> device);
            bool checkFormatSupport(VkFormat format) const noexcept;
            bool generateMipmap(std::shared_ptr<Image> image,
                uint32_t baseLevel,
                VkFilter filter,
                std::shared_ptr<CommandBuffer> commandBuffer,
                bool flush) const noexcept;
            bool commit(std::shared_ptr<CommandBuffer> commandBuffer) const noexcept;

        private:
            std::shared_ptr<Device> device;
            std::shared_ptr<Queue> queue;
        };
    } // namespace aux
} // namespace magma
