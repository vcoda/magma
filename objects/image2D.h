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
    /* Two-dimensional image. */

    class Image2D : public Image
    {
    public:
        explicit Image2D(std::shared_ptr<Device> device,
            VkFormat format,
            const VkExtent2D& extent,
            uint32_t mipLevels,
            const Sharing& sharing = Sharing(),
            std::shared_ptr<IAllocator> allocator = nullptr);
        explicit Image2D(std::shared_ptr<Device> device,
            VkFormat format,
            const VkExtent2D& extent,
            std::shared_ptr<Buffer> buffer,
            VkDeviceSize bufferOffset,
            const ImageMipmapLayout& mipOffsets,
            std::shared_ptr<CommandBuffer> cmdBuffer,
            const Sharing& sharing = Sharing(),
            std::shared_ptr<IAllocator> allocator = nullptr,
            bool flush = true);
        explicit Image2D(std::shared_ptr<Device> device,
            VkFormat format,
            const VkExtent2D& extent,
            const ImageMipmapData& mipData,
            const ImageMipmapLayout& mipSizes,
            std::shared_ptr<CommandBuffer> cmdBuffer,
            const Sharing& sharing = Sharing(),
            std::shared_ptr<IAllocator> allocator = nullptr,
            CopyMemoryFunction copyFn = nullptr);

    protected:
        Image2D(std::shared_ptr<Device> device,
            VkFormat format,
            const VkExtent2D& extent,
            uint32_t mipLevels,
            uint32_t samples,
            VkImageTiling tiling,
            VkImageUsageFlags usage,
            const Sharing& sharing,
            std::shared_ptr<IAllocator> allocator);
        Image2D(std::shared_ptr<Device> device,
            VkImage handle,
            VkFormat format,
            const VkExtent2D& extent);
    };
} // namespace magma
