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
            std::shared_ptr<Allocator> allocator = nullptr,
            const Sharing& sharing = Sharing());
        explicit Image2D(std::shared_ptr<CommandBuffer> cmdBuffer,
            VkFormat format,
            const VkExtent2D& extent,
            std::shared_ptr<const SrcTransferBuffer> buffer,
            const MipmapLayout& mipOffsets,
            const CopyLayout& bufferLayout = {0, 0, 0},
            std::shared_ptr<Allocator> allocator = nullptr,
            const Sharing& sharing = Sharing(),
            bool flush = true);
        explicit Image2D(std::shared_ptr<CommandBuffer> cmdBuffer,
            VkFormat format,
            const VkExtent2D& extent,
            const MipmapData& mipData,
            const MipmapLayout& mipSizes,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Sharing& sharing = Sharing(),
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
            std::shared_ptr<Allocator> allocator);
        Image2D(std::shared_ptr<Device> device,
            VkImage handle,
            VkFormat format,
            const VkExtent2D& extent);
    };

    /* Two-dimensional image with linear tiling. */

    class LinearTiledImage2D : public Image2D
    {
    public:
        explicit LinearTiledImage2D(std::shared_ptr<Device> device,
            VkFormat format,
            const VkExtent2D& extent,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Sharing& sharing = Sharing());
    };

    /* Supports unfiltered loads, stores, and atomics in a shader.
       Do transition of image layout to VK_IMAGE_LAYOUT_GENERAL using image barrier. */

    class StorageImage2D : public Image
    {
    public:
        explicit StorageImage2D(std::shared_ptr<Device> device,
            VkFormat format,
            const VkExtent2D& extent,
            uint32_t mipLevels,
            uint32_t samples,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Sharing& sharing = Sharing());
    };
} // namespace magma
